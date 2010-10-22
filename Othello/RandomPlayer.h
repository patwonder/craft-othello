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
								RandomPlayer.h
		作者：Patrick
		概述：包含类 RandomPlayer 的声明。
			RandomPlayer 类实现根据规则随机走棋的黑白棋电脑玩家。

********************************************************************************
*/

#include "AbstractPlayer.h"

namespace Othello {

int GetTickCount();

public ref class RandomPlayer : public AbstractPlayer {
private:
	static System::Random rnd;
	int stepDelay;
	System::String^ name;
	Chess color;
public:
	RandomPlayer(int delay) : stepDelay(delay) {
		name = "随机";
	}
	virtual void init(GameContext^ gc, Chess color) {
		/*
		using System::Diagnostics::Debug;
		Debug::WriteLine("Initialized " + name + ". I am the " 
			+ (color == Chess::BLACK ? "Black" : "White") + " side.");
		Debug::WriteLine("  The opponent's name is " + gc->getOpponentName());
		if (gc->getTotalTimeLimit() == 0)
			Debug::WriteLine("  No time limit for this game.");
		else
			Debug::WriteLine("  The time limit for this game is " + gc->getTotalTimeLimit() + " ms.");
		*/
	}
	virtual int myTurn(GameContext^ gc, Move lastMove) {
		/*
		using System::Diagnostics::Debug;
		Debug::WriteLine(name + ": " + "My turn, current step : " + gc->getCurrentStep());
		Debug::WriteLine("  Available positions: " + gc->getAvailableCount());
		*/

		if (gc->getAvailableCount() == 0) return 0;
		/*
		for (int i = 0; i < gc->getAvailableCount(); i++)
			Debug::Write(gc->getAvailableMove(i) + " ");
		Debug::WriteLine("");
		*/
		int index = rnd.Next(gc->getAvailableCount());
		/*
		Debug::WriteLine("  Take " + gc->getAvailableMove(index));
		*/
		return index;
	}
	virtual void reset() {
		/*
		using System::Diagnostics::Debug;
		Debug::WriteLine(name + ": Game reseted.");
		*/
	}
	virtual System::String^ getName() {
		return name;
	}
	virtual int getStepDelay() {
		return stepDelay;
	}
	virtual void setStepDelay(int delay) {
		stepDelay = delay;
	}
};

}
