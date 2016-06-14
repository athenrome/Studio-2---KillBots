#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "kf/kf_vector2.h"


class Opponent
{
public:
	std::string name;
	std::vector <kf::Vector2> trackedPositions; //most recent at the end most common at the front
	kf::Vector2 lastKnownPos;
	kf::Vector2 currPos;

	float lastSightingTime = 0; //frames since it was last seen
	float trackingTimeout = 20; //frames since it was addeded to the recent positions

	kf::Vector2 estPos;
	float estSpeed;

	Opponent(std::string _name, kf::Vector2 &_pos)
	{
		name = _name;
		currPos = _pos;
		lastKnownPos = _pos;

		trackedPositions.push_back(currPos);
		std::cout << "created new opponent " << name << std::endl;
	}

	Opponent();
	virtual ~Opponent();

	void Spotted(kf::Vector2 &spottedLocation)
	{

		lastKnownPos = trackedPositions[trackedPositions.size()];//move currposition to last known position

		trackedPositions.push_back(spottedLocation);//insert at last postion
		
		currPos = trackedPositions[trackedPositions.size()];//makes the last position the current location

		lastSightingTime = 0; //resets timers
		trackingTimeout = 20;
		
	}

	


	void UpdateOpponent()
	{
		lastSightingTime += 1;
		trackingTimeout -= 1;
		
		if (trackedPositions.size() > 0)//if it isnt empty
		{
			if (lastSightingTime > trackingTimeout)
			{//remove oldest tracked position
				trackedPositions.erase(trackedPositions.begin());//first postion
			}
		}

		
	}
};


