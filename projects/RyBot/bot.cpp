#include "bot.h"
#include "time.h"



extern "C"
{
	BotInterface27 BOT_API *CreateBot27()
	{
		return new RyBot();
	}
}

RyBot::RyBot()
{
	m_rand(time(0)+(int)this);
}

RyBot::~RyBot()
{
}

void RyBot::init(const BotInitialData &initialData, BotAttributes &attrib)
{
	attrib.health = 2;
	attrib.motor = 1;
	attrib.weaponSpeed = 3;
	attrib.weaponStrength = 2;

	matchData = initialData;
	botData = attrib;

	PlaceMapWaypoints();
	ConnectMapWaypoints();
	AssignWaypointCost();
	
}

void RyBot::update(const BotInput &input, BotOutput27 &output)
{
	//update the players current pos
	currPos = input.position;


	if (firstRun == true)
	{
		std::cout << currPos << std::endl;
		FindStartingPos();
		GeneratePath();
		firstRun = false;
	}

	moveTargetDist = DistanceBetweenPoints(currPos, waypointTarget.pos);

	//DONT REMOVE
	output.spriteFrame = (output.spriteFrame + 1) % 2;

	
	output.lookDirection = waypointTarget.pos - input.position;
	
	output.moveDirection = waypointTarget.pos - input.position;

	if (moveTargetDist < 10)//set move speed
	{
		output.motor = -botData.motor;//slow to stop
	}
	else
	{
		output.motor = botData.motor; //MAX SPEED!!!!!!!!!!!!!!!!!!
	}

	

	std::cout << "Moving to Target: " << waypointTarget.pos << " Distance to Move Target: " << moveTargetDist << std::endl;


}

void RyBot::PlaceMapWaypoints()
{
	int currX = 0;
	int currY = 0;

	bool finished = false;

	int waypointsPlaced = 0;

	mapWaypoints.push_back(WaypointRow(currY));

	while (finished == false)
	{
		//check if wall or empty, True = wall False empty
		if (matchData.mapData.data[(currX + currY)*matchData.mapData.width].wall == true)
		{
			mapWaypoints[currY].rowWaypoints.push_back(Waypoint(kf::Vector2(currX, currY), currX, true));
			waypointsPlaced++;
		}
		else
		{
			mapWaypoints[currY].rowWaypoints.push_back(Waypoint(kf::Vector2(currX, currY), currX, false));
			waypointsPlaced++;
		}

		//move check coordinates
		if (currX > matchData.mapData.width)
		{//reset check row position
			currX = 0;
			currY++;


			mapWaypoints.push_back(WaypointRow(currY));
		}
		else
		{//move across row
			currX++;
		}

		//if there are no more rows
		if (currY > matchData.mapData.height)
		{
			finished = true;
			

			std::cout << "Finished " << waypointsPlaced << " Waypoints Placed" << std::endl;
		}
	}

	
}

void RyBot::ConnectMapWaypoints()
{
	int connectedPoints = 0;

	for each (WaypointRow row in mapWaypoints)//index through all waypoints
	{
		for each (Waypoint point in row.rowWaypoints)
		{
			if (row.rowID > 0)//above
			{
				point.adjWaypoints.push_back(mapWaypoints[row.rowID - 1].rowWaypoints[point.rowPos]);
			}

			if (point.rowPos > 0)//left
			{
				point.adjWaypoints.push_back(row.rowWaypoints[point.rowPos - 1]);
			}

			if (point.rowPos < matchData.mapData.width)//right
			{
				point.adjWaypoints.push_back(row.rowWaypoints[point.rowPos + 1]);
			}

			if (row.rowID < matchData.mapData.height)//below
			{
				point.adjWaypoints.push_back(mapWaypoints[row.rowID + 1].rowWaypoints[point.rowPos]);
			}

			connectedPoints++;
		}
	} 

	std::cout << "Connected: " << connectedPoints << " points" << std::endl;

}

void RyBot::AssignWaypointCost()
{
	for each (WaypointRow row in mapWaypoints)//index through all waypoints
	{
		for each (Waypoint waypoint in row.rowWaypoints)
		{
			for each (Waypoint point in waypoint.adjWaypoints)
			{

			}
		}
	}
}

void RyBot::GeneratePath()
{
	botPath.clear();



	waypointTarget = startPoint;

	//std::cout << startPoint.pos << std::endl;
}

void RyBot::FindStartingPos()
{
	Waypoint closestWaypoint;

	float closestDist = 1000000;

	//round the curr pos to whole numbers
	int roundX = roundf(currPos.x);
	int roundY = roundf(currPos.y);
	currPos = kf::Vector2(roundX, roundY);

	for each (WaypointRow row in mapWaypoints)
	{
		for each (Waypoint point in row.rowWaypoints)
		{
			/*float pointDist = DistanceBetweenPoints(currPos, point.pos);

			if (pointDist < closestDist)
			{
				closestDist = pointDist;

				closestWaypoint = point;
			}*/

			if (point.pos == currPos )
			{
				closestWaypoint = point;
			}
		}
	}

	startPoint = closestWaypoint;

	std::cout << "Closest Waypoint at: " << startPoint.pos << std::endl;
	
}

void RyBot::FindDestPos()
{
	destPoint = mapWaypoints[mapWaypoints.end];
}


void RyBot::result(bool won)
{
}

void RyBot::bulletResult(bool hit)
{
	//result from the last bullet hit
	if (hit == true)
	{
		shotQuota++;
		std::cout << "Bullet Hit" << std::endl;
	}
}

