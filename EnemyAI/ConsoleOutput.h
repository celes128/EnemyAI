#pragma once

#include "EventSystem.h"

class ConsoleOutput {
public:
	ConsoleOutput(EventSystem *eventSystem);
	
	static void EventCallback(const Event &e, void *listenerData);

private:
	void handle_event(const Event &e);
};