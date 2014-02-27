//Author: Daniel Aven Bross
//Date: 2/26/2014
//
//Board Representation for AI project

#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H

#include <vector>
using std::vector;
#include <iostream>
using std::ostream;

const int HEIGHT = 10;
const int WIDTH = 6;

//Scheme for char vectors:
//RED:		piece - r	king - R
//BLACK: 	piece - b	king - B
//BUFFER: 	X
//EMPTY: 	_

class CheckerBoard {

public:
	//Sets up a basic checker board setup
	CheckerBoard(){
		for(int i=0; i<HEIGHT; i++){
			for(int j=0; j<WIDTH; j++){
				if(i==0 || i == HEIGHT-1)	_board.push_back('X');
				else if(i%2==0){
					if(j==0)	_board.push_back('X');
					else if(i<4)	_board.push_back('r');
					else if(i>5)	_board.push_back('b');
					else _board.push_back('_');
				}
				else{
					if(j==WIDTH-1)	_board.push_back('X');
					else if(i<4)	_board.push_back('r');
					else if(i>5)	_board.push_back('b');
					else _board.push_back('_');
				}
			}
		}
	}
	
	//Copy Constructor
	CheckerBoard(const CheckerBoard & other): _board(other._board) {}
	
	//Returns  whether the inputs make a valid move (assumes inputs are for a possible move)
	bool checkMove(int end) const{
		return (_board[end]=='_');
	}
	
	//Returns whether the given inputs make a valid jump (assumes inputs are for a possible jump)
	bool checkJump(int start, int target, int end, bool isRed) const{
		if(isRed){
			if(_board[target]=='X' || (_board[target]!='b' && _board[target]!='B'))	return false;
			if(_board[end]=='_')	return true;
			return false;
		}
		else{
			if(_board[target]=='X' || (_board[target]!='r' && _board[target]!='R'))	return false;
			if(_board[end]=='_')	return true;
			return false;
		}
	}
	
	//Make the given move (assumes the input values are a valid move)
	void makeMove(int start, int end){
		_board[end]=_board[start];
		_board[start]='_';
		
		//Update kings
		if(_board[end]=='r' && end > 8*WIDTH) _board[end]='R';
		else if(_board[end]=='b' && end < 2*WIDTH) _board[end]='B';
	}
	
	//Make the given jump move (assumes input values are a valid jump)
	void makeJump(int start, int target, int end){
		_board[end]=_board[start];
		_board[start]='_';
		_board[target]='_';
		
		//Update kings
		if(_board[end]=='r' && end > 8*WIDTH) _board[end]='R';
		else if(_board[end]=='b' && end < 2*WIDTH) _board[end]='B';
	}
	
	//Will eventually use this with neural net :D
	//Returns a board in standard form without the buffer
	/*vector<char> stdBoard(){
		vector<char> result;
		for(int i=1; i<HEIGHT-1; i++){
			for(int j=1; j<WIDTH-1; j++){
				result.push_back(_board[i*WIDTH+j]);
			}
		}
		return result;
	}*/
	
	//Returns the character for the piece at the given position 
	//(note col doesn't actually mean column but distance from the left since squares are offset)
	char getPiece(int row, int col) const{
		return _board[row*WIDTH+col];
	}
	
private:
	vector<char> _board;
};

//cout print operator overload for the CheckerBoard class
ostream & operator<<(ostream & os, const CheckerBoard & cb){
	//Print the checkerboard, including blank spaces
	// _ is a possible but empty square, blank space are impossible squares
	for(int i=HEIGHT-2; i>0; i--){
		if(i%2==1) os<<" "<<" ";
		for(int j=WIDTH-2; j>0; j--){
			os<<cb.getPiece(i,j);
			if(j>1) os<<" "<<" "<<" ";
			else os<< " ";
		}
		if(i%2==0) os<<" "<<" ";
		os<<"\n";
	}
	return os;
}

//Returns all possible boards that could result from a turn of the player of the given color
vector<CheckerBoard> posMoves(const CheckerBoard & board, bool isRed){
	char piece, king;	//Tracks what characters are pieces and kings for this color
	int coef;	//Tracks the direction we are going
	
	//Vector to store a board for each move
	vector<CheckerBoard> allmoves;
	
	//Set variables up based on color
	if(isRed) { piece = 'r'; king = 'R'; coef = 1; }
	else      { piece = 'b'; king = 'B'; coef = -1; }
	
	//Go and find all possible resultant boards from possible moves
	for(int i=1; i<HEIGHT-1; i++){
		for(int j=1; j<WIDTH-1; j++){
			int curPos = i*WIDTH+j;	//Current position
			int count = 0;	//Loop counter
			int x=0, jx = 1;	//Extra bits to be added depending on odd or even row (jx is for the jump row)
			if(i%2==0) { x=1; jx = 0; }	//Determine whether we are on an odd or even row (jx will be the opposite since it is an adjacent row)
			
			//Prepare to search based on piece type
			if(board.getPiece(i, j) == piece) count = 1;
			else if(board.getPiece(i, j) == king) { count = 2; coef*=-1; }
			else coef*=-1;
			
			//Find all possible moves for the piece (loops for kings to look both directions)
			while(count!=0){
				if(board.checkMove(curPos+coef*WIDTH)){
					CheckerBoard tempb(board);
					tempb.makeMove(curPos, curPos+coef*WIDTH);
					allmoves.push_back(tempb);
				}
				else if(board.checkJump(curPos, curPos+coef*(WIDTH)+x, (curPos+coef*WIDTH+x)+coef*WIDTH+jx, isRed)){
					CheckerBoard tempb(board);
					tempb.makeJump(curPos, curPos+coef*(WIDTH)+x, (curPos+coef*(WIDTH)+x)+coef*(WIDTH)+jx);
					allmoves.push_back(tempb);
				}
				if(board.checkMove(curPos+coef*(WIDTH-1)+x)){
					CheckerBoard tempb(board);
					tempb.makeMove(curPos, curPos+coef*(WIDTH-1)+x);
					allmoves.push_back(tempb);
				}
				else if(board.checkJump(curPos, curPos+coef*(WIDTH-1)+x, (curPos+coef*(WIDTH-1)+x)+coef*(WIDTH-1)+jx, isRed)){
					CheckerBoard tempb(board);
					tempb.makeJump(curPos, curPos+coef*(WIDTH-1)+x, (curPos+coef*(WIDTH-1)+x)+coef*(WIDTH-1)+jx);
					allmoves.push_back(tempb);
				}
				count--;	//Decrement counter
				coef*=-1;	//Swap directions
			}
			coef*=-1;	//Swap back to original direction
		}
	}
	
	return allmoves;	//Return array of all possible moves
}

#endif