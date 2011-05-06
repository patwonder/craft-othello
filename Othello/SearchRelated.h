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
								SearchRelated.h
		作者：Patrick
		概述：包含和搜索相关类的声明

********************************************************************************
*/

#include "Common.h"
#include "ChessPicBox.h"

namespace Othello {

//概述：搜索监视器类
public interface class SearchDisplayer {
public:
	virtual void setSearchState(bool searching) = 0;
	virtual void setResult(System::String^ result) = 0;
	virtual void setSpeed(System::String^ speed) = 0;
	virtual void setTotalNum(System::String^ totalNum) = 0;
	virtual void setProgress(int percent) = 0;
	virtual void setProgressState(bool failed);
	virtual void setFocusedMove(int x, int y) = 0;
	virtual void setSelectedMove(int x, int y) = 0;
	virtual void showPrincipalVariation(System::String^ pv);
	virtual void resetComponents() = 0;
	virtual void searchStarted() = 0;
	virtual void searchEnded() = 0;
};

//概述：搜索选项类
public value class SearchOptions {
public:
	int midGameDepth;
	int partialExactStep80;
	int partialExactStep95;
	int partialExactStep99;
	int winLossStep;
	int exactGameStep;
	SearchOptions(int midDepth, int pExact50, int pExact90, int pExact99, int exactStep, int wlStep)
		: midGameDepth(midDepth), partialExactStep80(pExact50),
		partialExactStep95(pExact90), partialExactStep99(pExact99), exactGameStep(exactStep),
		winLossStep(wlStep)
	{ ; }
};

namespace Players {
	/*
	概述：获取指定类别AI玩家的名字

	参数：
	type：指定的AI玩家类别

	返回值：type类AI玩家的名字
	*/
	System::String^ getAIPlayerName(PlayerType type);

	/*
	概述：获取指定类别AI玩家的搜索选项

	参数：
	type：指定的AI玩家类别

	返回值：type类AI玩家的搜索选项
	*/
	SearchOptions getAISearchOptions(PlayerType type);

	/*
	概述：获取指定类别玩家的ToolTip说明

	参数：
	type：指定的玩家类别

	返回值：type类玩家的ToolTip说明
	*/
	System::String^ getPlayerTooltip(PlayerType type);

	/*
	概述：判断指定玩家类别是否是AI玩家

	参数：
	type：指定的玩家类别

	返回值：若type是AI玩家，返回true，否则返回false
	*/
	bool isAIPlayer(PlayerType type);

	/*
	概述：判断指定玩家类别是否是电脑玩家(包括随机、大食)

	参数：
	type：指定的玩家类别

	返回值：若type是电脑玩家，返回true，否则返回false
	*/
	bool isComputerPlayer(PlayerType type);
}

} // namespace Othello
