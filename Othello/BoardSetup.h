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
								BoardSetup.h
		作者：Patrick
		概述：包含类 BoardSetup 的声明。
			BoardSetup 类实现“预设棋局”窗体。

********************************************************************************
*/

#include "Common.h"
#include "ChessPicBox.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

using namespace System::Drawing;


namespace Othello {

	/// <summary>
	/// Summary for BoardSetup
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class BoardSetup : public System::Windows::Forms::Form
	{
	public:
		BoardSetup(Board^ board, Chess color,
			Image^ black, Image^ white, Image^ av);
		property Board^ InitBoard {
			Board^ get() {
				return board;
			}
		}
		property Chess FirstPlayer {
			Chess get() {
				return firstPlayer;
			}
		}	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~BoardSetup()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  boardPanel;
	protected: 
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnCancel;
	private: System::Windows::Forms::GroupBox^  gbColor;
	private: System::Windows::Forms::GroupBox^  gbFirstPlayer;
	private: System::Windows::Forms::RadioButton^  rbColorAvailable;
	private: System::Windows::Forms::RadioButton^  rbColorWhite;
	private: System::Windows::Forms::RadioButton^  rbColorBlack;
	private: System::Windows::Forms::RadioButton^  rbFirstWhite;
	private: System::Windows::Forms::RadioButton^  rbFirstBlack;

	private:
		Board^ board;
		Chess currentColor;
		Chess firstPlayer;
		array<ChessPicBox^, 2>^ picBoard;
		Image^ bChess, ^wChess, ^avChess;
		bool automatic;
		static const int MAX_POS_LENGTH = WIDTH * HEIGHT + 2;
	private: System::Windows::Forms::Button^  btnReset;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txtPos;

	private: System::Windows::Forms::Button^  btnCopy;
	private: System::Windows::Forms::Button^  btnPaste;
	private: System::Windows::Forms::Label^  lblCopied;
	private: System::Windows::Forms::Timer^  tmrCopied;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->boardPanel = (gcnew System::Windows::Forms::Panel());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->gbColor = (gcnew System::Windows::Forms::GroupBox());
			this->rbColorAvailable = (gcnew System::Windows::Forms::RadioButton());
			this->rbColorWhite = (gcnew System::Windows::Forms::RadioButton());
			this->rbColorBlack = (gcnew System::Windows::Forms::RadioButton());
			this->gbFirstPlayer = (gcnew System::Windows::Forms::GroupBox());
			this->rbFirstWhite = (gcnew System::Windows::Forms::RadioButton());
			this->rbFirstBlack = (gcnew System::Windows::Forms::RadioButton());
			this->btnReset = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtPos = (gcnew System::Windows::Forms::TextBox());
			this->btnCopy = (gcnew System::Windows::Forms::Button());
			this->btnPaste = (gcnew System::Windows::Forms::Button());
			this->lblCopied = (gcnew System::Windows::Forms::Label());
			this->tmrCopied = (gcnew System::Windows::Forms::Timer(this->components));
			this->gbColor->SuspendLayout();
			this->gbFirstPlayer->SuspendLayout();
			this->SuspendLayout();
			// 
			// boardPanel
			// 
			this->boardPanel->Location = System::Drawing::Point(14, 51);
			this->boardPanel->Name = L"boardPanel";
			this->boardPanel->Size = System::Drawing::Size(320, 320);
			this->boardPanel->TabIndex = 11;
			// 
			// btnOK
			// 
			this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnOK->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnOK->Location = System::Drawing::Point(340, 309);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(84, 28);
			this->btnOK->TabIndex = 9;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &BoardSetup::btnOK_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnCancel->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnCancel->Location = System::Drawing::Point(340, 343);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(84, 28);
			this->btnCancel->TabIndex = 10;
			this->btnCancel->Text = L"取消";
			this->btnCancel->UseVisualStyleBackColor = true;
			this->btnCancel->Click += gcnew System::EventHandler(this, &BoardSetup::btnCancel_Click);
			// 
			// gbColor
			// 
			this->gbColor->Controls->Add(this->rbColorAvailable);
			this->gbColor->Controls->Add(this->rbColorWhite);
			this->gbColor->Controls->Add(this->rbColorBlack);
			this->gbColor->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->gbColor->Location = System::Drawing::Point(340, 51);
			this->gbColor->Name = L"gbColor";
			this->gbColor->Size = System::Drawing::Size(84, 96);
			this->gbColor->TabIndex = 3;
			this->gbColor->TabStop = false;
			this->gbColor->Text = L"颜色";
			// 
			// rbColorAvailable
			// 
			this->rbColorAvailable->AutoSize = true;
			this->rbColorAvailable->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->rbColorAvailable->Location = System::Drawing::Point(17, 66);
			this->rbColorAvailable->Name = L"rbColorAvailable";
			this->rbColorAvailable->Size = System::Drawing::Size(53, 17);
			this->rbColorAvailable->TabIndex = 5;
			this->rbColorAvailable->TabStop = true;
			this->rbColorAvailable->Text = L"空格";
			this->rbColorAvailable->UseVisualStyleBackColor = true;
			this->rbColorAvailable->CheckedChanged += gcnew System::EventHandler(this, &BoardSetup::rbColorAvailable_CheckedChanged);
			// 
			// rbColorWhite
			// 
			this->rbColorWhite->AutoSize = true;
			this->rbColorWhite->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->rbColorWhite->Location = System::Drawing::Point(17, 44);
			this->rbColorWhite->Name = L"rbColorWhite";
			this->rbColorWhite->Size = System::Drawing::Size(53, 17);
			this->rbColorWhite->TabIndex = 4;
			this->rbColorWhite->TabStop = true;
			this->rbColorWhite->Text = L"白子";
			this->rbColorWhite->UseVisualStyleBackColor = true;
			this->rbColorWhite->CheckedChanged += gcnew System::EventHandler(this, &BoardSetup::rbColorWhite_CheckedChanged);
			// 
			// rbColorBlack
			// 
			this->rbColorBlack->AutoSize = true;
			this->rbColorBlack->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->rbColorBlack->Location = System::Drawing::Point(17, 22);
			this->rbColorBlack->Name = L"rbColorBlack";
			this->rbColorBlack->Size = System::Drawing::Size(53, 17);
			this->rbColorBlack->TabIndex = 3;
			this->rbColorBlack->TabStop = true;
			this->rbColorBlack->Text = L"黑子";
			this->rbColorBlack->UseVisualStyleBackColor = true;
			this->rbColorBlack->CheckedChanged += gcnew System::EventHandler(this, &BoardSetup::rbColorBlack_CheckedChanged);
			// 
			// gbFirstPlayer
			// 
			this->gbFirstPlayer->Controls->Add(this->rbFirstWhite);
			this->gbFirstPlayer->Controls->Add(this->rbFirstBlack);
			this->gbFirstPlayer->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->gbFirstPlayer->Location = System::Drawing::Point(340, 153);
			this->gbFirstPlayer->Name = L"gbFirstPlayer";
			this->gbFirstPlayer->Size = System::Drawing::Size(84, 80);
			this->gbFirstPlayer->TabIndex = 6;
			this->gbFirstPlayer->TabStop = false;
			this->gbFirstPlayer->Text = L"先手方";
			// 
			// rbFirstWhite
			// 
			this->rbFirstWhite->AutoSize = true;
			this->rbFirstWhite->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->rbFirstWhite->Location = System::Drawing::Point(17, 42);
			this->rbFirstWhite->Name = L"rbFirstWhite";
			this->rbFirstWhite->Size = System::Drawing::Size(53, 17);
			this->rbFirstWhite->TabIndex = 7;
			this->rbFirstWhite->TabStop = true;
			this->rbFirstWhite->Text = L"白方";
			this->rbFirstWhite->UseVisualStyleBackColor = true;
			this->rbFirstWhite->CheckedChanged += gcnew System::EventHandler(this, &BoardSetup::rbFirstWhite_CheckedChanged);
			// 
			// rbFirstBlack
			// 
			this->rbFirstBlack->AutoSize = true;
			this->rbFirstBlack->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->rbFirstBlack->Location = System::Drawing::Point(17, 20);
			this->rbFirstBlack->Name = L"rbFirstBlack";
			this->rbFirstBlack->Size = System::Drawing::Size(53, 17);
			this->rbFirstBlack->TabIndex = 6;
			this->rbFirstBlack->TabStop = true;
			this->rbFirstBlack->Text = L"黑方";
			this->rbFirstBlack->UseVisualStyleBackColor = true;
			this->rbFirstBlack->CheckedChanged += gcnew System::EventHandler(this, &BoardSetup::rbFirstBlack_CheckedChanged);
			// 
			// btnReset
			// 
			this->btnReset->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnReset->Location = System::Drawing::Point(340, 239);
			this->btnReset->Name = L"btnReset";
			this->btnReset->Size = System::Drawing::Size(84, 28);
			this->btnReset->TabIndex = 8;
			this->btnReset->Text = L"重设";
			this->btnReset->UseVisualStyleBackColor = true;
			this->btnReset->Click += gcnew System::EventHandler(this, &BoardSetup::btnReset_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(143, 12);
			this->label1->TabIndex = 10;
			this->label1->Text = L"局面代码：(黑X 白O 空-)";
			// 
			// txtPos
			// 
			this->txtPos->Location = System::Drawing::Point(14, 24);
			this->txtPos->Name = L"txtPos";
			this->txtPos->Size = System::Drawing::Size(410, 21);
			this->txtPos->TabIndex = 0;
			this->txtPos->TextChanged += gcnew System::EventHandler(this, &BoardSetup::txtPos_TextChanged);
			// 
			// btnCopy
			// 
			this->btnCopy->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnCopy->Location = System::Drawing::Point(298, 4);
			this->btnCopy->Name = L"btnCopy";
			this->btnCopy->Size = System::Drawing::Size(60, 20);
			this->btnCopy->TabIndex = 1;
			this->btnCopy->Text = L"复制";
			this->btnCopy->UseVisualStyleBackColor = true;
			this->btnCopy->Click += gcnew System::EventHandler(this, &BoardSetup::btnCopy_Click);
			// 
			// btnPaste
			// 
			this->btnPaste->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnPaste->Location = System::Drawing::Point(364, 4);
			this->btnPaste->Name = L"btnPaste";
			this->btnPaste->Size = System::Drawing::Size(60, 20);
			this->btnPaste->TabIndex = 2;
			this->btnPaste->Text = L"粘贴";
			this->btnPaste->UseVisualStyleBackColor = true;
			this->btnPaste->Click += gcnew System::EventHandler(this, &BoardSetup::btnPaste_Click);
			// 
			// lblCopied
			// 
			this->lblCopied->AutoSize = true;
			this->lblCopied->Location = System::Drawing::Point(203, 9);
			this->lblCopied->Name = L"lblCopied";
			this->lblCopied->Size = System::Drawing::Size(89, 12);
			this->lblCopied->TabIndex = 12;
			this->lblCopied->Text = L"已复制到剪贴板";
			this->lblCopied->Visible = false;
			// 
			// tmrCopied
			// 
			this->tmrCopied->Interval = 3000;
			this->tmrCopied->Tick += gcnew System::EventHandler(this, &BoardSetup::tmrCopied_Tick);
			// 
			// BoardSetup
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->btnCancel;
			this->ClientSize = System::Drawing::Size(435, 384);
			this->Controls->Add(this->lblCopied);
			this->Controls->Add(this->btnPaste);
			this->Controls->Add(this->btnCopy);
			this->Controls->Add(this->txtPos);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnReset);
			this->Controls->Add(this->gbFirstPlayer);
			this->Controls->Add(this->gbColor);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->boardPanel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->KeyPreview = true;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"BoardSetup";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"预设棋局";
			this->Load += gcnew System::EventHandler(this, &BoardSetup::BoardSetup_Load);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &BoardSetup::BoardSetup_FormClosing);
			this->gbColor->ResumeLayout(false);
			this->gbColor->PerformLayout();
			this->gbFirstPlayer->ResumeLayout(false);
			this->gbFirstPlayer->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: System::Void picBoard_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		private: void setBoard();
		private: void initialize();
				 void setBoardFromFFO();
				 void setFFOFromBoard();
				 static System::String^ getFFOString(Board^ board, Chess mover);
				 static Chess getBoard(System::String^ ffopos, Board^ board);

		private:System::Void rbColorBlack_CheckedChanged(System::Object ^sender, System::EventArgs ^e);
		private:System::Void rbColorWhite_CheckedChanged(System::Object ^sender, System::EventArgs ^e);
		private:System::Void rbColorAvailable_CheckedChanged(System::Object ^sender, System::EventArgs ^e);
		private:System::Void rbFirstBlack_CheckedChanged(System::Object ^sender, System::EventArgs ^e);
		private:System::Void rbFirstWhite_CheckedChanged(System::Object ^sender, System::EventArgs ^e);
		private:System::Void btnOK_Click(System::Object ^sender, System::EventArgs ^e);
		private:System::Void btnCancel_Click(System::Object ^sender, System::EventArgs ^e);
		private:System::Void BoardSetup_FormClosing(System::Object ^sender, System::Windows::Forms::FormClosingEventArgs ^e);
		private:System::Void btnReset_Click(System::Object ^sender, System::EventArgs ^e);
		private:System::Void BoardSetup_Load(System::Object^  sender, System::EventArgs^  e);
		private:System::Void txtPos_TextChanged(System::Object ^sender, System::EventArgs ^e);
		private:System::Void btnCopy_Click(System::Object ^sender, System::EventArgs ^e);
		private:System::Void btnPaste_Click(System::Object ^sender, System::EventArgs ^e);
		private:System::Void tmrCopied_Tick(System::Object ^sender, System::EventArgs ^e);
	};
}
