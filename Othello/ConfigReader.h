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
								ConfigReader.h
		作者：Patrick
		概述：包含类 ConfigReader 的声明。
			ConfigReader 类封装了对配置文件的读取操作，提供易于使用的接口。

********************************************************************************
*/

namespace Othello {

//配置文件的一个分类
public ref class Catagory {
public:
	Catagory(System::String^ name) {
		this->name = name;
		attributes = 
			gcnew System::Collections::Generic::SortedDictionary<System::String^, System::String^>();
	}
	property System::String^ Name {
		System::String^ get() {
			return name;
		}
		void set(System::String^ name) {
			this->name = name;
		}
	}
	void addAttribute(System::String^ key, System::String^ value) {
		attributes[key] = value;
	}
	bool removeAttribute(System::String^ key) {
		return attributes->Remove(key);
	}
	void clearAttributes() {
		attributes->Clear();
	}
	System::String^ getAttribute(System::String^ key, System::String^ defaultValue) {
		System::String^ value;
		if (attributes->TryGetValue(key, value))
			return value;
		else return defaultValue;
	}
	int getIntAttribute(System::String^ key, int defaultValue) {
		System::String^ value;
		if (attributes->TryGetValue(key, value))
			return System::Int32::Parse(value);
		else
			return defaultValue;
	}
	System::Collections::Generic::SortedDictionary<System::String^, System::String^>::Enumerator
		getEnumerator() {
			return attributes->GetEnumerator();
	}
private:
	System::String^ name;
	System::Collections::Generic::SortedDictionary<System::String^, System::String^>^ attributes;
};

//配置文件读取工具
public ref class ConfigReader {
public:
	ConfigReader() {
		configFileName = "";
		catagories = 
			gcnew System::Collections::Generic::SortedDictionary<System::String^, Catagory^>();
	}
	bool readConfigFromFile(System::String^ filePath);
	Catagory^ getCatagory(System::String^ key) {
		Catagory^ value;
		if (catagories->TryGetValue(key, value))
			return value;
		else return nullptr;
	}
	System::String^ getConfigFileName() {
		return configFileName;
	}
	void clearConfig() {
		configFileName = "";
		catagories->Clear();
	}
private:
	enum class LineType { CATAGORY, ATTRIBUTE, RUBBISH };
	Catagory^ newCatagory(System::String^ catagory);
	LineType parseLine(System::String^ line);
	System::String^ parseCatagoryName(System::String^ line);
	void parseAttribute(System::String^ line, Catagory^ catagory);
	bool isIdentifier(System::String^ id);

	System::String^ configFileName;
	System::Collections::Generic::SortedDictionary<System::String^, Catagory^>^ catagories;
};

} // namespace Othello
