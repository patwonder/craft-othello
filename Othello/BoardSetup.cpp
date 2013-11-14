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
								BoardSetup.cpp
		作者：Patrick
		概述：包含类 BoardSetup 的定义。
			BoardSetup 类实现“预设棋局”窗体。

********************************************************************************
*/

#include "StdAfx.h"
#include "BoardSetup.h"
#include "ChessBoard.h"
#include "BoardResolver.h"

using namespace Othello;

BoardSetup::BoardSetup(Board^ board, Chess color,
					   Image^ black, Image^ white, Image^ av) {
	InitializeComponent();
	//
	//TODO: Add the constructor code here
	//
	lastClicked = nullptr;
	automatic = false;
	bChess = black;
	wChess = white;
	avChess = av;
	this->board = board;
	this->firstPlayer = color;
	const int panelWidth = boardPanel->Width;
	const int panelHeight = boardPanel->Height;
	picBoard = gcnew array<ChessPicBox^, 2>(WIDTH, HEIGHT);
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			picBoard[i, j] = gcnew ChessPicBox(i, j);
			boardPanel->Controls->Add(picBoard[i, j]);
			picBoard[i, j]->Location = System::Drawing::Point(panelWidth * i / WIDTH, panelHeight * j / HEIGHT);
			picBoard[i, j]->Size = System::Drawing::Size(panelWidth / WIDTH, panelHeight / HEIGHT);
			picBoard[i, j]->Visible = true;
			picBoard[i, j]->Name = L"picBoard(" + i + "," + j + ")";
			picBoard[i, j]->BorderStyle = System::Windows::Forms::BorderStyle::None;
			picBoard[i, j]->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			picBoard[i, j]->TabStop = false;
			picBoard[i, j]->MouseDown += gcnew MouseEventHandler(this, & BoardSetup::picBoard_MouseDown);
		}
	initialize();
	setBoard();
	setFFOFromBoard();
	DialogResult = Windows::Forms::DialogResult::Cancel;
}

void BoardSetup::changeColor() {
	switch (currentColor) {
	case Chess::BLACK :
		currentColor = Chess::WHITE;
		rbColorWhite->Checked = true;
		break;
	case Chess::WHITE :
		currentColor = Chess::AVAILABLE;
		rbColorAvailable->Checked = true;
		break;
	default:
		currentColor = Chess::BLACK;
		rbColorBlack->Checked = true;
	}
	lastClicked = nullptr;
}

System::Void BoardSetup::picBoard_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	if (e->Button == System::Windows::Forms::MouseButtons::Right) {
		changeColor();
		//setFFOFromBoard();
	} else if (e->Button == System::Windows::Forms::MouseButtons::Left) {
		ChessPicBox^ picSender = safe_cast<ChessPicBox^>(sender);
		if (picSender == lastClicked) {
			// no sufficient indications about last clicked square - disabled
			//changeColor();
		}
		int x = picSender->getXIndex(), y = picSender->getYIndex();
		board[x, y] = currentColor;
		switch (currentColor) {
		case Chess::BLACK :
			picSender->Image = bChess;
			break;
		case Chess::WHITE :
			picSender->Image = wChess;
			break;
		default:
			picSender->Image = avChess;
		}
		setFFOFromBoard();
		lastClicked = picSender;
	}
}

void BoardSetup::initialize() {
	if (board == nullptr) board = ChessBoard::getDefaultBoard(1);
	else board = safe_cast<Board^>(board->Clone());
	currentColor = Chess::BLACK;
	rbColorBlack->Checked = true;
}

void BoardSetup::setBoard() {
	if (firstPlayer == Chess::BLACK) {
		rbFirstBlack->Checked = true;
	} else {
		rbFirstWhite->Checked = true;
	}
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			switch (board[i, j]) {
			case Chess::BLACK :
				picBoard[i, j]->Image = bChess;
				break;
			case Chess::WHITE :
				picBoard[i, j]->Image = wChess;
				break;
			default:
				picBoard[i, j]->Image = avChess;
			}
		}
}

System::Void Othello::BoardSetup::rbColorBlack_CheckedChanged(System::Object ^sender, System::EventArgs ^e) {
	if (rbColorBlack->Checked) {
		currentColor = Chess::BLACK;
		lastClicked = nullptr;
	}
}

System::Void Othello::BoardSetup::rbColorWhite_CheckedChanged(System::Object ^sender, System::EventArgs ^e) {
	if (rbColorWhite->Checked) {
		currentColor = Chess::WHITE;
		lastClicked = nullptr;
	}
}

System::Void Othello::BoardSetup::rbColorAvailable_CheckedChanged(System::Object ^sender, System::EventArgs ^e) {
	if (rbColorAvailable->Checked) {
		currentColor = Chess::AVAILABLE;
		lastClicked = nullptr;
	}
}

System::Void Othello::BoardSetup::rbFirstBlack_CheckedChanged(System::Object ^sender, System::EventArgs ^e) {
	if (rbFirstBlack->Checked) firstPlayer = Chess::BLACK;
	setFFOFromBoard();
	lastClicked = nullptr;
}

System::Void Othello::BoardSetup::rbFirstWhite_CheckedChanged(System::Object ^sender, System::EventArgs ^e) {
	if (rbFirstWhite->Checked) firstPlayer = Chess::WHITE;
	setFFOFromBoard();
	lastClicked = nullptr;
}

System::Void Othello::BoardSetup::btnOK_Click(System::Object ^sender, System::EventArgs ^e) {
	//this->Close();
}

System::Void Othello::BoardSetup::btnCancel_Click(System::Object ^sender, System::EventArgs ^e) {
	//this->Close();
}

System::Void Othello::BoardSetup::BoardSetup_FormClosing(System::Object ^sender, System::Windows::Forms::FormClosingEventArgs ^e) {
	//mainForm->Enabled = true;
}

System::Void Othello::BoardSetup::btnReset_Click(System::Object ^sender, System::EventArgs ^e) {
	board = ChessBoard::getDefaultBoard(1);
	firstPlayer = Chess::BLACK;
	setBoard();
	setFFOFromBoard();
}

System::Void BoardSetup::BoardSetup_Load(System::Object^  sender, System::EventArgs^  e) {
	txtPos->MaxLength = MAX_POS_LENGTH;
}

void BoardSetup::setBoardFromFFO() {
	Chess mover = getBoard(txtPos->Text, board);
	if (mover == Chess::AVAILABLE) return;

	firstPlayer = mover;
	setBoard();

	setFFOFromBoard();
	lastClicked = nullptr;
}

void BoardSetup::setFFOFromBoard() {
	automatic = true;
	int selStart = txtPos->SelectionStart;
	int selLength = txtPos->SelectionLength;
	txtPos->Text = getFFOString(board, firstPlayer);
	txtPos->SelectionStart = selStart;
	txtPos->SelectionLength = selLength;
	automatic = false;
}

String^ BoardSetup::getFFOString(Board^ board, Chess mover) {
	return BoardResolver::getBoardString(board, mover);
}

Chess BoardSetup::getBoard(System::String^ ffopos, Board^ board) {
	return BoardResolver::processBoard(ffopos, board);
}

System::Void BoardSetup::txtPos_TextChanged(System::Object ^sender, System::EventArgs ^e) {
	if (automatic) return;
	setBoardFromFFO();
}

System::Void BoardSetup::btnCopy_Click(System::Object ^sender, System::EventArgs ^e) {
	try {
		Clipboard::SetText(txtPos->Text);
		tmrCopied->Enabled = false;
		lblCopied->Visible = true;
		tmrCopied->Enabled = true;
	} catch (...) {
	}
}

System::Void BoardSetup::btnPaste_Click(System::Object ^sender, System::EventArgs ^e) {
	try {
		int selStart = txtPos->SelectionStart;
		int selLength = txtPos->SelectionLength;
		String^ text = Clipboard::GetText();
		if (text->Length == 0) return;
		if (text->Length > txtPos->MaxLength) {
			text = text->Substring(0, txtPos->MaxLength);
		}
		txtPos->Text = text;
		txtPos->SelectionStart = selStart;
		txtPos->SelectionLength = selLength;
	} catch (...) {
	}
}

System::Void BoardSetup::tmrCopied_Tick(System::Object ^sender, System::EventArgs ^e) {
	tmrCopied->Enabled = false;
	lblCopied->Visible = false;
}
