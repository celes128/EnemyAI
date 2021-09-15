#include "EventSystem.h"

#include <algorithm>

EventSystem::EventSystem()
	:
	m_listeners(Event::Type::Count)
{
}

void EventSystem::AddListener(
	EventListenerCallback callback,
	void *data,
	const std::vector<Event::Type> &types
)
{
	EventListener listener{ callback, data };

	for (auto type : types) {
		// Add the listener only if it does not already listen to this type of event
		auto it = std::find(m_listeners[type].begin(), m_listeners[type].end(), listener);
		if (it == m_listeners[type].end()) {
			m_listeners[type].push_back(listener);
		}
	}
}

void EventSystem::ProcessEvents()
{
	while (!m_events.empty()) {
		auto e = m_events.front();
		m_events.pop();

		for (auto &listener : m_listeners[e.type]) {
			listener.callback(e, listener.data);
		}
	}
}