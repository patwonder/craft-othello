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

/*
********************************************************************************
								WThorReader.cpp
		作者：Patrick
		概述：包含类 WThorReader 的定义。
			WThorReader 类封装了对 WThor 对局库的访问，用以实现残局模式。

********************************************************************************
*/

#include "StdAfx.h"
#include "WThorReader.h"

using namespace Othello;
using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;

WThorReader::WThorReader(void) {
	players = gcnew List<WThorPlayer>();
	tours = gcnew List<WThorTour>();
	br = nullptr;
	fs = nullptr;
	currentFileName = "";
}

bool WThorReader::loadPlayers(System::String^ jouFileName) {
	FileStream^ fs;
	try {
		fs = gcnew System::IO::FileStream(jouFileName, 
			System::IO::FileMode::Open, System::IO::FileAccess::Read);
	} catch (System::Exception^) {
		return false;
	}
	players->Clear();
	BinaryReader^ br = gcnew BinaryReader(fs);
	try {
		fs->Seek(8, SeekOrigin::Begin);
		int records = br->ReadUInt16();
		fs->Seek(WTHOR_HEADER_LENGTH, SeekOrigin::Begin);
		char buffer[JOU_RECORD_LENGTH + 1];
		buffer[JOU_RECORD_LENGTH] = '\0';
		for (int i = 0; i < records; i++) {
			array<unsigned char>^ name = br->ReadBytes(JOU_RECORD_LENGTH);
			for (int i = 0; i < JOU_RECORD_LENGTH; i++)
				buffer[i] = name[i];
			players->Add(WThorPlayer(gcnew String(buffer)));
		}
	} catch (Exception^) {
		players->Clear();
		br->Close();
		return false;
	}
	return true;
}

bool WThorReader::loadTours(System::String^ trnFileName) {
	FileStream^ fs;
	try {
		fs = gcnew System::IO::FileStream(trnFileName, 
			System::IO::FileMode::Open, System::IO::FileAccess::Read);
	} catch (System::Exception^) {
		return false;
	}
	tours->Clear();
	BinaryReader^ br = gcnew BinaryReader(fs);
	try {
		fs->Seek(8, SeekOrigin::Begin);
		int records = br->ReadUInt16();
		fs->Seek(WTHOR_HEADER_LENGTH, SeekOrigin::Begin);
		char buffer[TRN_RECORD_LENGTH + 1];
		buffer[TRN_RECORD_LENGTH] = '\0';
		for (int i = 0; i < records; i++) {
			array<unsigned char>^ tour = br->ReadBytes(TRN_RECORD_LENGTH);
			for (int i = 0; i < TRN_RECORD_LENGTH; i++)
				buffer[i] = tour[i];
			tours->Add(WThorTour(gcnew String(buffer)));
		}
	} catch (Exception^) {
		tours->Clear();
		br->Close();
		return false;
	}
	return true;
}

bool WThorReader::loadGameFile(System::String^ wtbFileName) {
	if (br) {
		if (wtbFileName->Equals(currentFileName)) return true;
		br->Close();
		br = nullptr;
		fs = nullptr;
	}
	try {
		fs = gcnew System::IO::FileStream(wtbFileName, 
			System::IO::FileMode::Open, System::IO::FileAccess::Read);
	} catch (System::Exception^) {
		return false;
	}
	br = gcnew BinaryReader(fs);
	try {
		unsigned char century = br->ReadByte();
		unsigned char year = br->ReadByte();
		header.year = century * 100 + year;
		header.month = br->ReadByte();
		header.day = br->ReadByte();
		header.recordNum = br->ReadUInt32();
		br->ReadUInt16();
		header.tourYear = br->ReadUInt16();
		header.boardSize = br->ReadByte();
		header.isSolitaires = br->ReadByte() ? true : false;
		header.theoricalScoreDepth = br->ReadByte();
		fs->Seek(0, SeekOrigin::Begin);
	} catch (Exception^) {
		br->Close();
		br = nullptr;
		fs = nullptr;
		return false;
	}
	currentFileName = wtbFileName;
	return true;
}

void WThorReader::unloadGameFile() {
	if (!br) return;
	br->Close();
	br = nullptr;
	fs = nullptr;
	currentFileName = "";
}

int WThorReader::getGameCount() {
	return br ? header.recordNum : 0;
}

int WThorReader::getTourYear() {
	return br ? header.tourYear : 0;
}

bool WThorReader::readGame(int index, WThorGame^ game) {
	if (!br) return false;
	if (index >= header.recordNum || index < 0) return false;
	try {
		fs->Seek(WTHOR_HEADER_LENGTH + GAME_RECORD_LENGTH * index, SeekOrigin::Begin);
		int tourIndex = br->ReadUInt16();
		int blackIndex = br->ReadUInt16();
		int whiteIndex = br->ReadUInt16();
		int result = br->ReadByte();
		br->ReadByte();
		array<unsigned char>^ moveSequence = br->ReadBytes(60);
		int moveCount = 60;
		while (moveCount && moveSequence[moveCount - 1] == 0) moveCount--;
		if (moveCount == 0) return false;
		game->tour = 
			(tourIndex < tours->Count) ? tours[tourIndex] : WThorTour("Tour " + tourIndex.ToString());
		game->black = 
			(blackIndex < players->Count) ? players[blackIndex] : WThorPlayer("Player " + blackIndex.ToString());
		game->white = 
			(whiteIndex < players->Count) ? players[whiteIndex] : WThorPlayer("Player " + whiteIndex.ToString());
		game->year = header.tourYear;
		game->result = result * 2 - WIDTH * HEIGHT;
		game->moveSequence = gcnew array<Drawing::Point>(moveCount);
		for (int i = 0; i < moveCount; i++) {
			unsigned char move = moveSequence[i];
			game->moveSequence[i] = Drawing::Point(move % 10 - 1, move / 10 - 1);
		}
	} catch (Exception^) {
		return false;
	}
	return true;
}

WThorReader::~WThorReader() {
	if (br)
		unloadGameFile();
}
