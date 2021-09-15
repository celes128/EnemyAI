#pragma once

#include "framework.h"

const int fTarSelf =				1 << 0;

const int fTarAlly =				1 << 1;
const int fTarOpponent =			1 << 2;

const int fTarEntity =				1 << 3;
const int fTarParty =				1 << 4;
const int fTarEverybody =			1 << 5;

const int fTarDefaultAlly =			1 << 6;
const int fTarDefaultOpponent =		1 << 7;

class TargetingData {
public:
	/*	
		USAGE EXAMPLE

		// - targets the caster only.
		TargetingData(fTarSelf);

		// - targets allies or opponents,
		// - targets only one entity (i.e. single target),
		// - targets opponents by default.
		TargetingData(fTarAlly | fTarOpponent | fTarEntity | fTarDefaultOpponent);

		// - targets allies or opponents,
		// - targets only parties,
		// - default target is an opponent.
		TargetingData(fTarAlly | fTarOpponent | fTarParty | fTarDefaultOpponent);
	*/
	TargetingData(u8 flags = fTarSelf) : m_flags(flags)
	{}

	/*
		USAGE EXAMPLE
		TargettingData tarData;
		// ...

		if (tarData.Flags(fTarSelf)) {
			// ... cast the spell on the caster
		}
		else if (tarData.Flags(fTarAlly)) {
			if (tarData.Flags(fTarEntity & fTarParty)) {
				// ... cast the spell either on an ally or the caster's party
			}
			else if (tarData.Flag(fTarEntity)) {
				// ... cast the spell on an ally
			}
			else if (tarData.Flag(fTarParty)) {
				// ... cast the spell on the caster's party
			}
		}
	*/
	auto Flags(int flag) const
	{
		return m_flags & flag;
	}

private:
	u8		m_flags;
};