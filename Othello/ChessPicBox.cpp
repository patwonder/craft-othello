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

/*
********************************************************************************
								ChessPicBox.cpp
		作者：Patrick
		概述：包含类 ChessPicBox 的定义。
			ChessPicBox 类封装了棋子表示，根据不同的状态显示不同的棋子图片。

********************************************************************************
*/

#include "stdafx.h"
#include "ChessPicBox.h"

using namespace Othello;
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

void ChessPicBox::setBlackChess(System::Drawing::Image^ bChessOrigin) {
	ChessPicBox::bChessOrigin = bChessOrigin;
	if (width > 0 && height > 0) {
		bChess = bChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setWhiteChess(System::Drawing::Image^ wChessOrigin) {
	ChessPicBox::wChessOrigin = wChessOrigin;
	if (width > 0 && height > 0) {
		wChess = wChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setAvBlackChess(System::Drawing::Image^ abChessOrigin) {
	ChessPicBox::abChessOrigin = abChessOrigin;
	if (width > 0 && height > 0) {
		abChess = abChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setAvWhiteChess(System::Drawing::Image^ awChessOrigin) {
	ChessPicBox::awChessOrigin = awChessOrigin;
	if (width > 0 && height > 0) {
		awChess = awChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setSelAvBlackChess(System::Drawing::Image^ sabChessOrigin) {
	ChessPicBox::sabChessOrigin = sabChessOrigin;
	if (width > 0 && height > 0) {
		sabChess = sabChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setSelAvWhiteChess(System::Drawing::Image^ sawChessOrigin) {
	ChessPicBox::sawChessOrigin = sawChessOrigin;
	if (width > 0 && height > 0) {
		sawChess = sawChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setBdBlackChess(System::Drawing::Image^ bBChessOrigin) {
	ChessPicBox::bBChessOrigin = bBChessOrigin;
	if (width > 0 && height > 0) {
		bBChess = bBChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setBdWhiteChess(System::Drawing::Image^ bWChessOrigin) {
	ChessPicBox::bWChessOrigin = bWChessOrigin;
	if (width > 0 && height > 0) {
		bWChess = bWChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setFocusBlackChess(System::Drawing::Image^ fBChessOrigin) {
	ChessPicBox::fBChessOrigin = fBChessOrigin;
	if (width > 0 && height > 0) {
		fBChess = fBChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setFocusWhiteChess(System::Drawing::Image^ fWChessOrigin) {
	ChessPicBox::fWChessOrigin = fWChessOrigin;
	if (width > 0 && height > 0) {
		fWChess = fWChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setNextBlackChess(System::Drawing::Image^ nBChessOrigin) {
	ChessPicBox::nBChessOrigin = nBChessOrigin;
	if (width > 0 && height > 0) {
		nBChess = nBChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}
void ChessPicBox::setNextWhiteChess(System::Drawing::Image^ nWChessOrigin) {
	ChessPicBox::nWChessOrigin = nWChessOrigin;
	if (width > 0 && height > 0) {
		nWChess = nWChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}

void ChessPicBox::setFocusSelBlackChess(System::Drawing::Image^ fsbChessOrigin) {
	ChessPicBox::fsbChessOrigin = fsbChessOrigin;
	if (width > 0 && height > 0) {
		fsbChess = fsbChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}

void ChessPicBox::setFocusSelWhiteChess(System::Drawing::Image^ fswChessOrigin) {
	ChessPicBox::fswChessOrigin = fswChessOrigin;
	if (width > 0 && height > 0) {
		fswChess = fswChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}

void ChessPicBox::setNextSelBlackChess(System::Drawing::Image^ nsbChessOrigin) {
	ChessPicBox::nsbChessOrigin = nsbChessOrigin;
	if (width > 0 && height > 0) {
		nsbChess = nsbChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}

void ChessPicBox::setNextSelWhiteChess(System::Drawing::Image^ nswChessOrigin) {
	ChessPicBox::nswChessOrigin = nswChessOrigin;
	if (width > 0 && height > 0) {
		nswChess = nswChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	}
}

void ChessPicBox::setSize(int width, int height) {
	ChessPicBox::width = width;
	ChessPicBox::height = height;
	bChess = bChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	wChess = wChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	abChess = abChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	awChess = awChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	sabChess = sabChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	sawChess = sawChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	bBChess = bBChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	bWChess = bWChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	fBChess = fBChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	fWChess = fWChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	nBChess = nBChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	nWChess = nWChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	fsbChess = fsbChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	fswChess = fswChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	nsbChess = nsbChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
	nswChess = nswChessOrigin->GetThumbnailImage(width, height, commonImageAbort, IntPtr::Zero);
}

void ChessPicBox::setColor(Othello::Chess color, bool selected, bool bordered, bool focused, bool available, bool tip, bool next) {
	if (this->color != color || this->selected != selected || this->bordered != bordered 
			|| this->focused != focused || this->available != available || this->tip != tip || this->next != next) {
		this->color = color;
		this->selected = selected;
		this->bordered = bordered;
		this->focused = focused;
		this->available = available;
		this->tip = tip;
		this->next = next;
		performChange();
	}
}

void ChessPicBox::setSelected(bool selected) {
	if (this->selected != selected) {
		this->selected = selected;
		performChange();
	}
}

void ChessPicBox::setAvailable(bool available) {
	if (this->available != available) {
		this->available = available;
		performChange();
	}
}

void ChessPicBox::setBordered(bool bordered) {
	if (this->bordered != bordered) {
		this->bordered = bordered;
		performChange();
	}
}

void ChessPicBox::setFocused(bool focused) {
	if (this->focused != focused) {
		this->focused = focused;
		performChange();
	}
}

void ChessPicBox::setTip(bool tip) {
	if (this->tip != tip) {
		this->tip = tip;
		performChange();
	}
}

void ChessPicBox::setNext(bool next) {
	if (this->next != next) {
		this->next = next;
		performChange();
	}
}

void ChessPicBox::performChange() {
	switch(color) {
	case Chess::AVAILABLE:
		this->Visible = false;
		break;
	case Chess::BLACK:
		if (focused || tip) {
			if (selected)
				this->Image = fsbChess;
			else
				this->Image = fBChess;
		} else if (next) {
			if (selected)
				this->Image = nsbChess;
			else
				this->Image = nBChess;
		} else if (selected) {
			this->Image = sabChess;
		} else if (available) {
			this->Image = abChess;
		} else if (bordered) {
			this->Image = bBChess;
		} else {
			this->Image = bChess;
		}
		this->Visible = true;
		break;
	case Chess::WHITE:
		if (focused || tip) {
			if (selected)
				this->Image = fswChess;
			else
				this->Image = fWChess;
		} else if (next) {
			if (selected)
				this->Image = nswChess;
			else
				this->Image = nWChess;
		} else if (selected) {
			this->Image = sawChess;
		} else if (available) {
			this->Image = awChess;
		} else if (bordered) {
			this->Image = bWChess;
		} else {
			this->Image = wChess;
		}
		this->Visible = true;
		break;
	}
}