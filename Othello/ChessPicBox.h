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
								ChessPicBox.h
		作者：Patrick
		概述：包含类 ChessPicBox 的声明。
			ChessPicBox 类封装了棋子表示，根据不同的状态显示不同的棋子图片。

********************************************************************************
*/

#include "Common.h"

namespace Othello {

public ref class ChessPicBox : public System::Windows::Forms::PictureBox {
private:
	int x, y;
	static System::Drawing::Image^ bChess, ^bChessOrigin;
	static System::Drawing::Image^ wChess, ^wChessOrigin;
	static System::Drawing::Image^ abChess, ^abChessOrigin;
	static System::Drawing::Image^ awChess, ^awChessOrigin;
	static System::Drawing::Image^ sabChess, ^sabChessOrigin;
	static System::Drawing::Image^ sawChess, ^sawChessOrigin;
	static System::Drawing::Image^ bBChess, ^bBChessOrigin;
	static System::Drawing::Image^ bWChess, ^bWChessOrigin;
	static System::Drawing::Image^ fBChess, ^fBChessOrigin;
	static System::Drawing::Image^ fWChess, ^fWChessOrigin;
	static System::Drawing::Image^ nBChess, ^nBChessOrigin;
	static System::Drawing::Image^ nWChess, ^nWChessOrigin;
	static System::Drawing::Image^ fsbChess, ^fsbChessOrigin;
	static System::Drawing::Image^ fswChess, ^fswChessOrigin;
	static System::Drawing::Image^ nsbChess, ^nsbChessOrigin;
	static System::Drawing::Image^ nswChess, ^nswChessOrigin;
	static System::Drawing::Image::GetThumbnailImageAbort^ commonImageAbort
		= gcnew System::Drawing::Image::GetThumbnailImageAbort(ChessPicBox::imageAbort);
	static bool imageAbort() {
		return false;
	}
	static int width = 0, height = 0;
	Chess color;
	bool available, selected, bordered, focused, tip, next;

public:
	ChessPicBox(int x, int y) {
		this->x = x;
		this->y = y;
		color = Chess::AVAILABLE;
		selected = bordered = focused = available = tip = false;
		performChange();
	}
	int getXIndex() { return x; }
	int getYIndex() { return y; }
	System::Drawing::Point getLocation() {
		return System::Drawing::Point(x, y);
	}
	static void setBlackChess(System::Drawing::Image^ bChessOrigin);
	static void setWhiteChess(System::Drawing::Image^ wChessOrigin);
	static void setAvBlackChess(System::Drawing::Image^ abChessOrigin);
	static void setAvWhiteChess(System::Drawing::Image^ awChessOrigin);
	static void setSelAvBlackChess(System::Drawing::Image^ sabChessOrigin);
	static void setSelAvWhiteChess(System::Drawing::Image^ sawChessOrigin);
	static void setBdBlackChess(System::Drawing::Image^ bBChessOrigin);
	static void setBdWhiteChess(System::Drawing::Image^ bWChessOrigin);
	static void setFocusBlackChess(System::Drawing::Image^ fBChessOrigin);
	static void setFocusWhiteChess(System::Drawing::Image^ fWChessOrigin);
	static void setNextBlackChess(System::Drawing::Image^ nBChessOrigin);
	static void setNextWhiteChess(System::Drawing::Image^ nWChessOrigin);
	static void setFocusSelBlackChess(System::Drawing::Image^ fsbChessOrigin);
	static void setFocusSelWhiteChess(System::Drawing::Image^ fswChessOrigin);
	static void setNextSelBlackChess(System::Drawing::Image^ nsbChessOrigin);
	static void setNextSelWhiteChess(System::Drawing::Image^ nswChessOrigin);
	static void setSize(int width, int height);

	void setColor(Chess color, bool selected, bool bordered, bool focused, bool available, bool tip, bool next);
	void setSelected(bool selected);
	void setBordered(bool bordered);
	void setFocused(bool focused);
	void setAvailable(bool available);
	void setTip(bool tip);
	void setNext(bool next);

	Chess getColor() {
		return color;
	}
	bool isSelected() {
		return selected;
	}
	bool isBordered() {
		return bordered;
	}
	bool isFocused() {
		return focused;
	}
	bool isAvailable() {
		return available;
	}
	bool isTip() {
		return tip;
	}
	bool isNext() {
		return next;
	}
	void performChange();
};

}