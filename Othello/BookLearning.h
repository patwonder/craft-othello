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
								BookLearning.h
		作者：Patrick
		概述：包含类 BookLearning 的声明。
			BookLearning 类实现“棋谱学习”窗体。

********************************************************************************
*/

#include "GameContext.h"
#include "Solver.h"
#include "Common.h"
#include "Conversions.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

using namespace System::Drawing;

namespace Othello {

	/// <summary>
	/// Summary for BookLearning
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class BookLearning : public System::Windows::Forms::Form
	{
	public:
		BookLearning(GameContext^ gc)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->gc = gc;
			isDone = false;
			solver = NULL;
			DialogResult = Windows::Forms::DialogResult::OK;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~BookLearning()
		{
			if (components) {
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

	private: System::Windows::Forms::Timer^  tmrLearn;
			 GameContext^ gc;
	private: System::Windows::Forms::ProgressBar^  pbLearn;
			 volatile bool isDone;
			 CraftEngine::Solver* solver;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tmrLearn = (gcnew System::Windows::Forms::Timer(this->components));
			this->pbLearn = (gcnew System::Windows::Forms::ProgressBar());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"宋体", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label1->Location = System::Drawing::Point(33, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(152, 16);
			this->label1->TabIndex = 0;
			this->label1->Text = L"正在学习，请稍候！";
			// 
			// tmrLearn
			// 
			this->tmrLearn->Enabled = true;
			this->tmrLearn->Interval = 1;
			this->tmrLearn->Tick += gcnew System::EventHandler(this, &BookLearning::tmrLearn_Tick);
			// 
			// pbLearn
			// 
			this->pbLearn->Location = System::Drawing::Point(14, 36);
			this->pbLearn->Name = L"pbLearn";
			this->pbLearn->Size = System::Drawing::Size(191, 21);
			this->pbLearn->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->pbLearn->TabIndex = 1;
			// 
			// BookLearning
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(213, 65);
			this->ControlBox = false;
			this->Controls->Add(this->pbLearn);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"BookLearning";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"棋谱学习";
			this->Load += gcnew System::EventHandler(this, &BookLearning::BookLearning_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &BookLearning::BookLearning_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &BookLearning::BookLearning_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void BookLearning_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->Cursor = Cursors::WaitCursor;
			 }
	private: System::Void BookLearning_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 }
	private: System::Void tmrLearn_Tick(System::Object^  sender, System::EventArgs^  e) {
				 tmrLearn->Enabled = false;
				 using namespace System::Threading;
				 solver = CraftEngine::Solver::newInstance();
				 Thread^ learnerThread = gcnew Thread(gcnew ThreadStart(this, &BookLearning::learnerStarter));
				 learnerThread->Start();
				 while (!isDone) {
					 System::Threading::Thread::Sleep(40);
					 System::Windows::Forms::Application::DoEvents();
					 if (solver)
						 pbLearn->Value = solver->getBookPercent();
				 }
				 delete solver;
				 solver = NULL;
				 this->Close();
			 }
	private: 
		void parseGame() {
			Board^ board = gc->getInitialBoard();
			int bd[WIDTH * HEIGHT];
			Conversions::convertBoard(board, bd);
			int steps[WIDTH * HEIGHT];
			int pptr = 0;
			for (int i = 1; i < gc->getCurrentStep(); i++) {
				Othello::Move move = gc->getMove(i);
				if (move.getColor() != Chess::AVAILABLE) {
					steps[pptr++] = move.getX() * HEIGHT + move.getY();
				}
			}
			int fp = CraftEngine::Solver::AV;
			for (int i = 1; i < gc->getCurrentStep(); i++) {
				Othello::Move move = gc->getMove(i);
				if (move.getColor() != Chess::AVAILABLE) {
					fp = (move.getColor() == Chess::BLACK) ? CraftEngine::Solver::BLACK : CraftEngine::Solver::WHITE;
					break;
				}
			}
			if (fp != CraftEngine::Solver::AV) solver->parseGame(bd, steps, pptr, fp);
		}
		void learnerStarter() {
			parseGame();
			solver->extendBook();
			System::Threading::Thread::Sleep(500);
			isDone = true;
		}
	private: System::Void BookLearning_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 if (!isDone)
					 e->Cancel = true;
				 else {
					 this->Cursor = Cursors::Default;
				 }
			 }
	};
}
