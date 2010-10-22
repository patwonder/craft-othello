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
								Setting.h
		作者：Patrick
		概述：包含类 UserInfo 的声明。
			UserInfo 类包含了程序所需的各种选项以及玩家统计信息。

********************************************************************************
*/

#include "Common.h"

#define DEFAULT_THEME_NAME "default"
#ifdef CHRISTMAS
#define CHRISTMAS_THEME_NAME "xmas"
#endif

namespace Othello {

public enum class GameResult { LOSS, DRAW, WIN };

public ref class Statistics {
private:
	int win, draw, loss;
	int series;
	GameResult seriesState;
	int longestWin, longestDraw, longestLoss;
	int bestScore;
public:
	Statistics() {
		this->Statistics::Statistics(0, 0, 0, 0, GameResult::DRAW, 0, 0, 0, -WIDTH * HEIGHT);
	}
	Statistics(int win, int draw, int loss, int series, GameResult seriesState, 
		int longestWin, int longestDraw, int longestLoss, int bestScore) {
		this->win = win;
		this->draw = draw;
		this->loss = loss;
		this->longestWin = longestWin;
		this->longestDraw = longestDraw;
		this->longestLoss = longestLoss;
		this->series = series;
		this->seriesState = seriesState;
		this->bestScore = bestScore;
	}
	int getWin() { return win; }
	int getDraw() { return draw; }
	int getLoss() { return loss; }
	int getLongestWin() { return longestWin; }
	int getLongestDraw() { return longestDraw; }
	int getLongestLoss() { return longestLoss; }
	int getSeries() { return series; }
	int getBestScore() { return bestScore; }
	GameResult getSeriesState() { return seriesState; }
	void recordGame(int score) {
		GameResult result;
		if (score > 0) result = GameResult::WIN;
		else if (score == 0) result = GameResult::DRAW;
		else result = GameResult::LOSS;
		if (result == seriesState)
			series++;
		else {
			series = 1;
			seriesState = result;
		}
		switch (result) {
		case GameResult::WIN:
			win++;
			if (series > longestWin)
				longestWin = series;
			break;
		case GameResult::DRAW:
			draw++;
			if (series > longestDraw)
				longestDraw = series;
			break;
		case GameResult::LOSS:
			loss++;
			if (series > longestLoss)
				longestLoss = series;
			break;
		}
		if (score > bestScore)
			bestScore = score;
	}
	void reset() {
		win = draw = loss = series = 0;
		seriesState = GameResult::DRAW;
		longestWin = longestDraw = longestLoss = 0;
		bestScore = -WIDTH * HEIGHT;
	}
};

public ref class UserInfo {
private:
	System::String^ userName;
	int playerTypeCount;
	array<PlayerType>^ playerTypes;
	array<Statistics^>^ statistics;
	bool playSound;
	int stepDelay;
	int tableSize;
	bool autoLearn;
	bool showEvaluation;
	bool showProgress;
	bool showSpeed;
	bool freeMode;
	int bookTolerance; // reserved
	int customEmpties;
	PlayerType analyzer;
	PlayerType black, white;
	System::String^ theme;
	bool useBook;
	bool autoCleanTable;
public:
	UserInfo(void) {
		playerTypeCount = 0;
		playerTypes = gcnew array<PlayerType>(MAX_PLAYERTYPES);
		statistics = gcnew array<Statistics^>(MAX_PLAYERTYPES);
		UserName = System::Environment::UserName;
		PlaySound = true;
		StepDelay = 1000;
		TableSize = 0x100000; // 64MB
		AutoLearn = true;
		ShowEvaluation = true;
		ShowProgress = true;
		ShowSpeed = true;
		FreeMode = false;
		BookTolerance = 3 * RULER;
		Analyzer = PlayerType::UNIVERSE;
		BlackType = PlayerType::GUI;
		WhiteType = PlayerType::EARTH;
		CustomEmpties = 10;
#ifdef CHRISTMAS
		Theme = CHRISTMAS_THEME_NAME;
#else
		Theme = DEFAULT_THEME_NAME;
#endif
		UseBook = true;
		AutoCleanTable = true;
	}
	void addStatistics(PlayerType type, Statistics^ stat) {
		for (int i = 0; i < playerTypeCount; i++)
			if (playerTypes[i] == type) {
				statistics[i] = stat;
				return;
			}
		playerTypes[playerTypeCount] = type;
		statistics[playerTypeCount] = stat;
		playerTypeCount++;
	}
	Statistics^ getStatistics(PlayerType type) {
		for (int i = 0; i < playerTypeCount; i++)
			if (playerTypes[i] == type)
				return statistics[i];
		addStatistics(type, gcnew Statistics());
		return getStatistics(type);
	}
	property bool AutoLearn
	{
		bool get()
		{
			return autoLearn;
		}
		void set(bool value)
		{
			autoLearn = value;
		}
	}
	property int CustomEmpties
	{
		int get()
		{
			return customEmpties;
		}
		void set(int value)
		{
			customEmpties = value;
		}
	}
	property bool FreeMode
	{
		bool get()
		{
			return freeMode;
		}
		void set(bool value)
		{
			freeMode = value;
		}
	}
	property bool PlaySound
	{
		bool get()
		{
			return playSound;
		}
		void set(bool value)
		{
			playSound = value;
		}
	}
	property bool ShowEvaluation
	{
		bool get()
		{
			return showEvaluation;
		}
		void set(bool value)
		{
			showEvaluation = value;
		}
	}
	property bool ShowProgress
	{
		bool get()
		{
			return showProgress;
		}
		void set(bool value)
		{
			showProgress = value;
		}
	}
	property bool ShowSpeed
	{
		bool get()
		{
			return showSpeed;
		}
		void set(bool value)
		{
			showSpeed = value;
		}
	}
	property int StepDelay
	{
		int get()
		{
			return stepDelay;
		}
		void set(int value)
		{
			stepDelay = value;
		}
	}
	property int TableSize
	{
		int get()
		{
			return tableSize;
		}
		void set(int value)
		{
			tableSize = value;
		}
	}
	property System::String^ UserName
	{
		System::String^ get()
		{
			return userName;
		}
		void set(System::String^ value)
		{
			userName = value;
		}
	}
	property int BookTolerance
	{
		int get()
		{
			return bookTolerance;
		}
		void set(int value)
		{
			bookTolerance = value;
		}
	}
	property PlayerType Analyzer
	{
		PlayerType get()
		{
			return analyzer;
		}
		void set(PlayerType value)
		{
			analyzer = value;
		}
	}
	property PlayerType BlackType
	{
		PlayerType get()
		{
			return black;
		}
		void set(PlayerType value)
		{
			black = value;
		}
	}
	property PlayerType WhiteType
	{
		PlayerType get()
		{
			return white;
		}
		void set(PlayerType value)
		{
			white = value;
		}
	}
	property System::String^ Theme
	{
		System::String^ get()
		{
			return theme;
		}
		void set(System::String^ value)
		{
			theme = value;
		}
	}
	property bool UseBook
	{
		bool get()
		{
			return useBook;
		}
		void set(bool value)
		{
			useBook = value;
		}
	}
	property bool AutoCleanTable
	{
		bool get()
		{
			return autoCleanTable;
		}
		void set(bool value)
		{
			autoCleanTable = value;
		}
	}
};

} // namespace Othello
