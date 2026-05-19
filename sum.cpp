// sum.cpp
#include <cassert>
#include "random_walk.h"

using namespace sum::random_walk;

int main()
{
	{
		atom A(2, 0);
		auto A2 = atoms(2, A);
		auto A2b = A2.begin();
		auto A2e = A2.end();
		auto a = *A2b;
		++A2b;
		assert(A2b == A2e);
	}
	/*
	{
		atom A(1, 0);
		auto A2 = atoms(2, A);
		assert(atom(2,0) == *A2);
		++A2;
		assert(atom(2, 1) == *A2);
		++A2;
		assert(A2.end() == A2);
	}
	*/

	auto EV = value([](atom<signed> A) { return A.k; }, 10);
	double v = EV(atom(0, 0));
	v = EV(atom(10, 0));
	v = EV(atom(10, 5));
	v = EV(atom(10, 10));

	return 0;
}