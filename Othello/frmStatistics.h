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
								frmStatistics.h
		作者：Patrick
		概述：包含类 frmStatistics 的声明。
			frmStatistics 类实现“统计信息”窗体。

********************************************************************************
*/

#include "Setting.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

using namespace System::Drawing;


namespace Othello {

	public interface class StatisticsController {
	public:
		virtual void statisticsWindowShown() = 0;
		virtual void acquirePause(bool paused) = 0;
		virtual void statisticsWindowClosed() = 0;
	};

	/// <summary>
	/// Summary for frmStatistics
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmStatistics : public System::Windows::Forms::Form
	{
	public:
		frmStatistics(UserInfo^ userInfo, StatisticsController^ controller)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->userInfo = userInfo;
			this->controller = controller;
			controller->statisticsWindowShown();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmStatistics()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txtName;
	protected: 

	private: System::Windows::Forms::ListBox^  lstDifficulty;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  lblWin;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  lblDraw;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  lblLoss;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  lblWinPercentage;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  lblCurrentState;
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnClear;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  lblLongestWin;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  lblLongestDraw;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  lblLongestLoss;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  lblBestScore;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		array<PlayerType>^ playerList;
		UserInfo^ userInfo;
		StatisticsController^ controller;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtName = (gcnew System::Windows::Forms::TextBox());
			this->lstDifficulty = (gcnew System::Windows::Forms::ListBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->lblWin = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->lblDraw = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->lblLoss = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->lblWinPercentage = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->lblCurrentState = (gcnew System::Windows::Forms::Label());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnClear = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->lblLongestWin = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->lblLongestDraw = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->lblLongestLoss = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->lblBestScore = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 16);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"当前用户：";
			// 
			// txtName
			// 
			this->txtName->Location = System::Drawing::Point(16, 36);
			this->txtName->Name = L"txtName";
			this->txtName->ReadOnly = true;
			this->txtName->Size = System::Drawing::Size(108, 21);
			this->txtName->TabIndex = 0;
			this->txtName->TabStop = false;
			// 
			// lstDifficulty
			// 
			this->lstDifficulty->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F));
			this->lstDifficulty->FormattingEnabled = true;
			this->lstDifficulty->ItemHeight = 14;
			this->lstDifficulty->Location = System::Drawing::Point(16, 63);
			this->lstDifficulty->Name = L"lstDifficulty";
			this->lstDifficulty->Size = System::Drawing::Size(107, 158);
			this->lstDifficulty->TabIndex = 1;
			this->lstDifficulty->SelectedIndexChanged += gcnew System::EventHandler(this, &frmStatistics::lstDifficulty_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(146, 27);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"获胜的游戏：";
			// 
			// lblWin
			// 
			this->lblWin->Location = System::Drawing::Point(235, 27);
			this->lblWin->Name = L"lblWin";
			this->lblWin->Size = System::Drawing::Size(100, 12);
			this->lblWin->TabIndex = 4;
			this->lblWin->Text = L"Win";
			this->lblWin->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(146, 51);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(89, 12);
			this->label3->TabIndex = 3;
			this->label3->Text = L"为平局的游戏：";
			// 
			// lblDraw
			// 
			this->lblDraw->Location = System::Drawing::Point(235, 51);
			this->lblDraw->Name = L"lblDraw";
			this->lblDraw->Size = System::Drawing::Size(100, 12);
			this->lblDraw->TabIndex = 4;
			this->lblDraw->Text = L"Draw";
			this->lblDraw->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(146, 75);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(77, 12);
			this->label4->TabIndex = 3;
			this->label4->Text = L"失败的游戏：";
			// 
			// lblLoss
			// 
			this->lblLoss->Location = System::Drawing::Point(235, 75);
			this->lblLoss->Name = L"lblLoss";
			this->lblLoss->Size = System::Drawing::Size(100, 12);
			this->lblLoss->TabIndex = 4;
			this->lblLoss->Text = L"Loss";
			this->lblLoss->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(146, 99);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(65, 12);
			this->label6->TabIndex = 3;
			this->label6->Text = L"获胜概率：";
			// 
			// lblWinPercentage
			// 
			this->lblWinPercentage->Location = System::Drawing::Point(235, 99);
			this->lblWinPercentage->Name = L"lblWinPercentage";
			this->lblWinPercentage->Size = System::Drawing::Size(100, 12);
			this->lblWinPercentage->TabIndex = 4;
			this->lblWinPercentage->Text = L"WinPercentage";
			this->lblWinPercentage->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(146, 123);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(65, 12);
			this->label8->TabIndex = 3;
			this->label8->Text = L"当前状态：";
			// 
			// lblCurrentState
			// 
			this->lblCurrentState->Location = System::Drawing::Point(235, 123);
			this->lblCurrentState->Name = L"lblCurrentState";
			this->lblCurrentState->Size = System::Drawing::Size(100, 12);
			this->lblCurrentState->TabIndex = 4;
			this->lblCurrentState->Text = L"CurrentState";
			this->lblCurrentState->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// btnOK
			// 
			this->btnOK->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnOK->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnOK->Location = System::Drawing::Point(137, 249);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(96, 32);
			this->btnOK->TabIndex = 2;
			this->btnOK->Text = L"关闭";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &frmStatistics::btnOK_Click);
			// 
			// btnClear
			// 
			this->btnClear->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnClear->Location = System::Drawing::Point(239, 249);
			this->btnClear->Name = L"btnClear";
			this->btnClear->Size = System::Drawing::Size(96, 32);
			this->btnClear->TabIndex = 3;
			this->btnClear->Text = L"全部重置(&R)";
			this->btnClear->UseVisualStyleBackColor = true;
			this->btnClear->Click += gcnew System::EventHandler(this, &frmStatistics::btnClear_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(146, 147);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(65, 12);
			this->label5->TabIndex = 3;
			this->label5->Text = L"最长连胜：";
			// 
			// lblLongestWin
			// 
			this->lblLongestWin->Location = System::Drawing::Point(235, 147);
			this->lblLongestWin->Name = L"lblLongestWin";
			this->lblLongestWin->Size = System::Drawing::Size(100, 12);
			this->lblLongestWin->TabIndex = 4;
			this->lblLongestWin->Text = L"LongestWin";
			this->lblLongestWin->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(146, 171);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(65, 12);
			this->label9->TabIndex = 3;
			this->label9->Text = L"最长连平：";
			// 
			// lblLongestDraw
			// 
			this->lblLongestDraw->Location = System::Drawing::Point(235, 171);
			this->lblLongestDraw->Name = L"lblLongestDraw";
			this->lblLongestDraw->Size = System::Drawing::Size(100, 12);
			this->lblLongestDraw->TabIndex = 4;
			this->lblLongestDraw->Text = L"LongestDraw";
			this->lblLongestDraw->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(146, 195);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(65, 12);
			this->label11->TabIndex = 3;
			this->label11->Text = L"最长连败：";
			// 
			// lblLongestLoss
			// 
			this->lblLongestLoss->Location = System::Drawing::Point(235, 195);
			this->lblLongestLoss->Name = L"lblLongestLoss";
			this->lblLongestLoss->Size = System::Drawing::Size(100, 12);
			this->lblLongestLoss->TabIndex = 4;
			this->lblLongestLoss->Text = L"LongestLoss";
			this->lblLongestLoss->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(146, 219);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(65, 12);
			this->label13->TabIndex = 3;
			this->label13->Text = L"最佳比分：";
			// 
			// lblBestScore
			// 
			this->lblBestScore->Location = System::Drawing::Point(235, 219);
			this->lblBestScore->Name = L"lblBestScore";
			this->lblBestScore->Size = System::Drawing::Size(100, 12);
			this->lblBestScore->TabIndex = 4;
			this->lblBestScore->Text = L"BestScore";
			this->lblBestScore->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// frmStatistics
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->btnOK;
			this->ClientSize = System::Drawing::Size(347, 293);
			this->Controls->Add(this->btnClear);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->lblBestScore);
			this->Controls->Add(this->lblLongestLoss);
			this->Controls->Add(this->lblLongestDraw);
			this->Controls->Add(this->lblLongestWin);
			this->Controls->Add(this->lblCurrentState);
			this->Controls->Add(this->lblWinPercentage);
			this->Controls->Add(this->lblLoss);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->lblDraw);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->lblWin);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->lstDifficulty);
			this->Controls->Add(this->txtName);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->KeyPreview = true;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"frmStatistics";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"统计信息";
			this->Load += gcnew System::EventHandler(this, &frmStatistics::frmStatistics_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &frmStatistics::frmStatistics_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &frmStatistics::frmStatistics_FormClosing);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &frmStatistics::frmStatistics_KeyDown);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }
private: System::Void frmStatistics_Load(System::Object^  sender, System::EventArgs^  e) {
			 CenterToParent();
			 txtName->Text = userInfo->UserName;
			 playerList = gcnew array<PlayerType>(MAX_PLAYERTYPES);
			 lstDifficulty->Items->Add("随机");
			 playerList[0] = PlayerType::RANDOM;
			 lstDifficulty->Items->Add("大食");
			 playerList[1] = PlayerType::BIG_EAT;
			 lstDifficulty->Items->Add("地球");
			 playerList[2] = PlayerType::EARTH;
			 lstDifficulty->Items->Add("云朵");
			 playerList[3] = PlayerType::CLOUDS;
			 lstDifficulty->Items->Add("轨道");
			 playerList[4] = PlayerType::ORBIT;
			 lstDifficulty->Items->Add("月亮");
			 playerList[5] = PlayerType::MOON;
			 lstDifficulty->Items->Add("星星");
			 playerList[6] = PlayerType::STAR;
			 lstDifficulty->Items->Add("星系");
			 playerList[7] = PlayerType::GALAXY;
			 lstDifficulty->Items->Add("宇宙");
			 playerList[8] = PlayerType::UNIVERSE;
			 lstDifficulty->SelectedIndex = 0;
		 }
private: System::Void frmStatistics_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
		 }
private: 
	void refreshDisplay() {
		int index = lstDifficulty->SelectedIndex;
		Statistics^ stat = userInfo->getStatistics(playerList[index]);
		int total = stat->getWin() + stat->getDraw() + stat->getLoss();
		String^ winPercentage = (total) ? ((stat->getWin() * 100.0 / total).ToString("0.0") + "%")
			: "N/A";
		String^ currentState;
		if (total) {
			currentState = stat->getSeries() + " 连";
			switch (stat->getSeriesState()) {
				 case GameResult::WIN:
					 currentState += "胜";
					 break;
				 case GameResult::DRAW:
					 currentState += "平";
					 break;
				 case GameResult::LOSS:
					 currentState += "败";
			}
		} else currentState = "N/A";
		lblWin->Text = stat->getWin().ToString();
		lblDraw->Text = stat->getDraw().ToString();
		lblLoss->Text = stat->getLoss().ToString();
		lblWinPercentage->Text = winPercentage;
		lblCurrentState->Text = currentState;
		lblLongestWin->Text = stat->getLongestWin().ToString();
		lblLongestDraw->Text = stat->getLongestDraw().ToString();
		lblLongestLoss->Text = stat->getLongestLoss().ToString();
		String^ bestScore;
		if (total) {
			if (stat->getBestScore() == 0) {
				bestScore = "平局";
			} else {
				bestScore = (32 + stat->getBestScore() / 2).ToString() + ":"
					+ (32 - stat->getBestScore() / 2).ToString()
					+ " " + ((stat->getBestScore() > 0) ? "胜" : "负");
			}
		} else bestScore = "N/A";
		lblBestScore->Text = bestScore;
	}
	System::Void lstDifficulty_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		refreshDisplay();
	}
private:
	void restoreStatistics() {
		//controller->acquirePause(true);
		Windows::Forms::DialogResult res = MessageBox::Show(this, "确实要重置统计信息吗？",
			"重置统计信息", MessageBoxButtons::YesNo, MessageBoxIcon::Question,
			MessageBoxDefaultButton::Button2);
		//controller->acquirePause(false);
		if (res == Windows::Forms::DialogResult::No) return;
		for (int i = 0; i < lstDifficulty->Items->Count; i++)
			userInfo->getStatistics(playerList[i])->reset();
		refreshDisplay();
	}
	System::Void btnClear_Click(System::Object^  sender, System::EventArgs^  e) {
		restoreStatistics();
	}

private: System::Void frmStatistics_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 e->Handled = true;
			 if (e->Modifiers == Keys::None) {
				 switch (e->KeyCode) {
				 case Keys::R:
					 restoreStatistics();
					 break;
				 default:
					 e->Handled = false;
				 }
			 } else {
				 e->Handled = false;
			 }
		 }
private: System::Void frmStatistics_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 controller->statisticsWindowClosed();
		 }
};
}
