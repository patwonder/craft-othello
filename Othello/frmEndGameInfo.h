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
								frmEndGameInfo.h
		作者：Patrick
		概述：包含类 frmEndGameInfo 的声明。
			frmEndGameInfo 类实现“残局信息”窗体。

********************************************************************************
*/

#include "Common.h"
#include "WThorReader.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Drawing;


namespace Othello {

	public interface class EndGameInfoController {
	public:
		virtual void endGameInfoShown() = 0;
		virtual void endGameInfoClosed() = 0;
		virtual void acquireChange() = 0;
	};

	/// <summary>
	/// frmEndGameInfo 摘要
	///
	/// 警告: 如果更改此类的名称，则需要更改
	///          与此类所依赖的所有 .resx 文件关联的托管资源编译器工具的
	///          “资源文件名”属性。否则，
	///          设计器将不能与此窗体的关联
	///          本地化资源正确交互。
	/// </summary>
	public ref class frmEndGameInfo : public System::Windows::Forms::Form
	{
	public:
		frmEndGameInfo(EndGameInfoController^ controller) {
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->controller = controller;
			controller->endGameInfoShown();
		}
		frmEndGameInfo(EndGameInfoController^ controller,
			WThorGame^ game, int bestResult, int empties, Chess player)
		{
			this->frmEndGameInfo::frmEndGameInfo(controller);
			showInfo(game, bestResult, empties, player);
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~frmEndGameInfo()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::TextBox^  txtDescription;
	private: System::Windows::Forms::Button^  btnStart;
	private: System::Windows::Forms::Button^  btnChangeOne;
	protected: 
	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;
		EndGameInfoController^ controller;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtDescription = (gcnew System::Windows::Forms::TextBox());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->btnChangeOne = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"棋局描述：";
			// 
			// txtDescription
			// 
			this->txtDescription->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->txtDescription->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F));
			this->txtDescription->Location = System::Drawing::Point(12, 24);
			this->txtDescription->Multiline = true;
			this->txtDescription->Name = L"txtDescription";
			this->txtDescription->ReadOnly = true;
			this->txtDescription->Size = System::Drawing::Size(317, 160);
			this->txtDescription->TabIndex = 0;
			// 
			// btnStart
			// 
			this->btnStart->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnStart->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnStart->Location = System::Drawing::Point(131, 190);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(96, 32);
			this->btnStart->TabIndex = 1;
			this->btnStart->Text = L"开始残局";
			this->btnStart->UseVisualStyleBackColor = true;
			this->btnStart->Click += gcnew System::EventHandler(this, &frmEndGameInfo::btnStart_Click);
			// 
			// btnChangeOne
			// 
			this->btnChangeOne->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnChangeOne->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnChangeOne->Location = System::Drawing::Point(233, 190);
			this->btnChangeOne->Name = L"btnChangeOne";
			this->btnChangeOne->Size = System::Drawing::Size(96, 32);
			this->btnChangeOne->TabIndex = 2;
			this->btnChangeOne->Text = L"换一个(&A)";
			this->btnChangeOne->UseVisualStyleBackColor = true;
			this->btnChangeOne->Click += gcnew System::EventHandler(this, &frmEndGameInfo::btnChangeOne_Click);
			// 
			// frmEndGameInfo
			// 
			this->AcceptButton = this->btnStart;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->btnStart;
			this->ClientSize = System::Drawing::Size(342, 232);
			this->Controls->Add(this->btnChangeOne);
			this->Controls->Add(this->btnStart);
			this->Controls->Add(this->txtDescription);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->KeyPreview = true;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"frmEndGameInfo";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"残局信息";
			this->Load += gcnew System::EventHandler(this, &frmEndGameInfo::frmEndGameInfo_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &frmEndGameInfo::frmEndGameInfo_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		static String^ getResultDescription(WThorGame^ game, int bestResult, int empties, Chess player) {
			String^ result = "这是由 " + game->black.Name + "(黑) 与 "
				+ game->white.Name + "(白) 在" + game->year + "年的 "
				+ game->tour.Name + " 比赛上下的一盘棋。";
			result += "\r\n最终 ";
			if (game->result > 0) {
				result += game->black.Name + " 战胜了 " + game->white.Name;
				result += "，比分为 " + (game->result / 2 + 32).ToString()
					+ ":" + (32 - game->result / 2).ToString();
			} else if (game->result < 0) {
				result += game->white.Name + " 战胜了 " + game->black.Name;
				result += "，比分为 " + (32 - game->result / 2).ToString()
					+ ":" + (game->result / 2 + 32).ToString();
			} else {
				result += game->black.Name + " 与 " + game->white.Name + " 握手言和";
			}
			result += "。\r\n您现在的角色是 "
				+ ((player == Chess::BLACK) ? (game->black.Name + "(黑)") : (game->white.Name + "(白)"));
			result += "。\r\n棋局还剩下最后" + empties + "步，" + "最佳结果是 ";
			if (bestResult > 0) {
				result += (bestResult / 2 + 32).ToString()
					+ ":" + (32 - bestResult / 2).ToString() + " 胜";
			} else if (bestResult < 0) {
				result += (bestResult / 2 + 32).ToString()
					+ ":" + (32 - bestResult / 2).ToString() + " 负";
			} else {
				result += "平局";
			}
			result += "。\r\n准备好迎接挑战！";
			return result;
		}
public:
		void showInfo(WThorGame^ game, int bestResult, int empties, Chess player) {
			txtDescription->Text = getResultDescription(game, bestResult, empties, player);
		}
private: System::Void frmEndGameInfo_Load(System::Object^  sender, System::EventArgs^  e) {
			 CenterToParent();
		 }
private: System::Void btnStart_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void frmEndGameInfo_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 controller->endGameInfoClosed();
		 }
private: System::Void btnChangeOne_Click(System::Object^  sender, System::EventArgs^  e) {
			 controller->acquireChange();
		 }
};

}
