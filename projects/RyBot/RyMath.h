#pragma once

#include "kf/kf_random.h"
#include "kf/kf_vector2.h"

float DistanceBetweenPoints(kf::Vector2 currPos, kf::Vector2 targetPos)
{
	float distance = (currPos - targetPos).length();//  sqrtf((currPos.x * targetPos.x) + (currPos.y * targetPos.y));

	return distance;
}

int RandomRange(int min, int max)
{
	int newNum = rand() % (max - min) + min + 1;
	//std::cout << newNum << std::endl;
	return newNum;
}
