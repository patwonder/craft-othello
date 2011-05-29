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
								frmAbout.h
		作者：Patrick
		概述：包含类 frmAbout 的声明。
			frmAbout 类实现“关于”窗体。

********************************************************************************
*/

#include "Common.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Othello {

	/// <summary>
	/// frmAbout 摘要
	///
	/// 警告: 如果更改此类的名称，则需要更改
	///          与此类所依赖的所有 .resx 文件关联的托管资源编译器工具的
	///          “资源文件名”属性。否则，
	///          设计器将不能与此窗体的关联
	///          本地化资源正确交互。
	/// </summary>
	public ref class frmAbout : public System::Windows::Forms::Form
	{
	public:
		frmAbout(void)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~frmAbout()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnOK;
	protected: 

	private: System::Windows::Forms::Label^  lblInfo;
	private: System::Windows::Forms::PictureBox^  pic;
	private: System::Windows::Forms::Label^  lblTitle;
	private: System::Windows::Forms::LinkLabel^  lblPage;
	private: System::Windows::Forms::LinkLabel^  lblCopyleft;



	private: System::Windows::Forms::Button^  btnThanks;
	private: System::Windows::Forms::PictureBox^  picGPL;
	protected: 


	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmAbout::typeid));
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->lblInfo = (gcnew System::Windows::Forms::Label());
			this->pic = (gcnew System::Windows::Forms::PictureBox());
			this->lblTitle = (gcnew System::Windows::Forms::Label());
			this->lblPage = (gcnew System::Windows::Forms::LinkLabel());
			this->lblCopyleft = (gcnew System::Windows::Forms::LinkLabel());
			this->btnThanks = (gcnew System::Windows::Forms::Button());
			this->picGPL = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pic))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picGPL))->BeginInit();
			this->SuspendLayout();
			// 
			// btnOK
			// 
			this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnOK->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnOK->Location = System::Drawing::Point(227, 327);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(78, 28);
			this->btnOK->TabIndex = 0;
			this->btnOK->Text = L"关闭";
			this->btnOK->UseVisualStyleBackColor = true;
			// 
			// lblInfo
			// 
			this->lblInfo->Location = System::Drawing::Point(36, 86);
			this->lblInfo->Name = L"lblInfo";
			this->lblInfo->Size = System::Drawing::Size(269, 84);
			this->lblInfo->TabIndex = 1;
			this->lblInfo->Text = L"程序设计: \r\nE-mail: \r\nQQ: \r\n如有什么意见或建议, 欢迎与我联系.";
			// 
			// pic
			// 
			this->pic->Cursor = System::Windows::Forms::Cursors::Default;
			this->pic->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pic.Image")));
			this->pic->Location = System::Drawing::Point(9, 8);
			this->pic->Margin = System::Windows::Forms::Padding(0);
			this->pic->Name = L"pic";
			this->pic->Size = System::Drawing::Size(64, 64);
			this->pic->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pic->TabIndex = 2;
			this->pic->TabStop = false;
			// 
			// lblTitle
			// 
			this->lblTitle->Location = System::Drawing::Point(76, 8);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->Size = System::Drawing::Size(230, 64);
			this->lblTitle->TabIndex = 3;
			this->lblTitle->Text = L"lblTitle";
			this->lblTitle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lblPage
			// 
			this->lblPage->LinkArea = System::Windows::Forms::LinkArea(16, 39);
			this->lblPage->Location = System::Drawing::Point(13, 285);
			this->lblPage->Name = L"lblPage";
			this->lblPage->Size = System::Drawing::Size(293, 40);
			this->lblPage->TabIndex = 1;
			this->lblPage->TabStop = true;
			this->lblPage->Text = L"欢迎访问 Craft 的主页\r\nhttp://code.google.com/p/craft-othello/";
			this->lblPage->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->lblPage->UseCompatibleTextRendering = true;
			this->lblPage->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &frmAbout::lblPage_LinkClicked);
			// 
			// lblCopyleft
			// 
			this->lblCopyleft->LinkArea = System::Windows::Forms::LinkArea(55, 28);
			this->lblCopyleft->Location = System::Drawing::Point(36, 217);
			this->lblCopyleft->Name = L"lblCopyleft";
			this->lblCopyleft->Size = System::Drawing::Size(273, 59);
			this->lblCopyleft->TabIndex = 1;
			this->lblCopyleft->TabStop = true;
			this->lblCopyleft->Text = L"Copyright © 2008-2011  Patrick\r\n本程序遵循GNU GPL协议, 详情请见:\r\nhttp://www.gnu.org/license" 
				L"s/";
			this->lblCopyleft->UseCompatibleTextRendering = true;
			this->lblCopyleft->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &frmAbout::lblCopyleft_LinkClicked);
			// 
			// btnThanks
			// 
			this->btnThanks->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnThanks->Location = System::Drawing::Point(16, 327);
			this->btnThanks->Name = L"btnThanks";
			this->btnThanks->Size = System::Drawing::Size(78, 28);
			this->btnThanks->TabIndex = 2;
			this->btnThanks->Text = L"致谢";
			this->btnThanks->UseVisualStyleBackColor = true;
			this->btnThanks->Click += gcnew System::EventHandler(this, &frmAbout::btnThanks_Click);
			// 
			// picGPL
			// 
			this->picGPL->Cursor = System::Windows::Forms::Cursors::Default;
			this->picGPL->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"picGPL.Image")));
			this->picGPL->Location = System::Drawing::Point(9, 171);
			this->picGPL->Margin = System::Windows::Forms::Padding(0);
			this->picGPL->Name = L"picGPL";
			this->picGPL->Size = System::Drawing::Size(88, 31);
			this->picGPL->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->picGPL->TabIndex = 2;
			this->picGPL->TabStop = false;
			// 
			// frmAbout
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->btnOK;
			this->ClientSize = System::Drawing::Size(318, 368);
			this->Controls->Add(this->lblTitle);
			this->Controls->Add(this->picGPL);
			this->Controls->Add(this->pic);
			this->Controls->Add(this->lblCopyleft);
			this->Controls->Add(this->lblInfo);
			this->Controls->Add(this->btnThanks);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->lblPage);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->KeyPreview = true;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"frmAbout";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"关于";
			this->Load += gcnew System::EventHandler(this, &frmAbout::frmAbout_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pic))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picGPL))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void frmAbout_Load(System::Object^  sender, System::EventArgs^  e) {
				 for each (Control^ ctrl in Controls) {
					 ctrl->Font = SystemFonts::MessageBoxFont;
				 }
				 lblTitle->Font = gcnew System::Drawing::Font(lblTitle->Font->Name, 2.0f * lblTitle->Font->SizeInPoints, FontStyle::Bold, GraphicsUnit::Point);

				 lblInfo->Text = "程序设计: " __AUTHOR__ "\r\nE-mail: " __AUTHOR_EMAIL__ "\r\nQQ: " __AUTHOR_QQ__ "\r\n如有什么意见或建议, 欢迎与我联系.";

				 String^ title = __APP_NAME__ + " " + __APP_VERSION__;
				 lblTitle->Text = title;
				 Text = "关于 " + __APP_NAME__;
			 }
	private: System::Void lblPage_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e);
	private: System::Void btnThanks_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void lblCopyleft_LinkClicked(System::Object ^sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs ^e);
	};
}
