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
								PatternLearning.h
		作者：Patrick
		概述：包含类 PatternLearning 的声明。
			PatternLearning 类实现“模板学习”窗体。
		
		注：此类为进行模板计算专用，正式版中不包含此内容。

********************************************************************************
*/

#include "Solver.h"
#include "ChessBoard.h"

#ifdef LEARN

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

using namespace System::Drawing;


namespace Othello {

	/// <summary>
	/// Summary for PatternLearning
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class PatternLearning : public System::Windows::Forms::Form
	{
	public:
		PatternLearning(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PatternLearning()
		{
			if (components)
			{
				delete components;
			}
			delete solver;
		}
	private: System::Windows::Forms::OpenFileDialog^  dlgChooseFile;
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
		static const int MAXFILE = 100;
		bool learning;
		bool isDone;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  lblFile;
	private: System::Windows::Forms::Label^  lblGame;
			 Solver* solver;

	//value struct WTHORHeader {
	//	unsigned char Century;
	//	unsigned char Year;
	//	unsigned char Month;
	//	unsigned char Day;
	//	unsigned long Anzahl;
	//	unsigned short TourAnz;
	//	unsigned short  YearOfGames;
	//	unsigned char SizeOfBoard;
	//	unsigned char IsAnEndgameSolitaireFile;
	//	unsigned char NumberOfEmptiesForTheoricalScoreCalculation;
	//	unsigned char Reserved;
	//};

	//value struct WTHORRecord {
	//	unsigned short TourNumber;
	//	unsigned short Black;
	//	unsigned short White;
	//	unsigned char RealScore;
	//	unsigned char TheoryScore;
	//	array<unsigned char>^ MoveList;
	//};
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
			this->label1 = (gcnew System::Windows::Forms::Label());
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
			this->lstFiles->SelectedIndexChanged += gcnew System::EventHandler(this, &PatternLearning::lstFiles_SelectedIndexChanged);
			// 
			// txtInfo
			// 
			this->txtInfo->Location = System::Drawing::Point(126, 13);
			this->txtInfo->Multiline = true;
			this->txtInfo->Name = L"txtInfo";
			this->txtInfo->Size = System::Drawing::Size(148, 237);
			this->txtInfo->TabIndex = 1;
			this->txtInfo->ScrollBars = System::Windows::Forms::ScrollBars::Both;
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
			this->btnOpenFile->Click += gcnew System::EventHandler(this, &PatternLearning::btnOpenFile_Click);
			// 
			// btnLearn
			// 
			this->btnLearn->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnLearn->Location = System::Drawing::Point(12, 220);
			this->btnLearn->Name = L"btnLearn";
			this->btnLearn->Size = System::Drawing::Size(96, 30);
			this->btnLearn->TabIndex = 2;
			this->btnLearn->Text = L"开始学习";
			this->btnLearn->UseVisualStyleBackColor = true;
			this->btnLearn->Click += gcnew System::EventHandler(this, &PatternLearning::btnLearn_Click);
			// 
			// pbLearn
			// 
			this->pbLearn->Location = System::Drawing::Point(11, 284);
			this->pbLearn->Name = L"pbLearn";
			this->pbLearn->Size = System::Drawing::Size(261, 24);
			this->pbLearn->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->pbLearn->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(16, 261);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(89, 12);
			this->label1->TabIndex = 4;
			this->label1->Text = L"正在处理：";
			// 
			// lblFile
			// 
			this->lblFile->Location = System::Drawing::Point(96, 261);
			this->lblFile->Name = L"lblFile";
			this->lblFile->Size = System::Drawing::Size(70, 12);
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
			// PatternLearning
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 320);
			this->Controls->Add(this->lblGame);
			this->Controls->Add(this->lblFile);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pbLearn);
			this->Controls->Add(this->btnLearn);
			this->Controls->Add(this->btnOpenFile);
			this->Controls->Add(this->txtInfo);
			this->Controls->Add(this->lstFiles);
			this->Name = L"PatternLearning";
			this->Text = L"模板学习";
			this->Load += gcnew System::EventHandler(this, &PatternLearning::PatternLearning_Load);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &PatternLearning::PatternLearning_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void PatternLearning_Load(System::Object^  sender, System::EventArgs^  e) {
				 fileList = gcnew array<System::String^>(MAXFILE);
				 fileCount = 0;
				 btnLearn->Enabled = false;
				 learning = false;
			 }
	private: System::Void PatternLearning_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
				 if (learning)
					 e->Cancel = true;
			 }
private: System::Void btnOpenFile_Click(System::Object^  sender, System::EventArgs^  e) {
			 dlgChooseFile->Filter = "Pattern Configuration Database(*.pcd)|*.PCD";
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
			 //WTHORHeader header;
			 int header;
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
				 //header.Century = br->ReadByte();
				 //header.Year = br->ReadByte();
				 //header.Month = br->ReadByte();
				 //header.Day = br->ReadByte();
				 //header.Anzahl = br->ReadUInt32();
				 //header.TourAnz = br->ReadUInt16();
				 //header.YearOfGames = br->ReadUInt16();
				 //header.SizeOfBoard= br->ReadByte();
				 //header.IsAnEndgameSolitaireFile = br->ReadByte();
				 //header.NumberOfEmptiesForTheoricalScoreCalculation = br->ReadByte();
				 //header.Reserved = br->ReadByte();
				 header = br->ReadInt32();
				 String^ info = "";
				 info += "局面数量: " + header;
				 //info += "文件建立年份: " + (int)(header.Century * 100 + header.Year) + "\r\n";
				 //info += "文件建立日期: " + (int)header.Month + " 月 " + (int)header.Day + " 日\r\n";
				 //info += "对局总数: " + header.Anzahl + "\r\n";
				 //info += "对局年份: " + header.YearOfGames + "\r\n";
				 //info += "棋盘大小: " + ((header.SizeOfBoard == 0 || header.SizeOfBoard == 8) ? "8 * 8" : "其他") + "\r\n";
				 //info += "是否Solitaire游戏: " + ((header.IsAnEndgameSolitaireFile == 0) ? "否" : "是") + "\r\n";
				 //info += "终局计算步数: " + (int)header.NumberOfEmptiesForTheoricalScoreCalculation + "\r\n";
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
private:
	void solverStarter() {
		solver->learnPreparedSituation();
		isDone = true;
	}
private: System::Void btnLearn_Click(System::Object^  sender, System::EventArgs^  e) {
			 learning = true;
			 btnLearn->Enabled = false;
			 btnOpenFile->Enabled = false;
			 solver = new Solver();
			 solver->setLearingRate(2);
			 int timeCount = 500;
			 for (int i = 0; i < fileCount; i++) {
				 addFile(fileList[i]);
			 }
			 Threading::ThreadStart^ solverThreadStart = gcnew Threading::ThreadStart(this, &PatternLearning::solverStarter);
			 for (int times = 0; times < timeCount; times++) {
				 lblFile->Text = (times + 1).ToString() + "/" + timeCount;
				 isDone = false;
				 Threading::Thread^ solverThread = gcnew Threading::Thread(solverThreadStart);
				 solverThread->Start();
				 while (!isDone) {
					 lblGame->Text = solver->getPercent() + "%";
					 Application::DoEvents();
					 Threading::Thread::Sleep(20);
				 }
				 UpdateResult result = solver->updateScore();
				 solver->writePatterns();
				 lblGame->Text = solver->getPercent() + "%";
				 pbLearn->Value = (times + 1) * 100 / timeCount;
				 txtInfo->Text += "第" + (times + 1) + "次迭代: " + result.getTotalSum() + "\r\n"
					 + "平均估值变化: " + result.getAverageWeightAlteration() + "\r\n"
					 + "最大估值变化: " + result.getMaximunWeightAlteration() + "\r\n";
			 }
			 delete solver;
			 System::Windows::Forms::MessageBox::Show(this, "处理完毕。", "模板学习");
			 btnLearn->Enabled = true;
			 btnOpenFile->Enabled = true;
			 learning = false;
		 }
private: void addFile(String^ currentFile) {
			 //WTHORHeader header;
			 int header;
			 System::IO::FileStream^ fs;
			 try {
				 fs = gcnew System::IO::FileStream(currentFile, System::IO::FileMode::Open);
			 } catch (System::IO::IOException^) {
				 System::Windows::Forms::MessageBox::Show(this, "Cannot open file: " + currentFile, "Error", 
					 System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
				 return;
			 }
			 System::IO::BinaryReader^ br = gcnew System::IO::BinaryReader(fs);
			 //header.Century = br->ReadByte();
			 //header.Year = br->ReadByte();
			 //header.Month = br->ReadByte();
			 //header.Day = br->ReadByte();
			 //header.Anzahl = br->ReadUInt32();
			 //header.TourAnz = br->ReadUInt16();
			 //header.YearOfGames = br->ReadUInt16();
			 //header.SizeOfBoard= br->ReadByte();
			 //header.IsAnEndgameSolitaireFile = br->ReadByte();
			 //header.NumberOfEmptiesForTheoricalScoreCalculation = br->ReadByte();
			 //header.Reserved = br->ReadByte();
			 header = br->ReadInt32();
			 for (int i = 0; i < header; i++) {
				 //WTHORRecord record;
				 Solver::PCDRecord record;
				 //record.TourNumber = br->ReadUInt16();
				 //record.Black = br->ReadUInt16();
				 //record.White = br->ReadUInt16();
				 //record.RealScore = br->ReadByte();
				 //record.TheoryScore = br->ReadByte();
				 //record.MoveList = gcnew array<unsigned char>(60);
				 record.stage = br->ReadInt32();
				 record.b1 = br->ReadUInt64();
				 record.b2 = br->ReadUInt64();
				 record.result = br->ReadInt32();
				 //for (int i = 0; i < 60; i++) {
					// record.MoveList[i] = br->ReadByte();
				 //}
				 //Board^ bd = safe_cast<Board^>(ChessBoard::defaultBoard->Clone());
				 //int moveList[60];
				 //for (int i = 0; i < 60; i++) {
					// unsigned char currentMove = record.MoveList[i];
					// if (currentMove == 0) {
					//	 moveList[i] = -1;
					//	 break;
					// }
					// moveList[i] = (currentMove % 10 - 1) * HEIGHT + (currentMove / 10 - 1);
				 //}
				 //solver->parseGame(bd, moveList);
				 solver->addPreparedSituation(record);
			 }
			 br->Close();
		 }
};
}

#endif //LEARN