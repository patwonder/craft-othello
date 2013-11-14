/*
*************************************************************************
    Craft is an othello program with relatively high AI.
    Copyright (C) 2008-2011  Patrick

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

/*
********************************************************************************
								Conversions.cpp
		作者：Patrick
		概述：包含常用转换函数的定义。

********************************************************************************
*/

#include "StdAfx.h"
#include "Solver.h"
#include "Conversions.h"

using namespace CraftEngine;
using namespace CraftEngineNet;

Move Conversions::intToMove(int theInt) {
	if (theInt < 0) theInt = -theInt - 1;
	if (theInt >= WIDTH * HEIGHT) return Move(-1, -1);
	return Move(theInt / HEIGHT, theInt % HEIGHT);
}

int Conversions::moveToInt(Move theMove) {
	return theMove.X * HEIGHT + theMove.Y;
}

Chess Conversions::intToChess(int theInt) {
	switch (theInt) {
	case Solver::BLACK:
		return Chess::BLACK;
	case Solver::WHITE:
		return Chess::WHITE;
	default:
		return Chess::AVAILABLE;
	}
}

int Conversions::chessToInt(Chess theChess) {
	switch (theChess) {
	case Chess::BLACK:
		return Solver::BLACK;
	case Chess::WHITE:
		return Solver::WHITE;
	case Chess::AVAILABLE:
	default:
		return Solver::AV;
	}
}

System::String^ Conversions::moveToString(Move theMove) {
	if (theMove.X < 0 || theMove.X >= WIDTH || theMove.Y < 0 || theMove.Y >= HEIGHT)
		return "--";
	return System::Char('A' + theMove.X).ToString() + System::Char('1' + theMove.Y).ToString();
}

System::String^ Conversions::intToString(int theInt) {
	if (theInt < 0) theInt = -theInt - 1;
	if (theInt >= WIDTH * HEIGHT)
		return "--";
	return System::Char('A' + theInt / HEIGHT).ToString() + System::Char('1' + theInt % HEIGHT);
}

void Conversions::convertBoard(Board board, int dest[]) {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			dest[i * HEIGHT + j] = (board[i, j] == Chess::BLACK) ? CraftEngine::Solver::BLACK
				: ((board[i, j] == Chess::WHITE) ? CraftEngine::Solver::WHITE : CraftEngine::Solver::AV);
		}
	}
}
