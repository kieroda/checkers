//Author: Daniel Aven Bross
//Date: 2/26/2014
//
//Test of the CheckerBoard class and posMoves function


#include <iostream>
#include "board.h"

int main(){
	CheckerBoard cb;
	
	//Uss posMoves function to grab an array of all possible boards resulting from red turn 1
	vector<CheckerBoard> daMovez(posMoves(cb, true));
	
	//Print the boards :D
	for(int i=0; i<daMovez.size(); i++) std::cout << daMovez[i] << "\n";
}