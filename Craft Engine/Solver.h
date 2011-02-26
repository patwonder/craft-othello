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

#ifndef SOLVER_H
#define SOLVER_H
/*
********************************************************************************
								Solver.h
		作者：Patrick
		概述：包含类 Solver 的声明。
			Solver 是 Craft 的核心 AI 引擎。

********************************************************************************
*/

#include "Common.h"
#include "BitBoard.h"
#include <string>

#ifdef _CLASSINDLL
	#define CALLSPEC __fastcall
#else
	#define CALLSPEC
#endif

//#define REEVALUATE
#define ENDGAME_USE_NEGASCOUT
#define MIDGAME_USE_NEGASCOUT
//#define USE_MTD_F
// ETC(Enhanced Transposition Cutoff)
//#define USE_ETC
#define USE_ETC_AT_ROOT
#define STABILITY
#define USE_STABILITY
#define COMPACT

namespace CraftEngine {

class CLASSINDLL_CLASS_DECL AnalyzeResult {
	int bestEvals[WIDTH * HEIGHT];
	int bestMoves[WIDTH * HEIGHT];
	int playedEvals[WIDTH * HEIGHT];
	bool playedMoveInBook[WIDTH * HEIGHT];
	int playedMoves[WIDTH * HEIGHT];
	int player[WIDTH * HEIGHT];
	int moves;
	int firstEmpty;
public:
	AnalyzeResult();
	void addResult(int best, int bestMove, int played, int playedMove, int color);
	void setFirstEmpty(int empty);
	int getBestEval(int moveIndex) const {
		return bestEvals[moveIndex];
	}
	int getBestMove(int moveIndex) const {
		return bestMoves[moveIndex];
	}
	int getPlayedEval(int moveIndex) const {
		return playedEvals[moveIndex];
	}
	int getPlayedMove(int moveIndex) const {
		return playedMoves[moveIndex];
	}
	int getPlayer(int moveIndex) const {
		return player[moveIndex];
	}
	int getMaxMoves() const {
		return moves;
	}
	int getEmpties(int moveIndex) const {
		return firstEmpty - moveIndex;
	}
	void clear();
};

class CLASSINDLL_CLASS_DECL SolverResult {
private:
	int result;
	int bestMove;
public:
	SolverResult(int r, int bm);
	int getResult() {
		return result;
	}
	int getBestMove() {
		return bestMove;
	}
};

struct CLASSINDLL_CLASS_DECL CutPair {
	int shallowDepth; // Shallow search depth
	int deepDepth; // Deep search depth
	// v'= a*v + b + e, where e/sigma is normally distributed
	float a, b, sigma;
	float a_r;

	CutPair() {
		shallowDepth = 0;
		deepDepth = 0;
		a = 1.0f;
		b = 0.0f;
		sigma = 0.0f;
		a_r = 1.0f;
	}

	CutPair(int shallow, int deep, float a, float b, float sigma) {
		shallowDepth = shallow;
		deepDepth = deep;
		this->a = a;
		this->b = b;
		this->sigma = sigma;
		a_r = 1.0f / a;
	}
};

class CLASSINDLL_CLASS_DECL Solver {
public:
	// temporary
	//void getStabilityBound(int& lower, int& upper) {
	//	calcStabilityBound(black, white, lower, upper);
	//}

	// constants
	static const int AV = 0;
	static const int BLACK = 1;
	static const int WHITE = 2;
	static const int MAXSTEP = 8 * 8;
	static const int INFINITE = 1000000000;
	static const int RULER = CraftEngine::RULER;
	static const std::string DEFAULT_PATTERN_PATH;
	static const std::string DEFAULT_BOOK_PATH;
	
	// initialization and cleanup
	static bool initialize();
	static bool initialize(std::string patternPath, std::string bookPath);
	static void cleanup();
	static int getInitPart();
	static int getInitPercent();

	static Solver* newInstance();
	static Solver* newInstance(int board[]);
	~Solver() { delete analyzeResult; }

	// search stuff
	void setBoard(int board[MAXSTEP]);
	void makeMove(int pos, int color);
	void unMakeMove();
	int getMobility(int color) const;

	// search control
	SolverResult solve(int color, int depth);
	SolverResult solve(int color, int depth, bool useBook);
	SolverResult solveExact(int color, bool winLoss);
	SolverResult partialSolveExact(int color, bool winLoss, int percentage, int& actualPercentage);
	SolverResult solveBigEat(int color, int depth);
	void abortSearch();
	void abortSearchComplete();

	// dynamic search display
	unsigned long long getEvNum() const;
	int getPercent() const;
	int getPartialResult() const;
	int getPartialDepth() const;
	int getFocusedMove() const;
	int getSelectedMove() const;
	int getBookPercent() const;
	void clearSearchStats();
	int getEpcPercentage() const {
		return currentEpcPercentage;
	}
	int getPV(int pv[], int length);

	// the transposition table stuff
	static void clearCache();
	static void setCacheSize(size_t newCacheSize);
	static size_t getCacheSize();

	// used for pattern learning
	static const int PARTIALDEPTH_WLD = -1;
	static const int PARTIALDEPTH_EXACT = -2;
	void parseGame(int board[], int steps[], int totalSteps, int fp = BLACK);
	void clearGame();//

	// used for book learning
	static bool saveBook();
	void extendBook();
	void setBookTolerance(int tolerance);
	static void setBookDepth(int depth);
	static void setBookEndDepth(int depth);

	// used for analysis
	void analyzeGame(int midDepth, int wlStep, int endStep);
	AnalyzeResult* getAnalyzeResult();
	
private:

	static volatile int initPart;
	static volatile int initPercent;

	// used for searching
	struct Reinterpret_BitBoard {
		unsigned short p1;
		unsigned short p2;
		unsigned short p3;
		unsigned short p4;
	};

	struct AsmBitBoard {
		unsigned int high;
		unsigned int low;
	};

	// useful search constants
	static const int END_USE_SORT_DEPTH = 10;
	static const int MID_USE_SORT_DEPTH = 4;
	static const int END_WINDOW_SIZE = 5;
	static const int MID_WINDOW_SIZE = 2001;
	static const int DEEP_COVER = 1;
	static const int MAXDEPTH = 30;
	static const int MAX_STACK_SIZE = 1024;
	static const unsigned char DIR1 = 1;
	static const unsigned char DIR2 = 2;
	static const unsigned char DIR3 = 4;
	static const unsigned char DIR4 = 8;
	static const unsigned char DIR5 = 16;
	static const unsigned char DIR6 = 32;
	static const unsigned char DIR7 = 64;
	static const unsigned char DIR8 = 128;
	static const unsigned char DIR15 = DIR1 | DIR5;
	static const unsigned char DIR2468 = DIR2 | DIR4 | DIR6 | DIR8;
	static const unsigned char DIR37 = DIR3 | DIR7;
#ifdef COMPACT
	static unsigned char bitTable[0x10000];
	static unsigned char fastCountPattern[8 * 256];
#else
	static int bitTable[0x10000];
	static int fastCountPattern[8 * 256];
#endif
	static BitBoard posTable[MAXSTEP];
	static int moveOrder[MAXSTEP];
	static BitBoard orderTable[MAXSTEP];
	static BitBoard lineTable[MAXSTEP][MAXSTEP];
	static unsigned char dirMask[MAXSTEP];
	static char fastFlipPattern[8 * 256 * 256][2];
	static BitBoard neighborhood[MAXSTEP];

	// MPC-related
	static const int MIN_MPC_DEPTH = 7;
	static const int MAX_MPC_DEPTH = 15;
	static const int MPC_TRIES = 2;
	static const int MPC_STAGES = 5;
	static const int MPC_DEPTH_THRESHOLD = 8;
	static const float MPC_PERCENTILE[MPC_STAGES];
	static const int MPC_PERCENTAGE[MPC_STAGES];
	static const int MPC_STAGE[MAXSTEP + 1];
	static CutPair mpcThreshold[MPC_STAGES][MAX_MPC_DEPTH + 1][MPC_TRIES];
	static inline bool betacuttable(int beta) {
		return beta < INFINITE - MAXSTEP && beta > -INFINITE + MAXSTEP;
	}
	static inline bool alphacuttable(int alpha) {
		return alpha > -INFINITE + MAXSTEP && alpha < INFINITE - MAXSTEP;
	}
	int search_mpc(BitBoard& my, BitBoard& op, int depth, int alpha, int beta, bool lastFound);

	// EPC-related -- EPC stands for End-game Prob Cut
	static const int MIN_EPC_DEPTH = 16;
	static const int MAX_EPC_DEPTH = 23;
	static const int EPC_TRIES = 2;
	static const int EPC_STAGES = 9;
	static const float EPC_PERCENTILE[EPC_STAGES + 1];
	static const int EPC_PERCENTAGE[EPC_STAGES + 1];
	static CutPair epcThreshold[MAX_EPC_DEPTH + 1][EPC_TRIES];
	int currentEpcStage;
	volatile int currentEpcPercentage;
	int searchExact_epc(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound);
	SolverResult solveExactInternal(int color, bool winLoss, int epcStage);

	// fastest-first heuristic
	static const int MID_WIPEOUT_MOBILITY_WEIGHT = (int)(3.0 * RULER);
	static const int MID_WIPEOUT_THRESHOLD = 70 * RULER;
	static const int MID_VERY_HIGH_EVAL_BONUS = 10 * RULER;
	static const int END_MOBILITY_WEIGHT = (int)(3.0 * RULER);
	static const int END_WIPEOUT_MOBILITY_WEIGHT = (int)(6.0 * RULER);
	static const int END_WIPEOUT_THRESHOLD = 60 * RULER;
	static const int END_VERY_HIGH_EVAL_BONUS = 20 * RULER;
	static const int END_FASTEST_FIRST_DEPTH = 20;
	static inline int weighted_mobility_bits(const BitBoard& mobility);

	// evaluations outside winning/losing points does not need to be considered in depth
	static const int MID_SEARCH_BOUND = INFINITE - MAXSTEP + 1;

	// MTD(f)
#ifdef USE_MTD_F
	int mtdExact(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound);
	int mtdExact_epc(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound);
	int mtdExact_main(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound,
		int (Solver::*mainSearchFunction)(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound));
#endif
	
	struct EmptyNode {
		int pos;
		int holeid;
		EmptyNode* succ;
		EmptyNode* prev;
	};

	// Parity heuristic
	static const int MAX_USE_PARITY_DEPTH = 15;
	static const int END_PARITY_WEIGHT[MAXSTEP];
	unsigned int regionParity;
	unsigned int holeids[MAXSTEP];
	void prepareParity();
	static inline bool notAccrossBorder(int i, int j, BitBoard mask);
	inline void setParity(EmptyNode* emptyPos);
	inline void unSetParity(EmptyNode* emptyPos);
	int searchExact_parity(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound);

#ifdef STABILITY
	// stability cut
	static int twoTo3Base[256];
#ifdef COMPACT
	static unsigned char stab_my[6561];
	static unsigned char stab_op[6561];
#else
	static unsigned int stab_my[6561];
	static unsigned int stab_op[6561];
#endif
	static int bits16(unsigned int num);
	static void lineSearch(unsigned int my, unsigned int op, unsigned int& stable_my, unsigned int& stable_op);
	static void calcStabilityBound(const BitBoard& my, const BitBoard& op, int& lower, int& upper);
#endif

	BitBoard black, white;
	BitBoard bstack[MAXSTEP + 1];
	BitBoard wstack[MAXSTEP + 1];
	int pstack[MAXSTEP + 1];
	int mstack[MAXSTEP + 1];
	int empties;
	int stackptr;
	int results[MAXSTEP];
	int positions[MAXSTEP];
	EmptyNode emptyList[MAXSTEP + 2];
	EmptyNode* emptyHead, *emptyTail;
	EmptyNode* emptyStack[MAXSTEP + 1];
	EmptyNode* emptyPtr[MAXSTEP];
	int sortStack[MAX_STACK_SIZE];
	EmptyNode* emptySortStack[MAX_STACK_SIZE];
	int sortResultStack[MAX_STACK_SIZE];
	int sortStackPtr;

	static std::string bookPath;
	static std::string patternPath;

	Solver();
	Solver(int board[MAXSTEP]);
	static void initConstants();
	void setBitBoard(BitBoard black, BitBoard white);
	static int bitGet1s(unsigned int num);
	static int bits(const BitBoard& bb);
	static BitBoard mobility(const BitBoard& b1, const BitBoard& b2);
	static void putChess(int pos, BitBoard& b1, BitBoard& b2);
	static bool checkedPutChess(int pos, BitBoard& b1, BitBoard& b2);
	void setEmpties();
	void makeMoveAndSetEmpties(EmptyNode* emptyPos, BitBoard& my, BitBoard& op);
	bool checkedMakeMoveAndSetEmpties(EmptyNode* emptyPos, BitBoard& my, BitBoard& op);
	void unMakeMoveAndSetEmpties();
	int search(BitBoard& my, BitBoard& op, int depth, int alpha, int beta, bool lastFound);
	int fastSearch(BitBoard& my, BitBoard& op, int depth, int alpha, int beta, bool lastFound);	
	int checkedSearch(BitBoard& my, BitBoard& op, int depth, int alpha, int beta, bool lastFound);
	int searchExact(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound);
	int fastSearchExact(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound);
	int fastSearchExact_TwoEmpty_NoLastFound(BitBoard& my, BitBoard& op, int empty1, int empty2, int beta);
	int fastSearchExact_TwoEmpty(BitBoard& my, BitBoard& op, int empty1, int empty2, int alpha, int beta);
	int fastSearchExact_ThreeEmpty(BitBoard& my, BitBoard& op, int empty1, int empty2, int empty3, int alpha, int beta, bool lastFound);
	int fastSearchExact_FourEmpty(BitBoard& my, BitBoard& op, int empty1, int empty2, int empty3, int empty4, int alpha, int beta, bool lastFound);
	int searchBigEat(int color, int alpha, int beta, int depth, bool lastFound);
	int endToMid(int value);
	void makeMove(int pos, BitBoard& my, BitBoard& op);
	bool checkedMakeMove(int pos, BitBoard& my, BitBoard& op);
	int evaluate(const BitBoard& my, const BitBoard& op);
	int evaluate_direct(const BitBoard& my, const BitBoard& op);
	int evaluate_diff(const BitBoard& my, const BitBoard& op);
	int strongEvaluate(BitBoard& my, BitBoard& op);
	int getResult(const BitBoard& my);
	int getResultNoEmpty(const BitBoard& my);
	int getResultNoCount(const BitBoard& my);
	int getResultNoEmptyNoCount(const BitBoard& my);
	double getSmallerRnd(int rate = 2);
	SolverResult particularSolve(int color, int depth, int move);
	int getEndSortSearchDepth() const;
	static int getMidSortSearchDepth(int depth);
	static int getEndSortSearchLowerBound1(int alpha);
	static int getEndSortSearchLowerBound(int alpha);
	static int getEndSortSearchUpperBound(int beta);

	// used for dynamic search state display
	static const int MAX_PV_LENGTH = 20;
	volatile int percent, subPercent, currentBlock;
	volatile unsigned long long evnum;
	volatile int partialResult, partialDepth;
	volatile int focusedMove, selectedMove;
	volatile bool aborted;
	int principleVariation[MAX_PV_LENGTH];
	volatile int pvLength;

	// the transposition table stuff
	struct TPInfo {
		int lower;
		int upper;
		short depth;
		short pos;
		unsigned int flags;
		BitBoard my;
		BitBoard op;
	};
	static const unsigned int TP_MPC = 0x1; // mark mpc nodes

	void initPV();
	void setPV(BitBoard& my, BitBoard& op, int depth, int firstMove);
	void setBookPV(BitBoard& my, BitBoard& op, int firstMove);
	// extract principle variation from transposition table ONLY - no ETC used
	int searchPV(BitBoard& my, BitBoard& op, int depth, bool lastFound, int* pvStart, int* pvEnd);

	static unsigned int bZobrist[0x10000];
	static unsigned int wZobrist[0x10000];
	static const size_t TABLE_LEN = 0x20000;
	static size_t currentTableSize;
	static const unsigned int TABLE_MASK = 0x0001ffff;
	static unsigned int currentTableMask;
	//static const int DEPTH_EXACT = 200;
	static const int TYPE_ALPHA = 0;
	static const int TYPE_EXACT = 1;
	static const int TYPE_BETA = 2;
	//static const int RESERVED_EMPTY = 5;

	// new table structure, 64 bytes long which fits perfectly in a cache line
	struct TPEntry {
		TPInfo deeper;
		TPInfo newer;
	};

	static TPEntry* tp;
	void saveTp(int zobPos, const BitBoard& my, const BitBoard& op, 
		int lower, int upper, int pos, int depth, unsigned int flags);

	static void initTable();
	static bool checkTableSize(size_t tableSize);
	unsigned int getZobKey() const;
	static unsigned int getZobKey(const BitBoard& my, const BitBoard& op);

	// used for book & learning
	struct BasicBookNode {
		BitBoard my, op;
		char moveCount;
		char* moves;
		int* evals;
		char fatherCount;
		BitBoard* fatherMy, *fatherOp;
		char* fatherMove;
	};
	static class Book;
	class BookNode {
	private:
		BasicBookNode* node;
		int transformState;
		friend class Book;

		BitBoard getTransformed(BitBoard bb) const;
		int getTransformed(int pos) const;
		BitBoard setTransformed(BitBoard bb) const;
		int setTransformed(int pos) const;
		BookNode(BasicBookNode* node, int transformState = 0);
	public:
		BookNode();
		operator void*();

		BitBoard getMy() const;
		BitBoard getOp() const;
		int getMoveCount() const;
		int getMove(int index) const;
		int getEval(int index) const;
		int getFatherCount() const;
		BitBoard getFatherMy(int index) const;
		BitBoard getFatherOp(int index) const;
		int getFatherMove(int index) const;

		void setMy(const BitBoard& my);
		void setOp(const BitBoard& op);
		void setMoveCount(int moveCount);
		void setMove(int index, int move);
		void setEval(int index, int eval);
		void setEvalSymmetric(int index, int eval);
		void sortByEval();
		void setFatherCount(int fatherCount);
		void setFatherMy(int index, const BitBoard& fatherMy);
		void setFatherOp(int index, const BitBoard& fatherOp);
		void setFatherMove(int index, int fatherMove);
	};
	static class Book {
	private:
		static const int DEFAULT_CAPACITY = 10;
		static const int LOAD_FACTOR = 75;
		int capacity, size;
		BasicBookNode** nodes;

		void clear();
		BasicBookNode* simpleGet(const BitBoard& my, const BitBoard& op) const;
	public:
		Book();
		~Book();
		Book(int capacity);
		BookNode get(BitBoard my, BitBoard op);
		void insert(const BookNode& node);
		void inflate();
		int getSize() const;
		int getCapacity() const;
		BookNode get(int index) const;
#ifdef REEVALUATE
		void reevaluate();
		void reevaluateNode(BookNode node);
#endif
	};

	static const BitBoard defaultMy = 0x0000000810000000ull;
	static const BitBoard defaultOp = 0x0000001008000000ull;
	static const int DEFAULT_BOOK_DEPTH = 8;
	static const int DEFAULT_BOOK_END_DEPTH = 20;
	static const int EVAL_RANGE = 12 * RULER;

	static bool extendingBook;
	static bool isBookChanged;
	static Book* book;
	static Solver* staticSolver;
	int maxEval, tolerance;
	static int bookDepth;
	static int bookEndDepth;

	static bool initBook(std::string bookPath);
	static void setDefaultNode();
	static void addFather(const BookNode& node);
	static void correctEval();
	static void propagateEval(const BookNode& from);
	static bool loadBook(std::string bookFile);
	static bool saveBook(std::string bookFile);
	void expandNode(BookNode& node);
	void extendSingle();
	static void connectChilds(BookNode& node);

	// the pattern constants and pre-calculated data(speed-up)
	static const int STAGES = 17;
	static const int PATTERNS = 11;
	static const int ACTUAL_PATTERNS = 46;
	static const int MAX_PATTERN_COUNT = 59049;
	static const int TOTAL_PATTERN_COUNT = 167265;
	static const int ALL_STAGE_PATTERN_COUNT = STAGES * TOTAL_PATTERN_COUNT;
	static const int pow3[10];
	static const int stage[MAXSTEP + 1];
	static const int rstage[MAXSTEP + 1];
	static const int patternCount[PATTERNS];
	static const int patternPtr[ACTUAL_PATTERNS];

	static int *patternValues;
	static float *deltaSum;
	static int *occurance;
	static int patternCorrection[STAGES];
	static float correctionDeltaSum[STAGES];
	static int correctionOccurance[STAGES];

	static unsigned short myPV[256][8][ACTUAL_PATTERNS]; // max 59049 < 65535
	//static int opPV[256][8][ACTUAL_PATTERNS]; // since opPV = 2 * myPV
	static unsigned char pPtr[256][8][ACTUAL_PATTERNS];
	static unsigned char pVCount[256][8];
	static int posDepend[MAXSTEP][ACTUAL_PATTERNS];
	static int patternDependCount[ACTUAL_PATTERNS];
	static int patternOffset[STAGES][ACTUAL_PATTERNS];

	// for evaluate_diff
	int pattern[ACTUAL_PATTERNS];
	BitBoard diff_my_last;
	BitBoard diff_op_last;
	BitBoard diff_empty_last;

	// learing part
	static bool initPatterns(std::string patternPath);
	static void flipSingle(int dir, BitBoard& bb);
	static void flipPos(int dir, int& pos);
	static void reverseSingle(BitBoard& bb);
	static void reversePos(int& pos);
	static void transformSingle(int pattern, BitBoard& bb);
	static void transformPos(int pattern, int& pos);
	static const int SYMMETRICS = 8;
	static const int antiPattern[SYMMETRICS];
	void flipBoard(int dir);
	void reverseBoard();
	void reverseColor();
	static void addDependency(int pos, int pInd);
	static void multipleAdd(int pos, int step, int count, int pInd);
	static bool loadPatterns(std::string patternFile);

	// used for analysis
	AnalyzeResult* analyzeResult;

	// fast-flip functions
	typedef bool (CALLSPEC *FlipFunction)(BitBoard& b1, BitBoard& b2);
	static const FlipFunction flipFunction[MAXSTEP];

	static bool CALLSPEC putChess_A1(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_A8(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_H1(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_H8(BitBoard& b1, BitBoard& b2);

	static bool CALLSPEC putChess_A2(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_A7(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_B1(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_B8(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_G1(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_G8(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_H2(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_H7(BitBoard& b1, BitBoard& b2);

	static bool CALLSPEC putChess_A3(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_A6(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_C1(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_C8(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_F1(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_F8(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_H3(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_H6(BitBoard& b1, BitBoard& b2);

	static bool CALLSPEC putChess_A4(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_A5(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_D1(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_D8(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_E1(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_E8(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_H4(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_H5(BitBoard& b1, BitBoard& b2);

	static bool CALLSPEC putChess_B2(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_B7(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_G2(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_G7(BitBoard& b1, BitBoard& b2);

	static bool CALLSPEC putChess_B3(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_B6(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_C2(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_C7(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_F2(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_F7(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_G3(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_G6(BitBoard& b1, BitBoard& b2);

	static bool CALLSPEC putChess_B4(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_B5(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_D2(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_D7(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_E2(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_E7(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_G4(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_G5(BitBoard& b1, BitBoard& b2);

	static bool CALLSPEC putChess_C3(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_C6(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_F3(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_F6(BitBoard& b1, BitBoard& b2);

	static bool CALLSPEC putChess_C4(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_C5(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_D3(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_D6(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_E3(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_E6(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_F4(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_F5(BitBoard& b1, BitBoard& b2);

	static bool CALLSPEC putChess_D4(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_D5(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_E4(BitBoard& b1, BitBoard& b2);
	static bool CALLSPEC putChess_E5(BitBoard& b1, BitBoard& b2);

	// count-flip functions
	typedef int (CALLSPEC *CountFunction)(const BitBoard& b1);
	static const CountFunction countFunction[MAXSTEP];

	static int CALLSPEC countFlips_A1(const BitBoard& b1);
	static int CALLSPEC countFlips_A8(const BitBoard& b1);
	static int CALLSPEC countFlips_H1(const BitBoard& b1);
	static int CALLSPEC countFlips_H8(const BitBoard& b1);

	static int CALLSPEC countFlips_A2(const BitBoard& b1);
	static int CALLSPEC countFlips_A7(const BitBoard& b1);
	static int CALLSPEC countFlips_B1(const BitBoard& b1);
	static int CALLSPEC countFlips_B8(const BitBoard& b1);
	static int CALLSPEC countFlips_G1(const BitBoard& b1);
	static int CALLSPEC countFlips_G8(const BitBoard& b1);
	static int CALLSPEC countFlips_H2(const BitBoard& b1);
	static int CALLSPEC countFlips_H7(const BitBoard& b1);

	static int CALLSPEC countFlips_A3(const BitBoard& b1);
	static int CALLSPEC countFlips_A6(const BitBoard& b1);
	static int CALLSPEC countFlips_C1(const BitBoard& b1);
	static int CALLSPEC countFlips_C8(const BitBoard& b1);
	static int CALLSPEC countFlips_F1(const BitBoard& b1);
	static int CALLSPEC countFlips_F8(const BitBoard& b1);
	static int CALLSPEC countFlips_H3(const BitBoard& b1);
	static int CALLSPEC countFlips_H6(const BitBoard& b1);

	static int CALLSPEC countFlips_A4(const BitBoard& b1);
	static int CALLSPEC countFlips_A5(const BitBoard& b1);
	static int CALLSPEC countFlips_D1(const BitBoard& b1);
	static int CALLSPEC countFlips_D8(const BitBoard& b1);
	static int CALLSPEC countFlips_E1(const BitBoard& b1);
	static int CALLSPEC countFlips_E8(const BitBoard& b1);
	static int CALLSPEC countFlips_H4(const BitBoard& b1);
	static int CALLSPEC countFlips_H5(const BitBoard& b1);

	static int CALLSPEC countFlips_B2(const BitBoard& b1);
	static int CALLSPEC countFlips_B7(const BitBoard& b1);
	static int CALLSPEC countFlips_G2(const BitBoard& b1);
	static int CALLSPEC countFlips_G7(const BitBoard& b1);

	static int CALLSPEC countFlips_B3(const BitBoard& b1);
	static int CALLSPEC countFlips_B6(const BitBoard& b1);
	static int CALLSPEC countFlips_C2(const BitBoard& b1);
	static int CALLSPEC countFlips_C7(const BitBoard& b1);
	static int CALLSPEC countFlips_F2(const BitBoard& b1);
	static int CALLSPEC countFlips_F7(const BitBoard& b1);
	static int CALLSPEC countFlips_G3(const BitBoard& b1);
	static int CALLSPEC countFlips_G6(const BitBoard& b1);

	static int CALLSPEC countFlips_B4(const BitBoard& b1);
	static int CALLSPEC countFlips_B5(const BitBoard& b1);
	static int CALLSPEC countFlips_D2(const BitBoard& b1);
	static int CALLSPEC countFlips_D7(const BitBoard& b1);
	static int CALLSPEC countFlips_E2(const BitBoard& b1);
	static int CALLSPEC countFlips_E7(const BitBoard& b1);
	static int CALLSPEC countFlips_G4(const BitBoard& b1);
	static int CALLSPEC countFlips_G5(const BitBoard& b1);

	static int CALLSPEC countFlips_C3(const BitBoard& b1);
	static int CALLSPEC countFlips_C6(const BitBoard& b1);
	static int CALLSPEC countFlips_F3(const BitBoard& b1);
	static int CALLSPEC countFlips_F6(const BitBoard& b1);

	static int CALLSPEC countFlips_C4(const BitBoard& b1);
	static int CALLSPEC countFlips_C5(const BitBoard& b1);
	static int CALLSPEC countFlips_D3(const BitBoard& b1);
	static int CALLSPEC countFlips_D6(const BitBoard& b1);
	static int CALLSPEC countFlips_E3(const BitBoard& b1);
	static int CALLSPEC countFlips_E6(const BitBoard& b1);
	static int CALLSPEC countFlips_F4(const BitBoard& b1);
	static int CALLSPEC countFlips_F5(const BitBoard& b1);

	static int CALLSPEC countFlips_D4(const BitBoard& b1);
	static int CALLSPEC countFlips_D5(const BitBoard& b1);
	static int CALLSPEC countFlips_E4(const BitBoard& b1);
	static int CALLSPEC countFlips_E5(const BitBoard& b1);

};// class Solver

} // namespace Othello

#endif // SOLVER_H
