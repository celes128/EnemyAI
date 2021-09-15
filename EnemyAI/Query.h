#pragma once

#include <random>

#include "framework.h"
#include "Party.h"

struct Query {
public:
	Query() = delete;

	static Result AnyAliveEntity(IN Party &party, OUT Entity **entity);
	
	static Result AnyDeadEntity(IN Party &party, OUT Entity **entity);

	template <class T>
	static Result RandomElement(IN const std::vector<T> &v, OUT size_t *i)
	{
		assert(i);

		if (v.size() > 0) {
			*i = s_randEngine() % v.size();
			return kSuccess;
		}

		return kFailure;
	}

	template <class UnaryPredicate>
	static Result RandomEntityIf(IN Party &party, IN UnaryPredicate predicate, OUT Entity **entity)
	{
		assert(entity);

		auto const kPartySize = party.size();

		if (0 == kPartySize) {
			return kFailure;
		}

		auto j = s_randEngine() % kPartySize;
		auto i = j;
		for (uint counter = 0; counter < kPartySize; ++counter) {
			if (predicate(party[i])) {
				*entity = party[i];
				return kSuccess;
			}

			i = (i + 1) % kPartySize;
		}

		return kFailure;
	}

private:
	static std::default_random_engine s_randEngine;
};