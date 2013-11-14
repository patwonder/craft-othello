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
								dlgChooseMachineType.h
		作者：Patrick
		概述：包含类 dlgChooseMachineType 的声明。
			dlgChooseMachineType 类实现“选择黑（白）方”窗体。

********************************************************************************
*/

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

using namespace System::Drawing;

#include "Common.h"

namespace Othello {

	/// <summary>
	/// Summary for dlgChooseMachineType
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class dlgChooseMachineType : public System::Windows::Forms::Form
	{
	private: PlayerType current;
	private: System::Windows::Forms::RadioButton^  optStar;
	private: System::Windows::Forms::RadioButton^  optGalaxy;
	private: System::Windows::Forms::RadioButton^  optUniverse;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	public:
		dlgChooseMachineType(Chess color) {
			this->dlgChooseMachineType::dlgChooseMachineType(color, PlayerType::RANDOM);
		}
		dlgChooseMachineType(Chess color, PlayerType defaultType) {
			InitializeComponent();
			this->Text = (color == Chess::BLACK) ? "请选择黑方级别" : "请选择白方级别";
			switch (defaultType) {
			case PlayerType::BIG_EAT:
				optBigEat->Checked = true;
				break;
			case PlayerType::EARTH:
				optEarth->Checked = true;
				break;
			case PlayerType::CLOUDS:
				optClouds->Checked = true;
				break;
			case PlayerType::ORBIT:
				optOrbit->Checked = true;
				break;
			case PlayerType::MOON:
				optMoon->Checked = true;
				break;
			case PlayerType::STAR:
				optStar->Checked = true;
				break;
			case PlayerType::GALAXY:
				optGalaxy->Checked = true;
				break;
			case PlayerType::UNIVERSE:
				optUniverse->Checked = true;
				break;
			default:
				optRandom->Checked = true;
			}
			DialogResult = Windows::Forms::DialogResult::Cancel;
		}

		property PlayerType MachineType {
			PlayerType get() {
				return current;
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~dlgChooseMachineType()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnCancel;
	private: System::Windows::Forms::RadioButton^  optClouds;
	private: System::Windows::Forms::RadioButton^  optEarth;
	private: System::Windows::Forms::RadioButton^  optBigEat;
	private: System::Windows::Forms::RadioButton^  optRandom;
	private: System::Windows::Forms::RadioButton^  optOrbit;
	private: System::Windows::Forms::RadioButton^  optMoon;
	protected: 
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->optEarth = (gcnew System::Windows::Forms::RadioButton());
			this->optBigEat = (gcnew System::Windows::Forms::RadioButton());
			this->optRandom = (gcnew System::Windows::Forms::RadioButton());
			this->optClouds = (gcnew System::Windows::Forms::RadioButton());
			this->optOrbit = (gcnew System::Windows::Forms::RadioButton());
			this->optMoon = (gcnew System::Windows::Forms::RadioButton());
			this->optStar = (gcnew System::Windows::Forms::RadioButton());
			this->optGalaxy = (gcnew System::Windows::Forms::RadioButton());
			this->optUniverse = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btnOK
			// 
			this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnOK->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnOK->Location = System::Drawing::Point(139, 179);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(96, 32);
			this->btnOK->TabIndex = 12;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &dlgChooseMachineType::btnOK_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnCancel->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnCancel->Location = System::Drawing::Point(241, 179);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(96, 32);
			this->btnCancel->TabIndex = 13;
			this->btnCancel->Text = L"取消";
			this->btnCancel->UseVisualStyleBackColor = true;
			this->btnCancel->Click += gcnew System::EventHandler(this, &dlgChooseMachineType::btnCancel_Click);
			// 
			// optEarth
			// 
			this->optEarth->AutoSize = true;
			this->optEarth->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->optEarth->Location = System::Drawing::Point(230, 33);
			this->optEarth->Name = L"optEarth";
			this->optEarth->Size = System::Drawing::Size(95, 17);
			this->optEarth->TabIndex = 5;
			this->optEarth->TabStop = true;
			this->optEarth->Text = L"地球(&Earth)";
			this->optEarth->UseVisualStyleBackColor = true;
			this->optEarth->CheckedChanged += gcnew System::EventHandler(this, &dlgChooseMachineType::optEarth_CheckedChanged);
			// 
			// optBigEat
			// 
			this->optBigEat->AutoSize = true;
			this->optBigEat->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->optBigEat->Location = System::Drawing::Point(123, 33);
			this->optBigEat->Name = L"optBigEat";
			this->optBigEat->Size = System::Drawing::Size(107, 17);
			this->optBigEat->TabIndex = 4;
			this->optBigEat->TabStop = true;
			this->optBigEat->Text = L"大食(&Big Eat)";
			this->optBigEat->UseVisualStyleBackColor = true;
			this->optBigEat->CheckedChanged += gcnew System::EventHandler(this, &dlgChooseMachineType::optBigEat_CheckedChanged);
			// 
			// optRandom
			// 
			this->optRandom->AutoSize = true;
			this->optRandom->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->optRandom->Location = System::Drawing::Point(22, 33);
			this->optRandom->Name = L"optRandom";
			this->optRandom->Size = System::Drawing::Size(101, 17);
			this->optRandom->TabIndex = 3;
			this->optRandom->TabStop = true;
			this->optRandom->Text = L"随机(&Random)";
			this->optRandom->UseVisualStyleBackColor = true;
			this->optRandom->CheckedChanged += gcnew System::EventHandler(this, &dlgChooseMachineType::optRandom_CheckedChanged);
			// 
			// optClouds
			// 
			this->optClouds->AutoSize = true;
			this->optClouds->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->optClouds->Location = System::Drawing::Point(22, 93);
			this->optClouds->Name = L"optClouds";
			this->optClouds->Size = System::Drawing::Size(101, 17);
			this->optClouds->TabIndex = 6;
			this->optClouds->TabStop = true;
			this->optClouds->Text = L"云朵(&Clouds)";
			this->optClouds->UseVisualStyleBackColor = true;
			this->optClouds->CheckedChanged += gcnew System::EventHandler(this, &dlgChooseMachineType::optClouds_CheckedChanged);
			// 
			// optOrbit
			// 
			this->optOrbit->AutoSize = true;
			this->optOrbit->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->optOrbit->Location = System::Drawing::Point(123, 93);
			this->optOrbit->Name = L"optOrbit";
			this->optOrbit->Size = System::Drawing::Size(95, 17);
			this->optOrbit->TabIndex = 7;
			this->optOrbit->TabStop = true;
			this->optOrbit->Text = L"轨道(&Orbit)";
			this->optOrbit->UseVisualStyleBackColor = true;
			this->optOrbit->CheckedChanged += gcnew System::EventHandler(this, &dlgChooseMachineType::optOrbit_CheckedChanged);
			// 
			// optMoon
			// 
			this->optMoon->AutoSize = true;
			this->optMoon->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->optMoon->Location = System::Drawing::Point(230, 93);
			this->optMoon->Name = L"optMoon";
			this->optMoon->Size = System::Drawing::Size(89, 17);
			this->optMoon->TabIndex = 8;
			this->optMoon->TabStop = true;
			this->optMoon->Text = L"月亮(&Moon)";
			this->optMoon->UseVisualStyleBackColor = true;
			this->optMoon->CheckedChanged += gcnew System::EventHandler(this, &dlgChooseMachineType::optMoon_CheckedChanged);
			// 
			// optStar
			// 
			this->optStar->AutoSize = true;
			this->optStar->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->optStar->Location = System::Drawing::Point(22, 153);
			this->optStar->Name = L"optStar";
			this->optStar->Size = System::Drawing::Size(89, 17);
			this->optStar->TabIndex = 9;
			this->optStar->TabStop = true;
			this->optStar->Text = L"星星(&Star)";
			this->optStar->UseVisualStyleBackColor = true;
			this->optStar->CheckedChanged += gcnew System::EventHandler(this, &dlgChooseMachineType::optStar_CheckedChanged);
			// 
			// optGalaxy
			// 
			this->optGalaxy->AutoSize = true;
			this->optGalaxy->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->optGalaxy->Location = System::Drawing::Point(123, 153);
			this->optGalaxy->Name = L"optGalaxy";
			this->optGalaxy->Size = System::Drawing::Size(101, 17);
			this->optGalaxy->TabIndex = 10;
			this->optGalaxy->TabStop = true;
			this->optGalaxy->Text = L"星系(&Galaxy)";
			this->optGalaxy->UseVisualStyleBackColor = true;
			this->optGalaxy->CheckedChanged += gcnew System::EventHandler(this, &dlgChooseMachineType::optGalaxy_CheckedChanged);
			// 
			// optUniverse
			// 
			this->optUniverse->AutoSize = true;
			this->optUniverse->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->optUniverse->Location = System::Drawing::Point(230, 153);
			this->optUniverse->Name = L"optUniverse";
			this->optUniverse->Size = System::Drawing::Size(113, 17);
			this->optUniverse->TabIndex = 11;
			this->optUniverse->TabStop = true;
			this->optUniverse->Text = L"宇宙(&Universe)";
			this->optUniverse->UseVisualStyleBackColor = true;
			this->optUniverse->CheckedChanged += gcnew System::EventHandler(this, &dlgChooseMachineType::optUniverse_CheckedChanged);
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(12, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(317, 12);
			this->label1->TabIndex = 14;
			this->label1->Text = L"——简单——————————————————————";
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(12, 73);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(317, 12);
			this->label2->TabIndex = 14;
			this->label2->Text = L"——中等——————————————————————";
			// 
			// label3
			// 
			this->label3->Location = System::Drawing::Point(12, 133);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(317, 12);
			this->label3->TabIndex = 14;
			this->label3->Text = L"——困难——————————————————————";
			// 
			// dlgChooseMachineType
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->btnCancel;
			this->ClientSize = System::Drawing::Size(356, 223);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->optUniverse);
			this->Controls->Add(this->optOrbit);
			this->Controls->Add(this->optGalaxy);
			this->Controls->Add(this->optStar);
			this->Controls->Add(this->optEarth);
			this->Controls->Add(this->optClouds);
			this->Controls->Add(this->optMoon);
			this->Controls->Add(this->optBigEat);
			this->Controls->Add(this->optRandom);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->btnOK);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"dlgChooseMachineType";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"请选择对手级别";
			this->Load += gcnew System::EventHandler(this, &dlgChooseMachineType::dlgChooseMachineType_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &dlgChooseMachineType::dlgChooseMachineType_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void optRandom_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (optRandom->Checked) {
				 current = PlayerType::RANDOM;
			 }
		 }
private: System::Void optBigEat_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (optBigEat->Checked) {
				 current = PlayerType::BIG_EAT;
			 }
		 }
private: System::Void optEarth_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (optEarth->Checked) {
				 current = PlayerType::EARTH;
			 }
		 }
private: System::Void optClouds_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (optClouds->Checked) {
				 current = PlayerType::CLOUDS;
			 }
		 }
private: System::Void optOrbit_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (optOrbit->Checked) {
				 current = PlayerType::ORBIT;
			 }
		 }
private: System::Void optMoon_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (optMoon->Checked) {
				 current = PlayerType::MOON;
			 }
		 }
private: System::Void optStar_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (optStar->Checked) {
				 current = PlayerType::STAR;
			 }
		 }
private: System::Void optGalaxy_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (optGalaxy->Checked) {
				 current = PlayerType::GALAXY;
			 }
		 }
private: System::Void optUniverse_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (optUniverse->Checked) {
				 current = PlayerType::UNIVERSE;
			 }
		 }
private: System::Void btnCancel_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void dlgChooseMachineType_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
		 }
private: System::Void dlgChooseMachineType_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}
