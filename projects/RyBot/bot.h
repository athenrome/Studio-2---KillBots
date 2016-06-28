#ifndef BOT_H
#define BOT_H
#include "bot_interface.h"

#include <stdlib.h> 
#include <string>
#include <iostream>


#include "Opponent.h"
#include "RyMath.h"
#include "Waypoint.h"
#include "Path.h"

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

	void PlaceMapWaypoints();
	void ConnectMapWaypoints();
	void AssignWaypointCost();

	void GeneratePaths();
	void ChoosePath();

	void FindStartingPos();
	void FindDestPos();

	void UpdatePath();
	void ProcessMovement(const BotInput &input, BotOutput27 &output);
	void ProcessLooking(const BotInput &input, BotOutput27 &output);

	kf::Xor128 m_rand;

	BotInitialData matchData;
	BotAttributes botData;

	bool firstRun = true;

	std::vector<Opponent> opponents;
	Opponent currTarget;
	

	kf::Vector2 currPos;

	float lookAngle;
	float lookMoveDist = 1.5;
	float shootTarget;

	int shotQuota = 0;

	BotOutput27::Action lastAction;

	std::vector<WaypointRow> mapWaypoints;
	std::vector<Path> generatedPaths;
	Path chosenPath;
	int currPathLoc = 0;
	bool atWaypoint = false;
	bool returnJourney;

	Waypoint waypointTarget;
	float moveTargetDist;

	Waypoint startPoint;
	Waypoint destPoint;

	std::vector<Waypoint> ValidPoints;
	std::vector<Waypoint> ClosedPoints;
	
	bool CheckVector(Waypoint toFind, std::vector<Waypoint> toSearch)
	{
		bool found = false;

		for each (Waypoint point in toSearch)
		{
			if (point.pos == toFind.pos)
			{
				found = true;
				break;
			}
		}
		return found;
	}

};



#endif