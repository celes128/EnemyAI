#pragma once

#include "PartyStatusFrame.h"
#include "Party.h"
#include "Bank.h"
#include "Spell.h"

struct Event;
class EventSystem;

class ConsoleOutput {
public:
	ConsoleOutput(
		EventSystem *eventSystem,
		const Bank<SpellData> *spellDataBank,
		const std::vector<const Party *> &parties
	);
	
	static void EventCallback(const Event &e, void *listenerData);

private:
	void handle_event(const Event &e);

	// Constructor utils
	void listen_to_all_events(EventSystem *eventSystem);

private:
	const Bank<SpellData>	*m_spellDataBank;
	PartyStatusFrame		m_partyStatusFrame;
};