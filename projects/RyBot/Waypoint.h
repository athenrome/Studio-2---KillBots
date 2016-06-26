#pragma once

#include "kf/kf_vector2.h"
#include <vector>
#include <iostream>


class Waypoint
{
public:
	int rowPos;
	kf::Vector2 pos;
	
	std::vector<Waypoint> adjWaypoints;

	float cost;

	bool isWall;

	bool completed;
	bool inPath;

	//bool topAccessible = false;
	//bool botAccessible = false;
	//bool leftAccessible = false;
	//bool rightAccessible = false;

	Waypoint(kf::Vector2 _pos, int _rowPos, bool _isWall)
	{
		rowPos = _rowPos;
		pos = _pos;
		isWall = _isWall;
		inPath = false;

		if (isWall == true)
		{
			std::cout << "Created waypoint at: " << _pos << " as Wall" << std::endl;
		}
		else
		{
			std::cout << "Created waypoint at: " << _pos << " as Empty" << std::endl;
		}
	}

	Waypoint();

	~Waypoint();

	void SetWaypointCost(Waypoint lastWaypoint)
	{

	}
};

class WaypointRow
{
public:
	int rowID;

	std::vector<Waypoint> rowWaypoints;

	WaypointRow(int _ID)
	{
		rowID = _ID;
	}
	WaypointRow();
	~WaypointRow();

private:

};

