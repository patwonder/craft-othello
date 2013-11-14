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
								FlashWindow.h
		作者：Patrick
		概述：包含 FlashWindowEx 函数的声明。

********************************************************************************
*/

namespace Othello {

	const unsigned int FLASHW_ALL = 0x3;
	const unsigned int FLASHW_CAPTION = 0x1;
	const unsigned int FLASHW_STOP = 0;
	const unsigned int FLASHW_TIMER = 0x4;
	const unsigned int FLASHW_TIMERNOFG = 0xC;
	const unsigned int FLASHW_TRAY = 0x2;

	extern bool FlashWindowEx(System::IntPtr hwnd, unsigned int flags, 
		unsigned int count, unsigned int timeout);
} // namespace Othello
