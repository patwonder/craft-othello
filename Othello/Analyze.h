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
								Analyze.h
		作者：Patrick
		概述：定义棋局分析结果需要用到的类：
			AnalyzedMove 和 DetailedAnalyzeResult

********************************************************************************
*/

#include "Common.h"

namespace Othello {

//概述：定义问题级别
public enum class QuestionLevel {
	NONE,	//没有问题
	NORMAL, //有一点问题
	BAD		//有很大问题
};

//概述：类 AnalyzedMove 表示一步棋的分析结果
public value class AnalyzedMove {
private:
	System::String^ generalDescription;
	System::String^ sPlayedMoveEval, ^sBestMoveEval;
	int iPlayedMoveEval, iBestMoveEval;
	System::String^ sPlayedMove, ^sBestMove;
	System::Drawing::Point pPlayedMove, pBestMove;
	QuestionLevel questionLevel;
	Chess color;
public:
	//构造函数，初始化 AnalyzedMove 对象，设定分析结果
	AnalyzedMove(System::String^ description, Chess player,
		System::String^ sPlayedEval, System::String^ sBestEval,
		int iPlayedEval, int iBestEval, System::String^ sPlayed, System::String^ sBest,
		System::Drawing::Point pPlayed, System::Drawing::Point pBest, QuestionLevel ql) {
			generalDescription = description;
			sPlayedMoveEval = sPlayedEval;
			sBestMoveEval = sBestEval;
			iPlayedMoveEval = iPlayedEval;
			iBestMoveEval = iBestEval;
			sPlayedMove = sPlayed;
			sBestMove = sBest;
			pPlayedMove = pPlayed;
			pBestMove = pBest;
			questionLevel = ql;
			color = player;
	}
	//获取这步棋分析结果的总体描述
	System::String^ getGeneralDescription() {
		return generalDescription;
	}
	//获取字符串表示的已下棋步估值
	System::String^ getSPlayedMoveEval() {
		return sPlayedMoveEval;
	}
	//获取字符串表示的最佳棋步的估值
	System::String^ getSBestMoveEval() {
		return sBestMoveEval;
	}
	//获取 int 表示的已下棋步的估值
	int getIPlayedMoveEval() {
		return iBestMoveEval;
	}
	//获取 int 表示的最佳棋步的估值
	int getIBestMoveEval() {
		return iBestMoveEval;
	}
	//获取字符串表示的已下棋步
	System::String^ getSPlayedMove() {
		return sPlayedMove;
	}
	//获取字符串表示的最佳棋步
	System::String^ getSBestMove() {
		return sBestMove;
	}
	//获取由 System::Drawing::Point 对象表示的已下棋步
	System::Drawing::Point getPPlayedMove() {
		return pPlayedMove;
	}
	//获取由 System::Drawing::Point 对象表示的最佳棋步
	System::Drawing::Point getPBestMove() {
		return pBestMove;
	}
	//获取问题级别
	QuestionLevel getQuestionLevel() {
		return questionLevel;
	}
	//获取这步棋的走棋方
	Chess getPlayer() {
		return color;
	}
};

//概述：类 DetailedAnalyzeResult 表示一局棋的分析结果
public ref class DetailedAnalyzeResult {
	System::Collections::Generic::List<AnalyzedMove>^ moves;
public:
	//初始化为空 DetailedAnalyzeResult 对象
	DetailedAnalyzeResult() {
		moves = gcnew System::Collections::Generic::List<AnalyzedMove>();
		moves->Add(AnalyzedMove("", Chess::AVAILABLE, "", "", 0, 0, "", "", System::Drawing::Point(-1, -1), 
			System::Drawing::Point(-1, -1), QuestionLevel::NONE));
	}

	/*
	概述：获取某一步的分析结果

	参数：
	step：要获取的棋步序号

	返回值：第 step 步的分析结果
	*/
	AnalyzedMove getAnalyzedResult(int step) {
		return moves[step];
	}

	/*
	概述：往 DetailedAnalyzeResult 对象中加入棋步分析结果

	参数：
	move：要加入的棋步分析结果
	*/
	void addAnalyzedResult(AnalyzedMove move) {
		moves->Add(move);
	}

	//获取总的棋步数
	int getSteps() {
		return moves->Count;
	}

	//清空 DetailedAnalyzeResult 对象
	void clear() {
		moves->Clear();
		moves->Add(AnalyzedMove("", Chess::AVAILABLE, "", "", 0, 0, "", "", System::Drawing::Point(-1, -1), 
			System::Drawing::Point(-1, -1), QuestionLevel::NONE));
	}
};

} // namespace Othello
