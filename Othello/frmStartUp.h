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
								frmStartUp.h
		作者：Patrick
		概述：包含类 frmStartUp 的声明。
			frmStartUp类实现启动窗口。

********************************************************************************
*/

#include "Common.h"
#include "Solver.h"
#include "frmMain.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace CraftEngine;

namespace Othello {

	/// <summary>
	/// Summary for frmStartUp
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmStartUp : public System::Windows::Forms::Form
	{
	public:
		frmStartUp()
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			isDone = false;
			canClose = false;
		}
		property frmMain^ LoadedMainForm {
			frmMain^ get() {
				return mainForm;
			}
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmStartUp()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		volatile bool successful;
		volatile bool isDone;
		bool canClose;
		frmMain^ mainForm;
	private: System::Windows::Forms::Label^  lblVersion;
	private: System::Windows::Forms::Label^  lblName;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ProgressBar^  pbStartUp;
	private: System::Windows::Forms::Label^  lblState;
	private: System::Windows::Forms::Label^  label2;
#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmStartUp::typeid));
				 this->lblVersion = (gcnew System::Windows::Forms::Label());
				 this->lblName = (gcnew System::Windows::Forms::Label());
				 this->label1 = (gcnew System::Windows::Forms::Label());
				 this->pbStartUp = (gcnew System::Windows::Forms::ProgressBar());
				 this->lblState = (gcnew System::Windows::Forms::Label());
				 this->label2 = (gcnew System::Windows::Forms::Label());
				 this->SuspendLayout();
				 // 
				 // lblVersion
				 // 
				 this->lblVersion->AutoSize = false;
				 this->lblVersion->BackColor = System::Drawing::Color::Transparent;
				 this->lblVersion->ForeColor = System::Drawing::Color::White;
				 this->lblVersion->Font = (gcnew System::Drawing::Font(L"SimSun", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->lblVersion->Location = System::Drawing::Point(0, 83);
				 this->lblVersion->Name = L"lblVersion";
				 this->lblVersion->Size = System::Drawing::Size(216, 40);
				 this->lblVersion->TabIndex = 1;
				 this->lblVersion->Text = L"";
				 this->lblVersion->TextAlign = System::Drawing::ContentAlignment::TopCenter;
				 // 
				 // lblName
				 // 
				 this->lblName->AutoSize = true;
				 this->lblName->BackColor = System::Drawing::Color::Transparent;
				 this->lblName->ForeColor = System::Drawing::Color::White;
				 this->lblName->Font = (gcnew System::Drawing::Font(L"Times New Roman", 42, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(0)));
				 this->lblName->Location = System::Drawing::Point(36, 25);
				 this->lblName->Name = L"lblName";
				 this->lblName->Size = System::Drawing::Size(144, 64);
				 this->lblName->TabIndex = 0;
				 this->lblName->Text = "";
				 // 
				 // label1
				 // 
				 this->label1->AutoSize = true;
				 this->label1->BackColor = System::Drawing::Color::Transparent;
				 this->label1->ForeColor = System::Drawing::Color::White;
				 this->label1->Font = (gcnew System::Drawing::Font(L"SimSun", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->label1->Location = System::Drawing::Point(222, 151);
				 this->label1->Name = L"label1";
				 this->label1->Size = System::Drawing::Size(129, 29);
				 this->label1->TabIndex = 2;
				 this->label1->Text = L"正在启动";
				 // 
				 // pbStartUp
				 // 
				 this->pbStartUp->Dock = System::Windows::Forms::DockStyle::Bottom;
				 this->pbStartUp->Location = System::Drawing::Point(10, 221);
				 this->pbStartUp->Name = L"pbStartUp";
				 this->pbStartUp->Size = System::Drawing::Size(406, 20);
				 this->pbStartUp->TabIndex = 3;
				 // 
				 // lblState
				 // 
				 this->lblState->AutoSize = true;
				 this->lblState->BackColor = System::Drawing::Color::Transparent;
				 this->lblState->ForeColor = System::Drawing::Color::White;
				 this->lblState->Font = (gcnew System::Drawing::Font(L"SimSun", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->lblState->Location = System::Drawing::Point(228, 204);
				 this->lblState->Name = L"lblState";
				 this->lblState->Size = System::Drawing::Size(0, 16);
				 this->lblState->TabIndex = 4;
				 // 
				 // label2
				 // 
				 this->label2->BackColor = System::Drawing::Color::Transparent;
				 this->label2->ForeColor = System::Drawing::Color::White;
				 this->label2->Font = (gcnew System::Drawing::Font(L"SimSun", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->label2->Location = System::Drawing::Point(214, 40);
				 this->label2->Name = L"label2";
				 this->label2->Size = System::Drawing::Size(180, 80);
				 this->label2->TabIndex = 5;
				 this->label2->Text = L"Author: Patrick\r\nE-mail: patrick880905@sina.com\r\nQQ: 511754081\r\n";
				 // 
				 // frmStartUp
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
				 this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
				 this->ClientSize = System::Drawing::Size(406, 261);
				 this->Controls->Add(this->label2);
				 this->Controls->Add(this->lblState);
				 this->Controls->Add(this->pbStartUp);
				 this->Controls->Add(this->label1);
				 this->Controls->Add(this->lblVersion);
				 this->Controls->Add(this->lblName);
				 this->ShowIcon = false;
				 this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
				 this->Name = L"frmStartUp";
				 this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
				 this->Text = " 启动中……";
				 this->ShowInTaskbar = false;
				 this->Cursor = Cursors::WaitCursor;
				 this->Load += gcnew System::EventHandler(this, &frmStartUp::frmStartUp_Load);
				 this->Shown += gcnew System::EventHandler(this, &frmStartUp::frmStartUp_Shown);
				 this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &frmStartUp::frmStartUp_FormClosing);
				 this->ResumeLayout(false);
				 this->PerformLayout();

			 }
#pragma endregion

	private: void initStarter() {
				 isDone = false;

				 String^ executablePath = Application::ExecutablePath;

				 String^ bookPath = executablePath->Substring(0, executablePath->LastIndexOf('\\') + 1) + "book.craft";
				 String^ patternPath = executablePath->Substring(0, executablePath->LastIndexOf('\\') + 1) + "data.craft";

				 char* bp = (char*)(int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(bookPath);
				 char* pp = (char*)(int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(patternPath);
				 successful = Solver::initialize(pp, bp);
				 System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)bp);
				 System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)pp);

				 isDone = true;
			 }
			 void doInit() {
				 using namespace System::Threading;
				 Thread^ startUpThread = gcnew Thread(gcnew ThreadStart(this, &frmStartUp::initStarter));
				 startUpThread->Start();
				 int percent;
				 int part;
				 bool anotherLoop = true;
				 while (anotherLoop) {
					 anotherLoop = !isDone;
					 Thread::Sleep(40);
					 part = Solver::getInitPart();
					 percent = Solver::getInitPercent();
					 switch (part) {
								 case 0:
								 case 1:
								 case 2:
									 lblState->Text = "正在初始化……";
									 break;
								 case 3:
									 lblState->Text = "正在载入模板……";
									 break;
								 case 4:
									 lblState->Text = "正在载入棋谱……";
									 break;
								 case 5:
									 lblState->Text = "完成";
					 }
					 pbStartUp->Value = percent;
					 Application::DoEvents();
				 }
				 if (!successful) {
					 MessageBox::Show(this, __APP_NAME__ + " 未能正常初始化。请确保程序文件是完整的。\n尝试重新安装应用程序以解决此问题。",
						 __APP_NAME__ + " 初始化失败", MessageBoxButtons::OK, MessageBoxIcon::Error);
				 } else {
					 mainForm = gcnew frmMain();
				 }
				 DialogResult = successful ? Windows::Forms::DialogResult::Yes
					 : Windows::Forms::DialogResult::No;
				 canClose = true;
				 this->Close();
			 }
	private: System::Void frmStartUp_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->Text = __APP_NAME__ + " 启动中";
				 lblName->Text = __APP_NAME__;
				 lblVersion->Text = "版本 " + __APP_VERSION__;
			 }
	private: System::Void frmStartUp_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
				 if (!canClose)
					 e->Cancel = true;
			 }
	private: delegate void SimpleDelegate();
	private: System::Void frmStartUp_Shown(System::Object^  sender, System::EventArgs^  e) {
				 BeginInvoke(gcnew SimpleDelegate(this, &frmStartUp::doInit));
			 }
	};
}
