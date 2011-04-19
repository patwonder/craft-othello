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
								Game.cpp
		作者：Patrick
		概述：包含类 Game 的定义。
			Game 类封装了游戏流程，控制游戏全过程，以及将游戏中的事件
			（如走棋，悔棋，游戏结束）等信息通知各个观察者。

********************************************************************************
*/

#include "StdAfx.h"
#include "Game.h"
#include "Common.h"

using namespace Othello;
using namespace System;
using System::Drawing::Point;
using System::Diagnostics::Debug;

Game::Game(AbstractPlayer^ pBlack, AbstractPlayer^ pWhite, Board^ board, Chess firstPlayer, int totalTimeLimit)
: chessBoard(gcnew ChessBoard(board)), black(pBlack), white(pWhite), timeLimit(totalTimeLimit)
, tmrBlack(gcnew Timer(totalTimeLimit)), tmrWhite(gcnew Timer(totalTimeLimit)) {
	if (firstPlayer != Chess::BLACK && firstPlayer != Chess::WHITE)
		throw gcnew ArgumentException(__MY_FUNCTION__ + safe_cast<int>(firstPlayer).ToString());
	if ((board->GetUpperBound(0) != WIDTH - 1) || (board->GetUpperBound(1) != HEIGHT - 1)
		|| (board->GetLowerBound(0) != 0) || (board->GetLowerBound(1) != 0))
		throw gcnew ArgumentException(__MY_FUNCTION__ + "Board size doesn't fit.");
	this->firstPlayer = gcnew Chess(firstPlayer);
	currentPlayer = gcnew Chess(firstPlayer);
	setLearnable(board);
	if (black == defaultPlayer) black = gcnew DefaultPlayer();
	if (white == defaultPlayer) white = gcnew DefaultPlayer();
	if (black == white) throw gcnew DuplicatePlayerException(__MY_FUNCTION__ + "Both player references are the same.");
	gcBlack = gcnew GameContext(chessBoard, white->getName(), Chess::BLACK, currentPlayer, this->firstPlayer, tmrBlack);
	gcWhite = gcnew GameContext(chessBoard, black->getName(), Chess::WHITE, currentPlayer, this->firstPlayer, tmrWhite);
	gameStarted = false;
	gameReseted = false;
	insideGoOn = false;
}

Game::~Game() {

}

void Game::startGame() {
	if (gameStarted)
		throw gcnew IllegalGameStateException(__MY_FUNCTION__ + "Game already started.");
	if (chessBoard->isGameEnded())
		throw gcnew IllegalGameStateException(__MY_FUNCTION__ + "Game already ended.");
	tmrBlack->start();
	black->init(gcBlack, Chess::BLACK);
	tmrBlack->stop();
	if (tmrBlack->isOverTimed()) {
		for each (Observer^ observer in observerList) {
			observer->playerOverTimed(Chess::BLACK, gcBlack, gcWhite);
		}
	}
	tmrWhite->start();
	white->init(gcWhite, Chess::WHITE);
	tmrWhite->stop();
	if (tmrWhite->isOverTimed()) {
		for each (Observer^ observer in observerList) {
			observer->playerOverTimed(Chess::WHITE, gcBlack, gcWhite);
		}
	}
	lastMove = Move::invalidMove;
	gameStarted = true;
	for each (Observer^ observer in observerList) {
		observer->gameStarted(*firstPlayer, gcBlack, gcWhite);
	}
}

void Game::resetGame() {
	black->reset();
	white->reset();
	chessBoard->resetBoard();
	(*currentPlayer) = (*firstPlayer);
	tmrBlack->reset();
	tmrWhite->reset();
	gameStarted = false;
	if (insideGoOn) gameReseted = true;
	for each (Observer^ observer in observerList) {
		observer->gameReseted((*firstPlayer), gcBlack, gcWhite);
	}
}

void Game::endGame() {
	black->reset();
	white->reset();
	gameStarted = false;
	if (insideGoOn) gameReseted = true;
	Chess winner;
	int nBlack = chessBoard->getNumber(Chess::BLACK);
	int nWhite = chessBoard->getNumber(Chess::WHITE);
	if (nBlack > nWhite) winner = Chess::BLACK;
	else if (nBlack < nWhite) winner = Chess::WHITE;
	else winner = Chess::AVAILABLE;
	if (tmrBlack->isOverTimed() && !tmrWhite->isOverTimed()) {
		nBlack = 0;
		nWhite = WIDTH * HEIGHT;
		winner = Chess::WHITE;
	}
	if (tmrWhite->isOverTimed() && !tmrBlack->isOverTimed()) {
		nBlack = WIDTH * HEIGHT;
		nWhite = 0;
		winner = Chess::BLACK;
	}
	for each (Observer^ observer in observerList) {
		observer->gameEnded(winner, nBlack, nWhite, gcBlack, gcWhite);
	}
}

void Game::goOn() {
	if (!gameStarted)
		throw gcnew IllegalGameStateException(__MY_FUNCTION__ + "Game not yet started.");
	insideGoOn = true;
	int startTick = GetTickCount();
	int stepDelay = 0;
	int pos = 0;
	if ((*currentPlayer) == Chess::BLACK) {
		stepDelay = black->getStepDelay();
		tmrBlack->start();
		pos = black->myTurn(gcBlack, lastMove);
		tmrBlack->stop();
		if (gameReseted) {
			gameReseted = false;
			insideGoOn = false;
			return;
		}
		if (tmrBlack->isOverTimed())
			for each (Observer^ observer in observerList)
				observer->playerOverTimed(Chess::BLACK, gcBlack, gcWhite);
	} else {
		stepDelay = white->getStepDelay();
		tmrWhite->start();
		pos = white->myTurn(gcWhite, lastMove);
		tmrWhite->stop();
		if (gameReseted) {
			gameReseted = false;
			insideGoOn = false;
			return;
		}
		if (tmrWhite->isOverTimed())
			for each (Observer^ observer in observerList)
				observer->playerOverTimed(Chess::WHITE, gcBlack, gcWhite);
	}

	int avCount = chessBoard->getAvailableCount(*currentPlayer);

	if (avCount != 0) {
		while (GetTickCount() - startTick < stepDelay && !gameReseted) {
			System::Threading::Thread::Sleep(40);
			System::Windows::Forms::Application::DoEvents();
		}
		if (gameReseted) {
			gameReseted = false;
			insideGoOn = false;
			return;
		}
	}

	if (avCount == 0) {
		chessBoard->recordInvalidMove(*currentPlayer);
		lastMove = Move::invalidMove;
	}
	else if (pos >= avCount || pos < 0) {
		System::Windows::Forms::MessageBox::Show((((*currentPlayer) == Chess::BLACK) ? "Black" : "White") 
			+ " side failed to provide a reasonable move.");
	} else {
		Point p = chessBoard->getAvailableMove(*currentPlayer, pos);
		chessBoard->putChess(p.X, p.Y, *currentPlayer);
		lastMove = chessBoard->getLastMove();
	}
	
	(*currentPlayer) = ((*currentPlayer) == Chess::BLACK)? Chess::WHITE : Chess::BLACK;

	for each (Observer^ observer in observerList)
		observer->playerMoved(lastMove, gcBlack, gcWhite);

	if (chessBoard->isGameEnded())
		endGame();

	insideGoOn = false;
	gameReseted = false;
}

void Game::goBack(int step) {
	black->reset(); white->reset();
	chessBoard->goBack(step);
	gameStarted = false;
	if (insideGoOn)	gameReseted = true;
	if (step & 1)
		if ((*currentPlayer) == Chess::BLACK) {
			(*currentPlayer) = Chess::WHITE;
		} else {
			(*currentPlayer) = Chess::BLACK;
		}
	for each (Observer^ observer in observerList)
		observer->gameGoneBack(step, gcBlack, gcWhite);
}

void Game::goForward(int step) {
	black->reset(); white->reset();
	chessBoard->goOn(step);
	gameStarted = false;
	if (insideGoOn) gameReseted = true;
	if (step & 1)
		if ((*currentPlayer) == Chess::BLACK) {
			(*currentPlayer) = Chess::WHITE;
		} else {
			(*currentPlayer) = Chess::BLACK;
		}
	for each (Observer^ observer in observerList)
		observer->gameGoneForward(step, gcBlack, gcWhite);
}

void Game::pauseGame() {
	black->reset(); white->reset();
	gameStarted = false;
	if (insideGoOn) gameReseted = true;
	for each (Observer^ observer in observerList)
		observer->gameHasBeenPaused(*currentPlayer, gcBlack, gcWhite);
}

void Game::addObserver(Observer^ observer) {
	if (observerList.Contains(observer)) return;
	observerList.Add(observer);
}

void Game::delObserver(Observer^ observer) {
	int index = observerList.IndexOf(observer);
	if (index < 0) return;
	observerList.RemoveAt(index);
}

void Game::changeGame(Board^ board, Chess firstPlayer) {
	black->reset(); white->reset();
	*(this->firstPlayer) = firstPlayer;
	(*currentPlayer) = firstPlayer;
	chessBoard->changeBoard(board);
	tmrBlack->reset();
	tmrWhite->reset();
	gameStarted = false;
	if (insideGoOn) gameReseted = true;
	for each (Observer^ observer in observerList) {
		observer->boardChanged(firstPlayer, gcBlack, gcWhite);
	}
}

void Game::changeGame(AbstractPlayer^ pBlack, AbstractPlayer^ pWhite) {
	black->reset();
	white->reset();
	black = pBlack; white = pWhite;
	if (black == nullptr) black = gcnew DefaultPlayer();
	if (white == nullptr) white = gcnew DefaultPlayer();
	gcBlack = gcnew GameContext(chessBoard, white->getName(), Chess::BLACK, currentPlayer, firstPlayer, tmrBlack);
	gcWhite = gcnew GameContext(chessBoard, black->getName(), Chess::WHITE, currentPlayer, firstPlayer, tmrWhite);
	gameStarted = false;
	if (insideGoOn) gameReseted = true;
	for each (Observer^ observer in observerList) {
		observer->playerChanged(*firstPlayer, gcBlack, gcWhite);
	}
}

void Game::changeGame(int newTimeLimit) {
	black->reset(); white->reset();
	timeLimit = newTimeLimit;
	tmrBlack->reset(newTimeLimit);
	tmrWhite->reset(newTimeLimit);
	gameStarted = false;
	if (insideGoOn) gameReseted = true;
	for each (Observer^ observer in observerList) {
		observer->timeLimitChanged(timeLimit, gcBlack, gcWhite);
	}
}

bool Game::isLearnable() {
	return learnable;
}

bool Game::parseGame(Board ^board, cli::array<Othello::Move> ^moveSequence, int moveCount, Chess first) {
	if (!checkGame(board, moveSequence, moveCount, first)) return false;
	black->reset(); white->reset();
	chessBoard->changeBoard(board);
	(*currentPlayer) = (*firstPlayer) = first;
	setLearnable(board);
	for (int i = 0; i < moveCount; i++) {
		if (chessBoard->getAvailableCount((*currentPlayer)) == 0) {
			chessBoard->recordInvalidMove(*currentPlayer);
			(*currentPlayer) = ((*currentPlayer) == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
		}
		chessBoard->putChess(moveSequence[i].getX(), moveSequence[i].getY(), (*currentPlayer));
		(*currentPlayer) = ((*currentPlayer) == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
	}
	if (!chessBoard->isGameEnded() && chessBoard->getAvailableCount((*currentPlayer)) == 0) {
		chessBoard->recordInvalidMove(*currentPlayer);
		(*currentPlayer) = ((*currentPlayer) == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
	}
	gameStarted = false;
	if (insideGoOn) gameReseted = true;
	for each (Observer^ observer in observerList) {
		observer->boardChanged(*firstPlayer, gcBlack, gcWhite);
	}
	return true;
}

bool Game::parseGame(Board ^board, cli::array<System::Drawing::Point> ^moveSequence, int moveCount, Chess first) {
	if (!checkGame(board, moveSequence, moveCount, first)) return false;
	black->reset(); white->reset();
	chessBoard->changeBoard(board);
	(*currentPlayer) = (*firstPlayer) = first;
	setLearnable(board);
	for (int i = 0; i < moveCount; i++) {
		if (chessBoard->getAvailableCount((*currentPlayer)) == 0) {
			chessBoard->recordInvalidMove(*currentPlayer);
			(*currentPlayer) = ((*currentPlayer) == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
		}
		chessBoard->putChess(moveSequence[i].X, moveSequence[i].Y, (*currentPlayer));
		(*currentPlayer) = ((*currentPlayer) == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
	}
	if (!chessBoard->isGameEnded() && chessBoard->getAvailableCount((*currentPlayer)) == 0) {
		chessBoard->recordInvalidMove(*currentPlayer);
		(*currentPlayer) = ((*currentPlayer) == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
	}
	gameStarted = false;
	if (insideGoOn) gameReseted = true;
	for each (Observer^ observer in observerList) {
		observer->boardChanged(*firstPlayer, gcBlack, gcWhite);
	}
	return true;
}

bool Game::checkGame(Board ^board, cli::array<Othello::Move> ^moveSequence, int moveCount, Chess first) {
	if (first == Chess::AVAILABLE) return false;
	Chess current = first;
	ChessBoard^ temp = gcnew ChessBoard(board);
	for (int i = 0; i < moveCount; i++) {
		if (temp->getAvailableCount(current) == 0)
			current = (current == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
		if (temp->isGameEnded()) return false;
		if (current != moveSequence[i].getColor()) return false;
		if (temp->getAvailableCount(current) == 0) return false;
		if (!temp->putChess(moveSequence[i].getX(), moveSequence[i].getY(), current)) return false;
		current = (current == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
	}
	return true;
}

bool Game::checkGame(Board ^board, cli::array<System::Drawing::Point> ^moveSequence, int moveCount, Chess first) {
	if (first == Chess::AVAILABLE) return false;
	Chess current = first;
	ChessBoard^ temp = gcnew ChessBoard(board);
	for (int i = 0; i < moveCount; i++) {
		if (temp->getAvailableCount(current) == 0)
			current = (current == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
		if (temp->isGameEnded()) return false;
		if (temp->getAvailableCount(current) == 0) return false;
		if (!temp->putChess(moveSequence[i].X, moveSequence[i].Y, current)) return false;
		current = (current == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
	}
	return true;
}

Board^ Game::getInitialBoard() {
	return chessBoard->getInitialBoard();
}

void Game::setLearnable(Board^ board) {
	bool board1fit = true, board2fit = false;
	Board^ bd = ChessBoard::getDefaultBoard(1);
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
			if (board[i, j] != bd[i, j]) {
				board1fit = false;
				goto MULTIPLE_BREAK_1;
			}
MULTIPLE_BREAK_1:
	if (!board1fit) {
		board2fit = true;
		bd = ChessBoard::getDefaultBoard(2);
		for (int i = 0; i < WIDTH; i++)
			for (int j = 0; j < HEIGHT; j++)
				if (board[i, j] != bd[i, j]) {
					board2fit = false;
					goto MULTIPLE_BREAK_2;
				}
	}
MULTIPLE_BREAK_2:
	learnable = board1fit || board2fit;
}
