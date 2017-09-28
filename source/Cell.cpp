#include "Cell.h"
#include "Board.h"
#include <string>
using namespace std;

//Load images for cells (doesn't play nice when placed in header file)
Fl_PNG_Image oneCell    ("graphics/1Cell.png");
Fl_PNG_Image twoCell    ("graphics/2Cell.png");
Fl_PNG_Image threeCell  ("graphics/3Cell.png");
Fl_PNG_Image fourCell   ("graphics/4Cell.png");
Fl_PNG_Image fiveCell   ("graphics/5Cell.png");
Fl_PNG_Image sixCell    ("graphics/6Cell.png");
Fl_PNG_Image sevenCell  ("graphics/7Cell.png");
Fl_PNG_Image eightCell  ("graphics/8Cell.png");
Fl_PNG_Image blankCell  ("graphics/blankCell.png");
Fl_PNG_Image openedCell ("graphics/openCell.png");
Fl_PNG_Image mineCell   ("graphics/mineCell.png");
Fl_PNG_Image explodeCell("graphics/explodeCell.png");
Fl_PNG_Image flaggedCell("graphics/flagCell.png");
Fl_PNG_Image markedCell ("graphics/markCell.png");
Fl_PNG_Image crossCell  ("graphics/xCell.png");

/*Cell Constructor:
 *	Calls Fl_Button's constructor with image height and witdh as size values 
 *	(image sizes are all hard-coded as 20x20 pixels) 
 *	All bool values are initialized to false*/
 
Cell::Cell(int x, int y, const char* label) : 
	Fl_Button(x, y, blankCell.w(), blankCell.h(), label), size(blankCell.w()), 
	numAdjacent(0), hasMine(false), isOpen(false), isFlagged(false), isMarked(false),
	lostMine(false)
	{
		this->box(FL_NO_BOX); //Initialize box type and image 
		this->image(blankCell);
		this->align(-1); //necessary to center images over cell
	}

void Cell::setMined(bool mine) { //Set whether or not a cell contains a mine
	this->hasMine = mine;
}

bool Cell::getMine() {
	return hasMine;
}

bool Cell::checkLost() {
	return lostMine;
}

bool Cell::getFlag() {
	return isFlagged;
}

bool Cell::getMark() {
	return isMarked;
}

bool Cell::checkOpen() {
	return isOpen;
}

void Cell::labelMines() {
	this->label("X");
	this->redraw();
}

void Cell::openCell() {
	if ((this->isOpen == false) && (this->isFlagged == false)) {//Cannot open an opened/flagged cell
		if (this->hasMine) {
			this->image(explodeCell);
			this->lostMine = true;
		}
		else {
			this->image(openedCell);
			this->drawNumber();
		}
		
		this->isOpen = true;
		this->redraw();
	}
	return;
}

void Cell::revealCell() {
	if (this->hasMine && !this->isFlagged) {
		if (!this->lostMine) {
			this->image(mineCell); //Normal mine if it's not the mine that lost the game
		}
	}
	if (!this->hasMine && this->isFlagged) {
		this->image(crossCell); //Mark any wrongly flagged cells with an X 
	}	
	this->redraw();
}

void Cell::showMine() {//Change all mine cells to flags (when player wins)
	if (this->hasMine) {
		this->image(flaggedCell);
		this->redraw();
	}
}

//Mouse click functions
void Cell::flagCell() {
	this->image(flaggedCell);
	this->isFlagged = true;
	this->redraw();
}

void Cell::markCell() {
	this->image(markedCell);
	this->isFlagged = false;
	this->isMarked = true;
	this->redraw();	
}

void Cell::unMarkCell() {
	this->image(blankCell);
	this->isMarked = false;
	this->redraw();
}

void Cell::setRowNum(int num) {
	rowNum = num;
}

void Cell::setColNum(int num) {
	colNum = num;
}

int Cell::getRowNum() {
	return rowNum;
}

int Cell::getColNum() {
	return colNum;
}

int Cell::getNumAdjacent() {
	return numAdjacent;
}

void Cell::setNumAdjacent(int num) {
	numAdjacent = num;
}

void Cell::hideCell() {
	this->hasMine = false;
	this->openCell();
	this->hide();
	this->redraw();
}

void Cell::resetCell() {
	this->image(blankCell);
	this->isMarked = false;
	this->isFlagged = false;
	this->hasMine = false;
	this->isOpen = false;
	this->lostMine = false;
	this->label(" ");
	this->activate(); //Reactivate the widget (otherwise board won't be interactive)
	this->redraw();
}

void Cell::drawNumber() {
	if (this->isOpen == true) {
		switch(numAdjacent) {
		case 1:
			this->image(oneCell);
			break;
		case 2:
			this->image(twoCell);
			break;
		case 3:
			this->image(threeCell);
			break;
		case 4:
			this->image(fourCell);
			break;
		case 5:
			this->image(fiveCell);
			break;
		case 6:
			this->image(sixCell);
			break;
		case 7:
			this->image(sevenCell);
			break;
		case 8:
			this->image(eightCell);
			break;
		default:
			this->image(openedCell);
			break;
		}
	}
	this->redraw();
}

int Cell::handle(int event) {
	switch(event) {
		case FL_PUSH: 
			if (Fl::event_button() == FL_LEFT_MOUSE) {//if LMB is clicked
				this->isMarked = false;
				this->do_callback();
				this->openCell();
				return 1;
			}
			else if(Fl::event_button() == FL_RIGHT_MOUSE) {//if RMB is clicked
				if (!this->isOpen && !this->isFlagged) {//Can't flag an open/flagged cell
					if (this->isMarked) {
						this->unMarkCell();//Unmark cells that contain ('?')
					}
					else {
						this->flagCell();
						this->do_callback();
					}
				}
				else if(this->isFlagged) {
					this->markCell();
					this->do_callback();
				}
				return 1;
			}
		default:
			return Fl_Widget::handle(event);
	}
}
