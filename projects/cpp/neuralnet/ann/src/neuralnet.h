
//////////////////////////////////////////////
//	Fully connected multilayered feed		//
//	forward	artificial neural network using	//
//	Backpropogation	algorithm for training.	//
//////////////////////////////////////////////


#ifndef NEURALNET_H
#define NEURALNET_H

#include <assert.h>
#include <iostream.h>
#include <stdio.h>
#include <math.h>
#include "net.h"

class CNeuralNet
{
public:
	CNeuralNet();
	~CNeuralNet();

	// initializes and allocates memory
	CNeuralNet(int nl,int *sz,double b,double a);

	// backpropogates error for one set of input
	double BackPropagate(double *in,double *tgt);

	void FeedForward(double *in);
	
	double GetOutput(int i) const;
//	double operator [] (int index) const;

private:
	double mean_square_error(double *tgt) const;	
	double sigmoid(double in);	// squashing function
	void find_hidden_delta(int layer_num);
	void apply_momentum(int layer_num);
	void adjust_weights(int layer_num);
	void calculate_outputs();
	void find_output_delta(double *tgt);

protected:
	NET m_Net;
};

class CSerializableNet : public CNeuralNet
{
public:
	CSerializableNet(int nl,int *sz,double b,double a);

	bool Write(FILE *fp);
	bool Read(FILE *fp);
	void DumpXML(FILE *fp);
//	bool Serialize(FILE *fp, bool read);

private:
//	size_t serialize_func(void *buffer, size_t size, size_t count, FILE *stream, bool read);
};

#endif // NEURALNET_H
