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

#pragma once
/*
********************************************************************************
								ChessBoard.h
		作者：Patrick
		概述：包含类 ChessBoard 的声明。
			ChessBoard 类封装了黑白棋棋盘表示、走棋规则，提供简单、
			易于使用的接口。

********************************************************************************
*/

#include "Common.h"

namespace Othello {
public ref class ChessBoard {
private:
	static const int DIRECTIONS = 8;
	static array<int>^ dx = gcnew array<int>(DIRECTIONS) {1, 1, 0, -1, -1, -1, 0, 1};
	static array<int>^ dy = gcnew array<int>(DIRECTIONS) {0, -1, -1, -1, 0, 1, 1, 1};

	ref struct MoveDetail {
		int endCount;
		System::Collections::Generic::List<System::Drawing::Point>^ ends;
		System::Collections::Generic::List<int>^ dirs;
		MoveDetail() {
			ends = gcnew System::Collections::Generic::List<System::Drawing::Point>(DIRECTIONS);
			dirs = gcnew System::Collections::Generic::List<int>(DIRECTIONS);
			endCount = 0;
		}
	};

	int currentStep;
	int nBlack, nWhite, nEmpty;
	Board^ board, ^initialBoard;
	System::Collections::Generic::List<Move> moves;
	System::Collections::Generic::List<MoveDetail^> moveDetails;
	System::Collections::Generic::List<System::Drawing::Point> avBlack;
	System::Collections::Generic::List<System::Drawing::Point> avWhite;
	
	int isAvailable(int x, int y);
	void updateState();
	static Board^ defaultBoard1 = gcnew Board(WIDTH, HEIGHT) {
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::WHITE,     Chess::BLACK,     Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::BLACK,     Chess::WHITE,     Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE}
}; // defines the default chessboard 1
	static Board^ defaultBoard2 = gcnew Board(WIDTH, HEIGHT) {
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::BLACK,     Chess::WHITE,     Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::WHITE,     Chess::BLACK,     Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE},
	{Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE, Chess::AVAILABLE}
}; // defines the default chessboard 2

public:	
	//EFFECTS: if type is neither 1 nor 2 throws System::ArgumentException,
	//  otherwise returns the default chessboard No. type, as defined above
	static Board^ getDefaultBoard(int type) {
		switch (type) {
		case 1:
			return safe_cast<Board^>(defaultBoard1->Clone());
		case 2:
			return safe_cast<Board^>(defaultBoard2->Clone());
		default:
			throw gcnew System::ArgumentException(__MY_FUNCTION__ + "Invalid board type \"" + type + "\"");
		}
	}

	//MODIFIES: this
	//EFFECTS: if x is not in the range 0..WIDTH-1 or y is not in the range 0..HEIGHT-1 throws System::ArgumentOutOfRangeException;
	//  if color is neither Chess::BLACK nor Chess::WHITE throws System::ArgumentException;
	//  otherwise put a chess at (x, y) of the chessboard with the specified color,
	//  if successful, returns true, else returns false
	bool putChess(int x, int y, Chess color);

	//MODIFIES: this
	//EFFECTS: add an invalid move to the moves list
	void recordInvalidMove(Chess color_reserved) {
		if (moves.Count > currentStep 
		&& moves[currentStep].getColor() != Chess::AVAILABLE) {
			moves.RemoveRange(currentStep, moves.Count - currentStep);
			moveDetails.RemoveRange(currentStep, moveDetails.Count - currentStep);
		}
		if (moves.Count == currentStep) {
			moves.Add(Move::invalidMove);
			moveDetails.Add(nullptr);
		}
		currentStep++;
	}

	//EFFECTS: if x is not in the range 0..WIDTH-1 or y is not in the range 0..HEIGHT-1
	//  throws System::ArgumentOutOfRangeException, otherwise returns the color at (x, y) of the chessboard
	Chess get(int x, int y) {
		if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0) throw gcnew System::ArgumentOutOfRangeException(__MY_FUNCTION__ + x + "," + y);
		return board[x, y];
	}

	//EFFECTS: if step is not in the range 0..(getMaxStep() - 1) throws System::ArgumentOutOfRangeException,
	//  otherwise returns the step-th move of this game
	Move getMove(int step) {
		if (step < 0 || step >= moves.Count) throw gcnew System::ArgumentOutOfRangeException(__MY_FUNCTION__ + step.ToString());
		return moves[step];
	}

	//EFFECTS: returns the step number of current step
	int getCurrentStep() { return currentStep; }

	//EFFECTS: returns the farthest step that can currently be seen
	int getMaxStep() { return moves.Count; }

	//EFFECTS: if index is not in the range 1..getAvailableCount(color) or color is neither Chess::BLACK nor Chess::WHITE
	//  throws System::ArgumentException; if index is not in the range 0..(getAvailableCount() - 1) throws System::ArgumentOutOfRangeException;
	//  otherwise returns the index-th available move for the current step
	System::Drawing::Point getAvailableMove(Chess color, int index) {
		if (index < 0 || index >= getAvailableCount(color))
			throw gcnew System::ArgumentOutOfRangeException(__MY_FUNCTION__ + index.ToString());
		if (color == Chess::BLACK)
			return avBlack[index];
		else
			return avWhite[index];
	}

	//EFFECTS: if color is neither Chess::BLACK nor Chess::WHITE throws System::ArgumentException,
	//  otherwise returns the number of all available moves for the current step
	int getAvailableCount(Chess color) {
		switch (color) {
			case Chess::BLACK : return avBlack.Count;
			case Chess::WHITE : return avWhite.Count;
			default : throw gcnew System::ArgumentException(__MY_FUNCTION__ + safe_cast<int>(color).ToString());
		}
	}
	
	//MODIFIES: this
	//EFFECTS: If step is not in the range 0..(currentStep - 1) throws System::ArgumentOutOfRangeException,
	//  otherwise set this to specified steps earlier.
	void goBack(int step);

	//MODIFIES: this
	//EFFECTS: If step is not in the range 0..(moves.Count - currentStep) throws System::ArgumentOutOfRangeException,
	//  otherwise set this to specified steps later.
	void goOn(int step);

	//constructor:
	ChessBoard() {
		this->ChessBoard::ChessBoard(defaultBoard1);
	}

	ChessBoard(Board^ board) {
		this->board = safe_cast<Board^>(board->Clone());
		initialBoard = safe_cast<Board^>(board->Clone());
		moves.Add(Move::invalidMove);
		moveDetails.Add(gcnew MoveDetail());
		currentStep = 1;
		updateState();
	}

	void changeBoard(Board^ board) {
		moves.Clear();
		moves.Add(Move::invalidMove);
		moveDetails.Clear();
		moveDetails.Add(nullptr);
		this->board = safe_cast<Board^>(board->Clone());
		initialBoard = safe_cast<Board^>(board->Clone());
		currentStep = 1;
		updateState();
	}

	void resetBoard() {
		moves.Clear();
		moves.Add(Move::invalidMove);
		moveDetails.Clear();
		moveDetails.Add(nullptr);
		this->board = safe_cast<Board^>(initialBoard->Clone());
		currentStep = 1;
		updateState();
	}
	
	//EFFECTS: if color is neither Chess::BLACK nor Chess::WHITE returns the number of empty squares,
	//  otherwise returns the number of chess for the specified player
	int getNumber(Chess color) {
		switch (color) {
			case Chess::BLACK : return nBlack;
			case Chess::WHITE : return nWhite;
			default : return nEmpty;
		}
	}

	bool isGameEnded() {
		return (avBlack.Count == 0 && avWhite.Count == 0);
	}

	Move getLastMove() {
		return moves[currentStep - 1];
	}

	Board^ getInitialBoard() {
		return safe_cast<Board^>(initialBoard->Clone());
	}
};
}//namespace Othello