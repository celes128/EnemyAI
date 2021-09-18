#include "ConsoleOutput.h"

#include <iostream>
#include <vector>

#include "EventSystem.h"
#include "Entity.h"
#include "wincons.h"

ConsoleOutput::ConsoleOutput(
	EventSystem *eventSystem,
	const Bank<SpellData> *spellDataBank,
	const std::vector<const Party *> &parties
)
	:
	m_spellDataBank(spellDataBank),
	m_partyStatusFrame(parties)
{
	listen_to_all_events(eventSystem);
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

		m_partyStatusFrame.Update();
		m_partyStatusFrame.Print();
	}break;

	case T::CombatTurnEnd: {
		cout << "The turn has ended." << endl;
		cout << "----------------------------------------------" << endl;
	}break;

	case T::ModifHP: {
		cout << e.asModifHP.target->Name() << " loses " << -1 * e.asModifHP.amount << " HP." << endl;
	}break;

	case T::Life: {
		WinCons::color_printf(
			WinCons::Color::Green,
			WinCons::INTENSIFY,
			"%s comes back to life (with %d HP).\n",
			e.asLife.entity->Name().c_str(), e.asLife.hpAmount);
	}break;

	case T::Death: {
		WinCons::color_printf(
			WinCons::Color::Red,
			WinCons::INTENSIFY,
			"%s has killed %s (%d damage).\n",
			e.asDeath.killer->Name().c_str(), e.asDeath.victim->Name().c_str(), e.asDeath.hpAmount);
	}break;

	case T::SpellCast: {
		// Get the spell name
		std::string spellName;
		Result result = m_spellDataBank->NameFromId(IN e.asSpellCast.spellId, OUT &spellName);

		if (ok(result)) {
			cout << e.asSpellCast.caster->Name() << " uses " << spellName
				<< " against " << e.asSpellCast.target->Name() << "."
				<< endl;
		}
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

void ConsoleOutput::listen_to_all_events(EventSystem *eventSystem)
{
	// Make the console listen to all events
	std::vector<Event::Type> eventTypes(0, Event::Type::Count);// reserve for only one malloc
	for (int i = 0; i < Event::Type::Count; i++) {
		eventTypes.push_back(static_cast<Event::Type>(i));
	}

	eventSystem->AddListener(EventCallback, this, eventTypes);
}



// Class static event callback function
void ConsoleOutput::EventCallback(const Event &e, void *listenerData)
{
	auto *instance = static_cast<ConsoleOutput *>(listenerData);
	instance->handle_event(e);
}