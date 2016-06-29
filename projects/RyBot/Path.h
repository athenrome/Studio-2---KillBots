#pragma once

#include <vector>>
#include <Waypoint.h>

class Path
{
public:
	int jumpCount;
	int totalCost;
	bool validPath;
	std::vector<Waypoint*> pathWaypoints;

	Path(std::vector <Waypoint*> _waypoints, bool _valid)
	{
		validPath = _valid;
		pathWaypoints = _waypoints;
		jumpCount = pathWaypoints.size();

		for each (Waypoint* point in pathWaypoints)
		{
			totalCost += point->cost;
		}
	}
	Path();
	~Path();
};

