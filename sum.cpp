// sum.cpp
#include <cassert>
#include "random_walk.h"

using namespace sum::random_walk;

int test_atoms()
{
	{
		atom A(0, 0);
		atom A2{ A };
		assert(A2 == A);
		A = A2;
		assert(!(A != A2));
	}
	{
		//atom A(1, 0);
		//auto A2 = atoms(2, A);
		auto A2 = atoms(2, atom(1,0));
		auto A3 = A2.begin();
		assert(atom(2, 0) == *A3);
		++A3;
		assert(atom(2, 1) == *A3);
		++A3;
		assert(A2.end() == A3);
	}

	return 0;
}

int main()
{
	assert(!test_atoms());

	auto EV = value([](atom<signed> A) { return A.k; }, 10);
	double v = EV(atom(0, 0));
	v = EV(atom(10, 0));
	v = EV(atom(10, 5));
	v = EV(atom(10, 10));

	return 0;
}