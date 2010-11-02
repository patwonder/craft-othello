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

// 这是主 DLL 文件。

#include "stdafx.h"

#include "CraftEngineNet.h"
#include "Conversions.h"

using namespace CraftEngineNet;
using namespace CraftEngine;
using namespace System;

Engine::Engine() {
	solver = Solver::newInstance();
}

void Engine::abortSearch(System::Threading::Thread ^searchThread) {
	solver->abortSearch();
	searchThread->Join();
	solver->abortSearchComplete();
}

void Engine::cleanup() {
	Solver::cleanup();
}

void Engine::clearCache() {
	Solver::clearCache();
}

void Engine::clearGame() {
	solver->clearGame();
}

void Engine::clearSearchStats() {
	solver->clearSearchStats();
}

void Engine::extendBook() {
	solver->extendBook();
}

int Engine::getBookPercent() {
	return solver->getBookPercent();
}

size_t Engine::getCacheSize() {
	return Solver::getCacheSize();
}

SearchStats Engine::getSearchStats() {
	unsigned long long evnum = solver->getEvNum();
	int p = solver->getFocusedMove();
	Move focused = Conversions::intToMove(p);
	p = solver->getSelectedMove();
	Move selected = Conversions::intToMove(p);
	int percent = solver->getPercent();
	int pDepth = solver->getPartialDepth();
	int pResult = solver->getPartialResult();
	int percentage = solver->getEpcPercentage();
	return SearchStats(evnum, focused, selected, percent,
		pDepth, pResult, percentage);
}

int Engine::getMobility(Chess side) {
	return solver->getMobility(Conversions::chessToInt(side));
}

void Engine::initialize() {
	if (!Solver::initialize())
		throw gcnew InitializationException("Craft engine failed to initialize.");
}

void Engine::initialize(String^ patternPath, String^ bookPath) {
	char* bp = (char*)(int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(bookPath);
	char* pp = (char*)(int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(patternPath);
	bool init = Solver::initialize(pp, bp);
	System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)bp);
	System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)pp);
	if (!init)
		throw gcnew InitializationException("Craft engine failed to initialize.");
}

void Engine::setBoard(Board board) {
	int bd[WIDTH * HEIGHT];
	Conversions::convertBoard(board, bd);

	solver->setBoard(bd);
}

void Engine::makeMove(Move point, Chess side) {
	int pos = Conversions::moveToInt(point);
	int color = Conversions::chessToInt(side);
	solver->makeMove(pos, color);
}

void Engine::parseGame(Board board, array<Move>^ steps) {
	Chess lFirstPlayer = Chess::BLACK;
	parseGame(board, steps, lFirstPlayer);
}

void Engine::parseGame(Board board, array<Move>^ steps, Chess firstPlayer) {
	int bd[WIDTH * HEIGHT];
	Conversions::convertBoard(board, bd);
	int length = steps->Length;
	int* st = new int[length];
	for (int i = 0; i < length; i++) {
		st[i] = Conversions::moveToInt(steps[i]);
	}
	int fp = Conversions::chessToInt(firstPlayer);
	solver->parseGame(bd, st, length, fp);
	delete[] st;
}

void Engine::saveBook() {
	Solver::saveBook();
}

void Engine::setBookTolerance(int tolerance) {
	solver->setBookTolerance(tolerance);
}

void Engine::setCacheSize(size_t newCacheSize) {
	try {
		Solver::setCacheSize(newCacheSize);
	} catch (...) {
		throw gcnew System::OutOfMemoryException("Craft engine failed to allocate memory.");
	}
}

SearchResult Engine::solve(Chess side, int depth) {
	int color = Conversions::chessToInt(side);
	SolverResult sr = solver->solve(color, depth);
	return SearchResult(Conversions::intToMove(sr.getBestMove()), sr.getResult(), sr.getBestMove() < 0);
}

SearchResult Engine::solve(Chess side, int depth, bool useBook) {
	int color = Conversions::chessToInt(side);
	SolverResult sr = solver->solve(color, depth, useBook);
	return SearchResult(Conversions::intToMove(sr.getBestMove()), sr.getResult(), sr.getBestMove() < 0);
}

SearchResult Engine::solveBigEat(Chess side, int depth) {
	int color = Conversions::chessToInt(side);
	SolverResult sr = solver->solveBigEat(color, depth);
	return SearchResult(Conversions::intToMove(sr.getBestMove()), sr.getResult(), sr.getBestMove() < 0);
}

SearchResult Engine::solveExact(Chess side, bool winLoss) {
	int color = Conversions::chessToInt(side);
	SolverResult sr = solver->solveExact(color, winLoss);
	return SearchResult(Conversions::intToMove(sr.getBestMove()), sr.getResult(), sr.getBestMove() < 0);
}

SearchResult Engine::partialSolveExact(Chess side, bool winLoss, int percentage, int% actualPercentage) {
	int ap;
	int color = Conversions::chessToInt(side);
	SolverResult sr = solver->partialSolveExact(color, winLoss, percentage, ap);
	actualPercentage = ap;
	return SearchResult(Conversions::intToMove(sr.getBestMove()), sr.getResult(), sr.getBestMove() < 0);
}

void Engine::unMakeMove() {
	solver->unMakeMove();
}

Engine::~Engine() {
	this->!Engine();
}

Engine::!Engine() {
	delete solver;
	solver = NULL;
}

