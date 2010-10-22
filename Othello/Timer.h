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
								Timer.h
		作者：Patrick
		概述：包含类 Timer 的声明。
			Timer 类封装了计时器，用于实现对各个玩家的走棋时限。
			目前没有使用到该类的功能。警告：代码未经测试！

********************************************************************************
*/

#include "Common.h"

namespace Othello {

int GetTickCount();

public ref class IllegalTimerStateException : public System::Exception {
public:
	IllegalTimerStateException() {}
	IllegalTimerStateException(System::String^ msg) : System::Exception(msg) {}
};
public ref class Timer {
private:
	int totalLimit;
	int timeLeft;
	int startTime;
	bool ticking;
	bool unlimited;
	int currentStep;
	//System::Collections::Generic::List<int> stepTime;
public:

	Timer(int limit) : totalLimit(limit), timeLeft(limit), ticking(false), unlimited(limit <= 0) {}

	void start() {
		if (unlimited) return;
		if (ticking) 
			//throw gcnew IllegalTimerStateException(__MY_FUNCTION__ + "Timer ticking.");
			return;
		ticking = true;
		startTime = GetTickCount();
	}

	void stop() {
		if (unlimited) return;
		if (!ticking) 
			//throw gcnew IllegalTimerStateException(__MY_FUNCTION__ + "Timer not ticking.");
			return;
		int step = GetTickCount() - startTime;
		//stepTime.Add(step);
		timeLeft -= step;
		ticking = false;
	}

	int getLeftTime() {
		if (ticking) {
			return timeLeft - (GetTickCount() - startTime);
		} else {
			return timeLeft;
		}
	}

	int getTotalTimeLimit() {
		return totalLimit;
	}

	bool isOverTimed() {
		return (timeLeft < 0);
	}

	void reset() {
		ticking = false;
		//stepTime.Clear();
		timeLeft = totalLimit;
	}

	void reset(int limit) {
		ticking = false;
		//stepTime.Clear();
		timeLeft = totalLimit = limit;
		unlimited = (limit <= 0);
	}

	//void goBack() {
	//	if (unlimited) return;
	//	if (ticking)
	//		//throw gcnew IllegalTimerStateException(__MY_FUNCTION__ + "Timer ticking.");
	//		ticking = false;
	//	if (stepTime.Count == 0)
	//		//throw gcnew IllegalTimerStateException(__MY_FUNCTION__ + "First step now.");
	//		return;
	//	timeLeft += stepTime[stepTime.Count - 1];
	//	stepTime.RemoveAt(stepTime.Count - 1);
	//}
};
}//namespace Othello
