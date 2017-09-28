#include "Cell.h"
#include "Board.h"

using namespace std;

int main (int argc, char** argv) {
	//int boardSize = 9; //Default board difficulty is beginner 
	int numMines  = 10;

	//Build & initialize the board
	Board *gameBoard = new Board(9,9);
	gameBoard->seedBoard(numMines); 

	return Fl::run();
}
