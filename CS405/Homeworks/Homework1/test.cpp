//test.cpp
//Author: Aven Bross
//Date: 2/03/2014
//Simple place to test neural net class

#include "neuralnet.h"
#include <iostream>
#include <chrono>

int main(){
	int s[4] = {32, 40, 10, 1};	//Contains the layer sizes to send to the NeuralNet class

	NeuralNet n1(s, 4);	//Create NeuralNet with 4 layers of the given sizes

	double in[32];			//Array to store input for evaluate function
	for(int i=0; i<32; i++)		//Make each input 1 for the test
		in[i]=100;
	
	//Set up timing variables
	auto start = std::chrono::steady_clock::now();
	auto curr = start;
	int count = 0;
	
	std::cout<< *(n1.evaluate(in))<<"\n";

	//See how many evaluations can be performed in 1 second (do 10 seconds then divide)
	do{
		n1.evaluate(in);
		count++;
		curr = std::chrono::steady_clock::now();
	}
	while((std::chrono::duration<double>(curr-start)).count() < 10.2);	//.2 found to be time for empty loop

	std::cout << count/10 << "\n";

	/* OLD CODE FOR FINDING EMPTY LOOP TIME
	for(int i=0; i<44000; i++){
		count++;
		curr = std::chrono::steady_clock::now();
		std::chrono::duration<double>(curr-start);
	}
	std::cout << std::chrono::duration<double>(curr-start).count() << "\n";*/	
}
