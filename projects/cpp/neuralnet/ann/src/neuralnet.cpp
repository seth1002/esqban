#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <io.h>
#include <fcntl.h>
#include "neuralnet.h"


CNeuralNet::CNeuralNet()
{
	memset(&m_Net, 0, sizeof(NET));
}

//	initializes and allocates memory on heap
CNeuralNet::CNeuralNet(int nl,int *sz,double b,double a)
{
	m_Net.size	= nl;
	m_Net.alpha	= a;
	m_Net.beta	= b;

	m_Net.layers = new LAYER[nl];
	memset(m_Net.layers, 0, nl * sizeof(LAYER));

	srand((unsigned)(time(NULL)));

	for(unsigned int l=0; l<m_Net.size; l++)
	{
		LAYER *layer = &m_Net.layers[l];
		layer->size = sz[l];
		layer->neurons = new NEURON[sz[l]];
		memset(layer->neurons, 0, sz[l] * sizeof(NEURON));
		for(int n=0; n<sz[l]; n++)
		{
			NEURON *neuron = &layer->neurons[n];
			neuron->weight = (double)rand() / (RAND_MAX / 2) - 1;
			if(l > 0)
			{
				LAYER *prev_layer = &m_Net.layers[l-1];
				int prev_lsize = prev_layer->size;
				neuron->connections = new CONNECTION[prev_lsize];
				memset(neuron->connections, 0, prev_lsize * sizeof(CONNECTION));

				for(int c=0; c<prev_lsize; c++)
				{
					CONNECTION *conn = &neuron->connections[c];
					conn->weight = (double)rand() / (RAND_MAX / 2) - 1;
				}
			}
		}
	}
}

CNeuralNet::~CNeuralNet()
{
	for(unsigned int l=0; l<m_Net.size; l++)
	{
		LAYER *layer = &m_Net.layers[l];
		for(unsigned int n=0; n<layer->size; n++)
		{
			NEURON *neuron = &layer->neurons[n];
			delete [] neuron->connections;
		}
		delete [] layer->neurons;
	}
	delete [] m_Net.layers;
}

//	sigmoid function
double CNeuralNet::sigmoid(double in)
{
	return (double)(1 / (1 + exp(-in)));
}

//	mean square error
double CNeuralNet::mean_square_error(double *tgt) const
{
	double mse=0;
	LAYER *layer = &m_Net.layers[m_Net.size-1];
	for(unsigned int n=0; n<layer->size; n++)
	{
		NEURON *neuron = &layer->neurons[n];
		mse += (tgt[n] - neuron->out) * (tgt[n] - neuron->out);
	}
	return mse/2;
}

//	returns n'th output of the net
double CNeuralNet::GetOutput(int n) const
{
	return m_Net.layers[m_Net.size-1].neurons[n].out;
}
/*
double CNeuralNet::operator [] (int index) const
{
	return m_Net.layers[m_Net.size-1].neurons[index].out;
}
*/
void CNeuralNet::calculate_outputs()
{
	double sum;

	//	assign output(activation) value 
	//	to each neuron usng sigmoid func
	for(unsigned int l=1; l<m_Net.size; l++)	// For each layer
	{
		LAYER *layer = &m_Net.layers[l];
		LAYER *prev_layer = &m_Net.layers[l-1];
		// For each neuron in current layer
		for(unsigned int n=0; n<layer->size; n++)
		{
			NEURON *neuron = &layer->neurons[n];
			sum = 0.f;
			// For input from each neuron in preceeding layer
			for(unsigned int c=0; c<prev_layer->size; c++)
			{
				CONNECTION *conn = &neuron->connections[c];
				// Apply weight to inputs and add to sum
				sum += (prev_layer->neurons[c].out * conn->weight);
			}
			// Apply bias
			sum += neuron->weight;
			// Apply sigmoid function
			m_Net.layers[l].neurons[n].out = sigmoid( sum );
		}
	}
}

// feed forward one set of input
// feed forwards activations for one set of inputs
void CNeuralNet::FeedForward(double *in)
{
	//	assign content to input layer
	for(unsigned int n=0; n<m_Net.layers[0].size; n++)
	{
		// output_from_neuron(i,j) Jth neuron in Ith LAYER
		m_Net.layers[0].neurons[n].out = in[n];
	}

	calculate_outputs();
}

//	find delta for output layer
void CNeuralNet::find_output_delta(double *tgt)
{
	LAYER *output_layer = &m_Net.layers[m_Net.size-1];
	for(unsigned int n=0; n<output_layer->size; n++)
	{
		NEURON *neuron = &output_layer->neurons[n];
		neuron->delta = neuron->out * (1 - neuron->out) * (tgt[n] - neuron->out);
	}
}

void CNeuralNet::find_hidden_delta(int layer_num)
{
	double sum;
	LAYER *layer = &m_Net.layers[layer_num];
	LAYER *out_layer = &m_Net.layers[layer_num+1];
	for(unsigned int n=0; n<layer->size; n++)
	{
		NEURON *neuron = &layer->neurons[n];
		sum = 0.0;
		for(unsigned int n1=0; n1<m_Net.layers[layer_num+1].size; n1++)
		{
			NEURON *out_neuron = &out_layer->neurons[n1];
			sum += out_neuron->delta * out_neuron->connections[n].weight;
		}
		neuron->delta = neuron->out * (1 - neuron->out) * sum;
	}
}

//	apply momentum ( does nothing if alpha=0 )
void CNeuralNet::apply_momentum(int layer_num)
{
	if(0.f == m_Net.alpha)
		return;

	LAYER *layer = &m_Net.layers[layer_num];
	LAYER *prev_layer = &m_Net.layers[layer_num-1];
	unsigned int prev_lsize = prev_layer->size;

	for(unsigned int n=0; n<layer->size; n++)
	{
		NEURON *neuron = &layer->neurons[n];
		for(unsigned int c=0; c<prev_lsize; c++)
		{
			CONNECTION *conn = &neuron->connections[c];
			conn->weight += m_Net.alpha * conn->prevDwt;
		}
		neuron->weight += m_Net.alpha * neuron->prevDwt;
	}
}

//	adjust weights usng steepest descent	
void CNeuralNet::adjust_weights(int layer_num)
{
	LAYER *layer = &m_Net.layers[layer_num];
	LAYER *prev_layer = &m_Net.layers[layer_num-1];
	unsigned int prev_lsize = prev_layer->size;

	for(unsigned int n=0; n<layer->size; n++)
	{
		NEURON *neuron = &layer->neurons[n];
		for(unsigned int c=0; c<prev_lsize; c++)
		{
			CONNECTION *conn = &neuron->connections[c];
			conn->prevDwt = m_Net.beta * neuron->delta * prev_layer->neurons[n].out;
			conn->weight += conn->prevDwt;
		}
		neuron->prevDwt = m_Net.beta * neuron->delta;
		neuron->weight += neuron->prevDwt;
	}
}

//	backpropogate errors from output
//	layer uptill the first hidden layer
double CNeuralNet::BackPropagate(double *in, double *tgt)
{
	//	update output values for each neuron
	FeedForward(in);

	//	find delta for output layer
	find_output_delta(tgt);

	//	find delta for hidden layers
	unsigned int l=0;
	for(l=m_Net.size-2; l>0; l--)
	{
		find_hidden_delta(l);
	}

	for(l=1; l<m_Net.size; l++)
	{
		apply_momentum(l);
		adjust_weights(l);
	}

	return mean_square_error(tgt);
}

















CSerializableNet::CSerializableNet(int nl,int *sz,double b,double a)
: CNeuralNet(nl, sz, b, a)
{

}

bool CSerializableNet::Write(FILE *fp)
{
#ifdef WINDOWS
	if( -1 == setmode( fileno ( fp ), O_BINARY ) )
		return false;
#endif
	if(1 != fwrite(&m_Net, sizeof(NET), 1, fp))
		return false;

	for(unsigned int l=0; l<m_Net.size; l++)
	{
		LAYER *layer = &m_Net.layers[l];
		if(1 != fwrite(&layer->size, sizeof(int), 1, fp))
			return false;
		for(unsigned int n=0; n<layer->size; n++)
		{
			NEURON *neuron = &layer->neurons[n];
			neuron->out = 0.f;
			if(1 != fwrite(neuron, sizeof(NEURON), 1, fp))
				return false;
			if(l > 0)
			{
				unsigned int len = m_Net.layers[l-1].size;
				if(len != fwrite(neuron->connections, sizeof(CONNECTION), len, fp))
					return false;
			}
		}
	}
	return true;
}

bool CSerializableNet::Read(FILE *fp)
{
#ifdef WINDOWS
	if( -1 == setmode( fileno ( fp ), O_BINARY ) )
		return false;
#endif
	if(1 != fread(&m_Net, sizeof(NET), 1, fp))
		return false;

	m_Net.layers = new LAYER[m_Net.size];
	memset(m_Net.layers, 0, sizeof(LAYER)*m_Net.size);

	for(unsigned int l=0; l<m_Net.size; l++)
	{
		LAYER *layer = &m_Net.layers[l];
		if(1 != fread(&layer->size, sizeof(int), 1, fp))
			return false;
		layer->neurons = new NEURON[layer->size];
		memset(layer->neurons, 0, sizeof(NEURON)*layer->size);
		for(unsigned int n=0; n<layer->size; n++)
		{
			NEURON *neuron = &layer->neurons[n];
			if(1 != fread(neuron, sizeof(NEURON), 1, fp))
				return false;
			neuron->out = 0.f;
			if(l > 0)
			{
				unsigned int len = m_Net.layers[l-1].size;
				neuron->connections = new CONNECTION[len];
				memset(neuron->connections, 0, sizeof(CONNECTION)*len);
				if(len != fread(neuron->connections, sizeof(CONNECTION), len, fp))
					return false;
			}
		}
	}
	return true;
}

void CSerializableNet::DumpXML(FILE *fp)
{
	fprintf(fp, "<net m_Net.alpha='%f' m_Net.beta='%f'>\n", m_Net.alpha, m_Net.beta);
	for(unsigned int l=0; l<m_Net.size; l++)
	{
		fprintf(fp, " <LAYER%d>\n", l);
		LAYER *layer = &m_Net.layers[l];
		for(unsigned int n=0; n<layer->size; n++)
		{
			NEURON *neuron = &layer->neurons[n];
			fprintf(fp, "  <NEURON%d weight='%f' delta='%f'>\n", n, neuron->weight, neuron->delta);
			if(l > 0)
			{
				for(unsigned int c=0; c<m_Net.layers[l-1].size; c++)
				{
					CONNECTION *conn = &neuron->connections[c];
					fprintf(fp, "   <CONNECTION%d weight='%f'/>\n", c, conn->weight);
				}
			}
			fprintf(fp, "  </NEURON%d>\n", n);
		}
		fprintf(fp, " </LAYER%d>\n", l);
	}
	fprintf(fp, "</net>\n");
}






/*

size_t CNeuralNet::serialize_func(void *buffer, size_t size, size_t count, FILE *stream, bool read)
{
	if(read)
		return fread(buffer, size, count, stream);
	else
		return fwrite((const void *)buffer, size, count, stream);
}

bool CNeuralNet::Serialize(FILE *fp, bool read)
{
	if( -1 == setmode( fileno ( fp ), O_BINARY ) )
		return false;

	size_t (*func)(void*, size_t, size_t, FILE*);
	func = read ? fread : fwrite;

	if(1 != func(&m_Net.size, sizeof(int), 1, fp))
		return false;

	if(1 != func(&m_Net.alpha, sizeof(double), 1, fp))
		return false;

	if(1 != func(&m_Net.beta, sizeof(double), 1, fp))
		return false;

	m_Net.layers = new LAYER[m_Net.size];
	if(read)
		memset(m_Net.layers, 0, sizeof(LAYER)*m_Net.size);

	for(unsigned int l=0; l<m_Net.size; l++)
	{
		LAYER *layer = &m_Net.layers[l];
		if(1 != func(&layer->size, sizeof(int), 1, fp))
			return false;
		layer->neurons = new NEURON[layer->size];
		if(read)
			memset(layer->neurons, 0, sizeof(NEURON)*layer->size);
		for(unsigned int n=0; n<layer->size; n++)
		{
			NEURON *neuron = &layer->neurons[n];
			if(1 != func(neuron, sizeof(NEURON), 1, fp))
				return false;
			if(l > 0)
			{
				unsigned int len = m_Net.layers[l-1].size;
				neuron->connections = new CONNECTION[len];
				if(read)
					memset(neuron->connections, 0, sizeof(CONNECTION)*len);
				if(len != func(neuron->connections, sizeof(CONNECTION), len, fp))
					return false;
			}
		}
	}
	return true;
}
*/
