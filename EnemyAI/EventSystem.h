#pragma once

#include <vector>
#include <queue>

#include "framework.h"

class Entity;

struct Event {
	enum Type {
		None,

		CombatBegin,
		CombatEnd,
		CombatTurnBegin,
		CombatTurnEnd,
		
		SpellCast,
		ModifHP,

		Death,

		ActionNone,

		Count
	};

	Type type;// Discriminant

	struct AsSpellCast {
		uint		spellId;
		Entity		*caster;
		Entity		*target;
	};

	struct AsModifHP {
		int			amount;
		Entity		*caster;
		Entity		*target;
	};

	struct AsDeath {
		Entity		*victim;
		Entity		*killer;
		int			hpAmount;
	};

	struct AsActionNone {
		Entity		*caster;
	};

	union {
		AsSpellCast		asSpellCast;
		AsModifHP		asModifHP;
		AsDeath			asDeath;
		AsActionNone	asActionNone;
	};

	static Event MakeCombatBegin() { return Event(Type::CombatBegin); }
	static Event MakeCombatEnd() { return Event(Type::CombatEnd); }
	static Event MakeCombatTurnBegin() { return Event(Type::CombatTurnBegin); }
	static Event MakeCombatTurnEnd() { return Event(Type::CombatTurnEnd); }

	static Event MakeSpellCast(uint spellId, Entity *caster, Entity *target)
	{
		Event e;

		e.type = Type::SpellCast;

		e.asSpellCast.spellId = spellId;
		e.asSpellCast.caster = caster;
		e.asSpellCast.target = target;

		return e;
	}

	static Event MakeModifHP(int amount, Entity *caster, Entity *target)
	{
		Event e;

		e.type = Type::ModifHP;

		e.asModifHP.amount = amount;
		e.asModifHP.caster = caster;
		e.asModifHP.target = target;

		return e;
	}

	static Event MakeDeath(Entity *victim, Entity *killer, int hpAmount)
	{
		Event e;

		e.type = Type::Death;

		e.asDeath.victim = victim;
		e.asDeath.killer = killer;
		e.asDeath.hpAmount = hpAmount;

		return e;
	}

	static Event MakeActionNone(Entity *caster)
	{
		Event e;

		e.type = Type::ActionNone;

		e.asActionNone.caster = caster;

		return e;
	}

	Event(Type type = Type::None) : type(type) {}
};

using EventListenerCallback = void(*)(const Event &e, void *listenerData);

struct EventListener {
	EventListenerCallback	callback;
	void					*data;
};

inline bool operator==(const EventListener &l, const EventListener &r)
{
	return l.callback == r.callback && l.data == r.data;
}
inline bool operator!=(const EventListener &l, const EventListener &r)
{
	return !(l == r);
}

class EventSystem {
public:
	EventSystem();

	void AddListener(
		EventListenerCallback callback,
		void *data,
		const std::vector<Event::Type> &types
	);

	void PostEvent(const Event &e)
	{
		m_events.push(e);
	}

	void ProcessEvents();

private:
	std::vector<std::vector<EventListener>> m_listeners;
	std::queue<Event> m_events;
};