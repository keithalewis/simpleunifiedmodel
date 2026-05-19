// sum.h - Simple Unified Model
#pragma once
#include <concepts>
#include <iterator>

#if 0
namespace sum {

	// Atom values
	template<auto GUID, auto Time>
	struct Atom {
		constexpr auto guid = GUID;
		constexpr auto time = Time;
	};

	// A subpartition is a forward iterator over atoms at a given time.
	template<typename Atom>
	concept Atoms = std::forward_iterator<Atom>;

	// Atoms in A_u entialed by A in A_t.
	template<template<typename> typename Atom, typename Time>
	concept Entails = requires(Atom<Time> QA, Time t, Time u) {
		{ Entails(A, t, u) } -> Atoms;
	};

	// m in ba(A_t)
	

	// multiply a function by a measure

	// stopping time {tau = n} atoms in A_t  
	template<template<typename> typename Atom, typename Time>
	concept StoppingTime = requires(Time t) {
		{ At(t) } -> Atoms<Atom<Time>>
	};

	// V_t D_t = (V_u D_u + \sum_{n < tau_j <= u} C_j D_tau_j)|A_t. 

} // namespace sum
#endif // 0