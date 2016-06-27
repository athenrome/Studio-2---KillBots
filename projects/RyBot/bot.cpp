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
	//DONT REMOVE
	output.spriteFrame = (output.spriteFrame + 1) % 2;

	//update the players current pos
	currPos = input.position;


	if (firstRun == true)
	{
		std::cout << currPos << std::endl;
		FindStartingPos();
		GeneratePaths();
		ChoosePath();
		firstRun = false;
	}

	

	ManagePath();

	ProcessLooking(input, output);
	ProcessMovement(input, output);


}

void RyBot::ProcessLooking(const BotInput &input, BotOutput27 &output)
{
	output.lookDirection = waypointTarget.pos - input.position;

	output.moveDirection = waypointTarget.pos - input.position;
}

void RyBot::ProcessMovement(const BotInput &input, BotOutput27 &output)
{
	moveTargetDist = DistanceBetweenPoints(currPos, waypointTarget.pos);
	if (moveTargetDist < 10)//set move speed
	{
		output.motor = -botData.motor;//slow to stop
	}
	else if (moveTargetDist < 5)
	{
		output.motor = 0;//stop moving
	}
	else
	{
		output.motor = botData.motor; //MAX SPEED!!!!!!!!!!!!!!!!!!
	}

	std::cout << "Moving to Target: " << waypointTarget.pos << " Distance to Move Target: " << moveTargetDist << std::endl;
}

void RyBot::ManagePath()
{
	waypointTarget = chosenPath.pathWaypoints[0];
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
	int assignedCosts = 0;
	int adjWalls = 0;
	int distanceToDest = 0;

	for each (WaypointRow row in mapWaypoints)//index through all waypoints
	{
		for each (Waypoint waypoint in row.rowWaypoints)
		{
			//reset values
			adjWalls = 0;
			distanceToDest = 0;

			//get cost values 
			distanceToDest = round(DistanceBetweenPoints(waypoint.pos, destPoint.pos));

			for each (Waypoint point in waypoint.adjWaypoints)
			{
				if (point.isWall == true)
				{
					adjWalls++;
				}
			}
			
			//process cost values
			if (adjWalls >= 3)//if tile is surrounded
			{
				adjWalls += 10000;
			}

			//create cost
			waypoint.cost = (adjWalls + distanceToDest);

			assignedCosts++;
		}
	}
	std::cout << "Assigned costs to: " << assignedCosts << " points" << std::endl;
}

void RyBot::GeneratePaths()
{
	generatedPaths.clear();

	int pathsToCreate = 12;
	
	int maxPathLen = 32;
	int pathLen = 0;

	bool pathFound = false;

	Waypoint lastPoint = startPoint;

	for (int i = 0; i < pathsToCreate; i++)
	{
		std::vector<Waypoint> pathPoints;
		pathPoints.clear();
		pathPoints.push_back(startPoint);//set startpoint as the first element in path

		int currCost = 100000000;

		while (pathFound == false)
		{
			for each (Waypoint point in startPoint.adjWaypoints)
			{
				if (point.cost < currCost)
				{
					lastPoint = point;
					pathPoints.push_back(point);
				}

				if (point.isDest == true)
				{
					lastPoint = point;
					pathFound = true;
					break;
				}
			}

			pathLen++;

			if (pathLen >= maxPathLen)
			{
				pathFound = true;
			}
		}

		generatedPaths.push_back(pathPoints);
	}
	std::cout << "Created " << generatedPaths.size() << " Paths" << std::endl;
}

void RyBot::ChoosePath()
{
	int shortestDist = 10000000; //set initial value high

	for each (Path path in generatedPaths)
	{
		if (path.jumpCount < shortestDist)
		{
			shortestDist = path.jumpCount;
			chosenPath = path;
		}
	}
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
	WaypointRow destRow = mapWaypoints[mapWaypoints.size()];
	destPoint = destRow.rowWaypoints[destRow.rowWaypoints.size()];
	destPoint.isDest = true;
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

