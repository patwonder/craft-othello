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
								Game.h
		作者：Patrick
		概述：包含类 Game 的声明。
			Game 类封装了游戏流程，控制游戏全过程，以及将游戏中的事件
			（如走棋，悔棋，游戏结束）等信息通知各个观察者。

********************************************************************************
*/

#include "Common.h"
#include "ChessBoard.h"
#include "AbstractPlayer.h"
#include "GameContext.h"
#include "Observer.h"
#include "Timer.h"

namespace Othello {
public ref class DuplicatePlayerException : public System::Exception {
public:
	DuplicatePlayerException(System::String^ msg) : System::Exception(msg) {}
};
public ref class IllegalGameStateException : public System::Exception {
public:
	IllegalGameStateException(System::String^ msg) : System::Exception(msg) {}
};
public ref class Game {
private:
	ref class DefaultPlayer : public AbstractPlayer {
	private:
		static const int DEFAULT_DELAY = 1000;
	public:
		virtual void init(GameContext^ gc, Chess color) {}
		virtual int myTurn(GameContext^ gc, 
			Move lastMove) { 
				System::Random rnd;
				return rnd.Next(gc->getAvailableCount()); 
		}
		virtual void reset() {}
		virtual System::String^ getName() { return "Default Player"; }
		virtual int getStepDelay() {
			return DEFAULT_DELAY;
		}
		virtual void setStepDelay(int delay) {
		}
	};
	
	static AbstractPlayer^ const defaultPlayer = nullptr;

	ChessBoard^ chessBoard;
	AbstractPlayer^ black;
	AbstractPlayer^ white;
	GameContext^ gcBlack;
	GameContext^ gcWhite;
	Timer^ tmrBlack;
	Timer^ tmrWhite;
	int timeLimit;
	Chess^ firstPlayer;
	bool gameStarted;
	bool gameReseted;
	bool insideGoOn;
	Chess^ currentPlayer;
	System::Collections::Generic::List<Observer^> observerList;
	Move lastMove;
	bool learnable;

	//Forbid illegal construction
	Game(const Game^) {}
	Game^ operator=(const Game^) { return nullptr; }
	void setLearnable(Board^ board);
public:
	//consturctors:
	Game(AbstractPlayer^ pBlack, AbstractPlayer^ pWhite) {
		this->Game::Game(pBlack, pWhite, ChessBoard::getDefaultBoard(1), Chess::BLACK, 0);
	}
	Game(AbstractPlayer^ pBlack, AbstractPlayer^ pWhite, int totalTimeLimit) {
		this->Game::Game(pBlack, pWhite, ChessBoard::getDefaultBoard(1), Chess::BLACK, totalTimeLimit);
	}
	Game(AbstractPlayer^ pBlack, AbstractPlayer^ pWhite, Board^ board) {
		this->Game::Game(pBlack, pWhite, board, Chess::BLACK, 0);
	}
	Game(AbstractPlayer^ pBlack, AbstractPlayer^ pWhite, Board^ board, Chess firstPlayer) {
		this->Game::Game(pBlack, pWhite, board, firstPlayer, 0);
	}
	Game(AbstractPlayer^ pBlack, AbstractPlayer^ pWhite, Board^ board, Chess firstPlayer, int totalTimeLimit);

	//destructor: (may not be used)
	~Game();

	void startGame();
	void resetGame();
	void endGame();
	void goOn();
	void goBack() { goBack(1); }
	void goBack(int step);
	void addObserver(Observer^ observer);
	void delObserver(Observer^ observer);
	bool isStarted() {
		return gameStarted;
	}
	bool canGoOn() {
		return !insideGoOn;
	}
	int getMaxStep() { return chessBoard->getMaxStep(); }
	Board^ getInitialBoard();
	Chess getFirstPlayer() { return *firstPlayer; }
	void goForward() { goForward(1); }
	void goForward(int step);
	void changeGame(Board^ board) { changeGame(board, *firstPlayer); }
	void changeGame(Board^ board, Chess firstPlayer);
	void changeGame(AbstractPlayer^ pBlack, AbstractPlayer^ pWhite);
	void changeGame(int newTimeLimit);
	bool parseGame(Board^ board, array<System::Drawing::Point>^ moveSequence, int moveCount, Chess first);
	bool parseGame(Board^ board, array<Othello::Move>^ moveSequence, int moveCount, Chess first);
	static bool checkGame(Board^ board, array<System::Drawing::Point>^ moveSequence, int moveCount, Chess first);
	static bool checkGame(Board^ board, array<Othello::Move>^ moveSequence, int moveCount, Chess first);
	
	bool isLearnable();
	GameContext^ getContext(Chess color) {
		switch (color) {
		case Chess::BLACK :
			return gcBlack;
		case Chess::WHITE :
			return gcWhite;
		default:
			throw gcnew System::ArgumentException(__MY_FUNCTION__ + "Illegal player color.");
		}
	}

	void pauseGame();
};
}//namespace Othello