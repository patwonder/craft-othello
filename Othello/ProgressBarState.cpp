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
								ProgressBarState.cpp
		作者：Patrick
		概述：包含类 ProgressBarStateRelated 的定义。
			ProgressBarStateRelated 类包含一些实用函数，用于进度条样式的处理。

********************************************************************************
*/

#include "stdafx.h"
#include <windows.h>
#include "ProgressBarState.h"

using namespace Othello;

ProgressBarState ProgressBarStateRelated::getState(System::Windows::Forms::ProgressBar^ pb) {
	return (ProgressBarState)(int)SendMessage((HWND)pb->Handle.ToPointer(), PBM_GETSTATE, 0, 0);
}

void ProgressBarStateRelated::setState(System::Windows::Forms::ProgressBar^ pb, ProgressBarState state) {
	SendMessage((HWND)pb->Handle.ToPointer(), PBM_SETSTATE, (WPARAM)state, 0);
}
