#pragma once

#include "framework.h"

struct ReadyEntityInfo;
struct Action;

Result strat_revive(IN ReadyEntityInfo &rei, OUT Action *action);