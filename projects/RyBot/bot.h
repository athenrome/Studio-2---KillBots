#ifndef BOT_H
#define BOT_H
#include "bot_interface.h"
#include "kf/kf_random.h"
#include "kf/kf_vector2.h"

#include "RyMath.h"

#ifdef BOT_EXPORTS
#define BOT_API __declspec(dllexport)
#else
#define BOT_API __declspec(dllimport)
#endif

class RyBot:public BotInterface27
{
public:
	RyBot();
	virtual ~RyBot();
	virtual void init(const BotInitialData &initialData, BotAttributes &attrib);
	virtual void update(const BotInput &input, BotOutput27 &output);
	virtual void result(bool won);
	virtual void bulletResult(bool hit);

	kf::Xor128 m_rand;

	BotInitialData matchData;
	
	BotAttributes botData;
};

kf::Vector2 ChooseMoveTarget(MapData mapData)
{
	float newX = RandomRange(-mapData.width, mapData.width);
	float newY = RandomRange(-mapData.height, mapData.height);

	kf::Vector2 newTarget = kf::Vector2(newX, newY);

	return newTarget;

	
}

#endif