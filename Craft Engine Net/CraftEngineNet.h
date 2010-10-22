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

// Craft Engine Net.h

#pragma once

#include "Solver.h"
#include "Definition.h"

using namespace System;

namespace CraftEngineNet {
	
	///Craft引擎 .NET版本
	public ref class Engine {
	private:
		CraftEngine::Solver* solver;

	public:
		///估值缩放比例，实际估值=(float)返回估值/RULER
		static const int RULER = CraftEngine::Solver::RULER;
		///在估值中表示无穷大
		static const int INFINITE = CraftEngine::Solver::INFINITE;
		///最大棋步，相当于棋盘格个数(8*8=64)
		static const int MAXSTEP = CraftEngine::Solver::MAXSTEP;

		///创建一个新的引擎
		Engine();

		///取消当前的搜索
		///参数：
		///  searchThread: 进行搜索的线程
		void abortSearch(System::Threading::Thread^ searchThread);
		///清理内存，释放资源
		static void cleanup();
		///清空缓存区域
		static void clearCache();
		///清空棋步序列
		void clearGame();
		///清除搜索状态
		void clearSearchStats();
		///以当前的棋步序列进行棋谱学习
		void extendBook();
		
		///返回棋谱学习进度，可跨线程操作
		int getBookPercent();
		///返回缓存的大小(单表节点数, 占用内存空间为返回值*64字节)
		static size_t getCacheSize();
		///返回搜索状态，可跨线程操作
		SearchStats getSearchStats();
		///返回指定游戏方的行动力，即可落子的位置总数
		///参数：
		///  side: 游戏方
		int getMobility(Chess side);

		///初始化Craft Engine
		///这个函数以默认参数初始化Craft Engine
		///模板文件使用当前目录下的pattern.craft文件
		///棋谱文件使用当前目录下的book.craft文件
		static void initialize();
		///初始化Craft Engine。
		///这个函数使用指定的参数初始化Craft Engine
		///参数：
		///  patternPath: 模板文件的路径
		///  bookPath: 棋谱文件的路径
		static void initialize(String^ patternPath, String^ bookPath);

		///走棋
		///参数：
		///  point: 要走的棋步
		///  side: 走棋方
		void makeMove(Move point, Chess side);
		///加载一盘棋(可用于分析、棋谱学习等)
		///参数：
		///  board: 初始棋盘
		///  steps: 棋步序列
		///默认由黑方先行
		void parseGame(Board board, array<Move>^ steps);
		///加载一盘棋(可用于分析、棋谱学习等)
		///参数：
		///  board: 初始棋盘
		///  steps: 棋步序列
		///  firstPlayer: 先行的一方
		void parseGame(Board board, array<Move>^ steps, Chess firstPlayer);

		///保存棋谱文件。
		///若棋谱在加载后未被修改，则不会重写该文件。
		static void saveBook();

		///设置当前棋盘
		///参数：
		///  board: 要设置的棋盘
		void setBoard(Board board);

		///设置棋谱随机性大小，默认为0
		///设置后返回的棋谱棋步的估值与最大估值之间的差距至多为tolerance
		///参数：
		///  tolerance: 随机性大小
		void setBookTolerance(int tolerance);
		///设置缓存大小(单表节点数, 占用内存空间为返回值*64字节)
		///参数：
		///  newCacheSize: 缓存大小，必须等于1 << N，0<=N<=30
		static void setCacheSize(size_t newCacheSize);

		///进行中局搜索(使用棋谱)，返回中局搜索结果
		///参数：
		///  side: 走棋方
		///  depth: 中局搜索的深度
		SearchResult solve(Chess side, int depth);
		///进行中局搜索，返回中局搜索结果
		///参数：
		///  side: 走棋方
		///  depth: 中局搜索的深度
		///  useBook: 是否使用棋谱
		SearchResult solve(Chess side, int depth, bool useBook);
		///进行大食搜索(使中局棋子数最大化)，返回大食搜索结果
		///参数：
		///  side: 走棋方
		///  depth: 大食搜索的深度
		SearchResult solveBigEat(Chess side, int depth);
		///进行终局搜索，返回终局搜索结果
		///参数：
		///  side: 走棋方
		///  winLoss: 是否进行胜负搜索
		///           指定false进行精确结果搜索
		///胜负搜索只判断胜负，搜索结果的估值比精确搜索更接近于0
		SearchResult solveExact(Chess side, bool winLoss);

		///进行终局选择性搜索，返回搜索结果
		///参数：
		///  side: 走棋方
		///  winLoss: 是否进行胜负搜索
		///           指定false进行精确结果的选择性搜索
		///  percentage: 最少要达到的可信度(百分比)，指定100相当于完全精确搜索
		///  actualPercentage: 实际搜索结果的可信度(百分比)
		///胜负搜索只判断胜负，搜索结果的估值比精确搜索更接近于0
		SearchResult partialSolveExact(Chess side, bool winLoss, int percentage, int% actualPercentage);

		///撤销上次的棋步
		void unMakeMove();

		///析构函数
		~Engine();
		!Engine();
	};
}
