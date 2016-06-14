#ifndef BOT_H
#define BOT_H
#include "bot_interface.h"

#include <stdlib.h> 
#include <string>
#include <iostream>


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

	bool firstRun = true;

	std::vector<Opponent> opponents;
	Opponent currTarget;
	

	kf::Vector2 currPos;
	kf::Vector2 lastPos;
	kf::Vector2 distTraveled;

	kf::Vector2 moveTarget;
	

	
	float maxTargetDist = 1000000;
	bool hasTarget = false;

	float lookAngle;
	float lookMoveDist = 1.5;
	float shootTarget;

	int shotQuota = 0;

	bool canMove = true;

	bool visibleOpponents = false;

	BotOutput27::Action lastAction;

	

	void FirstRun()
	{//initilization

		opponents.push_back(Opponent("blank", kf::Vector2(0, 0)));

		currTarget = opponents[0];

		firstRun = false;
		std::cout << "Bot Start" << std::endl;
	}

	void FindTarget()
	{
		hasTarget = false;
		float closestDistance = 10000;

		currTarget = opponents[0]; // default blank target
		

		for each (Opponent opp in opponents)
		{
			float newDistance = DistanceBetweenPoints(currPos, opp.currPos);
			if (newDistance < closestDistance)
			{
				closestDistance = newDistance;

				currTarget = opp;
				hasTarget = true;

				std::cout << "Found New Target" << std::endl;
			}
			
		}		

		
		
	}


	void CheckScanResult(std::vector<VisibleThing> scanResult)
	{
		visibleOpponents = false;
		hasTarget = false;

		for each (VisibleThing currThing in scanResult)
		{
			bool updatedOpp = false;

			if (currThing.type == 0)//0 = robot, 1 = bullet
			{
				for each (Opponent currOpp in opponents)
				{
					if (currThing.name == currOpp.name)
					{
						currOpp.Spotted(currThing.position);

						updatedOpp = true;
						visibleOpponents = true;						
					}
				}

				if (updatedOpp == false)//havent been able to find opponent
				{
					Opponent newTarget = Opponent(currThing.name, currThing.position);
					currTarget = newTarget;
					hasTarget = true;
					opponents.push_back(newTarget); //created a new opponent
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

	void PredictTarget()
	{
		
	}

	void GetAimDirection()
	{
		float result;

		shootTarget = lookAngle;//just for testing
	}

	void UpdateLookDirection(float toAdjust)
	{
		lookAngle += matchData.scanFOV * toAdjust;
		
	}


};

#endif