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

/*
********************************************************************************
								frmAbout.cpp
		作者：Patrick
		概述：包含类 frmAbout 的定义。
			frmAbout 类实现“关于”窗体。

********************************************************************************
*/

#include "StdAfx.h"
#include "frmAbout.h"
#include "frmThanks.h"

using namespace Othello;

System::Void frmAbout::lblPage_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
	if (e->Button != Windows::Forms::MouseButtons::Left) return;
	try {
		System::Diagnostics::Process::Start(lblPage->Text);
		lblPage->LinkVisited = true;
	} catch (Exception^) {

	}
}

System::Void frmAbout::btnThanks_Click(System::Object^  sender, System::EventArgs^  e) {
	frmThanks^ thanksForm = gcnew frmThanks();
	thanksForm->ShowDialog(this);
	delete thanksForm;
}
