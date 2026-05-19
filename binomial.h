// binomial.h - Binomial model
#pragma once
#include <cmath>
#include <cstdint>
#include <concepts>
#include <execution>
#include <functional>
#include <iterator>
#include <numeric>
#include <ranges>
#include <stdexcept>
//#include <type_traits>

#define ENSURE_HASH(x) #x
#define ENSURE_HASH_(x) ENSURE_HASH(x)
#define ENSURE(x) { if(!std::is_constant_evaluated() and !(x)) throw std::runtime_error(ENSURE_HASH_(x)); }

namespace sum::binomial {

	// Number of ways to choose k items from a collection of n items
	template<std::signed_integral I>
	constexpr I choose(I n, I k) {
		if (k > n) return 0;
		if (k == 0 || k == n) return 1;
		if (k > n - k) k = n - k;  // Optimization

		I result = 1;
		for (I i = 0; i < k; ++i) {
			result *= (n - i);
			result /= (i + 1);
		}
		return result;
	}	
	static_assert(choose(0, 0) == 1);
	static_assert(choose(4, 2) == 6);
	static_assert(choose(2, 4) == 0);

	// V_t = v where v in { 0, 1, ..., t }
	template<std::signed_integral I>
	struct atom {
		I t, v;
		constexpr atom(I t, I v)
			: t(t), v(v)
		{
			// ENSURE(0 <= v and v <= t)
		}
		constexpr bool operator==(const atom&) const = default;
	};
	static_assert(atom(0, 0) == atom(0, 0));
	static_assert(atom<signed>(0, 0) == atom<signed>(0, 0));
	static_assert(atom<signed>(1, 0) != atom<signed>(2, 1));

	// P(V_t = v) in binomial model
	template<std::signed_integral I>
	constexpr double P(atom<I> A)
	{
		return std::ldexp(choose(A.t, A.v), -A.t);
	}

	// Atoms of A_t contained in V_k = j.
	// iterator from atom(t,j) to atom(t, j + (t - k)) in A_t
	template<std::signed_integral I>
	constexpr auto atoms(I n, atom<I> A)
	{
		return std::views::iota(A.v, A.v + (n - A.t) + 1)
			 | std::views::transform([n](I i) { return atom(n, i); });
	}


	// (f(V_n) P)|A_k
	template<class F, std::signed_integral I> //, class Policy = std::execution::unsequenced_policy)
	constexpr auto value(F f, I n)
	{
		return [f, n](atom<I> A) {
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

} // namespace sum::binomial

#undef ENSURE_HASH_
#undef ENSURE_HASH
#undef ENSURE