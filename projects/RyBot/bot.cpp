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
	attrib.health=1.0;
	attrib.motor=1.0;
	attrib.weaponSpeed=1.0;
	attrib.weaponStrength=1.0;

	matchData = initialData;
	botData = attrib;
}

void RyBot::update(const BotInput &input, BotOutput27 &output)
{
	//update the players current pos
	currPos = input.position;
	
	//black magic?
	output.spriteFrame = (output.spriteFrame + 1) % 2;
	

	//TARGETING
	CheckScanResult(input.scanResult);

	if (lastScanTargets.size() > 0)
	{
		FindTarget();

		if (hasTarget && shotQuota <= 0)
		{
			std::cout << "found target" << std::endl;
			shotQuota = 3;
		}
	}
	//END TARGETING



	if (hasTarget == false && shotQuota <= 0)
	{//Fire at target

	 //update look angle
		UpdateLookDirection(lookMoveDist);

		//set new lookangle
		output.lookDirection.set(cos(lookAngle), sin(lookAngle));

		//search new location
		output.action = BotOutput27::scan;
		lastAction = BotOutput27::scan;

		std::cout << "scan" << std::endl;
		
	}
	else
	{//search for target
		
		//shoot bullet
		output.action = BotOutput::shoot;
		lastAction = BotOutput::shoot;
		shotQuota--;

		if (shotQuota <= 0)
		{
			hasTarget = false;
		}

		std::cout << "shooting" << std::endl;

	}


	

	//MOVEMENT
	if (canMove == true)
	{
		if (hasTarget == true)
		{//move towards target

			float distToTarget = DistanceToTarget(input.position, currTarget.currPos);

			if (distToTarget > 12)
			{//move closer
				output.moveDirection = currTarget.currPos - input.position;
				output.motor = botData.motor;
				std::cout << "moving to Target" << std::endl;
			}
			else
			{//move back
				std::cout << "moving away from Target" << std::endl;
			}
		}
		else if (lastAction == BotOutput27::Action::scan && shotQuota <= 0)
		{
			moveTarget.set(m_rand() % (matchData.mapData.width - 2) + 1.5, m_rand() % (matchData.mapData.height - 2) + 1.5);
			output.moveDirection = moveTarget - input.position;

			output.motor = botData.motor;
			std::cout << "moving randomly" << std::endl;
		}
		else
		{
			std::cout << "standing still" << std::endl;
		}
	}
	//END MOVEMENT



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

