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


}

void RyBot::update(const BotInput &input, BotOutput27 &output)
{
	//update the players current pos
	currPos = input.position;
	
	//black magic?
	output.spriteFrame = (output.spriteFrame + 1) % 2;
	

	

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

