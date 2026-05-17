// sum.cpp
#include <cassert>
#include "binomial.h"

using namespace sum::binomial;

int main()
{
	{
		atom A(2, 0);
		auto A2 = atoms(2, A);
		auto a = *A2;
		++A2;
		auto A2e = A2.end();
		assert(A2e == A2);
	}
	{
		atom A(1, 0);
		auto A2 = atoms(2, A);
		assert(atom(2,0) == *A2);
		++A2;
		assert(atom(2, 1) == *A2);
		++A2;
		assert(A2.end() == A2);
	}

	auto EV = value([](atom<signed> A) { return A.v; }, 10);
	double v = EV(atom(0, 0));
	v = EV(atom(10, 0));
	v = EV(atom(10, 5));
	v = EV(atom(10, 10));

	return 0;
}