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
								Conversions.h
		作者：Patrick
		概述：包含常用转换函数的声明。

********************************************************************************
*/

#include "Common.h"

namespace Othello {

namespace Conversions {
	//将 int 表示的棋步转换为 System::Drawing::Point 对象表示的棋步
	System::Drawing::Point intToPoint(int theInt);

	//将 int 表示的走棋方转换为 Chess 表示的走棋方
	Chess intToChess(int theInt);

	//将 System::Drawing::Point 对象表示的棋步转换为字符串表示的棋步
	System::String^ pointToString(System::Drawing::Point thePoint);

	//将 int 表示的棋步转换为字符串表示的棋步
	System::String^ intToString(int theInt);

	//将Board^表示的棋盘转换为CraftEngine接受的格式
	void convertBoard(Board^ board, int dest[]);

} // namespace Conversions

} // namespace Othello
