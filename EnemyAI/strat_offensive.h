#pragma once

#include "framework.h"

struct ReadyEntityInfo;
struct Action;

Result strat_offensive(IN ReadyEntityInfo &rei, OUT Action *action);