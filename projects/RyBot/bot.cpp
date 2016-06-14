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

	FirstRun();
}

void RyBot::update(const BotInput &input, BotOutput27 &output)
{
	//update the players current pos
	currPos = input.position;
	
	//black magic?
	output.spriteFrame = (output.spriteFrame + 1) % 2;
	

	//TARGETING

	CheckScanResult(input.scanResult);

	if (opponents.size() > 0 && visibleOpponents == true)
	{
		FindTarget();

		if (hasTarget && shotQuota <= 0)
		{
			std::cout << "targeting: " << currTarget.currPos << std::endl;
			shotQuota = 3;
		}
	}
	//END TARGETING



	if (shotQuota <= 0)
	{//look for target

	 //update look angle
		if (lastAction == BotOutput27::shoot)
		{//dont move
			UpdateLookDirection(0);
		}
		else
		{
			UpdateLookDirection(lookMoveDist);
		}
		

		//set new lookangle
		output.lookDirection.set(cos(lookAngle), sin(lookAngle));

		//search new location
		output.action = BotOutput27::scan;
		lastAction = BotOutput27::scan;

		std::cout << "scan" << std::endl;
		
	}
	else
	{//fire at target
		
		PredictTarget();
		GetAimDirection();

		output.lookDirection.set(cos(shootTarget), sin(shootTarget));
		
		//shoot bullet
		if (DistanceBetweenPoints(currPos, currTarget.currPos) < maxTargetDist)
		{
			output.action = BotOutput::shoot;
			lastAction = BotOutput::shoot;

			shotQuota--;
			std::cout << "shooting" << std::endl;
			if (shotQuota <= 0)
			{
				hasTarget = false;
			}
		}
		else
		{
			std::cout << "out of range" << std::endl;
		}
		

		

	}


	

	//MOVEMENT
	if (canMove == true)
	{
		if (hasTarget == true)
		{//move towards target

			float distToTarget = DistanceBetweenPoints(input.position, currTarget.currPos);

			if (distToTarget > 12)
			{//move closer
				output.moveDirection = currTarget.currPos - input.position;
				output.motor = botData.motor;
				//std::cout << "moving to Target" << std::endl;
			}
			else
			{//move back
				std::cout << "moving away from Target" << std::endl;
				output.moveDirection = kf::Vector2(currTarget.currPos.y - 1.5, currTarget.currPos.x - 1.5);
			}
		}
		else if (lastAction == BotOutput27::Action::scan && shotQuota <= 0)//move randomly
		{
			moveTarget.set(m_rand() % (matchData.mapData.width - 2) + 4.5, m_rand() % (matchData.mapData.height - 2) + 1.5);
			output.moveDirection = moveTarget - input.position;

			output.motor = botData.motor;
			//std::cout << "moving randomly" << std::endl;
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

