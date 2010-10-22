/*
*************************************************************************
    Craft is an othello program with relatively high AI.
    Copyright (C) 2008-2010  Patrick

    This file is part of Craft.

    Craft is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Craft is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Craft.  If not, see <http://www.gnu.org/licenses/>.

    Craft-Othello on Google Code: <http://code.google.com/p/craft-othello/>

    Patrick's E-mail: patrick880905@sina.com
    Patrick's Blog: <http://blog.sina.com.cn/patwonder>
*************************************************************************
*/

#pragma once
/*
********************************************************************************
								GameContext.h
		作者：Patrick
		概述：包含类 GameContext 的声明。
			GameContext 类是棋局上下文类，对 ChessBoard 类进行了只读包装，
			用于通知棋手和观察者当前局面形势。

********************************************************************************
*/

#include "Common.h"
#include "ChessBoard.h"
#include "Timer.h"

namespace Othello {

public ref class GameContext {
private:
	ChessBoard^ cb;
	Chess currentColor;
	Chess^ currentPlayer;
	Chess^ firstPlayer;
	Timer^ timer;
	System::String^ playerName;

	//Forbid illegal construction
	GameContext(const GameContext^) {}
	GameContext^ operator=(const GameContext^) {return nullptr;}
public:
	//constuctor:
	GameContext(ChessBoard^ c, System::String^ name, Chess color, Chess^ player, Chess^ fp, Timer^ tmr)
		: cb(c), playerName(name), currentColor(color), currentPlayer(player), firstPlayer(fp),
		timer(tmr) {};

	//EFFECTS: returns opponent player's name
	System::String^ getOpponentName() {
		return playerName;
	}

	//EFFECTS: if x is not in the range 1..WIDTH or y is not in the range 1..HEIGHT
	//  throws System::ArgumentOutOfRangeException, otherwise returns the color at (x, y) of the chessboard.
	Chess get(int x, int y) {
		return cb->get(x, y);
	}

	//EFFECTS: if step is not in the range 0..(maxStep - 1)
	//  throws System::ArgumentOutOfRangeException, otherwise returns the step-th move of this game
	Move getMove(int step) {
		return cb->getMove(step);
	}

	//EFFECTS: returns the step number of current step
	int getCurrentStep() {
		return cb->getCurrentStep();
	}

	//EFFECTS: returns the current player's color
	Chess getCurrentPlayer() {
		return (*currentPlayer);
	}

	//EFFECTS: returns the total time limit for the game, 0 for unlimited
	int getTotalTimeLimit() {
		return timer->getTotalTimeLimit();
	}

	//EFFECTS: returns the time left for current player
	int getLeftTime() {
		return timer->getLeftTime();
	}

	//EFFECTS: returns the last move; 
	//  if the opponent didn't have a move, returns Move::invalidMove;
	Move getLastMove() {
		return cb->getLastMove();
	}

	//EFFECTS: if index is not in the range 1..getAvailableCount() throws System::ArgumentOutOfRangeException,
	//  otherwise returns the index-th available move for the current step
	System::Drawing::Point getAvailableMove(int index) {
		return cb->getAvailableMove(currentColor, index);
	}

	//EFFECTS: returns the number of all available moves for the current step
	int getAvailableCount() {
		return cb->getAvailableCount(currentColor);
	}
	
	//EFFECTS: returns the number of chess for the current player
	int getNumber() {
		return cb->getNumber(currentColor);
	}

	//EFFECTS: returns the number of chess for the opponent
	int getOppNumber() {
		return cb->getNumber((currentColor == Chess::BLACK) ? Chess::WHITE : Chess::BLACK);
	}

	//EFFECTS: returns the number of empty squares
	int getEmptyNumber() {
		return cb->getNumber(Chess::AVAILABLE);
	}

	//EFFECTS: returns a bool indicating whether the game is ended
	bool isGameEnded() {
		return cb->isGameEnded();
	}

	//EFFECTS: returns the initial board of the game
	Board^ getInitialBoard() {
		return cb->getInitialBoard();
	}

	//EFFECTS: returns the color of the player who made the first move of the game
	Chess getFirstPlayer() {
		return *firstPlayer;
	}
};

}//namespace Othello
