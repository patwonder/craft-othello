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
								ConfigReader.cpp
		作者：Patrick
		概述：包含类 ConfigReader 的定义。
			ConfigReader 类封装了对配置文件的读取操作，提供易于使用的接口。

********************************************************************************
*/

#include "StdAfx.h"
#include "ConfigReader.h"

using namespace Othello;
using namespace System;
using namespace System::IO;

Catagory^ ConfigReader::newCatagory(String^ catagory) {
	Catagory^ newCatagory;
	if (catagories->TryGetValue(catagory, newCatagory))
		return newCatagory;
	else {
		Catagory^ newCatagory = gcnew Catagory(catagory);
		catagories->Add(catagory, newCatagory);
		return newCatagory;
	}
}

bool ConfigReader::readConfigFromFile(String^ filePath) {
	this->clearConfig();
	
	FileStream^ fs;
	try {
		fs = gcnew FileStream(filePath, FileMode::Open, FileAccess::Read);
	} catch (Exception^) {
		return false;
	}
	
	StreamReader^ reader = gcnew StreamReader(fs);
	try {
		Catagory^ catagory = newCatagory("");
		while (!reader->EndOfStream) {
			String^ line = reader->ReadLine();
			switch (parseLine(line)) {
			case LineType::CATAGORY:
				catagory = newCatagory(parseCatagoryName(line));
				break;
			case LineType::ATTRIBUTE:
				parseAttribute(line, catagory);
				break;
			default:
				break;
			}
		}
	} catch (Exception^) {
		catagories->Clear();
		return false;
	}

	reader->Close();
	configFileName = filePath;
	return true;
}

ConfigReader::LineType ConfigReader::parseLine(String^ line) {
	line = line->Trim('\t', ' ');
	if (line->Length > 2 && line[0] == '[' && line[line->Length - 1] == ']'
		&& isIdentifier(line->Substring(1, line->Length - 2)->Trim('\t', ' ')))
		return LineType::CATAGORY;
	int eqIdx = line->IndexOf('=');
	if (eqIdx > 0 && isIdentifier(line->Substring(0, eqIdx)->Trim('\t', ' ')))
		return LineType::ATTRIBUTE;
	return LineType::RUBBISH;
}

String^ ConfigReader::parseCatagoryName(String^ line) {
	line = line->Trim('\t', ' ');
	return line->Substring(1, line->Length - 2)->Trim('\t', ' ');
}

void ConfigReader::parseAttribute(String^ line, Catagory^ catagory) {
	int eqIdx = line->IndexOf('=');
	String^ key = line->Substring(0, eqIdx)->Trim('\t', ' ');
	String^ value = line->Substring(eqIdx + 1)->Trim('\t', ' ');
	if (value[0] == '\"' && value[value->Length - 1] == '\"')
		value = value->Substring(1, value->Length - 2);
	catagory->addAttribute(key, value);
}

bool ConfigReader::isIdentifier(String^ id) {
	if (id == nullptr) return false;
	if (id->Length == 0) return false;
	String^ firstCharSet = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	String^ charSet = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (firstCharSet->IndexOf(id[0]) < 0) return false;
	for (int i = 1; i < id->Length; i++) {
		if (charSet->IndexOf(id[i]) < 0) return false;
	}
	return true;
}
