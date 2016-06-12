#ifndef BOT_H
#define BOT_H
#include "bot_interface.h"

#include <stdlib.h> 

#include <iostream>
#include "kf/kf_random.h"
#include "kf/kf_vector2.h"

#include "Opponent.h"
#include "RyMath.h"

#ifdef BOT_EXPORTS
#define BOT_API __declspec(dllexport)
#else
#define BOT_API __declspec(dllimport)
#endif


class RyBot:public BotInterface27
{
public:
	RyBot();
	virtual ~RyBot();
	virtual void init(const BotInitialData &initialData, BotAttributes &attrib);
	virtual void update(const BotInput &input, BotOutput27 &output);
	virtual void result(bool won);
	virtual void bulletResult(bool hit);

	kf::Xor128 m_rand;

	BotInitialData matchData;
	BotAttributes botData;

	std::vector<Opponent> opponents;
	std::vector<Opponent> lastScanTargets;

	kf::Vector2 currPos;

	Opponent currTarget;
	bool hasTarget;

	float lookAngle;


	void FindTarget()
	{
		hasTarget = false;
		float closestDistance = 10000;
		Opponent newTarget;

		for each (Opponent opp in opponents)
		{
			float newDistance = DistanceToTarget(currPos, opp.lastKnownPos);
			if (newDistance < closestDistance)
			{
				closestDistance = newDistance;
				newTarget = opp;
			}
		}

		currTarget = newTarget;
		hasTarget = true;
	}


	void CheckScanResult(std::vector<VisibleThing> scanResult)
	{
		lastScanTargets.clear();

		for each (VisibleThing currThing in scanResult)
		{
			bool updatedOpp = false;

			if (currThing.type == 0)//0 = robot, 1 = bullet
			{
				

				for each (Opponent currOpp in opponents)
				{
					if (currThing.name == currOpp.name)
					{
						currOpp.lastKnownPos = currOpp.currPos; //turns previos curr pos into last known pos
						currOpp.currPos = currThing.position; 

						updatedOpp = true;

						lastScanTargets.push_back(currOpp);
					}
				}

				if (updatedOpp == false)//havent been able to find opponent
				{
					opponents.push_back(Opponent(currThing.name)); //created a new opponent
				}
			}
		}
	}

	kf::Vector2 ChooseMoveTarget(MapData mapData)
	{
		float newX = RandomRange(-mapData.width, mapData.width);
		float newY = RandomRange(-mapData.height, mapData.height);

		kf::Vector2 newTarget = kf::Vector2(newX, newY);

		return newTarget;
	}

	void UpdateLookDirection(float toAdjust)
	{
		lookAngle += matchData.scanFOV * toAdjust;
		
	}


};

#endif