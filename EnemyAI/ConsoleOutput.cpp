#include "ConsoleOutput.h"

#include <iostream>
#include <vector>

#include "Entity.h"
#include "wincons.h"

ConsoleOutput::ConsoleOutput(EventSystem *eventSystem)
{
	// Make the console listen to all events
	std::vector<Event::Type> eventTypes(0, Event::Type::Count);// reserve for only one malloc
	for (int i = 0; i < Event::Type::Count; i++) {
		eventTypes.push_back(static_cast<Event::Type>(i));
	}

	eventSystem->AddListener(EventCallback, this, eventTypes);
}

void ConsoleOutput::handle_event(const Event &e)
{
	using namespace std;
	using T = Event::Type;

	switch (e.type) {
	case T::CombatBegin: {
		cout << "The combat has started." << endl;
	}break;

	case T::CombatEnd: {
		cout << "The combat is over." << endl;
	}break;

	case T::CombatTurnBegin: {
		cout << "A new turn starts." << endl;
	}break;

	case T::CombatTurnEnd: {
		cout << "The turn has ended." << endl;
		cout << "----------------------------------------------" << endl;
	}break;

	case T::ModifHP: {
		cout << "HP modif from " << e.asModifHP.caster->Name()
			<< " to " << e.asModifHP.target->Name()
			<< " by " << e.asModifHP.amount << " HP."
			<< endl;
	}break;

	case T::Death: {
		WinCons::color_printf(
			WinCons::Color::Red,
			WinCons::INTENSIFY,
			"%s has killed %s (%d damage).\n",
			e.asDeath.killer->Name().c_str(), e.asDeath.victim->Name().c_str(), e.asDeath.hpAmount
		);
	}break;

	case T::SpellCast: {
		cout << "Spell (id=" << e.asSpellCast.spellId << ")"
			<< " casted by " << e.asSpellCast.caster->Name()
			<< " towards " << e.asSpellCast.target->Name() << "."
			<< endl;
	}break;

	case T::ActionNone: {
		cout << "Entity " << e.asActionNone.caster->Name()
			<< " has chosen a dummy action (Action::None)."
			<< endl;
	}break;

	default:
		break;
	}
}

// Class static event callback function
void ConsoleOutput::EventCallback(const Event &e, void *listenerData)
{
	auto *instance = static_cast<ConsoleOutput *>(listenerData);
	instance->handle_event(e);
}