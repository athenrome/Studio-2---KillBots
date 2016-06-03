#include "bot.h"
#include "time.h"

extern "C"
{
	BotInterface BOT_API *CreateBot()
	{
		return new Blank();
	}
}

Blank::Blank()
{
	m_rand(time(0)+(int)this);
}

Blank::~Blank()
{
}

void Blank::init(const BotInitialData &initialData, BotAttributes &attrib)
{
	m_initialData = initialData;
	attrib.health=1.0;
	attrib.motor=1.0;
	attrib.weaponSpeed=1.0;
	attrib.weaponStrength=1.0;
}

void Blank::update(const BotInput &input, BotOutput &output)
{
	output.moveDirection.set(m_rand.norm()*2.0-1.0, m_rand.norm()*2.0-1.0);
	output.motor = 0.0;
	output.lookDirection.set(0,1);
	output.action = BotOutput::scan;
}

void Blank::result(bool won)
{
}
