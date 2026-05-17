// binomial.h - Binomial model
#pragma once
#include <cmath>
#include <cstdint>
#include <concepts>
#include <execution>
#include <functional>
#include <iterator>
#include <numeric>
#include <stdexcept>
//#include <type_traits>

#define ENSURE_HASH(x) #x
#define ENSURE_HASH_(x) ENSURE_HASH(x)
#define ENSURE(x) { if(!std::is_constant_evaluated() and !(x)) throw std::runtime_error(ENSURE_HASH_(x)); }

namespace sum::binomial {

	// Number of ways to choose k items from a collection of n items
	template<std::signed_integral I>
	constexpr I choose(I n, I k)
	{
		// TODO: memoize?
		if (k < 0 or k > n) {
			return 0; // error
		}

		if (k == 0 or k == n) {
			return 1;
		}
		if (2 * k > n) {
			k = n - k;
		}

		return choose(n - 1, k - 1) + choose(n - 1, k);
	}
	static_assert(choose(0, 0) == 1);
	static_assert(choose(4, 2) == 6);
	static_assert(choose(2, 4) == 0);

	// V_t = v in { 0, 1, ..., t }
	template<std::signed_integral I>
	struct atom {
		I t, v;
		constexpr atom(I t, I v)
			: t(t), v(v)
		{
			// ENSURE(v <= t)
		}
		constexpr bool operator==(const atom&) const = default;
	};
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
	class atoms {
		I t, j, k, i;
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = atom<I>;
		using difference_type = std::ptrdiff_t;
		using pointer = atom<I>*;
		using reference = atom<I>&;

		constexpr atoms(I t, I j, I k, I i)
			: t(t), j(j), k(k), i(i)
		{
			// ENSURE(k <= t);
		}
		constexpr atoms(I t, atom<I> A)
			: atoms(t, A.v, A.t, A.v)
		{ }

		constexpr bool operator==(const atoms&) const = default;

		constexpr atoms begin() const noexcept
		{
			return atoms(t, j, k, j);
		}
		constexpr atoms end() const noexcept
		{
			return atoms(t, j, k, j + (t - k) + 1);
		}

		constexpr operator bool() const
		{
			return j <= i and i <= j + (t - k);
		}
		constexpr value_type operator*() const
		{
			return atom(t, i);
		}
		constexpr atoms& operator++()
		{
			++i;

			return *this;
		}
		constexpr atoms operator++(int)
		{
			auto temp = *this;
			operator++();

			return temp;
		}
	};
	//static_assert(atoms(1, 2) == atoms(1, 2));
	//static_assert(atoms(1, 2) != atoms(2, 1));

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