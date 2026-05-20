// random_walk.h - Binomial model
#pragma once
#include <cmath>
#include <concepts>
#include <ranges>
#include <stdexcept>

#define ENSURE_HASH(x) #x
#define ENSURE_HASH_(x) ENSURE_HASH(x)
// constexpr functions cannot
#define ENSURE(x) { if(!std::is_constant_evaluated() and !(x)) throw std::runtime_error(ENSURE_HASH_(x)); }

namespace sum::random_walk {

	// Number of ways to choose k items from a collection of n items
	template<std::signed_integral I>
	constexpr I choose(I n, I k) {
		if (k < 0 or k > n) return 0; // error
		if (k == 0 || k == n) return 1;
		if (k > n - k) k = n - k;  // choose(n, k) = choose(n, n - k)

		I Cnk = 1;
		for (I i = 0; i < k; ++i) {
			Cnk *= (n - i);
			Cnk /= (i + 1); // exact integer division
		}

		return Cnk;
	}
	static_assert(choose(0, 0) == 1);
	static_assert(choose(4, 2) == 6);
	static_assert(choose(2, 4) == 0); // error

	// One direction random walk.
	// P(X_j = 0) = 1/2 = P(X_j = 1)
	// V_n = X_1 + ... + X_n
	// Atom is the set {V_n = k} where k in { 0, 1, ..., n }
	template<std::signed_integral I> // TODO: require n to be std::unsigned_integral?
	struct atom {
		I n, k;
		constexpr atom(I n, I k)
			: n(n), k(k)
		{
			// ENSURE(0 <= k and k <= n)
		}
		constexpr bool operator==(const atom&) const = default;
	};
	static_assert(atom(0, 0) == atom(0, 0));
	static_assert(atom(1, 0) != atom(2, 1));

	// P(V_n = k) = C(n,k)/2^n in random_walk model
	template<std::signed_integral I>
	constexpr double P(atom<I> A)
	{
		return std::ldexp(choose(A.n, A.k), -A.n); // TODO: ldexp not constexpr in MSVC
	}

	// Atoms of A_n contained in V_k = j.
	// iterator from atom(n,j) to atom(n, j + (n - k)) in A_n
	template<std::signed_integral I>
	constexpr auto atoms(I n, atom<I> A) // TODO: atom, n ?
	{
		return std::views::iota(A.k, A.k + (n - A.n) + 1)
			| std::views::transform([n](I i) { return atom(n, i); });
	}

	// (f(V_n) P)|A_k is a replacement for expected value E[f(V_n)|A_k]
	template<class F, std::signed_integral I> //, class Policy = std::execution::unsequenced_policy)
	constexpr auto value(F f, I n) // TODO: replace n by a stopping time.
	{
		return [f, n](atom<I> A) {
			if (n > A.n) { // restrict to A_k, k <= n
				return std::numeric_limits<double>::quiet_NaN();
			}
			double v = 0;
			for (atom<I> B : atoms(n, A)) {
				v += f(B) * P(B);
			}
			return v;
			};
		/*
		return [f, n](atom<I> A) { // A in A_k
			atoms An(n, A);
			auto unop = [f](atom<I> B) { return f(B) * P(B); };
			return std::transform_reduce(An.begin(), An.end(), 0.0, std::plus<double>{}, unop);
		};
		*/
	}

	// one-sided random walk measure
	template<std::signed_integral I>
	struct V {
		// Measure of A
		static double operator()(atom<I> A)
		{
			return P(A);
		}
		// Return measure V|A_n multiplied by f restricted to A_k, k <= n.
		template<class F>
		static auto operator()(F f, I n) 
		{
			return value(f, n);
		}
	};

	// Two-sided random walk W_n = 2 V_n - n for binomial model.
	namespace binomial {

		template<std::signed_integral I>
		struct atom : public random_walk::atom<I>
		{
			using random_walk::atom<I>::atom;

			constexpr atom(const random_walk::atom<I> A)
				: random_walk::atom<I>(A.n, 2 * A.k - A.n)
			{}
			constexpr operator random_walk::atom<I>() const
			{
				using random_walk::atom<I>::n; 
				using random_walk::atom<I>::k;
				// ensure n + k is even
				return random_walk::atom<I>(n, (n + k) / 2);
			}
		};
		//static_assert(atom(0, 0) == random_walk::atom(0, 0));
		//static_assert(atom(random_walk::atom(2, 1)) == random_walk::atom(2, 0));

		// Measure on atoms of a partition.
		//<template<typename> class Atom, typename Time>
		template<std::signed_integral I>
		class W {
		public:
			static double operator()(atom<I> A)
			{
				return V(A); // converts to random_walk::atom
			}

			// (f(W_n)|A_k
			template<class F>
			static auto operator()(F f, I n) {
				return V(f, n) | std::views::transform(
					[](random_walk::atom A) { return binomial::atom(A); });
			}
		};

		// Random walk approximation to Brownian motion parameterized by step size dt.
		// B_t = W_floor(nt) / sqrt(n) where n = floor(nt/dt)
		namespace brownian {

			// Brownian motion atom B_t = w
			template<std::signed_integral I>
			struct atom {
				double t, w, dt; // TODO: encapsulate

				constexpr atom(double t, double w, double dt = 1)
					: t(t), w(w), dt(dt)
				{ }
				constexpr atom(binomial::atom<I> A, double dt = 1)
					: atom(t / dt, A.n/std::sqrt(t / dt), dt)
				{ }

				bool operator==(const atom&) const = default;
			};

			template<std::signed_integral I>
			class B {
				double dt;
			public:
				double operator()(brownian::atom<I>) const
				{
					return  0;
				}
				template<class F>
				auto operator()(F f, double t) const
				{
					return 0;
				}
			};

		} // namespace brownian

	} // namespace binomial

} // namespace sum::random_walk

#undef ENSURE_HASH_
#undef ENSURE_HASH
#undef ENSURE