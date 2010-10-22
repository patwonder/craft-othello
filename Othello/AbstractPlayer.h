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
								AbstractPlayer.h
		作者：Patrick
		概述：这是所有玩家类要实现的公共借口。

********************************************************************************
*/

#include "Common.h"
#include "GameContext.h"

namespace Othello {
public interface class AbstractPlayer {
public:
	/*
	概述：使用指定的棋局上下文和走棋方，初始化玩家对象

	参数：
	gc：指定的棋局上下文
	color：指定的走棋方
	*/
	virtual void init(GameContext^ gc, Chess color) = 0;

	/*
	概述：轮到玩家走棋，根据指定的棋局上下文和对手上一步棋，决定玩家走的棋。

	参数：
	gc：指定的棋局上下文
	lastMove：对手的上一步棋，
		若对手欠行，lastMove.getColor()返回Chess::AVAILABLE

	返回值：若玩家欠行，可返回任意值，
		否则返回值必须为[0, gc->getAvailable() - 1]区间内的整数，
		对应的棋步由gc->getAvailableMove()函数给出。
	*/
	virtual int myTurn(GameContext^ gc, Move lastMove) = 0;

	/*
	概述：在棋局结束或中途终止时被调用。玩家对象必须将游戏状态标记为终止。
	*/
	virtual void reset() = 0;

	/*
	概述：获取玩家名称

	返回值：当前玩家对象表示的玩家名称
	*/
	virtual System::String^ getName() = 0;

	/*
	概述：获取玩家的延迟响应时间

	返回值：当前玩家的延迟响应时间
	*/
	virtual int getStepDelay();

	/*
	概述：设置玩家的延迟响应时间

	参数：
	delay：希望设置的延迟响应时间

	注意：玩家对象不一定要在getStepDelay中返回该时间。
		具体例子可以看GUIPlayer。人类玩家下棋总是即时响应的。
	*/
	virtual void setStepDelay(int delay);
};

}//namespace Othello

/* AbstractPlayer 快速继承（直接复制）:

public ref class <ClassName> : public AbstractPlayer {
private:

public:
	<ClassName>() {}

	virtual void init(GameContext^ gc, Chess color) {

	}
	virtual int myTurn(GameContext^ gc, Move lastMove) {

	}
	virtual void reset() {

	}
	virtual System::String^ getName() {

	}
	virtual int getStepDelay() {
		return 0;
	}
	virtual void setStepDelay(int delay) {}
};

*/
