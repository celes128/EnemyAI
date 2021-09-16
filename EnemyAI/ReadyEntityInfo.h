#pragma once

class Entity;
class Party;

struct ReadyEntityInfo {
	Entity		&entity;
	Party		&allies;
	Party		&opponents;
};
