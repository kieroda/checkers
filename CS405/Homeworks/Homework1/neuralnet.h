//neuralnet.h
//Author: Aven Bross
//Date: 1/30/2014
//Neural net class

/* NOTE:
 * Apparently I like making things overly complicated
 * and decided to use arrays and pointers to do my net.
 * I also wrote a version with simply a vector of nodes
 * after I realised how easy and readable that is, but
 * decided to turn this one in just to add a different
 * way of doing it to the pool of trials.  I did this
 * at around 4:00 in the morning and was really proud
 * at the time, looking back though I have no idea
 * what I was thinking.
*/

/* DESCRIPTION:
 * Neural net with variable layers and layer size.
 * Initial weights are set via a uniform distribution.
 * Random numbers use a 32 bit mersenne twister
 * Evaluate function returns a pointer so you can grab
 *   multiple output nodes if needed.
*/

#ifndef NEURALNET_H
#define NEURALNET_H

#include <cmath>
#include <random>
#include <iostream>

const int WT_AMP = 1.0;	//Initial weights will be set using the distribution [-WT_AMP,WT_AMP]
const int MUT_STD_DEV = .75;	//The standard deviation of the normal distribution used for mutations

class NeuralNet {
public:
	//Contstruct a neural net with the given layers
	NeuralNet(int* size, int nLayers): _nLayers(nLayers){
		std::random_device rd;	//Create random device
		std::mt19937 el(rd());	//Use 32 bit mersenne twister
		std::uniform_real_distribution<float> udist(-1*WT_AMP,WT_AMP);	//create a uniform distribution over [-2.0,2.0]
		
		//Set up array storing layer sizes
		_size = new int[_nLayers];
		for(int i=0; i<_nLayers; i++)
			_size[i] = size[i];

		//Set up weight arrays (uses uniform distribution)
		_weights = new float**[_nLayers];
		for(int k=0; k<_nLayers-1; k++){			//For each layer k,
			_weights[k] = new float*[_size[k+1]];		//create an array of the size of the layer k+1
			for(int i=0; i<_size[k+1]; i++){		//Then for each element of this array,
				_weights[k][i] = new float[_size[k]];	//create an array the size of layer k to fill with weights.
				for(int j=0; j<_size[k]; j++)		//Then, for each element in that array,
					_weights[k][i][j] = udist(el);	//fill with a random number from our uniform distribution
			}
		}

		//Set up output arrays for layers
		_values = new float*[_nLayers];
		for(int i=0; i<_nLayers; i++)			//For each layer,
			_values[i]= new float[_size[i]];	//create an array of the size of that layer
	}

	//Copy constructor, same as normal constructor but copying values
	NeuralNet(const NeuralNet & other): _nLayers(other._nLayers) {
		
		//Copy size array
		_size = new int[_nLayers];
		for(int i=0; i<_nLayers; i++)
			_size[i] = other._size[i];

		//Copy weight arrays
		_weights = new float**[_nLayers];
		for(int k=0; k<_nLayers-1; k++){
			_weights[k] = new float*[_size[k+1]];
			for(int i=0; i<_size[k+1]; i++){
				_weights[k][i] = new float[_size[k]];
				for(int j=0; j<_size[k]; j++)
					_weights[k][i][j] = other._weights[k][i][j];
			}
		}

		//Set up output array
		_values = new float*[_nLayers];
		for(int i=0; i<_nLayers; i++)
			_values[i]= new float[_size[i]];
	}

	//Evaluate the network with the given inputs
	float* evaluate(float* input){

		//Grab input values and store in 1st layer
		for(int i=0; i<_size[0]; i++)
			_values[0][i]=input[i];

		//Evaluate through each layer of the neural net (skipping input layer)
		for(int k=1; k<_nLayers; k++){
			for(int i=0; i<_size[k]; i++){
				
				float temp = 0.0;

				//Sum each input*weight product
				for(int j=0; j<_size[k-1]; j++){
					temp+=_weights[k-1][i][j]*_values[k-1][j];
				}

				//Convert to the range (-1.0,1.0) using a simplification of sigmoid function
				temp = temp/(1+abs(temp));

				//Save value in output array
				_values[k][i] = temp;
			}
		}

		//Return a pointer to output neuron values
		return _values[_nLayers-1];
	}
	
	//Destructor (not sure if I used too many deletes)
	~NeuralNet(){
		for(int k=0; k<_nLayers-1; k++){
			for(int i=0; i<_size[k+1]; i++){
				delete _weights[k][i];
			}
			delete _weights[k];
		}
		delete _weights;

		for(int i=0; i<_nLayers; i++)
			delete _values[i];
		delete _values;

		delete _size;
	}

private:
	int _nLayers;		//Stores number of layers in network			STRUCTURE:
	int* _size;		//Stores size of each layer. 				_size[layer]
	float*** _weights;	//Stores weights each non-input node			_weights[layer][current node][input node]
	float** _values;	//Stores calculated values for each non input node	_values[layer][node]
};

#endif
