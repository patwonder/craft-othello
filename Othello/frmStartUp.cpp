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
								frmStartUp.cpp
		作者：Patrick
		概述：包含类 frmStartUp 的定义。
			frmStartUp类实现启动窗口。

********************************************************************************
*/

#include "StdAfx.h"
#include "frmStartUp.h"
#include "Solver.h"
#include "PbNoAnimation.h"

using namespace Othello;

void frmStartUp::initStarter() {
	isDone = false;
	String ^executablePath = Application::ExecutablePath;
	String ^bookPath = Path::GetDirectoryName(executablePath) + "\\book.craft";
	String ^patternPath = Path::GetDirectoryName(executablePath) + "\\data.craft";
	// we store book in appdata path in order to solve UAC-related problems
	String ^bookStorePath = Application::LocalUserAppDataPath + "\\book.craft";
	
	// determine whether we should copy book file from the installation path
	bool needCopy = true;
	if (File::Exists(bookStorePath))
		try {
			DateTime org = File::GetLastWriteTimeUtc(bookPath);
			DateTime store = File::GetLastWriteTimeUtc(bookStorePath);
			if (org.CompareTo(store) <= 0)
				needCopy = false;
	} catch (...) {
	}
	if (needCopy)
		// copy the file
		try {
			File::Copy(bookPath, bookStorePath, true);
	} catch (...) {
	}

	wchar_t* bp = (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(bookStorePath).ToPointer();
	wchar_t* pp = (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(patternPath).ToPointer();
	successful = Solver::initialize(pp, bp);
	System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)bp);
	System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)pp);

	isDone = true;
}
void frmStartUp::doInit() {
	using namespace System::Threading;
	mainForm = nullptr;
	Thread ^startUpThread = gcnew Thread(gcnew ThreadStart(this, &frmStartUp::initStarter));
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
			lblState->Text = "初始化……";
			break;
		case 3:
			lblState->Text = "载入模板……";
			break;
		case 4:
			lblState->Text = "载入棋谱及主界面……";
			break;
		case 5:
			lblState->Text = "完成";
		}

		setProgressBarValueNoAnimation(pbStartUp, percent);

		Application::DoEvents();
#ifndef REEVALUATE
		if (!mainForm && part >= 4) {
			mainForm = gcnew frmMain();
		}
#endif
	}
	if (!successful) {
		setProgressBarState(ProgressBarState::Error);
		MessageBox::Show(this, __APP_NAME__ + " 未能正常初始化。请确保程序文件是完整的。\n尝试重新安装应用程序以解决此问题。", __APP_NAME__ + " 初始化失败", MessageBoxButtons::OK, MessageBoxIcon::Error);
		if (mainForm) {
			delete mainForm;
			mainForm = nullptr;
		}
	} else {
		if (!mainForm)
			mainForm = gcnew frmMain();
	}
	DialogResult = successful ? Windows::Forms::DialogResult::Yes : Windows::Forms::DialogResult::No;
	canClose = true;
	this->Close();
}
System::Void frmStartUp::frmStartUp_Load(System::Object ^sender, System::EventArgs ^e) {
	this->Text = __APP_NAME__ + " 启动中";
	lblName->Text = __APP_NAME__;
	lblVersion->Text = "版本 " + __APP_VERSION__;
}
System::Void frmStartUp::frmStartUp_FormClosing(System::Object ^sender, System::Windows::Forms::FormClosingEventArgs ^e) {
	if (!canClose)
		e->Cancel = true;
}
System::Void frmStartUp::frmStartUp_Shown(System::Object ^sender, System::EventArgs ^e) {
	// mainly to put the doInit function in the back of the message loop
	BeginInvoke(gcnew SimpleDelegate(this, &frmStartUp::doInit));
}

void frmStartUp::setProgressBarState(ProgressBarState state) {
	ProgressBarStateRelated::setState(pbStartUp, state);
	pbStartUp->Invalidate();
}
