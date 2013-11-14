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
								ProgressBarState.h
		作者：Patrick
		概述：包含类 ProgressBarStateRelated 的声明。
			ProgressBarStateRelated 类包含一些实用函数，用于进度条样式的处理。

********************************************************************************
*/

namespace Othello {

enum class ProgressBarState {
	Normal = 1,
	Error = 2,
	Paused = 3
};

ref class ProgressBarStateRelated {
public:
	static const int CLR_WM_USER = 0x400;
	static const int PBM_SETSTATE = CLR_WM_USER + 16;
	static const int PBM_GETSTATE = CLR_WM_USER + 17;

	static ProgressBarState getState(System::Windows::Forms::ProgressBar^ pb);
	static void setState(System::Windows::Forms::ProgressBar^ pb, ProgressBarState state);
};

} // namespace Othello
