// binomial.h - Binomial model
#pragma once
#include <cmath>
#include <concepts>
#include <execution>
#include <iterator>
#include <numeric>
#include <ranges>
#include <stdexcept>
//#include <type_traits>

#define ENSURE_HASH(x) #x
#define ENSURE_HASH_(x) ENSURE_HASH(x)
#define ENSURE(x) { if (!x) throw std::runtime_error(ENSURE_HASH_(x)); }

namespace sum::binomial {

	// Number of ways to choose k items from a collection of n items
	template<std::signed_integral I>
	constexpr I choose(I n, I k)
	{
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


	// iterator from i to j inclusive
	template<std::signed_integral I>
	class interval {
		I b, e, i;
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = I;
		using difference_type = std::ptrdiff_t;
		using pointer = I*;
		using reference = I&;

		constexpr interval(I b, I e)
			: b(b), e(e), i(b)
		{
			ENSURE(b <= e);
		}

		// lexicographic order on b, e, i, di
		constexpr bool operator==(const interval&) const = default;
		static_assert(interval(1, 2) == interval(1, 2));
		static_assert(interval(1, 2) != interval(2, 1));

		constexpr interval begin() const noexcept
		{
			return interval(b, e, b);
		}
		constexpr interval end() const noexcept
		{
			return interval(b, e, e + 1);
		}

		constexpr operator bool() const
		{
			return b <= i and i <= e;
		}
		constexpr I operator*() const
		{
			return i;
		}
		constexpr interval& operator++()
		{
			++i;
		}
		constexpr interval operator++(int)
		{
			auto temp = *this;
			operator++();
			return temp;
		}
	};

	// V_k = n
	template<std::signed_integral I>
	class atom {
		I k, n;
	public:
		constexpr atom(I k, I n)
			: k(k), n(n)
		{
			ENSURE(k <= n);
		}
		double P() const
		{
			std::ldexp(choose(n, k), -n);
		}
	};

	template<std::signed_integral I>
	constexpr interval<I> atoms(I j, I k, I n)
	{
		ENSURE(0 <= j and j <= k and k <= n);
		
		return interval(j, j + n - k);
	}

	// int_A f V_n P
	template<class F, std::signed_integral I, class Policy = std::execution::unsequenced_policy)
	constexpr auto meas(F&& f, interval<I> A)
	{
		auto result = std::transform_reduce(
			a.begin(), a.end(), b.begin(),
			0,                    // initial value
			std::plus{},          // accumulation operation
			std::multiplies{}     // transform operation
		);
	}

} // namespace sum::binomial
