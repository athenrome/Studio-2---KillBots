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
	//output.moveDirection.set(m_rand.norm()*2.0-1.0, m_rand.norm()*2.0-1.0);
	
	//output.lookDirection.set(0,1);
	
	output.spriteFrame = (output.spriteFrame+1)%2;
	
	
	//TARGETING
	currPos = input.position;

	CheckScanResult(input.scanResult);
	
	if (lastScanTargets.size() > 0)
	{
		hasTarget = true;
		FindTarget();
	}
	else
	{
		hasTarget = false;
	}
	//END TARGETING

	//FIRING
	if (hasTarget == true)
	{
		output.action = BotOutput::shoot;

		output.lookDirection = currTarget.currPos - input.position;
		
		
		lookAngle = atan2(output.lookDirection.y, output.lookDirection.x);

		std::cout << "Shoot" << std::endl;
	}
	else
	{
		UpdateLookDirection(2);

		output.lookDirection.set(cos(lookAngle), sin(lookAngle));
		output.action = BotOutput27::scan;
		std::cout << "scan" << std::endl;
	}
	//END FIRING

	//MOVEMENT
	output.moveDirection = ChooseMoveTarget(matchData.mapData);
	output.motor = botData.motor;
	//END MOVEMENT

}



void RyBot::result(bool won)
{
}

void RyBot::bulletResult(bool hit)
{

}

