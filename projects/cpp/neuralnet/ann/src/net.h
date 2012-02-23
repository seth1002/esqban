#ifndef NET_H
#define NET_H

struct CONNECTION
{
	double weight;
	double prevDwt;	// storage for weight-change made in previous epoch
};

struct NEURON
{
	double out;
	double delta;
	CONNECTION *connections;
	// bias
	double weight;
	double prevDwt;
};

struct LAYER
{
	unsigned int size;
	NEURON *neurons;
};

struct NET
{
	LAYER *layers;
	unsigned int size;
	double alpha;
	double beta;
};

#endif // NET_H
