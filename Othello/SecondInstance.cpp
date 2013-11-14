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
								SecondInstance.cpp
		作者：Patrick
		概述：包含类 SecondInstance 的定义。
			SecondInstance 类包含一些实用函数，用于重复进程的处理。

********************************************************************************
*/

#include "stdafx.h"
#include <windows.h>
#include <vcclr.h>
#include "SecondInstance.h"

using namespace Othello;

bool SecondInstance::CLRPostMessage(System::IntPtr hWnd, int msg, System::IntPtr wParam, System::IntPtr lParam) {
	return (bool)(::PostMessage((HWND)hWnd.ToPointer(), msg, (WPARAM)wParam.ToPointer(), (LPARAM)lParam.ToPointer()));
}

int SecondInstance::CLRRegisterWindowMessage(System::String^ message) {
	pin_ptr<const wchar_t> msgptr = PtrToStringChars(message);
	return ::RegisterWindowMessage(msgptr);
}

bool SecondInstance::CLRShowWindow(System::IntPtr hWnd, int nCmdShow) {
	return ShowWindow((HWND)hWnd.ToPointer(), nCmdShow);
}
