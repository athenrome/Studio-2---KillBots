#include "bot.h"
#include "time.h"

extern "C"
{
	BotInterface27 BOT_API *CreateBot27()
	{
		return new Blank();
	}
}

const int layercount = 2;
const int neuroncount = 4;

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
	net.layers.resize(layercount);
	for (int l = 0; l < layercount; ++l)
	{
		net.layers[l].neurons.resize(neuroncount);
		for (int n = 0; n < neuroncount; ++n)
		{
			net.layers[l].neurons[n].weights.resize(neuroncount);
			for (int w = 0; w < neuroncount; ++w)
			{
				net.layers[l].neurons[n].weights[w] = (m_rand.norm()*2.0 - 1.0)*5.0;
			}
		}
	}
}

void Blank::update(const BotInput &input, BotOutput27 &output)
{
	output.moveDirection.set(m_rand.norm()*2.0-1.0, m_rand.norm()*2.0-1.0);
	//output.motor = 0.0;
	output.lookDirection.set(0,1);
	output.action = BotOutput::shoot;
	output.spriteFrame = 0;

	std::vector<float> inputs(neuroncount);
	std::vector<float> outputs(neuroncount);
	kf::Vector2 len = input.position;

	inputs[0] = input.position.x;
	inputs[1] = input.position.y;
	inputs[2] = 0;
	inputs[3] = 0;
	for (int l = 0; l < layercount; ++l)
	{
		for (int n = 0; n < neuroncount; ++n)
		{
			float sum = 0.0f;
			for (int w = 0; w < neuroncount; ++w)
			{
				sum += net.layers[l].neurons[n].weights[w] * inputs[w];
			}
			outputs[n] = sum;// (1.0f / (1.0f + exp(-sum)));
		}
		inputs = outputs;
	}
	output.moveDirection.set(outputs[0] * 2.0 - 1.0, outputs[1] * 2.0 - 1.0);
	output.lookDirection.set(outputs[2] * 2.0 - 1.0, outputs[3] * 2.0 - 1.0);
	output.motor = 1.0f;
//	output.text.clear();
//	char buf[100];
//	sprintf(buf, "%f", outputs[0]);
//	output.text.push_back(TextMsg(buf, input.position - kf::Vector2(0.0f, 1.0f), 0.0f, 0.7f, 1.0f, 80));

}

void Blank::result(bool won)
{
}

void Blank::bulletResult(bool hit)
{

}
