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
								Craft.h
		作者：Patrick
		概述：包含类 Craft 的声明。
			Craft 类实现使用 CraftEngine 走棋的黑白棋AI玩家。

********************************************************************************
*/

#include "AbstractPlayer.h"
#include "Setting.h"
#include "SearchRelated.h"
using namespace System;

namespace CraftEngine {
class Solver;
class SolverResult;
} // namespace CraftEngine

namespace Othello {

public ref class Craft : public AbstractPlayer {
private:
	static System::Random^ random = gcnew System::Random();
	int myColor, opColor;
	int empties;
	static const int PV_LENGTH = 20;
	SearchOptions options;
	UserInfo^ userInfo;
	System::String^ myName;
	SearchDisplayer^ displayer;
	CraftEngine::Solver* solver;
	volatile bool isDone;
	bool forced;
	bool terminated;
	bool endSolve;
	CraftEngine::SolverResult* res;
	int percentage;
	System::Threading::Thread^ solverThread;
	System::Threading::ThreadStart^ solverThreadStart;
	int focusedMove;
	int selectedMove;

	System::String^ pvToString(int pv[], int len);
	void solverStarter();
	System::String^ getResultDescription();
	System::String^ getPartialResultDescription(int partialResult, int partialDepth, int percentage);
	System::String^ getSpeedDescription(int speed);
	System::String^ getTotalNumDescription(unsigned long long totalNum);
	void resetComponents();
	void setFocusedMove(int move);
	void setSelectedMove(int move);
public:
	Craft(SearchDisplayer^ displayer, SearchOptions options, 
		  System::String^ name, UserInfo^ userInfo);
	virtual void init(GameContext^ gc, Chess color);
	virtual int myTurn(GameContext^ gc, Move lastMove);
	virtual void reset();
	virtual System::String^ getName();
	virtual int getStepDelay() {
		return userInfo->StepDelay;
	}
	virtual void setStepDelay(int delay) {}
	void forceMove();
	int getTip(GameContext^ gc, Move lastMove, bool endSolve);
	SearchOptions getSearchOptions() { return options; }
	~Craft();
	!Craft();
};

}
