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
								CSingleInstance.h
		作者：Patrick
		概述：包含类 CSingleInstance 的声明。
			CSingleInstance 类用互斥锁实现重复进程的检测。

********************************************************************************
*/

public ref class CSingleInstance {
private:
    //our Mutex member variable
    System::Threading::Mutex^ m_mutex;	
public:
    CSingleInstance(String^ mutexname) {
        m_mutex = gcnew System::Threading::Mutex(true, mutexname);
    }
    ~CSingleInstance() {
        //we must release it when the CSingleInstance object is destroyed
		//m_mutex->ReleaseMutex();
    }
	void closeMutex() {
		m_mutex->Close();
	}
    bool isRunning() {
        //you can replace 10 with 1 if you want to save 9 ms
        return !m_mutex->WaitOne(10, true);
    }
};
