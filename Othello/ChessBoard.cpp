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
								ChessBoard.cpp
		作者：Patrick
		概述：包含类 ChessBoard 的定义。
			ChessBoard 类封装了黑白棋棋盘表示、走棋规则，提供简单、
			易于使用的接口。

********************************************************************************
*/

#include "StdAfx.h"
#include "ChessBoard.h"

using namespace System;
using System::Drawing::Point;
using namespace Othello;

inline bool inBound(int x, int y) {
	return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}

bool ChessBoard::putChess(int x, int y, Chess color) {
	if (color != Chess::BLACK && color != Chess::WHITE)
		throw gcnew ArgumentException(__MY_FUNCTION__ + safe_cast<int>(color).ToString());
	if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
		throw gcnew ArgumentOutOfRangeException(__MY_FUNCTION__ + x.ToString() + ", " + y.ToString());
	if (board[x, y] != Chess::AVAILABLE) return false;
	Chess opColor = (color == Chess::BLACK) ? Chess::WHITE : Chess::BLACK;
	MoveDetail^ md = gcnew MoveDetail();
	for (int i = 0; i < DIRECTIONS; i++) {
		int x0 = x + dx[i], y0 = y + dy[i];
		if (inBound(x0, y0)) {
			if (board[x0, y0] == opColor) {
				x0 += dx[i]; y0 += dy[i];
				while (inBound(x0, y0)) {
					if (board[x0, y0] != opColor) break;
					x0 += dx[i]; y0 += dy[i];
				}
				if (!inBound(x0, y0)) continue;
				if (board[x0, y0] != color) continue;
				md->endCount++;
				md->ends->Add(System::Drawing::Point(x0, y0));
				md->dirs->Add(i);
				x0 -= dx[i]; y0 -= dy[i];
				for (; x0 != x || y0 != y; x0 -= dx[i], y0 -= dy[i])
					board[x0, y0] = color;
			}
		}
	}
	if (md->endCount > 0) {
		board[x, y] = color;
		if (moves.Count > currentStep 
		&& (moves[currentStep].getX() != x || moves[currentStep].getY() != y || moves[currentStep].getColor() != color)) {
			moves.RemoveRange(currentStep, moves.Count - currentStep);
			moveDetails.RemoveRange(currentStep, moveDetails.Count - currentStep);
		}
		if (moves.Count == currentStep) {
			moves.Add(Move(x, y, color));
			moveDetails.Add(md);
		}
		currentStep++;
		updateState();
		return true;
	}
	return false;
}

int ChessBoard::isAvailable(int x, int y) {
	bool blackFound = false, whiteFound = false;
	for (int i = 0; i < DIRECTIONS; i++) {
		int x0 = x + dx[i], y0 = y + dy[i];
		if (inBound(x0, y0)) {
			if (!blackFound && board[x0, y0] == Chess::WHITE) { //handles BLACK side
				int x1 = x0 + dx[i], y1 = y0 + dy[i];
				while (inBound(x1, y1)) {
					if (board[x1, y1] != Chess::WHITE) break;
					x1 += dx[i]; y1 += dy[i];
				}
				if (!inBound(x1, y1)) continue;
				if (board[x1, y1] == Chess::BLACK) blackFound = true;
			} else if (!whiteFound && board[x0, y0] == Chess::BLACK) { //handles WHITE side
				int x1 = x0 + dx[i], y1 = y0 + dy[i];
				while (inBound(x1, y1)) {
					if (board[x1, y1] != Chess::BLACK) break;
					x1 += dx[i]; y1 += dy[i];
				}
				if (!inBound(x1, y1)) continue;
				if (board[x1, y1] == Chess::WHITE) whiteFound = true;
			}
		}
		if (blackFound && whiteFound) break;
	}
	return (blackFound ? 1 : 0) + (whiteFound ? 2 : 0);
}

void ChessBoard::updateState() {
	avBlack.Clear();
	avWhite.Clear();
	nBlack = 0;
	nWhite = 0;
	nEmpty = 0;
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < WIDTH; j++) {
			int av;
			switch(board[i, j]) {
			case Chess::AVAILABLE :
				av = isAvailable(i, j);
				if (av & 1) { //handles black side
					avBlack.Add(Point(i, j));
				}
				if (av & 2) { //handles white side
					avWhite.Add(Point(i, j));
				}
				nEmpty++;
				break;
			case Chess::BLACK :
				nBlack++;
				break;
			case Chess::WHITE :
				nWhite++;
				break;
			}
		}
	if (avBlack.Count == 0 && avWhite.Count == 0) { // no more moves to go, calculate the end result
		int empties = WIDTH * HEIGHT - nBlack - nWhite;
		if (nBlack > nWhite) // the winner gets the empties
			nBlack += empties;
		else if (nWhite > nBlack)
			nWhite += empties;
	}
}

void ChessBoard::goBack(int step) {
	if (step < 0 || step >= currentStep)
		throw gcnew System::ArgumentOutOfRangeException(__MY_FUNCTION__ + step.ToString());
	for (int i = 0; i < step; i++) {
		Move currentMove = moves[currentStep - 1];
		if (currentMove.getColor() != Chess::AVAILABLE) { // a valid move
			int x = currentMove.getX(), y = currentMove.getY();
			Chess opColor = (currentMove.getColor() == Chess::BLACK) ? Chess::WHITE : Chess::BLACK;
			board[x, y] = Chess::AVAILABLE;
			MoveDetail^ md = moveDetails[currentStep - 1];
			for (int j = 0; j < md->endCount; j++) {
				Point to = md->ends[j];
				int dir = md->dirs[j];
				int x1 = x + dx[dir], y1 = y + dy[dir];
				int x0 = to.X, y0 = to.Y;
				while (x1 != x0 || y1 != y0) {
					board[x1, y1] = opColor;
					x1 += dx[dir]; y1 += dy[dir];
				}
			}
		}
//		moves.RemoveAt(currentStep - 1);
//		moveDetails.RemoveAt(currentStep - 1);
		currentStep--;
	}
	updateState();
}

void ChessBoard::goOn(int step) {
	if (currentStep + step > moves.Count)
		throw gcnew System::ArgumentOutOfRangeException(__MY_FUNCTION__ + step.ToString());
	for (int i = 0; i < step; i++) {
		Move currentMove = moves[currentStep];
		if (currentMove.getColor() != Chess::AVAILABLE) { // a valid move
			int x = currentMove.getX(), y = currentMove.getY();
			Chess myColor = (currentMove.getColor() == Chess::BLACK) ? Chess::BLACK : Chess::WHITE;
			board[x, y] = myColor;
			MoveDetail^ md = moveDetails[currentStep];
			for (int j = 0; j < md->endCount; j++) {
				Point to = md->ends[j];
				int dir = md->dirs[j];
				int x1 = x + dx[dir], y1 = y + dy[dir];
				int x0 = to.X, y0 = to.Y;
				while (x1 != x0 || y1 != y0) {
					board[x1, y1] = myColor;
					x1 += dx[dir]; y1 += dy[dir];
				}
			}
		}
		currentStep++;
	}
	updateState();
}
