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
	attrib.health = 1;
	attrib.motor = 1;
	attrib.weaponSpeed = 1;
	attrib.weaponStrength = 1;

	matchData = initialData;
	botData = attrib;

	
	firstRun = true;
	currPathLoc = 0;
}

void RyBot::update(const BotInput &input, BotOutput27 &output)
{
	//DONT REMOVE
	output.spriteFrame = (output.spriteFrame + 1) % 2;

	//update the players current pos
	currPos = input.position;


	if (firstRun == true)
	{
		PlaceMapWaypoints();
		ConnectMapWaypoints();
		
		FindStartingPos();
		FindDestPos();

		//A* stuff
		AssignWaypointCost();
		GeneratePaths();
		ChoosePath();

		firstRun = false;

		waypointTarget = chosenPath.pathWaypoints[0];//targetWaypoint(kf::Vector2(32, 32), 1, false);



		


	}

	output.lines.clear();

	Waypoint* pastPoint = startPoint;

	for each (Waypoint* point in chosenPath.pathWaypoints)
	{
		Line l;
		l.start = pastPoint->pos;
		l.end = point->pos;
		l.r = 1;
		l.g = 1;
		l.b = 0;
		output.lines.push_back(l);

		pastPoint = point;
	}

	if (DistanceBetweenPoints(input.position, waypointTarget->pos) < 1)
	{
		std::cout << "At waypoint: " << currPathLoc << " At location: "  << waypointTarget->pos << std::endl;
		atWaypoint = true;

		UpdatePath();
	}
	else
	{
		ProcessLooking(input, output);
		ProcessMovement(input, output);
	}
}

void RyBot::ProcessLooking(const BotInput &input, BotOutput27 &output)
{
	output.lookDirection = waypointTarget->pos - input.position;

	
}

void RyBot::ProcessMovement(const BotInput &input, BotOutput27 &output)
{
	moveTargetDist = DistanceBetweenPoints(currPos, waypointTarget->pos);

	output.moveDirection = waypointTarget->pos - input.position;

	if (moveTargetDist < 2)//set move speed
	{
		output.motor = -botData.motor;//slow to stop
	}
	else if (moveTargetDist < 1)
	{
		output.motor = 0;//stop moving
	}
	else
	{
		output.motor = botData.motor; //MAX SPEED!!!!!!!!!!!!!!!!!!
	}



	std::cout << "Moving to Target: " << waypointTarget->pos << " Distance to Move Target: " << moveTargetDist << std::endl;
}

void RyBot::UpdatePath()
{
	if (atWaypoint == true)//check if its time for a new waypoint
	{

		if (returnJourney == true && currPathLoc <= 1)
		{
			returnJourney = false;
		}
		
		if (currPathLoc == chosenPath.jumpCount - 1)
		{
			returnJourney = true;
		}

		if (returnJourney == true)
		{
			currPathLoc--;
		}
		else
		{
			currPathLoc++;
		}

		

		
		atWaypoint = false;

		

		waypointTarget = chosenPath.pathWaypoints[currPathLoc];
		std::cout << "Next Waypoint: " << currPathLoc << " Move to: " << waypointTarget->pos << std::endl;
		
		
	}
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
		if (matchData.mapData.data[ currX + currY *matchData.mapData.width].wall == true)
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
		if (currX == matchData.mapData.width)
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
		if (currY == matchData.mapData.height)
		{
			finished = true;
			

			std::cout << "Finished " << waypointsPlaced << " Waypoints Placed" << std::endl;
		}
	}

	
}

void RyBot::ConnectMapWaypoints()
{
	int connectedPoints = 0;

	//for (WaypointRow &row:mapWaypoints)//index through all waypoints
	//{
	//	for(Waypoint &point:row.rowWaypoints)
	//	{

	Waypoint* newAdj;

	for (int y = 0; y < mapWaypoints.size();++y)
	{
		for (int x = 0; x < mapWaypoints[y].rowWaypoints.size(); ++x)
		{

			WaypointRow &row = mapWaypoints[y];
			Waypoint &point = mapWaypoints[y].rowWaypoints[x];

			if (point.isWall != true)
			{
				if (row.rowID > 0)//above
				{
					//newAdj = mapWaypoints[row.rowID - 1].rowWaypoints[point.rowPos];

					newAdj = &mapWaypoints[row.rowID - 1].rowWaypoints[point.rowPos];

					if (newAdj->isWall == false)
					{
						point.adjWaypoints.push_back(newAdj);
					}

					
				}

				if (point.rowPos > 0)//left
				{

					newAdj = &row.rowWaypoints[point.rowPos - 1];

					if (newAdj->isWall == false)
					{
						point.adjWaypoints.push_back(newAdj);
					}
				}

				if (point.rowPos < matchData.mapData.width - 1)//right
				{
					newAdj = &mapWaypoints[row.rowID - 1].rowWaypoints[point.rowPos + 1];

					if (newAdj->isWall == false)
					{
						point.adjWaypoints.push_back(newAdj);
					}

				}

				if (row.rowID < matchData.mapData.height - 1)//below
				{
					newAdj = &mapWaypoints[row.rowID + 1].rowWaypoints[point.rowPos];

					if (newAdj->isWall == false)
					{
						point.adjWaypoints.push_back(newAdj);
					}

				}

				connectedPoints++;
			}
			
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
			
			distanceToDest = round(DistanceBetweenPoints(waypoint.pos, destPoint->pos));

			

			for each (Waypoint *point in waypoint.adjWaypoints)
			{
				if (point->isWall == true)
				{
					adjWalls++;
				}
			}
			
			//process cost values
			if (adjWalls >= 3)//if tile is surrounded
			{
				adjWalls += 10000;
			}

			if (waypoint.isWall == true)
			{
				adjWalls += 99999;
			}

			//create cost
			waypoint.cost = (adjWalls + distanceToDest);

			assignedCosts++;
			//std::cout << "Assigned costs to waypoint " << assignedCosts << " with a cost of: " << waypoint.cost <<  std::endl;
		}
	}
	std::cout << "Assigned costs to: " << assignedCosts << " points" << std::endl;
}

void RyBot::GeneratePaths()
{
	generatedPaths.clear();

	int pathsToCreate = 12;
	
	int maxPathLen = 128;
	int pathLen = 0;

	bool pathComplete = false;



	std::vector<Waypoint*> pathPoints;

	pathPoints.clear();
	pathPoints.push_back(startPoint);//set start point as the first point in path

	int adjPointsNum = 0;

	int bestCost = 1000000;

	Waypoint* lastPoint = startPoint;
	Waypoint* currPoint;


	while (pathComplete == false)
	{
		bestCost = 1000000;

		for each (Waypoint *adjPoint in lastPoint->adjWaypoints)//sort through adj points
		{
			if (adjPoint->isValid == true)
			{
				if (adjPoint->cost <= bestCost)
				{
					bestCost = adjPoint->cost;
					currPoint = adjPoint;
					adjPoint->isValid = false;//lock this point
				}
			}

		}

		pathPoints.push_back(currPoint);//add to the path
		lastPoint = currPoint; // prep for next cycle

		if (pathPoints.size() >= maxPathLen)
		{
			pathComplete = true;
			chosenPath = Path(pathPoints, true);
		}


	}

	generatedPaths.push_back(Path(pathPoints, CheckVector(destPoint, pathPoints)));
	std::cout << "Created " << generatedPaths.size() << " Paths" << std::endl;

}

void RyBot::ChoosePath()
{

	chosenPath = generatedPaths[0];

	

	std::cout << "Found path:  " << chosenPath.jumpCount << " jump length" << std::endl;
}

void RyBot::FindStartingPos()
{
	Waypoint closestWaypoint;

	float closestDist = 1000000;

	//round the curr pos to whole numbers
	int roundX = roundf(currPos.x);
	int roundY = roundf(currPos.y);
	currPos = kf::Vector2(roundX, roundY);

	startPoint = &mapWaypoints[roundY].rowWaypoints[roundX];

	if (startPoint->isWall == true)
	{
		startPoint = Waypoint()
	}

	//for each (WaypointRow row in mapWaypoints)
	//{
	//	for each (Waypoint point in row.rowWaypoints)
	//	{
	//		if (point.pos == currPos)
	//		{
	//			closestWaypoint = point;
	//		}
	//	}
	//}

	//startPoint = closestWaypoint;

	std::cout << "Starting Point at: " << startPoint->pos << std::endl;
	
}

void RyBot::FindDestPos()
{
	
	//destPoint = mapWaypoints[matchData.mapData.height].rowWaypoints[matchData.mapData.width];//choose the bottom right corner as the destination point
	destPoint = &mapWaypoints[29].rowWaypoints[29];//choose the bottom right corner as the destination point

	std::cout << "Destination at: " << destPoint->pos << std::endl;
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

