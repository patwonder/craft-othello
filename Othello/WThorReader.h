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
								WThorReader.h
		作者：Patrick
		概述：包含类 WThorReader 的声明。
			WThorReader 类封装了对 WThor 对局库的访问，用以实现残局模式。

********************************************************************************
*/

#include "Common.h"

//文件头结构：
//1字节  文件建立年份的百位部分，如20表示20**年
//1字节  文件建立年份的低2位
//1字节  文件建立的月份
//1字节  文件建立的日期
//4字节  N1，文件包含的记录数（这个是对局库用的）
//2字节  N2，文件包含的记录数（这个是棋手库和比赛库用的）
//2字节  对局年份
//1字节  P1，棋盘大小，0或8表示8*8
//1字节  P2，游戏类型，1表示solitaires游戏，0表示其他（包括黑白棋）
//1字节  P3，最佳终局计算的搜索深度，一般为22
//1字节  保留
//
//对局库记录结构，对于8*8黑白棋，每条记录68字节
//2字节  比赛名称，这里只是个序号，具体名称要查比赛库中的相应记录
//2字节  黑方棋手名字，这里只是个序号，具体名字要查棋手库
//2字节  白方棋手名字
//1字节  对局结束时黑方的棋子数（多余空格归胜方）
//1字节  黑方理论最佳得子，即在剩余P3个空格时，进行最佳终局计算，黑方的得子数
//60字节 棋步序列，每步棋一个字节，记为行号*10+列号，如A1=11，A2=21。欠行的步不作记录，只是将最后多余字节填0
//
//棋手库记录结构
//每条记录20字节，字符串类型（以结尾），记录棋手名字。对于程序，一般用括号加注程序作者，如
//
//Thor(quin/becquet)
//
//比赛库记录结构
//每条记录26字节，字符串类型，记录比赛名称。

namespace Othello {

public value class WThorHeader {
public:
	int year, month, day;
	int recordNum;
	int tourYear;
	int boardSize;
	bool isSolitaires;
	int theoricalScoreDepth;
};

public value class WThorPlayer {
private:
	System::String^ name;
public:
	WThorPlayer(System::String^ name) {
		this->name = name;
	}
	property System::String^ Name {
		System::String^ get() {
			return name;
		}
	}
};

public value class WThorTour {
private:
	System::String^ name;
public:
	WThorTour(System::String^ name) {
		this->name = name;
	}
	property System::String^ Name {
		System::String^ get() {
			return name;
		}
	}
};

public value class WThorGame {
public:
	WThorTour tour;
	WThorPlayer black, white;
	int year;
	int result;
	array<System::Drawing::Point>^ moveSequence;
};

public ref class WThorReader {
	System::Collections::Generic::List<WThorPlayer>^ players;
	System::Collections::Generic::List<WThorTour>^ tours;
	WThorHeader header;
	System::IO::BinaryReader^ br;
	System::IO::FileStream^ fs;
	System::String^ currentFileName;

	static const int JOU_RECORD_LENGTH = 20;
	static const int TRN_RECORD_LENGTH = 26;
	static const int WTHOR_HEADER_LENGTH = 16;
	static const int GAME_RECORD_LENGTH = 68;
public:
	WThorReader();
	bool loadPlayers(System::String^ jouFileName);
	bool loadTours(System::String^ trnFileName);
	bool loadGameFile(System::String^ wtbFileName);
	void unloadGameFile();
	int getGameCount();
	int getTourYear();
	bool readGame(int index, WThorGame^ game);
	~WThorReader();
};

} // namespace Othello
