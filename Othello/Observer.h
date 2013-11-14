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
								Observer.h
		作者：Patrick
		概述：这是所有观察者类要实现的接口

********************************************************************************
*/

#include "Common.h"

namespace Othello {
public interface class Observer {
public:
	/*
	概述：当某一走棋方走了一步棋时被调用

	参数：
	move：刚走的那步棋
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void playerMoved(Move move, 
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;

	/*
	概述：游戏开始时被调用

	参数：
	firstPlayer：先手方
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void gameStarted(Chess firstPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;

	/*
	概述：游戏中途结束时被调用

	参数：
	firstPlayer：先手方
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void gameReseted(Chess firstPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;

	/*
	概述：游戏结束时被调用

	参数：
	winner：获胜方，平局为 Chess::AVAILABLE
	nBlack：黑方最终棋子数
	nWhite：白方最终棋子数
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void gameEnded(Chess winner, int nBlack, int nWhite, 
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;

	/*
	概述：当游戏暂停时被调用

	参数：
	currentPlayer：当前走棋方
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void gameHasBeenPaused(Chess currentPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;

	/*
	概述：改变初始棋盘时被调用

	参数：
	firstPlayer：先手方
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void boardChanged(Chess firstPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;

	/*
	概述：中途切换棋手时被调用

	参数：
	firstPlayer：先手方
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void playerChanged(Chess currentPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;

	/*
	概述：改变时限时被调用

	参数：
	timeLimit：新的时限
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void timeLimitChanged(int timeLimit,
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;

	/*
	概述：游戏中途结束时被调用

	参数：
	overTimer：超时方
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void playerOverTimed(Chess overTimer,
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;

	/*
	概述：一方悔棋时被调用

	参数：
	step：悔棋的步数
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void gameGoneBack(int step,
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;

	/*
	概述：一方取消悔棋时被调用

	参数：
	step：取消悔棋的步数
	gcBlack：黑方的棋局上下文
	gcWhite：白方的棋局上下文
	*/
	virtual void gameGoneForward(int step,
		GameContext^ gcBlack, GameContext^ gcWhite) = 0;
};

} // namespace Othello

/* Observer 快速继承（直接复制）

public ref class <ClassName> : public Observer {
private:

public:
	<ClassName>() {}

	virtual void playerMoved(Move move, 
		GameContext^ gcBlack, GameContext^ gcWhite) {

	}

	virtual void gameStarted(Chess firstPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	
	}

	virtual void gameReseted(Chess firstPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	
	}

	virtual void gameEnded(Chess winner, int nBlack, int nWhite, 
		GameContext^ gcBlack, GameContext^ gcWhite) {
	
	}

	virtual void gameHasBeenPaused(Chess currentPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) {

	}

	virtual void boardChanged(Chess firstPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) {

	}

	virtual void playerChanged(Chess currentPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	
	}

	virtual void timeLimitChanged(int timeLimit,
		GameContext^ gcBlack, GameContext^ gcWhite) {

	}

	virtual void playerOverTimed(Chess overTimer,
		GameContext^ gcBlack, GameContext^ gcWhite) {

	}

	virtual void gameGoneBack(int step,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	
	}

	virtual void gameGoneForward(int step,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	}
};

*/