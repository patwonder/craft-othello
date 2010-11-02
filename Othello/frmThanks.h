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
								frmThanks.h
		作者：Patrick
		概述：包含类 frmThanks 的声明。
			frmThanks 类实现“致谢”窗体。

********************************************************************************
*/

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Othello {

	/// <summary>
	/// frmThanks 摘要
	///
	/// 警告: 如果更改此类的名称，则需要更改
	///          与此类所依赖的所有 .resx 文件关联的托管资源编译器工具的
	///          “资源文件名”属性。否则，
	///          设计器将不能与此窗体的关联
	///          本地化资源正确交互。
	/// </summary>
	public ref class frmThanks : public System::Windows::Forms::Form
	{
	public:
		frmThanks(void)
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
		~frmThanks()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ListBox^  lstHelpers;

	private: System::Windows::Forms::Button^  btnOK;
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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->lstHelpers = (gcnew System::Windows::Forms::ListBox());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->Font = (gcnew System::Drawing::Font(L"微软雅黑", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(217, 39);
			this->label1->TabIndex = 2;
			this->label1->Text = L"感谢下列人员在 Craft 开发过程中给予的支持和帮助：";
			// 
			// lstHelpers
			// 
			this->lstHelpers->Font = (gcnew System::Drawing::Font(L"微软雅黑", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->lstHelpers->FormattingEnabled = true;
			this->lstHelpers->ItemHeight = 17;
			this->lstHelpers->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Maxi", L"Zach", L"Coder"});
			this->lstHelpers->Location = System::Drawing::Point(12, 51);
			this->lstHelpers->Name = L"lstHelpers";
			this->lstHelpers->SelectionMode = System::Windows::Forms::SelectionMode::None;
			this->lstHelpers->Size = System::Drawing::Size(217, 106);
			this->lstHelpers->TabIndex = 1;
			// 
			// btnOK
			// 
			this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnOK->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnOK->Font = (gcnew System::Drawing::Font(L"微软雅黑", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->btnOK->Location = System::Drawing::Point(151, 163);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(78, 29);
			this->btnOK->TabIndex = 0;
			this->btnOK->Text = L"关闭";
			this->btnOK->UseVisualStyleBackColor = true;
			// 
			// frmThanks
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->btnOK;
			this->ClientSize = System::Drawing::Size(241, 201);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->lstHelpers);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->KeyPreview = true;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"frmThanks";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"致谢";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
