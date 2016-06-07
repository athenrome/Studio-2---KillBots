#pragma once

#include "bot.h"

class Opponent
{
public:
	kf::Vector2 lastKnownPos;

	kf::Vector2 estPos;
	float estSpeed;

	Opponent();
	~Opponent();

private:

};