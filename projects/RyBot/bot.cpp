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
	currPos = input.position;

	output.spriteFrame = (output.spriteFrame + 1) % 2;
	

	//TARGETING
	CheckScanResult(input.scanResult);

	if (lastScanTargets.size() > 0)
	{
		FindTarget();

		if (hasTarget && shotQuota <= 0)
		{
			shotQuota = 3;
		}
	}
	else
	{
		hasTarget = false;
		std::cout << "No Target" << std::endl;
	}
	//END TARGETING



	if (hasTarget == true)
	{//Fire at target
		shotQuota--;

		//look at target
		output.lookDirection = currTarget.currPos - input.position;

		//shoot bullet
		output.action = BotOutput::shoot;
		lastAction = BotOutput::shoot;
	}
	else
	{

	}

	//LOOKING
	if (hasTarget == false)
	{//search for target
		
		lookAngle += matchData.scanFOV*lookMoveDist;
		output.lookDirection.set(cos(lookAngle), sin(lookAngle));
		output.action = BotOutput27::scan;
		

		std::cout << "scan" << std::endl;



	}
	else
	{//look at target

		output.lookDirection = currTarget.lastKnownPos - input.position;

		output.lookDirection = lookAngle - input.position;
				

		std::cout << "Target Observe" << std::endl;
	}
	//END LOOKING



	//MOVEMENT
	if (hasTarget == true)
	{//move towards target

		float distToTarget = DistanceToTarget(input.position, currTarget.currPos);

		if (distToTarget > 50)
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
	else
	{ //move randomly
		
		moveTarget.set(m_rand() % (matchData.mapData.width - 2) + 1.5, m_rand() % (matchData.mapData.height - 2) + 1.5);
		output.moveDirection = moveTarget - input.position;

		output.moveDirection = moveTarget - input.position;
		output.motor = botData.motor;
		std::cout << "moving randomly" << std::endl;

	}
	//END MOVEMENT

}



void RyBot::result(bool won)
{
}

void RyBot::bulletResult(bool hit)
{

}

