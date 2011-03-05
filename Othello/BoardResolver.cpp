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

#include "StdAfx.h"

#include "BoardResolver.h"

using namespace Othello;
using namespace System;

Chess BoardResolver::parseChess(Char ch) {
	ch = Char::ToUpper(ch);
	if (Array::IndexOf(BLACK, ch) >= 0)
		return Chess::BLACK;
	else
		if (Array::IndexOf(WHITE, ch) >= 0)
			return Chess::WHITE;
		else
			return Chess::AVAILABLE;
}

Board^ BoardResolver::parseBoard(String ^ffopos_board) {
	if (ffopos_board->Length != WIDTH * HEIGHT)
		return nullptr;
	Board ^result = gcnew Board(WIDTH, HEIGHT);
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			result[j, i] = parseChess(ffopos_board[i * WIDTH + j]);
	return result;
}

Chess BoardResolver::parseMover(String ^ffopos_mover) {
	if (ffopos_mover->Length != 1)
		return Chess::AVAILABLE;
	return parseChess(ffopos_mover[0]);
}

Char BoardResolver::chessToChar(Chess chess) {
	if (chess == Chess::BLACK)
		return BLACK[0];
	else
		if (chess == Chess::WHITE)
			return WHITE[0];
		else
			return EMPTY;
}

Chess BoardResolver::processBoard(String ^ffopos, Board ^board) {
	// check params first
	array<String^> ^ffopos_parts = ffopos->Split(DELIMERS, StringSplitOptions::RemoveEmptyEntries);
	if (ffopos_parts->Length != 2)
		return Chess::AVAILABLE;
	Board ^bd = parseBoard(ffopos_parts[0]);
	if (bd == nullptr)
		return Chess::AVAILABLE;
	Chess mover = parseMover(ffopos_parts[1]);
	if (mover == Chess::AVAILABLE)
		return Chess::AVAILABLE;
	
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			board[i, j] = bd[i, j];
		}
	}
	return mover;
}

String^ BoardResolver::getBoardString(Board ^board, Chess mover) {
	String ^str = "";
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			str += chessToChar(board[j, i]);
	str += " " + chessToChar(mover);
	return str;
}
