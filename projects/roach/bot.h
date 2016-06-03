#ifndef BOT_H
#define BOT_H
#include "bot_interface.h"
#include "kf/kf_random.h"

#ifdef BOT_EXPORTS
#define BOT_API __declspec(dllexport)
#else
#define BOT_API __declspec(dllimport)
#endif

class Robot :public BotInterface
{
public:
	Robot();
	virtual ~Robot();
	virtual void init(const BotInitialData &initialData, BotAttributes &attrib);
	virtual void update(const BotInput &input, BotOutput &output);
	virtual void result(bool won);

	kf::Xor128 m_rand;

	BotInitialData m_initdata;

	float m_lookAngle;
	kf::Vector2 m_lastKnownPos;
	kf::Vector2 m_moveTarget;
	int m_burst;
};


#endif