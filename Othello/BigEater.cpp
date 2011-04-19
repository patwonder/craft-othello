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
								BigEater.cpp
		作者：Patrick
		概述：包含类 BigEater 的定义。
			BigEater 类实现根据大食策略走棋的黑白棋AI玩家。

********************************************************************************
*/

#include "StdAfx.h"
#include "BigEater.h"
#include "Solver.h"

using namespace Othello;
using namespace System::Threading;
using namespace CraftEngine;

BigEater::BigEater(int delay) : stepDelay(delay) {
	res = new SolverResult(0, -1);
	solver = Solver::newInstance();
	solverThreadStart = gcnew ThreadStart(this, &BigEater::solverStarter);
	isDone = false;
}

void BigEater::solverStarter() {
	try {
		(*res) = solver->solveBigEat(myColor, 2);
		isDone = true;
	} catch (System::Exception^) {
		System::Diagnostics::Debug::WriteLine("Search thread aborted.");
	}
}

void BigEater::init(GameContext^ gc, Chess color) {
	int board[Solver::MAXSTEP];
	for (int i = 0; i < Solver::MAXSTEP; i++)
		switch (gc->get(i / 8, i % 8)) {
		case Chess::BLACK :
			board[i] = Solver::BLACK;
			break;
		case Chess::WHITE :
			board[i] = Solver::WHITE;
			break;
		case Chess::AVAILABLE :
			board[i] = Solver::AV;
		}
	solver->setBoard(board);
	myColor = (color == Chess::BLACK) ? Solver::BLACK : Solver::WHITE;
	opColor = Solver::BLACK + Solver::WHITE - myColor;
}

int BigEater::myTurn(GameContext ^gc, Move lastMove) {
	terminated = false;
	if (lastMove.getColor() != Chess::AVAILABLE)
		solver->makeMove(lastMove.getX() * HEIGHT + lastMove.getY(), opColor);
	if (gc->getAvailableCount() == 0) return 0;
	if (gc->getAvailableCount() == 1) {
		System::Drawing::Point myMove = gc->getAvailableMove(0);
		solver->makeMove(myMove.X * HEIGHT + myMove.Y, myColor);
		return 0;
	}
	isDone = false;
	//solverThread = gcnew Thread(solverThreadStart);
	//solverThread->Start();
	//while (!isDone) {
	//	System::Windows::Forms::Application::DoEvents();
	//	if (terminated) return 0;
	//	Thread::Sleep(40);
	//}
	// Since little work to be done, we call solverStarter directly in this thread
	solverStarter();
	solver->makeMove(res->getBestMove(), myColor);
	for (int i = 0; i < gc->getAvailableCount(); i++) {
		System::Drawing::Point thisMove = gc->getAvailableMove(i);
		if (thisMove.X * HEIGHT + thisMove.Y == res->getBestMove())
			return i;
	}
	// it will never get here
	return 0;
}

BigEater::~BigEater() {
	this->!BigEater();
}

BigEater::!BigEater() {
	delete solver;
	delete res;
	solver = NULL;
	res = NULL;
}
