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

#include "Solver.h"

using namespace System;

namespace CraftEngineNet {
	
	///表示棋子颜色和游戏方
	public enum class Chess {
		AVAILABLE, BLACK, WHITE
	};

	///棋步，即棋盘上的一个点
	public value class Move {
	private:
		int x, y;
	public:
		Move(int theX, int theY)
			: x(theX), y(theY) {}
		property int X {
			int get() {
				return x;
			}
			void set(int value) {
				x = value;
			}
		}
		property int Y {
			int get() {
				return y;
			}
			void set(int value) {
				y = value;
			}
		}
	};

	typedef array<Chess, 2>^ Board;

	///搜索结果
	public value class SearchResult {
	private:
		Move move;
		int evaluation;
		bool bookMove;
	public:
		SearchResult(Move move, int evaluation, bool bookMove) {
			this->move = move;
			this->evaluation = evaluation;
			this->bookMove = bookMove;
		}
		///最佳棋步
		property Move Move {
			CraftEngineNet::Move get() {
				return move;
			}
		}
		///估值
		///分中局估值和终局估值两种
		///* 终局估值的范围为-64~64
		///* 中局估值为实际估值*RULER(=10000)的结果
		/// -若中局估值落在-INFINITE~(-INFINITE + 63)范围内，
		///  则代表在中局搜索中检测到必输局面，对应的终局估值为
		///  Evaluation + INFINITE - 64
		/// -若中局估值落在(INFINITE - 63)~INFINITE范围内，
		///  则代表在中局搜索中检测到必胜局面，对应的终局估值为
		///  Evaluation - INFINITE + 64
		property int Evaluation {
			int get() {
				return evaluation;
			}
		}

		property System::String^ MidEvaluationString {
			System::String^ get() {
				if (evaluation < -CraftEngine::Solver::INFINITE + CraftEngine::Solver::MAXSTEP) {
					return (evaluation + CraftEngine::Solver::INFINITE - CraftEngine::Solver::MAXSTEP).ToString();
				} else if (evaluation > CraftEngine::Solver::INFINITE - CraftEngine::Solver::MAXSTEP) {
					return "+" + (evaluation - CraftEngine::Solver::INFINITE + CraftEngine::Solver::MAXSTEP).ToString();
				} else {
					return ((evaluation > 0) ? "+" : "") 
						+ ((double)evaluation / CraftEngine::RULER).ToString("0.00");
				}
			}
		}

		///结果是否从棋谱直接得到
		property bool BookMove {
			bool get() {
				return bookMove;
			}
		}
	};

	///搜索状态
	public value class SearchStats {
	private:
		unsigned long long evaluationCount;
		Move focusedMove;
		Move selectedMove;
		int percent;
		int partialDepth;
		int partialResult;
		int epcPercentage;
	public:
		///终局搜索
		static const int PARTIALDEPTH_EXACT = CraftEngine::Solver::PARTIALDEPTH_EXACT;
		///胜负搜索
		static const int PARTIALDEPTH_WLD = CraftEngine::Solver::PARTIALDEPTH_WLD;

		SearchStats(unsigned long long evnum, Move focused,
			Move selected, int percent, int pDepth, int pResult, int percentage) {
				evaluationCount = evnum;
				focusedMove = focused;
				selectedMove = selected;
				this->percent = percent;
				partialDepth = pDepth;
				partialResult = pResult;
				epcPercentage = percentage;
		}

		///局面计数，即已经进行估值的局面的数目
		property unsigned long long EvaluationCount {
			unsigned long long get() {
				return evaluationCount;
			}
		}
		///当前正在搜索的棋步
		property Move FocusedMove {
			Move get() {
				return focusedMove;
			}
		}
		///当前认为最佳的棋步
		property Move SelectedMove {
			Move get() {
				return selectedMove;
			}
		}
		///搜索完成的百分比(只是一个估计)
		property int Percent {
			int get() {
				return percent;
			}
		}
		///正在进行的搜索深度
		property int PartialDepth {
			int get() {
				return partialDepth;
			}
		}
		///当前深度的当前最佳估值
		property int PartialResult {
			int get() {
				return partialResult;
			}
		}

		///终局选择性搜索的可信度(1%~100%)
		property int EPCPercentage {
			int get() {
				return epcPercentage;
			}
		}
	};

	public ref class InitializationException : public System::Exception {
	public:
		InitializationException() {
			
		}
		InitializationException(System::String^ message)
			: Exception(message) {
			
		}
		InitializationException(System::String^ message, System::Exception^ innerException)
			: Exception(message, innerException) {
			
		}

	};

	///一些全局常量
	public ref class Global abstract {
	public:
		///棋盘横向长度
		static const int Width = CraftEngine::WIDTH;
		///棋盘纵向长度
		static const int Height = CraftEngine::HEIGHT;
		///Infinity常量
		static const int Infinite = CraftEngine::Solver::INFINITE;
	};
} // namespace CraftEngineNet
