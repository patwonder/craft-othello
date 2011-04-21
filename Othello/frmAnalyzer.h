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
								frmAnalyzer.h
		作者：Patrick
		概述：包含类 frmAnalyzer 的声明。
			frmAnalyzer 类实现“棋局分析”窗体。

********************************************************************************
*/

#include "GameContext.h"
#include "SearchRelated.h"
#include "Analyze.h"
#include "ChessBoard.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

using namespace System::Drawing;

namespace CraftEngine {
class Solver;
class AnalyzeResult;
} // namespace CraftEngine

namespace Othello {

	public interface class AnalyzeController {
	public:
		virtual void analyzeStarted() = 0;
		virtual void analyzeEnded() = 0;
		virtual void analyzerOpened() = 0;
		virtual void analyzerClosed() = 0;
		virtual void showMove(int step) = 0;
		virtual void startFromCurrentStep() = 0;
	};

	/// <summary>
	/// frmAnalyzer 摘要
	///
	/// 警告: 如果更改此类的名称，则需要更改
	///          与此类所依赖的所有 .resx 文件关联的托管资源编译器工具的
	///          “资源文件名”属性。否则，
	///          设计器将不能与此窗体的关联
	///          本地化资源正确交互。
	/// </summary>
	public ref class frmAnalyzer : public System::Windows::Forms::Form
	{
	public:
		frmAnalyzer(AnalyzeController^ controller, GameContext^ gc,
			PlayerType defaultPlayerType);
	protected:
		~frmAnalyzer();
		!frmAnalyzer();
	private:
		AnalyzeController^ controller;
		GameContext^ gc;
		System::Threading::ThreadStart^ solverStart;
		System::Threading::Thread^ solverThread;
		CraftEngine::Solver* solver;
		CraftEngine::AnalyzeResult* analyzeResult;
		int currentStep, currentMaxStep;
		int previousStep;
		bool isGameEnded;
		System::String^ endDescription;
		volatile bool isDone;
		bool analyzing;
		bool aborted;
		PlayerType currentPlayerType;
		SearchOptions options;
		DetailedAnalyzeResult^ dAnalyzeResult;
		static const Char black = 'X';
		static const Char white = 'O';
		static const Char av = '.';
		static const Char played = '@';
		static const Char best = '*';
		static String^ const COLUMN = "  A B C D E F G H ";
	private: System::Windows::Forms::ComboBox^  cbAnalyzer;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ProgressBar^  pbAnalyze;
	private: System::Windows::Forms::Label^  lblState;
	private: System::Windows::Forms::ListBox^  lstResult;
	private: System::Windows::Forms::Button^  btnSave;
	private: System::Windows::Forms::Button^  btnClose;
	private: System::Windows::Forms::Button^  btnAnalyze;
	private: System::Windows::Forms::Panel^  pnlButtons;
	private: System::Windows::Forms::Button^  btnContinue;
	private: System::ComponentModel::IContainer^  components;

	protected: 

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->cbAnalyzer = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->pbAnalyze = (gcnew System::Windows::Forms::ProgressBar());
			this->lblState = (gcnew System::Windows::Forms::Label());
			this->lstResult = (gcnew System::Windows::Forms::ListBox());
			this->btnSave = (gcnew System::Windows::Forms::Button());
			this->btnClose = (gcnew System::Windows::Forms::Button());
			this->btnAnalyze = (gcnew System::Windows::Forms::Button());
			this->pnlButtons = (gcnew System::Windows::Forms::Panel());
			this->btnContinue = (gcnew System::Windows::Forms::Button());
			this->pnlButtons->SuspendLayout();
			this->SuspendLayout();
			// 
			// cbAnalyzer
			// 
			this->cbAnalyzer->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbAnalyzer->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->cbAnalyzer->FormattingEnabled = true;
			this->cbAnalyzer->Location = System::Drawing::Point(12, 24);
			this->cbAnalyzer->Name = L"cbAnalyzer";
			this->cbAnalyzer->Size = System::Drawing::Size(162, 20);
			this->cbAnalyzer->TabIndex = 0;
			this->cbAnalyzer->SelectedIndexChanged += gcnew System::EventHandler(this, &frmAnalyzer::cbAnalyzer_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"分析者";
			// 
			// pbAnalyze
			// 
			this->pbAnalyze->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->pbAnalyze->Location = System::Drawing::Point(180, 24);
			this->pbAnalyze->MarqueeAnimationSpeed = 50;
			this->pbAnalyze->Name = L"pbAnalyze";
			this->pbAnalyze->Size = System::Drawing::Size(233, 20);
			this->pbAnalyze->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->pbAnalyze->TabIndex = 2;
			// 
			// lblState
			// 
			this->lblState->AutoSize = true;
			this->lblState->Location = System::Drawing::Point(180, 9);
			this->lblState->Name = L"lblState";
			this->lblState->Size = System::Drawing::Size(0, 12);
			this->lblState->TabIndex = 3;
			// 
			// lstResult
			// 
			this->lstResult->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->lstResult->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lstResult->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F));
			this->lstResult->FormattingEnabled = true;
			this->lstResult->HorizontalScrollbar = true;
			this->lstResult->ItemHeight = 14;
			this->lstResult->Location = System::Drawing::Point(12, 50);
			this->lstResult->Name = L"lstResult";
			this->lstResult->ScrollAlwaysVisible = true;
			this->lstResult->Size = System::Drawing::Size(401, 226);
			this->lstResult->TabIndex = 1;
			this->lstResult->SelectedIndexChanged += gcnew System::EventHandler(this, &frmAnalyzer::lstResult_SelectedIndexChanged);
			// 
			// btnSave
			// 
			this->btnSave->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnSave->Location = System::Drawing::Point(90, 0);
			this->btnSave->Name = L"btnSave";
			this->btnSave->Size = System::Drawing::Size(84, 28);
			this->btnSave->TabIndex = 4;
			this->btnSave->Text = L"保存结果(&S)";
			this->btnSave->UseVisualStyleBackColor = true;
			this->btnSave->Click += gcnew System::EventHandler(this, &frmAnalyzer::btnSave_Click);
			// 
			// btnClose
			// 
			this->btnClose->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnClose->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnClose->Location = System::Drawing::Point(180, 0);
			this->btnClose->Name = L"btnClose";
			this->btnClose->Size = System::Drawing::Size(84, 28);
			this->btnClose->TabIndex = 5;
			this->btnClose->Text = L"关闭(&C)";
			this->btnClose->UseVisualStyleBackColor = true;
			this->btnClose->Click += gcnew System::EventHandler(this, &frmAnalyzer::btnClose_Click);
			// 
			// btnAnalyze
			// 
			this->btnAnalyze->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnAnalyze->Location = System::Drawing::Point(0, 0);
			this->btnAnalyze->Name = L"btnAnalyze";
			this->btnAnalyze->Size = System::Drawing::Size(84, 28);
			this->btnAnalyze->TabIndex = 3;
			this->btnAnalyze->Text = L"开始分析(&A)";
			this->btnAnalyze->UseVisualStyleBackColor = true;
			this->btnAnalyze->Click += gcnew System::EventHandler(this, &frmAnalyzer::btnAnalyze_Click);
			// 
			// pnlButtons
			// 
			this->pnlButtons->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->pnlButtons->Controls->Add(this->btnClose);
			this->pnlButtons->Controls->Add(this->btnAnalyze);
			this->pnlButtons->Controls->Add(this->btnSave);
			this->pnlButtons->Location = System::Drawing::Point(149, 282);
			this->pnlButtons->Name = L"pnlButtons";
			this->pnlButtons->Size = System::Drawing::Size(264, 28);
			this->pnlButtons->TabIndex = 5;
			// 
			// btnContinue
			// 
			this->btnContinue->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnContinue->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnContinue->Location = System::Drawing::Point(12, 282);
			this->btnContinue->Name = L"btnContinue";
			this->btnContinue->Size = System::Drawing::Size(84, 28);
			this->btnContinue->TabIndex = 2;
			this->btnContinue->Text = L"继续游戏(&C)";
			this->btnContinue->UseVisualStyleBackColor = true;
			this->btnContinue->Click += gcnew System::EventHandler(this, &frmAnalyzer::btnContinue_Click);
			// 
			// frmAnalyzer
			// 
			this->AcceptButton = this->btnAnalyze;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->btnClose;
			this->ClientSize = System::Drawing::Size(425, 320);
			this->Controls->Add(this->pnlButtons);
			this->Controls->Add(this->btnContinue);
			this->Controls->Add(this->lstResult);
			this->Controls->Add(this->lblState);
			this->Controls->Add(this->pbAnalyze);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->cbAnalyzer);
			this->KeyPreview = true;
			this->MinimumSize = System::Drawing::Size(400, 200);
			this->Name = L"frmAnalyzer";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"棋局分析";
			this->Load += gcnew System::EventHandler(this, &frmAnalyzer::frmAnalyzer_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &frmAnalyzer::frmAnalyzer_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &frmAnalyzer::frmAnalyzer_FormClosing);
			this->pnlButtons->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		void addAnalyzers();
		String^ getAnalyzerString(PlayerType type);
		String^ getTwoCharRep(int value);
		void solverStarter();
		void parseGame();
		AnalyzedMove getAnalyzedResult(int step, int empties, Chess color,
			int playedMove, int playedEval, 
			int bestMove, int bestEval, SearchOptions options);
		String^ getResultDescription(int result, int bestMove, int empties);
		String^ getEndDescription();
		void saveResult();
		void saveResult(String^ fileName);
		System::String^ chessBoardToString(ChessBoard^ cb);
		System::String^ chessBoardToString(ChessBoard^ cb, AnalyzedMove move);
	private:System::Void frmAnalyzer_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
	private:System::Void cbAnalyzer_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private:System::Void frmAnalyzer_Load(System::Object ^sender, System::EventArgs ^e);
	private:System::Void btnClose_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void btnAnalyze_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void frmAnalyzer_FormClosed(System::Object ^sender, System::Windows::Forms::FormClosedEventArgs ^e);
	private:System::Void lstResult_SelectedIndexChanged(System::Object ^sender, System::EventArgs ^e);
	private:System::Void btnSave_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void btnContinue_Click(System::Object ^sender, System::EventArgs ^e);
	public:
		void startAnalyze();
		void stopAnalyze();
		int getSteps();
		AnalyzedMove getAnalyzedResult(int step);
		PlayerType getAnalyzerType();
	};
} //namespace Othello
