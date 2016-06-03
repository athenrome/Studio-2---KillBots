#include "bot.h"
#include "time.h"

extern "C"
{
	BotInterface BOT_API *CreateBot()
	{
		return new Robot();
	}
}

Robot::Robot()
{
	m_rand(time(0) + (int)this);
}

Robot::~Robot()
{
}

void Robot::init(const BotInitialData &initialData, BotAttributes &attrib)
{
	m_initdata = initialData;
	attrib.health         = 1.0;
	attrib.motor          = 1.0;
	attrib.weaponSpeed    = 3.0;
	attrib.weaponStrength = 2.0;
	m_moveTarget.set(m_rand() % (initialData.mapData.width - 2) + 1.5, m_rand() % (initialData.mapData.height - 2) + 1.5);
	m_burst = 0;
}

void Robot::update(const BotInput &input, BotOutput &output)
{
	output.lines.clear();
	output.moveDirection = m_moveTarget - input.position;
	output.motor = 1.0;
	if (output.moveDirection.length() < 0.5)
	{
		m_moveTarget.set(m_rand() % (m_initdata.mapData.width - 2) + 1.5, m_rand() % (m_initdata.mapData.height - 2) + 1.5);
		output.moveDirection = m_moveTarget - input.position;
	}

	Line l;
	l.start = input.position;
	l.end = m_moveTarget;
	l.r = 1;
	l.g = 1;
	l.b = 0;
	output.lines.push_back(l);

	if (m_burst > 0)
	{
		m_burst--;
		output.lookDirection = m_lastKnownPos - input.position;
		output.action = BotOutput::shoot;
	}
	else
	{
		bool found = false;
		for (int i = 0; i < input.scanResult.size(); ++i)
		{
			if (input.scanResult[i].type == VisibleThing::e_robot)
			{
				found = true;
				m_lastKnownPos = input.scanResult[i].position;

				break;
			}
		}
		if (found)
		{
			output.lookDirection = m_lastKnownPos - input.position;
			m_burst = 2;
			output.action = BotOutput::shoot;
			m_lookAngle = atan2(output.lookDirection.y, output.lookDirection.x);
		}
		else
		{
			output.lookDirection.set(cos(m_lookAngle), sin(m_lookAngle));
			output.action = BotOutput::scan;
			m_lookAngle += m_initdata.scanFOV*2.0;
		}
	}

	//How to check a tile
	//m_initdata.mapData.data[int(input.position.x)+int(input.position.y)*m_initdata.mapData.width].wall
	
}

void Robot::result(bool won)
{
}
