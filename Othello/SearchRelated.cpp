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
								SearchRelated.cpp
		作者：Patrick
		概述：包含 getAIPlayerName 和 getAISearchOptions 函数的定义。

********************************************************************************
*/

#include "StdAfx.h"
#include "SearchRelated.h"

using namespace Othello;

System::String^ Players::getPlayerTooltip(PlayerType type) {
	if (type == PlayerType::GUI) {
		return "人类玩家: 使用鼠标/键盘进行游戏的玩家"; // Not essentially played by humans, though = =|
	}
	if (type == PlayerType::RANDOM) {
		return "随机: 根据规则随机选择棋步的玩家";
	}
	if (type == PlayerType::BIG_EAT) {
		return "大食: 试图使中局子数最大化(大食策略)的玩家";
	}
	SearchOptions opt = getAISearchOptions(type);
	System::String^ tooltip = getAIPlayerName(type) + ": "
		+ opt.midGameDepth + "步 + " + opt.exactGameStep + "步完美终局";
	if (opt.partialExactStep80 > opt.exactGameStep) {
		tooltip += "\n于" + opt.partialExactStep80 + "空格时展开终局选择性搜索";
	}
	return tooltip;
}

System::String^ Players::getAIPlayerName(PlayerType type) {
	switch (type) {
	case PlayerType::RANDOM:
		return "随机";
	case PlayerType::BIG_EAT:
		return "大食";
	case PlayerType::EARTH:
		return "地球";
	case PlayerType::CLOUDS:
		return "云朵";
	case PlayerType::ORBIT:
		return "轨道";
	case PlayerType::MOON:
		return "月亮";
	case PlayerType::STAR:
		return "星星";
	case PlayerType::GALAXY:
		return "星系";
	case PlayerType::UNIVERSE:
		return "宇宙";
	default:
		return "未知对手";
	}
}

SearchOptions Players::getAISearchOptions(PlayerType type) {
	switch (type) {
	case PlayerType::EARTH:
		return SearchOptions(1, 6, 6, 6, 6, 6);
	case PlayerType::CLOUDS:
		return SearchOptions(2, 8, 8, 8, 8, 8);
	case PlayerType::ORBIT:
		return SearchOptions(4, 12, 12, 12, 12, 12);
	case PlayerType::MOON:
		return SearchOptions(8, 16, 16, 16, 16, 16);
	case PlayerType::STAR:
		return SearchOptions(12, 22, 22, 20, 20, 22);
	case PlayerType::GALAXY:
		return SearchOptions(16, 26, 24, 22, 22, 24);
	case PlayerType::UNIVERSE:
		return SearchOptions(18, 28, 26, 24, 24, 26);
	default:
		return SearchOptions(1, 6, 6, 6, 6, 6);
	}
}

bool Players::isAIPlayer(PlayerType type) {
	switch (type) {
	case PlayerType::EARTH:
	case PlayerType::CLOUDS:
	case PlayerType::ORBIT:
	case PlayerType::MOON:
	case PlayerType::STAR:
	case PlayerType::GALAXY:
	case PlayerType::UNIVERSE:
		return true;
	default:
		return false;
	}
}
