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
								BookLearning.cpp
		作者：Patrick
		概述：包含类 BookLearning 的定义。
			BookLearning 类实现“棋谱学习”窗体。

********************************************************************************
*/

#include "StdAfx.h"
#include "BookLearning.h"
#include "Solver.h"
#include "PbNoAnimation.h"

using namespace Othello;

BookLearning::BookLearning(GameContext ^gc) {
	InitializeComponent();
	//
	//TODO: Add the constructor code here
	//
	this->gc = gc;
	isDone = false;
	solver = NULL;
	DialogResult = Windows::Forms::DialogResult::OK;
}

BookLearning::~BookLearning() {
	if (components)
		delete components;
}

System::Void BookLearning::BookLearning_Load(System::Object ^sender, System::EventArgs ^e) {
	this->Cursor = Cursors::WaitCursor;
}

System::Void BookLearning::BookLearning_FormClosing(System::Object ^sender, System::Windows::Forms::FormClosingEventArgs ^e) {
	if (!isDone)
		e->Cancel = true;
	else
		this->Cursor = Cursors::Default;
}

void BookLearning::learnerStarter() {
	parseGame();
	solver->extendBook();
	//System::Threading::Thread::Sleep(500);
	isDone = true;
}

void BookLearning::parseGame() {
	Board ^board = gc->getInitialBoard();
	int bd[WIDTH * HEIGHT];
	Conversions::convertBoard(board, bd);
	int steps[WIDTH * HEIGHT];
	int pptr = 0;
	for (int i = 1; i < gc->getCurrentStep(); i++) {
		Othello::Move move = gc->getMove(i);
		if (move.getColor() != Chess::AVAILABLE)
			steps[pptr++] = move.getX() * HEIGHT + move.getY();
	}
	int fp = CraftEngine::Solver::AV;
	for (int i = 1; i < gc->getCurrentStep(); i++) {
		Othello::Move move = gc->getMove(i);
		if (move.getColor() != Chess::AVAILABLE) {
			fp = (move.getColor() == Chess::BLACK) ? CraftEngine::Solver::BLACK : CraftEngine::Solver::WHITE;
			break;
		}
	}
	if (fp != CraftEngine::Solver::AV)
		solver->parseGame(bd, steps, pptr, fp);
}

void BookLearning::doLearn() {
	using namespace System::Threading;
	solver = CraftEngine::Solver::newInstance();
	Thread ^learnerThread = gcnew Thread(gcnew ThreadStart(this, &BookLearning::learnerStarter));
	learnerThread->Start();
	while (!isDone) {
		System::Threading::Thread::Sleep(40);
		System::Windows::Forms::Application::DoEvents();
		if (solver)
			setProgressBarValueNoAnimation(pbLearn, solver->getBookPercent());
	}
	delete solver;
	solver = NULL;
	this->Close();
}

System::Void BookLearning::BookLearning_Shown(System::Object ^sender, System::EventArgs ^e) {
	this->ControlBox = false;
	BeginInvoke(gcnew SimpleDelegate(this, &BookLearning::doLearn));
}

System::Void BookLearning::BookLearning_FormClosed(System::Object ^sender, System::Windows::Forms::FormClosedEventArgs ^e) {

}
