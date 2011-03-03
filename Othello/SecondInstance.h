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
								SecondInstance.h
		作者：Patrick
		概述：包含类 SecondInstance 的声明。
			SecondInstance 类包含一些实用函数，用于重复进程的处理。

********************************************************************************
*/

namespace Othello {

ref class SecondInstance {
public:
	static const int CLR_HWND_BROADCAST = 0xffff;
	static const int CLR_SW_RESTORE = 9;
	static const int WM_SISTART = CLRRegisterWindowMessage("{9F93E3AD-147B-4881-BA59-85BFDF4C8CE5}");

	static bool CLRPostMessage(System::IntPtr hWnd, int msg, System::IntPtr wParam, System::IntPtr lParam);
	static int CLRRegisterWindowMessage(System::String^ message);

	static bool CLRShowWindow(System::IntPtr hWnd, int nCmdShow);
};

} // namespace Othello
