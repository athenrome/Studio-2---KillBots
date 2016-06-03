#ifndef BOT_H
#define BOT_H
#include "bot_interface.h"
#include "kf/kf_random.h"

#ifdef BOT_EXPORTS
#define BOT_API __declspec(dllexport)
#else
#define BOT_API __declspec(dllimport)
#endif

class Neuron
{
public:
	std::vector<float> weights;
};

class Layer
{
public:
	std::vector<Neuron> neurons;
};

class Network
{
public:
	std::vector<Layer> layers;
};

class Blank:public BotInterface27
{
public:
	Blank();
	virtual ~Blank();
	virtual void init(const BotInitialData &initialData, BotAttributes &attrib);
	virtual void update(const BotInput &input, BotOutput27 &output);
	virtual void result(bool won);
	virtual void bulletResult(bool hit);

	Network net;

	kf::Xor128 m_rand;
	BotInitialData m_initialData;
};


#endif