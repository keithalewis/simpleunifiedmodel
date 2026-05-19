// random_walk.h - Binomial model
#pragma once
#include <cmath>
#include <concepts>
#include <ranges>
#include <stdexcept>

#define ENSURE_HASH(x) #x
#define ENSURE_HASH_(x) ENSURE_HASH(x)
#define ENSURE(x) { if(!std::is_constant_evaluated() and !(x)) throw std::runtime_error(ENSURE_HASH_(x)); }

namespace sum::random_walk {

	// Number of ways to choose k items from a collection of n items
	template<std::signed_integral I>
	constexpr I choose(I n, I k) {
		if (k < 0 or k > n) return 0; // error
		if (k == 0 || k == n) return 1;
		if (k > n - k) k = n - k;  // Optimization

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
	template<std::signed_integral I>
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
		return std::ldexp(choose(A.n, A.k), -A.n);
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
		// Two-sided random walk W_n = 2 V_n - n
		namespace binomial {

			// Convert one-sided to two-sided atom.
			template<std::signed_integral I>
			constexpr atom<I> to_binomial(atom<I> A)
			{
				return atom(A.n, 2 * A.k - A.n);
			}
			static_assert(to_binomial(atom(2, 1)) == atom(2, 0));
			static_assert(to_binomial(atom(3, 1)) == atom(3, -1));

			// Measure on atoms of a partition.
			template<template<typename> class Atom, typename Time>
			class W {
			public:
				double operator()(Atom<Time> A)
				{
					return 0;
				}
				// Iterator over atoms of A_t containing A
				auto atoms(Time t, Atom<Time> A) {
					return 0;
				}
			};

		} // namespace binomial

		// Random walk approximation to Brownian motion parameterized by step size dt.
		namespace brownian {


		} // namespace brownian

} // namespace sum::random_walk

#undef ENSURE_HASH_
#undef ENSURE_HASH
#undef ENSURE