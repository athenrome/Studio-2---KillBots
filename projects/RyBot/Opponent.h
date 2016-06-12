#pragma once

#include "kf/kf_vector2.h"


class Opponent
{
public:
	std::string name;
	kf::Vector2 lastKnownPos;
	kf::Vector2 currPos;

	kf::Vector2 estPos;
	float estSpeed;

	Opponent(std::string _name)
	{
		name = _name;
	}

	Opponent();
	~Opponent();



};


