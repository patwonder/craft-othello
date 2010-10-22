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
								PatternPreparer.h
		作者：Patrick
		概述：包含类 PatternPreparer 的声明。
			PatternPreparer 类实现“模板准备”窗体。
		
		注：此类为进行模板计算专用，正式版中不包含此内容。

********************************************************************************
*/

#include "Solver.h"
#include "ChessBoard.h"

#ifdef USERECORD

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

using namespace System::Drawing;


namespace Othello {

	/// <summary>
	/// Summary for PatternPreparer
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class PatternPreparer : public System::Windows::Forms::Form
	{
	public:
		PatternPreparer(void)
		{
			InitializeComponent();
			dlgSaveFile = gcnew System::Windows::Forms::SaveFileDialog();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PatternPreparer()
		{
			if (components)
			{
				delete components;
			}
			delete solver;
		}
	private: System::Windows::Forms::OpenFileDialog^  dlgChooseFile;
	private: System::Windows::Forms::SaveFileDialog^  dlgSaveFile;
	private: System::Windows::Forms::ListBox^  lstFiles;
	private: System::Windows::Forms::TextBox^  txtInfo;
	private: System::Windows::Forms::Button^  btnOpenFile;
	private: System::Windows::Forms::Button^  btnLearn;
	private: System::Windows::Forms::ProgressBar^  pbLearn;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		array<System::String^>^ fileList;
		int fileCount;
		bool preparing, aborted, isDone;
		static const int MAXFILE = 100;
		int endStep, endResult;
	private: System::Windows::Forms::Label^  lblState;

	private: System::Windows::Forms::Label^  lblFile;
	private: System::Windows::Forms::Label^  lblGame;
			 Solver* solver;

	value struct WTHORHeader {
		unsigned char Century;
		unsigned char Year;
		unsigned char Month;
		unsigned char Day;
		unsigned long Anzahl;
		unsigned short TourAnz;
		unsigned short YearOfGames;
		unsigned char SizeOfBoard;
		unsigned char IsAnEndgameSolitaireFile;
		unsigned char NumberOfEmptiesForTheoricalScoreCalculation;
		unsigned char Reserved;
	};

	value struct WTHORRecord {
		unsigned short TourNumber;
		unsigned short Black;
		unsigned short White;
		unsigned char RealScore;
		unsigned char TheoryScore;
		array<unsigned char>^ MoveList;
	};
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->dlgChooseFile = (gcnew System::Windows::Forms::OpenFileDialog());
			this->lstFiles = (gcnew System::Windows::Forms::ListBox());
			this->txtInfo = (gcnew System::Windows::Forms::TextBox());
			this->btnOpenFile = (gcnew System::Windows::Forms::Button());
			this->btnLearn = (gcnew System::Windows::Forms::Button());
			this->pbLearn = (gcnew System::Windows::Forms::ProgressBar());
			this->lblState = (gcnew System::Windows::Forms::Label());
			this->lblFile = (gcnew System::Windows::Forms::Label());
			this->lblGame = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// dlgChooseFile
			// 
			this->dlgChooseFile->Multiselect = true;
			// 
			// lstFiles
			// 
			this->lstFiles->FormattingEnabled = true;
			this->lstFiles->HorizontalScrollbar = true;
			this->lstFiles->ItemHeight = 12;
			this->lstFiles->Location = System::Drawing::Point(12, 12);
			this->lstFiles->Name = L"lstFiles";
			this->lstFiles->Size = System::Drawing::Size(96, 148);
			this->lstFiles->TabIndex = 0;
			this->lstFiles->SelectedIndexChanged += gcnew System::EventHandler(this, &PatternPreparer::lstFiles_SelectedIndexChanged);
			// 
			// txtInfo
			// 
			this->txtInfo->Location = System::Drawing::Point(126, 13);
			this->txtInfo->Multiline = true;
			this->txtInfo->Name = L"txtInfo";
			this->txtInfo->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->txtInfo->Size = System::Drawing::Size(148, 237);
			this->txtInfo->TabIndex = 1;
			// 
			// btnOpenFile
			// 
			this->btnOpenFile->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnOpenFile->Location = System::Drawing::Point(12, 184);
			this->btnOpenFile->Name = L"btnOpenFile";
			this->btnOpenFile->Size = System::Drawing::Size(96, 30);
			this->btnOpenFile->TabIndex = 2;
			this->btnOpenFile->Text = L"打开文件";
			this->btnOpenFile->UseVisualStyleBackColor = true;
			this->btnOpenFile->Click += gcnew System::EventHandler(this, &PatternPreparer::btnOpenFile_Click);
			// 
			// btnLearn
			// 
			this->btnLearn->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnLearn->Location = System::Drawing::Point(12, 220);
			this->btnLearn->Name = L"btnLearn";
			this->btnLearn->Size = System::Drawing::Size(96, 30);
			this->btnLearn->TabIndex = 2;
			this->btnLearn->Text = L"开始处理";
			this->btnLearn->UseVisualStyleBackColor = true;
			this->btnLearn->Click += gcnew System::EventHandler(this, &PatternPreparer::btnLearn_Click);
			// 
			// pbLearn
			// 
			this->pbLearn->Location = System::Drawing::Point(11, 284);
			this->pbLearn->Name = L"pbLearn";
			this->pbLearn->Size = System::Drawing::Size(261, 24);
			this->pbLearn->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->pbLearn->TabIndex = 3;
			// 
			// lblState
			// 
			this->lblState->AutoSize = true;
			this->lblState->Location = System::Drawing::Point(16, 261);
			this->lblState->Name = L"lblState";
			this->lblState->Size = System::Drawing::Size(0, 12);
			this->lblState->TabIndex = 4;
			// 
			// lblFile
			// 
			this->lblFile->Location = System::Drawing::Point(96, 261);
			this->lblFile->Name = L"lblFile";
			this->lblFile->Size = System::Drawing::Size(88, 12);
			this->lblFile->TabIndex = 5;
			this->lblFile->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// lblGame
			// 
			this->lblGame->Location = System::Drawing::Point(190, 261);
			this->lblGame->Name = L"lblGame";
			this->lblGame->Size = System::Drawing::Size(82, 12);
			this->lblGame->TabIndex = 5;
			this->lblGame->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// PatternPreparer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 320);
			this->Controls->Add(this->lblGame);
			this->Controls->Add(this->lblFile);
			this->Controls->Add(this->lblState);
			this->Controls->Add(this->pbLearn);
			this->Controls->Add(this->btnLearn);
			this->Controls->Add(this->btnOpenFile);
			this->Controls->Add(this->txtInfo);
			this->Controls->Add(this->lstFiles);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"PatternPreparer";
			this->Text = L"模板准备";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &PatternPreparer::BookLearner_FormClosing);
			this->Load += gcnew System::EventHandler(this, &PatternPreparer::BookLearner_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void BookLearner_Load(System::Object^  sender, System::EventArgs^  e) {
				 fileList = gcnew array<System::String^>(MAXFILE);
				 fileCount = 0;
				 solver = new Solver();
				 btnLearn->Enabled = false;
				 preparing = false;
			 }
private: System::Void btnOpenFile_Click(System::Object^  sender, System::EventArgs^  e) {
			 dlgChooseFile->Filter = "WThor数据文件(*.wtb)|*.WTB";
			 dlgChooseFile->CheckFileExists = true;
			 dlgChooseFile->Title = "选择数据文件";
			 dlgChooseFile->FileName = "";
			 System::Windows::Forms::DialogResult res = dlgChooseFile->ShowDialog(this);
			 if (res == System::Windows::Forms::DialogResult::Cancel) {
				 return;
			 }
			 fileCount = dlgChooseFile->FileNames->Length;
			 fileList = safe_cast<array<System::String^>^>(dlgChooseFile->FileNames->Clone());
			 lstFiles->Items->Clear();
			 for (int i = 0; i < fileCount; i++)
				 lstFiles->Items->Add(fileList[i]);
			 btnLearn->Enabled = true;
		 }
private: System::Void lstFiles_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (lstFiles->SelectedIndex == -1) return;
			 readHeader(fileList[lstFiles->SelectedIndex]);
		 }
private: void readHeader(String^ currentFile) {
			 WTHORHeader header;
			 System::IO::FileStream^ fs;
			 try {
				 fs = gcnew System::IO::FileStream(currentFile, System::IO::FileMode::Open);
			 } catch (System::IO::IOException^) {
				 System::Windows::Forms::MessageBox::Show(this, "Cannot open file: " + currentFile, "Error", 
					 System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
				 return;
			 }
			 System::IO::BinaryReader^ br = gcnew System::IO::BinaryReader(fs);
			 try {
				 header.Century = br->ReadByte();
				 header.Year = br->ReadByte();
				 header.Month = br->ReadByte();
				 header.Day = br->ReadByte();
				 header.Anzahl = br->ReadUInt32();
				 header.TourAnz = br->ReadUInt16();
				 header.YearOfGames = br->ReadUInt16();
				 header.SizeOfBoard= br->ReadByte();
				 header.IsAnEndgameSolitaireFile = br->ReadByte();
				 header.NumberOfEmptiesForTheoricalScoreCalculation = br->ReadByte();
				 header.Reserved = br->ReadByte();
				 String^ info = "";
				 info += "文件建立年份: " + (int)(header.Century * 100 + header.Year) + "\r\n";
				 info += "文件建立日期: " + (int)header.Month + " 月 " + (int)header.Day + " 日\r\n";
				 info += "对局总数: " + header.Anzahl + "\r\n";
				 info += "对局年份: " + header.YearOfGames + "\r\n";
				 info += "棋盘大小: " + ((header.SizeOfBoard == 0 || header.SizeOfBoard == 8) ? "8 * 8" : "其他") + "\r\n";
				 info += "是否Solitaire游戏: " + ((header.IsAnEndgameSolitaireFile == 0) ? "否" : "是") + "\r\n";
				 info += "终局计算步数: " + (int)header.NumberOfEmptiesForTheoricalScoreCalculation + "\r\n";
				 txtInfo->Text = info;
				 //WTHORRecord record;
				 //record.TourNumber = br->ReadUInt16();
				 //info = "比赛名称序号: " + record.TourNumber + "\r\n";
				 //record.Black = br->ReadUInt16();
				 //info += "黑方棋手名称: " + record.Black + "\r\n";
				 //record.White = br->ReadUInt16();
				 //info += "白方棋手名称: " + record.White + "\r\n";
				 //record.RealScore = br->ReadByte();
				 //info += "实际比分: " + (int)record.RealScore + " : " + (int)(64 - record.RealScore) + "\r\n";
				 //record.TheoryScore = br->ReadByte();
				 //info += "理论最佳比分: " + (int)record.TheoryScore + " : " + (int)(64 - record.TheoryScore) + "\r\n";
				 //record.MoveList = gcnew array<unsigned char>(60);
				 //info += "棋步序列: ";
				 //for (int i = 0; i < 60; i++) {
					// record.MoveList[i] = br->ReadByte();
					// info += (int)record.MoveList[i] + " ";
				 //}
				 //System::Windows::Forms::MessageBox::Show(this, info);
				 br->Close();
			 } catch (System::IO::IOException^) {
				 System::Windows::Forms::MessageBox::Show(this, "File format error: " + currentFile, "Error", 
					 System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
				 return;
			 }
		 }

private: void solverStarter() {
			 solver->prepareCurrentGame(endStep, endResult);
			 Solver::clearCache();
			 isDone = true;
		 }
private: 
	void learnBook() {
		if (preparing) {
			aborted = true;
			lblState->Text = "正在停止...";
			lblFile->Visible = false;
			btnLearn->Enabled = false;
		} else {
			dlgSaveFile->Filter = "Pattern Configuration Database(*.pcd)|*.PCD";
			dlgSaveFile->Title = "选择保存位置";
			dlgSaveFile->FileName = "";
			System::Windows::Forms::DialogResult res = dlgSaveFile->ShowDialog(this);
			if (res == System::Windows::Forms::DialogResult::Cancel) {
				return;
			}
			String^ saveFileName = dlgSaveFile->FileName;
			lstFiles->Enabled = false;
			lblState->Text = "正在处理文件: ";
			lblFile->Visible = true;
			btnLearn->Text = "停止处理";
			txtInfo->Enabled = false;
			btnOpenFile->Enabled = false;
			preparing = true;
			aborted = false;
			for (int i = 0; i < fileCount; i++) {
				lblFile->Text = (i + 1).ToString() + "/" + (fileCount);
				learnFile(fileList[i]);
				System::Windows::Forms::Application::DoEvents();
				if (aborted) break;
			}
			solver->savePreparedConfiguration(saveFileName);
			if (!aborted)
				System::Windows::Forms::MessageBox::Show(this, "处理完毕。", "模板准备");
			btnLearn->Text = "开始处理";
			lblState->Text = "处理完毕";
			lblFile->Visible = false;
			txtInfo->Enabled = true;
			preparing = false;
			lstFiles->Enabled = true;
			btnOpenFile->Enabled = true;
			btnLearn->Enabled = true;
		}
	}
	System::Void btnLearn_Click(System::Object^  sender, System::EventArgs^  e) {
			 learnBook();
	}
private: void learnFile(String^ currentFile) {
			 WTHORHeader header;
			 System::IO::FileStream^ fs;
			 try {
				 fs = gcnew System::IO::FileStream(currentFile, System::IO::FileMode::Open);
			 } catch (System::IO::IOException^) {
				 System::Windows::Forms::MessageBox::Show(this, "Cannot open file: " + currentFile, "Error", 
					 System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
				 return;
			 }
			 System::IO::BinaryReader^ br = gcnew System::IO::BinaryReader(fs);
			 header.Century = br->ReadByte();
			 header.Year = br->ReadByte();
			 header.Month = br->ReadByte();
			 header.Day = br->ReadByte();
			 header.Anzahl = br->ReadUInt32();
			 header.TourAnz = br->ReadUInt16();
			 header.YearOfGames = br->ReadUInt16();
			 header.SizeOfBoard= br->ReadByte();
			 header.IsAnEndgameSolitaireFile = br->ReadByte();
			 header.NumberOfEmptiesForTheoricalScoreCalculation = br->ReadByte();
			 header.Reserved = br->ReadByte();
			 lblGame->Visible = true;
			 pbLearn->Value = 0;
			 endStep = 20;
			 for (unsigned int i = 0; i < header.Anzahl; i++) {
				 lblGame->Text = i + "/" + header.Anzahl;
				 System::Windows::Forms::Application::DoEvents();
				 WTHORRecord record;
				 record.TourNumber = br->ReadUInt16();
				 record.Black = br->ReadUInt16();
				 record.White = br->ReadUInt16();
				 record.RealScore = br->ReadByte();
				 record.TheoryScore = br->ReadByte();
				 endResult = record.TheoryScore * 2 - Solver::MAXSTEP;
				 record.MoveList = gcnew array<unsigned char>(60);
				 for (int i = 0; i < 60; i++) {
					 record.MoveList[i] = br->ReadByte();
				 }
				 Board^ bd = safe_cast<Board^>(ChessBoard::defaultBoard->Clone());
				 int moveList[60];
				 for (int i = 0; i < 60; i++) {
					 unsigned char currentMove = record.MoveList[i];
					 if (currentMove == 0) {
						 moveList[i] = -1;
						 break;
					 }
					 moveList[i] = (currentMove % 10 - 1) * HEIGHT + (currentMove / 10 - 1);
				 }
				 solver->parseGame(bd, moveList);
				 isDone = false;
				 using namespace System::Threading;
				 Thread^ solverThread = gcnew Thread(gcnew ThreadStart(this, &PatternPreparer::solverStarter));
				 solverThread->Start();
				 while (!isDone) {
					 Thread::Sleep(20);
					 Application::DoEvents();
				 }
				 pbLearn->Value = (i + 1) * 100 / (header.Anzahl);
				 if (aborted) break;
			 }
			 pbLearn->Value = 100;
			 br->Close();
			 lblGame->Visible = false;
		 }
private: System::Void BookLearner_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 if (preparing) {
				 e->Cancel = true;
			 }
		 }
};

}

#endif //USERECORD