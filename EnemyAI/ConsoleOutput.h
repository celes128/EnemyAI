#pragma once

#include "EventSystem.h"
#include "PartyStatusFrame.h"
#include "Party.h"

class ConsoleOutput {
public:
	ConsoleOutput(
		EventSystem *eventSystem,
		const std::vector<const Party *> &parties
	);
	
	static void EventCallback(const Event &e, void *listenerData);

private:
	void handle_event(const Event &e);

private:
	PartyStatusFrame	m_partyStatusFrame;
};