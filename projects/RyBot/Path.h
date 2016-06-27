#pragma once

#include <vector>>
#include <Waypoint.h>

class Path
{
public:
	int jumpCount;
	int totalCost;
	bool reachedWaypoint;
	std::vector<Waypoint> pathWaypoints;

	Path(std::vector <Waypoint> _waypoints)
	{
		pathWaypoints = _waypoints;
		jumpCount = pathWaypoints.size();

		for each (Waypoint point in pathWaypoints)
		{
			totalCost += point.cost;
		}
	}
	Path();
	~Path();
};

