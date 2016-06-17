#ifndef BOT_H
#define BOT_H
#include "bot_interface.h"

#include <stdlib.h> 
#include <string>
#include <iostream>


#include "Opponent.h"
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

	bool firstRun = true;

	std::vector<Opponent> opponents;
	Opponent currTarget;
	

	kf::Vector2 currPos;
	kf::Vector2 lastPos;
	kf::Vector2 distTraveled;

	kf::Vector2 moveTarget;
	

	
	float maxTargetDist = 1000000;
	bool hasTarget = false;

	float lookAngle;
	float lookMoveDist = 1.5;
	float shootTarget;

	int shotQuota = 0;

	bool canMove = true;

	bool visibleOpponents = false;

	BotOutput27::Action lastAction;

	
};

#endif