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

/*
********************************************************************************
								Solver.cpp
		作者：Patrick
		概述：包含类 Solver 的部分定义。
			Solver 是 Craft 的核心 AI 引擎。

********************************************************************************
*/

#include "StdAfx.h"
#define _CRT_RAND_S
#include <cstdlib>
#include <fstream>
#include "Solver.h"

#ifdef REEVALUATE
#include <sstream>
#endif

//#define COUNT_INTERNAL_NODES
#define USE_EVEN_DEEPENING

//const unsigned long long int ULONG_defaultNodemy = 0x0000001008000000ull;
namespace CraftEngine {

volatile int Solver::initPart = 0;
volatile int Solver::initPercent = 0;
#ifdef COMPACT
unsigned char Solver::bitTable[0x10000];
#else
int Solver::bitTable[0x10000];
#endif
BitBoard Solver::posTable[MAXSTEP];
BitBoard Solver::lineTable[MAXSTEP][MAXSTEP];
unsigned int Solver::bZobrist[0x10000];
unsigned int Solver::wZobrist[0x10000];
char Solver::fastFlipPattern[8 * 256 * 256][2];
#ifdef COMPACT
unsigned char Solver::fastCountPattern[8 * 256];
#else
int Solver::fastCountPattern[8 * 256];
#endif
BitBoard Solver::neighborhood[MAXSTEP];
Solver::TPEntry* Solver::tp = NULL;
size_t Solver::currentTableSize;
unsigned int Solver::currentTableMask;
int *Solver::patternValues;
float *Solver::deltaSum;
int *Solver::occurance;
int Solver::patternCorrection[STAGES];
float Solver::correctionDeltaSum[STAGES];
int Solver::correctionOccurance[STAGES];
unsigned short Solver::myPV[256][8][ACTUAL_PATTERNS];
//int Solver::opPV[256][8][ACTUAL_PATTERNS];
unsigned char Solver::pPtr[256][8][ACTUAL_PATTERNS];
unsigned char Solver::pVCount[256][8];
int Solver::posDepend[MAXSTEP][ACTUAL_PATTERNS];
int Solver::patternDependCount[ACTUAL_PATTERNS];
int Solver::patternOffset[STAGES][ACTUAL_PATTERNS];
Solver::Book* Solver::book;
int Solver::bookDepth;
int Solver::bookEndDepth;
Solver* Solver::staticSolver = NULL;
bool Solver::extendingBook;
bool Solver::isBookChanged;
bool Solver::fatherAdded;
std::string Solver::bookPath;
std::string Solver::patternPath;
const std::string Solver::DEFAULT_PATTERN_PATH = "data.craft";
const std::string Solver::DEFAULT_BOOK_PATH = "book.craft";

#ifdef STABILITY
int Solver::twoTo3Base[256];
#endif

//Move ordering
//1）A1, H1, A8, H8
//2）C1, F1, A3, H3, A6, H6, C8, F8
//3）C3, F3, C6, F6
//4）D1, E1, A4, H4, A5, H5, D8, E8
//5）D3, E3, C4, F4, C5, F5, D6, E6
//6）D2, E2, B4, G4, B5, G5, D7, E7
//7）C2, F2, B3, G3, B6, G6, C7, F7
//8）B1, G1, A2, H2, A7, H7, B8, G8
//9）B2, G2, B7, G7
int Solver::moveOrder[MAXSTEP] = {0, 56, 7, 63,
								  16, 40, 2, 58, 5, 61, 23, 47,
								  18, 42, 21, 45,
								  24, 32, 3, 59, 4, 60, 31, 39,
								  26, 34, 19, 43, 20, 44, 29, 37,
								  25, 33, 11, 51, 12, 52, 30, 38,
								  17, 41, 10, 50, 13, 53, 22, 46,
								  8, 48, 1, 57, 6, 62, 15, 55,
								  9, 49, 14, 54,
								  27, 28, 35, 36};
BitBoard Solver::orderTable[MAXSTEP];

unsigned char Solver::dirMask[MAXSTEP] = {
	193, 193, 241, 241, 241, 241, 112, 112,
	193, 193, 241, 241, 241, 241, 112, 112,
	199, 199, 255, 255, 255, 255, 124, 124,
	199, 199, 255, 255, 255, 255, 124, 124,
	199, 199, 255, 255, 255, 255, 124, 124,
	199, 199, 255, 255, 255, 255, 124, 124,
	  7,   7,  31,  31,  31,  31,  28,  28,
	  7,   7,  31,  31,  31,  31,  28,  28
};

const int Solver::pow3[10] = {
	1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683
};

const int Solver::stage[MAXSTEP + 1] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4,
	5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8,
	9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12,
	13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16
};
const int Solver::rstage[MAXSTEP + 1] = {
	16, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13,
	12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9,
	8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5,
	4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int Solver::patternCount[PATTERNS] = {
	pow3[9] * 3, // line 1 + 2X
	pow3[8],     // line 2
	pow3[8],     // line 3
	pow3[8],     // line 4
	pow3[8],     // diag 8
	pow3[7],     // diag 7
	pow3[6],     // diag 6
	pow3[5],     // diag 5
	pow3[4],     // diag 4
	pow3[9] * 3, // 2x5 square
	pow3[9]      // 3*3 square
};

const int Solver::patternPtr[ACTUAL_PATTERNS] = {
	0,0,0,0,     // line 1 + 2X
	1,1,1,1,     // line 2
	2,2,2,2,     // line 3
	3,3,3,3,     // line 4
	4,4,         // diag 8
	5,5,5,5,     // diag 7
	6,6,6,6,     // diag 6
	7,7,7,7,     // diag 5
	8,8,8,8,     // diag 4
	9,9,9,9,9,9,9,9, // 2x5 square
	10,10,10,10      // 3*3 square
};

/*
0 - 0
fh - fh
fd1 - fd1
fd2 - fd2
fv - fv
fv fh - fv fh
fv fd1 - fv fd2
fv fd2 - fv fd1
*/
const int Solver::antiPattern[SYMMETRICS] = {0, 1, 2, 3, 4, 5, 7, 6};

const Solver::FlipFunction Solver::flipFunction[MAXSTEP] = {
	// A
	Solver::putChess_A1,
	Solver::putChess_A2,
	Solver::putChess_A3,
	Solver::putChess_A4,
	Solver::putChess_A5,
	Solver::putChess_A6,
	Solver::putChess_A7,
	Solver::putChess_A8,
	// B
	Solver::putChess_B1,
	Solver::putChess_B2,
	Solver::putChess_B3,
	Solver::putChess_B4,
	Solver::putChess_B5,
	Solver::putChess_B6,
	Solver::putChess_B7,
	Solver::putChess_B8,
	// C
	Solver::putChess_C1,
	Solver::putChess_C2,
	Solver::putChess_C3,
	Solver::putChess_C4,
	Solver::putChess_C5,
	Solver::putChess_C6,
	Solver::putChess_C7,
	Solver::putChess_C8,
	// D
	Solver::putChess_D1,
	Solver::putChess_D2,
	Solver::putChess_D3,
	Solver::putChess_D4,
	Solver::putChess_D5,
	Solver::putChess_D6,
	Solver::putChess_D7,
	Solver::putChess_D8,
	// E
	Solver::putChess_E1,
	Solver::putChess_E2,
	Solver::putChess_E3,
	Solver::putChess_E4,
	Solver::putChess_E5,
	Solver::putChess_E6,
	Solver::putChess_E7,
	Solver::putChess_E8,
	// F
	Solver::putChess_F1,
	Solver::putChess_F2,
	Solver::putChess_F3,
	Solver::putChess_F4,
	Solver::putChess_F5,
	Solver::putChess_F6,
	Solver::putChess_F7,
	Solver::putChess_F8,
	// G
	Solver::putChess_G1,
	Solver::putChess_G2,
	Solver::putChess_G3,
	Solver::putChess_G4,
	Solver::putChess_G5,
	Solver::putChess_G6,
	Solver::putChess_G7,
	Solver::putChess_G8,
	// H
	Solver::putChess_H1,
	Solver::putChess_H2,
	Solver::putChess_H3,
	Solver::putChess_H4,
	Solver::putChess_H5,
	Solver::putChess_H6,
	Solver::putChess_H7,
	Solver::putChess_H8,
};

const Solver::CountFunction Solver::countFunction[MAXSTEP] = {
	// A
	Solver::countFlips_A1,
	Solver::countFlips_A2,
	Solver::countFlips_A3,
	Solver::countFlips_A4,
	Solver::countFlips_A5,
	Solver::countFlips_A6,
	Solver::countFlips_A7,
	Solver::countFlips_A8,
	// B
	Solver::countFlips_B1,
	Solver::countFlips_B2,
	Solver::countFlips_B3,
	Solver::countFlips_B4,
	Solver::countFlips_B5,
	Solver::countFlips_B6,
	Solver::countFlips_B7,
	Solver::countFlips_B8,
	// C
	Solver::countFlips_C1,
	Solver::countFlips_C2,
	Solver::countFlips_C3,
	Solver::countFlips_C4,
	Solver::countFlips_C5,
	Solver::countFlips_C6,
	Solver::countFlips_C7,
	Solver::countFlips_C8,
	// D
	Solver::countFlips_D1,
	Solver::countFlips_D2,
	Solver::countFlips_D3,
	Solver::countFlips_D4,
	Solver::countFlips_D5,
	Solver::countFlips_D6,
	Solver::countFlips_D7,
	Solver::countFlips_D8,
	// E
	Solver::countFlips_E1,
	Solver::countFlips_E2,
	Solver::countFlips_E3,
	Solver::countFlips_E4,
	Solver::countFlips_E5,
	Solver::countFlips_E6,
	Solver::countFlips_E7,
	Solver::countFlips_E8,
	// F
	Solver::countFlips_F1,
	Solver::countFlips_F2,
	Solver::countFlips_F3,
	Solver::countFlips_F4,
	Solver::countFlips_F5,
	Solver::countFlips_F6,
	Solver::countFlips_F7,
	Solver::countFlips_F8,
	// G
	Solver::countFlips_G1,
	Solver::countFlips_G2,
	Solver::countFlips_G3,
	Solver::countFlips_G4,
	Solver::countFlips_G5,
	Solver::countFlips_G6,
	Solver::countFlips_G7,
	Solver::countFlips_G8,
	// H
	Solver::countFlips_H1,
	Solver::countFlips_H2,
	Solver::countFlips_H3,
	Solver::countFlips_H4,
	Solver::countFlips_H5,
	Solver::countFlips_H6,
	Solver::countFlips_H7,
	Solver::countFlips_H8,
};

// MPC-related
const int Solver::MPC_STAGE[MAXSTEP + 1] = {
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0
};

const float Solver::MPC_PERCENTILE[MPC_STAGES] = {
	1.0f, 1.2f, 1.4f, 1.6f, 2.0f
};

const int Solver::MPC_PERCENTAGE[MPC_STAGES] = {
	68, 77, 84, 89, 95
};

// I don't know why I disabled the deeper cut pairs...
CutPair Solver::mpcThreshold[MPC_STAGES][MAX_MPC_DEPTH + 1][MPC_TRIES] = {
	// Stage 0
	{{CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, 
	{CutPair(3, 7, 0.7434f, 3707.0f, 36490.0f), CutPair()}, 
	{CutPair(4, 8, 0.9207f, -1965.0f, 23537.0f), CutPair()},
	{CutPair(3, 9, 0.7270f, 4416.0f, 37559.0f), CutPair(5, 9, 0.9156f, 2854.0f, 21088.0f)},
	{CutPair(4, 10, 0.8984f, -2451.0f, 23735.0f), CutPair()},
	{CutPair(3, 11, 0.7316f, 4934.0f, 38032.0f), CutPair(5, 11, 0.9162f, 3516.0f, 22834.0f)},
	{CutPair(4, 12, 0.8925f, 2381.0f, 25544.0f), CutPair()},
	{CutPair(3, 13, 0.6279f, 10081.0f, 33732.0f), CutPair(5, 13, 0.9057f, 4132.0f, 24476.0f)},
	{CutPair(4, 14, 0.8180f, -5512.0f, 22885.0f), CutPair()},
	{/*CutPair(3, 15, 0.6215f, 10572.0f, 32982.0f), */CutPair(5, 15, 0.8272f, 6768.0f, 21164.0f)}//,
	//{CutPair(4, 16, 0.8055f, -6491.0f, 23280.0f), CutPair()},
	//{CutPair(5, 17, 0.8005f, 7387.0f, 21339.0f), CutPair()}
	},
	// Stage 1
	{{CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, 
	{CutPair(3, 7, 0.9142f, 1181.0f, 29466.0f), CutPair()}, 
	{CutPair(4, 8, 0.9443f, -2337.0f, 24249.0f), CutPair()},
	{CutPair(3, 9, 0.8990f, 1864.0f, 32164.0f), CutPair(5, 9, 0.9473f, 1437.0f, 23740.0f)},
	{CutPair(4, 10, 0.9329f, -2958.0f, 26848.0f), CutPair()},
	{CutPair(3, 11, 0.8872f, 1901.0f, 33556.0f), CutPair(5, 11, 0.9361f, 1447.0f, 25514.0f)},
	{CutPair(4, 12, 0.9227f, -3300.0f, 28591.0f), CutPair()},
	{CutPair(3, 13, 0.8392f, 3383.0f, 32087.0f), CutPair(5, 13, 0.9274f, 1544.0f, 26990.0f)},
	{CutPair(4, 14, 0.8810f, -4291.0f, 27313.0f), CutPair()},
	{/*CutPair(3, 15, 0.8291f, 3523.0f, 32265.0f), */CutPair(5, 15, 0.8870f, 2723.0f, 25334.0f)}//,
	//{CutPair(4, 16, 0.8705f, -4204.0f, 27780.0f), CutPair()},
	//{CutPair(5, 17, 0.8764f, 2421.0f, 26069.0f), CutPair()}
	},
	// Stage 2
	{{CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, 
	{CutPair(3, 7, 0.9551f, -879.5f, 32486.0f), CutPair()}, 
	{CutPair(4, 8, 0.9743f, -716.6f, 28198.0f), CutPair()},
	{CutPair(3, 9, 0.9453f, -1486.0f, 35880.0f), CutPair(5, 9, 0.9753f, -1098.0f, 26174.0f)},
	{CutPair(4, 10, 0.9702f, -134.1f, 31587.0f), CutPair()},
	{CutPair(3, 11, 0.9488f, -2362.0f, 38708.0f), CutPair(5, 11, 0.9797f, -1991.0f, 29542.0f)},
	{CutPair(4, 12, 0.9675f, 954.1f, 34050.0f), CutPair()},
	{CutPair(3, 13, 0.9156f, -3006.0f, 38443.0f), CutPair(5, 13, 0.9769f, -3098.0f, 32346.0f)},
	{CutPair(4, 14, 0.9422f, 604.4f, 33683.0f), CutPair()},
	{/*CutPair(3, 15, 0.9070f, -4005.0f, 39260.0f), */CutPair(5, 15, 0.9392f, -3495.0f, 30774.0f)}//,
	//{CutPair(4, 16, 0.9359f, 1540.0f, 34598.0f), CutPair()},
	//{CutPair(5, 17, 0.9398f, -4286.0f, 32026.0f), CutPair()}
	},
	// Stage 3
	{{CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, 
	{CutPair(3, 7, 0.9938f, -2936.0f, 36266.0f), CutPair()}, 
	{CutPair(4, 8, 0.9989f, 1112.0f, 33145.0f), CutPair()},
	{CutPair(3, 9, 0.9981f, -3562.0f, 41218.0f), CutPair(5, 9, 1.0075f, -1582.0f, 30901.0f)},
	{CutPair(4, 10, 1.0073f, 1809.0f, 37836.0f), CutPair()},
	{CutPair(3, 11, 1.0036f, -4216.0f, 44989.0f), CutPair(5, 11, 1.0152f, -2232.0f, 35378.0f)},
	{CutPair(4, 12, 1.0169f, 1932.0f, 41496.0f), CutPair()},
	{CutPair(3, 13, 0.9901f, -4450.0f, 44886.0f), CutPair(5, 13, 1.0244f, -2301.0f, 38743.0f)},
	{CutPair(4, 14, 1.0061f, 273.9f, 40618.0f), CutPair()},
	{/*CutPair(3, 15, 0.9986f, -4841.0f, 47072.0f), */CutPair(5, 15, 1.0182f, -2801.0f, 38801.0f)}//,
	//{CutPair(4, 16, 1.0201f, 419.9f, 43187.0f), CutPair()},
	//{CutPair(5, 17, 1.0311f, -3525.0f, 41169.0f), CutPair()}
	},
	// Stage 4
	{{CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, 
	{CutPair(3, 7, 1.0005f, -1196.0f, 42079.0f), CutPair()}, 
	{CutPair(4, 8, 1.0195f, -1008.0f, 39693.0f), CutPair()},
	{CutPair(3, 9, 1.0192f, -878.8f, 49491.0f), CutPair(5, 9, 1.0217f, 809.0f, 38344.0f)},
	{CutPair(4, 10, 1.0352f, -1226.0f, 46889.0f), CutPair()},
	{CutPair(3, 11, 1.0339f, -1402.0f, 55171.0f), CutPair(5, 11, 1.0368f, 241.3f, 45432.0f)},
	{CutPair(4, 12, 1.0566f, -275.4f, 53311.0f), CutPair()},
	{CutPair(3, 13, 1.0408f, -4822.0f, 58924.0f), CutPair(5, 13, 1.0540f, -1628.0f, 52159.0f)},
	{CutPair(4, 14, 1.0572f, -2661.0f, 56809.0f), CutPair()},
	{/*CutPair(3, 15, 1.0512f, -6747.0f, 63197.0f), */CutPair(5, 15, 1.0561f, -5487.0f, 54595.0f)}//,
	//{CutPair(4, 16, 1.0615f, 2704.0f, 59085.0f), CutPair()},
	//{CutPair(5, 17, 1.0684f, -6800.0f, 55566.0f), CutPair()}
	}
};

// EPC-related -- EPC stands for End-game Prob Cut
const float Solver::EPC_PERCENTILE[EPC_STAGES + 1] = {
	0.2, 0.5, 0.75, 1.0, 1.3, 1.7, 2.1, 2.5, 3.5, 100.0
};

const int Solver::EPC_PERCENTAGE[EPC_STAGES + 1] = {
	16, 38, 55, 68, 81, 91, 96, 98, 99, 100
};

CutPair Solver::epcThreshold[MAX_EPC_DEPTH + 1][EPC_TRIES] = {
	{CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, 
	{CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, {CutPair()}, 
	{CutPair(4, 16, 0.00010040f, -0.7693f, 6.3598f), CutPair()},
	{CutPair(5, 17, 0.00010261f, 0.4715f, 5.9291f), CutPair()},
	{CutPair(6, 18, 0.00010189f, -0.6406f, 5.6213f), CutPair()},
	{CutPair(5, 19, 0.00010074f, 0.5541f, 5.7671f), CutPair(7, 19, 0.00010180f, 0.5936f, 5.0730f)},
	{CutPair(6, 20, 0.00010261f, -0.7832f, 5.2595f), CutPair()},
	{CutPair(7, 21, 0.00010199f, 0.7885f, 5.1472f), CutPair()},
	{CutPair(6, 22, 0.00010198f, -0.8967f, 5.0506f), CutPair(8, 22, 0.00010216f, -0.7883f, 4.6082f)},
	{CutPair(7, 23, 0.00010187f, 0.6390f, 4.7432f), CutPair()}//,
	//{CutPair(6, 24, 0.00010609f, -0.6127f, 4.5885f), CutPair(8, 24, 0.00010418f, -0.6412f, 4.1345f)},
	//{CutPair(7, 25, 0.00010444f, 0.4996f, 4.3869f), CutPair()}
	// the newly-added cut pairs may cause some problems - better to disable them
};

// parity heuristic
const int Solver::END_PARITY_WEIGHT[MAXSTEP] = {
	30000, 10000, 30000, 30000, 30000, 30000, 10000, 30000,
	10000, 20000, 30000, 30000, 30000, 30000, 20000, 10000,
	30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000,
	30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000,
	30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000,
	30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000,
	10000, 20000, 30000, 30000, 30000, 30000, 20000, 10000,
	30000, 10000, 30000, 30000, 30000, 30000, 10000, 30000
};

Solver* Solver::newInstance() {
	return new Solver();
}

Solver* Solver::newInstance(int board[]) {
	return new Solver(board);
}

int Solver::bitGet1s(unsigned int num) {
	const unsigned int mask11 = 0x55555555;
	const unsigned int mask12 = 0xaaaaaaaa;
	const unsigned int mask21 = 0x33333333;
	const unsigned int mask22 = 0xcccccccc;
	const unsigned int mask31 = 0x0f0f0f0f;
	const unsigned int mask32 = 0xf0f0f0f0;
	const unsigned int mask41 = 0x00ff00ff;
	const unsigned int mask42 = 0xff00ff00;
	const unsigned int mask51 = 0x0000ffff;
	const unsigned int mask52 = 0xffff0000;
	num = (num & mask11) + ((num & mask12) >> 1);
	num = (num & mask21) + ((num & mask22) >> 2);
	num = (num & mask31) + ((num & mask32) >> 4);
	num = (num & mask41) + ((num & mask42) >> 8);
	num = (num & mask51) + ((num & mask52) >> 16);
	return num;
}

Solver::Solver() {
	static int board[MAXSTEP];
	memset(board, 0, sizeof(board));
	this->Solver::Solver(board);
}

void Solver::initConstants() {
	for (int i = 0; i < 0x10000; i++) {
		bitTable[i] = bitGet1s(i);
	}
	BitBoard current = 0x8000000000000000ull;
	for (int i = 0; i < MAXSTEP; i++) {
		posTable[i] = current;
		current >>= 1;
	}
	memset(lineTable, 0, sizeof(lineTable));
	const unsigned long long mask1 = 0x7e7e7e7e7e7e7e7eull;
	const unsigned long long mask2 = 0x007e7e7e7e7e7e00ull;
	const unsigned long long mask3 = 0x00ffffffffffff00ull;
	static int shift[] = {1, 7, 9, 8};
	static unsigned long long mask[] = {mask1, mask2, mask2, mask3};
	BitBoard currentLine;
	for (int i = 0; i < MAXSTEP; i++) {
		for (int j = 0; j < 4; j++) {
			int current = i + shift[j];
			if (current & 0xffffffc0) continue;
			currentLine = 0;
			while (posTable[current] & mask[j]) {
				currentLine |= posTable[current];
				current += shift[j];
				lineTable[i][current] = currentLine;
				lineTable[current][i] = currentLine;
			}
		}
	}
	for (int i = 0; i < 0x10000; i++) {
		rand_s(&bZobrist[i]);
		rand_s(&wZobrist[i]);
	}
	// fast flip pattern is very sparse, planning to shrink it..
	// from 2^16 down to 3^8 saves a lot of space, also that will increase
	// cache hit rate
	const int tmpPosTable[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
	for (int b1 = 0; b1 < 256; b1++) {
		for (int b2 = 0; b2 < 256; b2++)
			if ((b1 & b2) == 0)
				for (int pos = 0; pos < 8; pos++) {
					if ((tmpPosTable[pos] & b1) || (tmpPosTable[pos] & b2)) continue;
					int placement = b1 | (b2 << 8) | (pos << 16);
					int delta = -1;
					while ((pos + delta >= 0) && (tmpPosTable[pos + delta] & b2)) delta--;
					if ((pos + delta >= 0) && (tmpPosTable[pos + delta] & b1))
						fastFlipPattern[placement][0] = delta;
					else fastFlipPattern[placement][0] = 0;
					delta = 1;
					while ((pos + delta < 8) && (tmpPosTable[pos + delta] & b2)) delta++;
					if ((pos + delta < 8) && (tmpPosTable[pos + delta] & b1))
						fastFlipPattern[placement][1] = delta;
					else fastFlipPattern[placement][1] = 0;
				}
		for (int pos = 0; pos < 8; pos++) {
			if (tmpPosTable[pos] & b1) continue;
			int b2 = (~(b1 | tmpPosTable[pos])) & 0xff;
			int count = 0;
			int placement = b1 | (b2 << 8) | (pos << 16);
			if (fastFlipPattern[placement][0] < -1)
				count += -fastFlipPattern[placement][0] - 1;
			if (fastFlipPattern[placement][1] > 1)
				count += fastFlipPattern[placement][1] - 1;
			fastCountPattern[(pos << 8) | b1] = count;
		}
	}
	for (int i = 0; i < MAXSTEP; i++) {
		BitBoard neighbor = 0;
		if (dirMask[i] & DIR1) neighbor |= posTable[i + 1];
		if (dirMask[i] & DIR2) neighbor |= posTable[i - 7];
		if (dirMask[i] & DIR3) neighbor |= posTable[i - 8];
		if (dirMask[i] & DIR4) neighbor |= posTable[i - 9];
		if (dirMask[i] & DIR5) neighbor |= posTable[i - 1];
		if (dirMask[i] & DIR6) neighbor |= posTable[i + 7];
		if (dirMask[i] & DIR7) neighbor |= posTable[i + 8];
		if (dirMask[i] & DIR8) neighbor |= posTable[i + 9];
		neighborhood[i] = neighbor;
	}
#ifdef STABILITY
	for (int i = 0; i < 256; i++) {
		int value = 0, pow = 0;
		int j = i;
		while (j) {
			if (j & 1) value += pow3[pow];
			j >>= 1; pow++;
		}
		twoTo3Base[i] = value;
	}
#if 0
	//calculate stable discs using a tiny search -- could be replaced by pre-calculated data
	// but search must be done for the first time :-)
	stab_my[0] = 0; stab_op[0] = 0;
	for (unsigned int b1 = 0; b1 < 256; b1++)
		for (unsigned int b2 = b1 + 1; b2 < 256; b2++)
			if ((b1 & b2) == 0) {
				unsigned int stable_my = b1, stable_op = b2;
				if (stable_my || stable_op)
					lineSearch(b1, b2, stable_my, stable_op);
				int index = twoTo3Base[b1] + (twoTo3Base[b2] << 1);
				stab_my[index] = stable_my;
				stab_op[index] = stable_op;
				// same for the reversed position
				index = twoTo3Base[b2] + (twoTo3Base[b1] << 1);
				stab_my[index] = stable_op;
				stab_op[index] = stable_my;
			}
	std::ofstream out("stability.txt");
	out << "unsigned int Solver::stab_my[6561] = {" << std::endl;
	out << "\t";
	for (int i = 0; i < 6560; i++) {
		out << stab_my[i] << ", ";
		if ((i + 1) % 20 == 0) out << std::endl << "\t";
	}
	out << stab_my[6560] << std::endl << "};" << std::endl;
	out << std::endl;
	out << "unsigned int Solver::stab_op[6561] = {" << std::endl;
	out << "\t";
	for (int i = 0; i < 6560; i++) {
		out << stab_op[i] << ", ";
		if ((i + 1) % 20 == 0) out << std::endl << "\t";
	}
	out << stab_op[6560] << std::endl << "};" << std::endl;
	out.close();
#endif // #if 0

#endif // STABILITY
}

#ifdef STABILITY
void Solver::lineSearch(unsigned int my, unsigned int op, unsigned int& stable_my, unsigned int& stable_op) {
	const unsigned int tmpPosTable[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
	if ((my | op) == 255)
		return;
	unsigned int empti = ~(my | op);
	for (int pos = 0; pos < 8; pos++) {
		if ((tmpPosTable[pos] & empti) == 0) continue;
		int work_my = my, work_op = op;
		int placement = my | (op << 8) | (pos << 16);
		int flips = fastFlipPattern[placement][0];
		for (int i = pos - 1; i > pos + flips; i--) {
			work_my |= tmpPosTable[i];
			work_op &= ~tmpPosTable[i];
		}
		flips = fastFlipPattern[placement][1];
		for (int i = pos + 1; i < pos + flips; i++) {
			work_my |= tmpPosTable[i];
			work_op &= ~tmpPosTable[i];
		}
		work_my |= tmpPosTable[pos];
		stable_my &= work_my;
		stable_op &= work_op;
		if (stable_my || stable_op) {
			lineSearch(work_my, work_op, stable_my, stable_op);
			if (!(stable_my || stable_op)) return;
		} else return;
	}
	for (int pos = 0; pos < 8; pos++) {
		if ((tmpPosTable[pos] & empti) == 0) continue;
		int work_my = my, work_op = op;
		int placement = op | (my << 8) | (pos << 16);
		int flips = fastFlipPattern[placement][0];
		for (int i = pos - 1; i > pos + flips; i--) {
			work_my &= ~tmpPosTable[i];
			work_op |= tmpPosTable[i];
		}
		flips = fastFlipPattern[placement][1];
		for (int i = pos + 1; i < pos + flips; i++) {
			work_my &= ~tmpPosTable[i];
			work_op |= tmpPosTable[i];
		}
		work_op |= tmpPosTable[pos];
		stable_my &= work_my;
		stable_op &= work_op;
		if (stable_my || stable_op) {
			lineSearch(work_my, work_op, stable_my, stable_op);
			if (!(stable_my || stable_op)) return;
		} else return;
	}
}
#endif

void Solver::initTable() {
	tp = new TPEntry[TABLE_LEN];
	currentTableSize = TABLE_LEN;
	currentTableMask = TABLE_MASK;
	clearCache();
	for (int i = 0; i < MAXSTEP; i++)
		orderTable[i] = posTable[moveOrder[i]];
	//patternValues = 0;
}

bool Solver::initBook(std::string bookPath) {
	extendingBook = false;
	isBookChanged = false;
	setBookDepth(DEFAULT_BOOK_DEPTH);
	setBookEndDepth(DEFAULT_BOOK_END_DEPTH);
	if (!loadBook(bookPath)) return false;
	Solver::bookPath = bookPath;
	staticSolver = new Solver();
	if (book->getSize() == 0)
		setDefaultNode();
	fatherAdded = false;
#ifdef REEVALUATE
	addFather(book->get(defaultMy, defaultOp));
	book->reevaluate();
#endif
	return true;
}

bool Solver::initialize() {
	return initialize(DEFAULT_PATTERN_PATH, DEFAULT_BOOK_PATH);
}

int Solver::getInitPart() {
	return initPart;
}

int Solver::getInitPercent() {
	return initPercent;
}

bool Solver::initialize(std::string patternPath, std::string bookPath) {
	std::locale::global(std::locale("")); // solving the 'path with chinese chars' problem

	initPercent = 10;

	initPart = 1;
	initConstants();
	initPercent = 25;

	initPart = 2;
	initTable();
	initPercent = 50;

	initPart = 3;
	bool result = initPatterns(patternPath);
	if (!result) return false;
	initPercent = 75;

	initPart = 4;
	result = initBook(bookPath);
	if (!result) return false;
	initPercent = 100;
	
	initPart = 5;
	return result;
}

void Solver::cleanup() {
	delete[] tp;
	delete[] patternValues;
	delete book;
	delete staticSolver;
	tp = NULL;
	patternValues = NULL;
	book = NULL;
	staticSolver = NULL;
}

Solver::Solver(int board[MAXSTEP]) {
	setBoard(board);
	evnum = 0;
	percent = subPercent = currentBlock = 0;
	tolerance = 0;
	analyzeResult = NULL;
	aborted = false;
	sortStackPtr = 0;
	pvLength = 0;
	selectedMove = focusedMove = -1;
	partialDepth = 0;
	partialResult = 0;

	// for evaluate_diff
	for (int i = 0; i < 2; i++) {
		memset(diff_set[i].pattern, 0, sizeof(diff_set[i].pattern));
		diff_set[i].diff_my_last = 0;
		diff_set[i].diff_op_last = 0;
		diff_set[i].diff_empty_last = ~(diff_set[i].diff_my_last | diff_set[i].diff_op_last);
	}
}

void Solver::setBookTolerance(int tolerance) {
	this->tolerance = tolerance;
}

void Solver::setBoard(int board[MAXSTEP]) {
	black = 0; white = 0;
	stackptr = 0;
	maxEval = -INFINITE;
	for (int i = 0; i < MAXSTEP; i++) {
		black <<= 1; white <<= 1;
		if (board[i] == BLACK)
			black |= 1;
		else if (board[i] == WHITE)
			white |= 1;
	}
	empties = MAXSTEP - bits(black) - bits(white);
}

void Solver::setBitBoard(BitBoard black, BitBoard white) {
	this->black = black;
	this->white = white;
	stackptr = 0;
	maxEval = 0; // reduce the affect of 1st book node eval
	empties = MAXSTEP - bits(black) - bits(white);
}

inline unsigned int Solver::getZobKey() const {
	return getZobKey(black, white);
}

inline unsigned int Solver::getZobKey(const BitBoard& my, const BitBoard& op){
	unsigned short* rmy = (unsigned short*)&my;
	unsigned short* rop = (unsigned short*)&op;
	return bZobrist[*rmy] ^ bZobrist[*(rmy + 1)] 
		^ bZobrist[*(rmy + 2)] ^ bZobrist[*(rmy + 3)]
		^ wZobrist[*rop] ^ wZobrist[*(rop + 1)] 
		^ wZobrist[*(rop + 2)] ^ wZobrist[*(rop + 3)];
}

inline int Solver::bits(const BitBoard& bb) {
#ifdef MACHINE_X64
	const unsigned long long c55 = 0x5555555555555555ull;
	const unsigned long long c33 = 0x3333333333333333ull;
	const unsigned long long c0f = 0x0f0f0f0f0f0f0f0full;
	const unsigned long long h01 = 0x0101010101010101ull;
	BitBoard x = bb;
	x -= (x >> 1) & c55;
	x = (x & c33) + ((x >> 2) & c33);
	x = (x + (x >> 4)) & c0f;
	return (x * h01) >> 56;
#else // it seems that table-based popcount performs much better on x86 arch
	unsigned short* rbb = (unsigned short*)&bb;
	return bitTable[*rbb] + bitTable[*(rbb + 1)] + bitTable[*(rbb + 2)] + bitTable[*(rbb + 3)];
#endif
}

// count the mobility of b1 over b2
BitBoard Solver::mobility(const BitBoard& b1, const BitBoard& b2) {
#ifdef MACHINE_X64
	//01111110
	//01111110
	//01111110
	//01111110
	//01111110
	//01111110
	//01111110
	//01111110
	//for directions 1 and 5
	const unsigned long long mask1 = 0x7e7e7e7e7e7e7e7eull;
	//00000000
	//01111110
	//01111110
	//01111110
	//01111110
	//01111110
	//01111110
	//00000000
	//for directions 2,4,6,8
	const unsigned long long mask2 = 0x007e7e7e7e7e7e00ull;
	//00000000
	//11111111
	//11111111
	//11111111
	//11111111
	//11111111
	//11111111
	//00000000
	//for directions 3 and 7
	const unsigned long long mask3 = 0x00ffffffffffff00ull;
	register BitBoard b3 = b1;
	register BitBoard b4 = b2;
	register BitBoard result = 0;

	register BitBoard b2m = b4 & mask1;
	register BitBoard m = ((b2m << 1) & b3) >> 2;
	result |= m; m &= b2m; m >>= 1;
	result |= m; m &= b2m; m >>= 1;
	result |= m; m &= b2m; m >>= 1;
	result |= m; m &= b2m; m >>= 1;
	result |= m; m &= b2m; m >>= 1;
	result |= m;
	m = ((b2m >> 1) & b3) << 2;
	result |= m; m &= b2m; m <<= 1;
	result |= m; m &= b2m; m <<= 1;
	result |= m; m &= b2m; m <<= 1;
	result |= m; m &= b2m; m <<= 1;
	result |= m; m &= b2m; m <<= 1;
	result |= m;

	b2m = b4 & mask2;
	m = ((b2m >> 7) & b3) << 14;
	result |= m; m &= b2m; m <<= 7;
	result |= m; m &= b2m; m <<= 7;
	result |= m; m &= b2m; m <<= 7;
	result |= m; m &= b2m; m <<= 7;
	result |= m; m &= b2m; m <<= 7;
	result |= m;
	m = ((b2m >> 9) & b3) << 18;
	result |= m; m &= b2m; m <<= 9;
	result |= m; m &= b2m; m <<= 9;
	result |= m; m &= b2m; m <<= 9;
	result |= m; m &= b2m; m <<= 9;
	result |= m; m &= b2m; m <<= 9;
	result |= m;
	m = ((b2m << 7) & b3) >> 14;
	result |= m; m &= b2m; m >>= 7;
	result |= m; m &= b2m; m >>= 7;
	result |= m; m &= b2m; m >>= 7;
	result |= m; m &= b2m; m >>= 7;
	result |= m; m &= b2m; m >>= 7;
	result |= m;
	m = ((b2m << 9) & b3) >> 18;
	result |= m; m &= b2m; m >>= 9;
	result |= m; m &= b2m; m >>= 9;
	result |= m; m &= b2m; m >>= 9;
	result |= m; m &= b2m; m >>= 9;
	result |= m; m &= b2m; m >>= 9;
	result |= m;

	b2m = b4 & mask3;
	m = ((b2m >> 8) & b3) << 16;
	result |= m; m &= b2m; m <<= 8;
	result |= m; m &= b2m; m <<= 8;
	result |= m; m &= b2m; m <<= 8;
	result |= m; m &= b2m; m <<= 8;
	result |= m; m &= b2m; m <<= 8;
	result |= m;
	m = ((b2m << 8) & b3) >> 16;
	result |= m; m &= b2m; m >>= 8;
	result |= m; m &= b2m; m >>= 8;
	result |= m; m &= b2m; m >>= 8;
	result |= m; m &= b2m; m >>= 8;
	result |= m; m &= b2m; m >>= 8;
	result |= m;

	return result & ~(b3 | b4);
#else
	/* 
	The following mobility code was originally written by Gunnar Andersson.
	The code is equivalent to the code above which was written by myself,
	and of course, Gunnar's code is faster. 
	I used it without too much modification. If you would like to use the code,
	you must also copy the following copyright note.
	*/

	/*
	Copyright (c) 2003, Gunnar Andersson
	All rights reserved.

	Redistribution and use in source and binary forms, with or
	without modification, are permitted provided that the
	following conditions are met:

	* Redistributions of source code must retain the above copyright notice,
	  this list of conditions and the following disclaimer. 

	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution. 

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
	THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
	PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
	BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
	THE POSSIBILITY OF SUCH DAMAGE.
	*/

    const BitBoard dir_mask0 = 0x007e7e7e7e7e7e00;
	const BitBoard dir_mask1 = 0x00ffffffffffff00;
	const BitBoard dir_mask2 = 0x007e7e7e7e7e7e00;
	const BitBoard dir_mask3 = 0x7e7e7e7e7e7e7e7e;
	const BitBoard dir_mask4 = 0x7e7e7e7e7e7e7e7e;
	const BitBoard dir_mask5 = 0x007e7e7e7e7e7e00;
	const BitBoard dir_mask6 = 0x00ffffffffffff00;
	const BitBoard dir_mask7 = 0x007e7e7e7e7e7e00;
	const BitBoard c0f = 0x0f0f0f0f0f0f0f0f;
	const BitBoard c33 = 0x3333333333333333;
	const BitBoard c55 = 0x5555555555555555;
	const AsmBitBoard my_bits = *reinterpret_cast<const AsmBitBoard*>(&b1);
	const AsmBitBoard opp_bits = *reinterpret_cast<const AsmBitBoard*>(&b2);
	BitBoard result;

    __asm {
        /* Ready for init data */
        mov     ebx, my_bits.low        ;
        mov     ecx, my_bits.high       ;
        mov     edi, opp_bits.low       ;
        mov     esi, opp_bits.high      ;
        //
        movd    mm0, ebx                ;
        psllq   mm0, 32                 ;
        movd    mm3, ecx                ;
        por     mm0, mm3                ; mm0 is BitBoard of my_bits
        movd    mm1, edi                ;
        psllq   mm1, 32                 ;
        movd    mm4, esi                ;
        por     mm1, mm4                ; mm1 is BitBoard of opp_bits
        pxor    mm2, mm2                ; mm2 <- 0x0000000000000000
        /* shift=-9   rowDelta=-1   colDelta=-1 */
        /* shift=+9   rowDelta=+1   colDelta=+1 */
        /* Disc #1, flip direction 0. */
        /* Disc #1, flip direction 7. */
        movq    mm3, mm1                ; mm3 <- opp_bits
        movq    mm4, mm0                ; mm4 <- my_bits
        movq    mm6, mm0                ; mm6 <- backup of my_bits
        pand    mm3, dir_mask0          ; 0x007e7e7e7e7e7e00
                                        ; dir_mask0 of value:
                                        ;   00000000
                                        ;   01111110
                                        ;   01111110
                                        ;   01111110
                                        ;   01111110
                                        ;   01111110
                                        ;   01111110
                                        ;   00000000
        push    esi                     ;
        psllq   mm4, 9                  ;
        psrlq   mm6, 9                  ;
        push    edi                     ;
        pand    mm4, mm3                ;
        pand    mm6, mm3                ;
        push    ecx                     ;
        /* Disc #2, flip direction 0. */
        /* Disc #2, flip direction 7. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        psllq   mm5, 9                  ;
        psrlq   mm7, 9                  ;
        push    ebx                     ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        and     edi, 0x7e7e7e7e         ; 0x7e7e7e7e
        and     esi, 0x7e7e7e7e         ; 0x7e7e7e7e
                                        ; value of:
                                        ; 011111110
                                        ; 011111110
                                        ; 011111110
                                        ; 011111110
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        shl     ebx, 1                  ;
        shl     ecx, 1                  ;
        /* Disc #3, flip direction 0. */
        /* Disc #3, flip direction 7. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        and     ebx, edi                ;
        and     ecx, esi                ;
        psllq   mm5, 9                  ;
        psrlq   mm7, 9                  ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        shl     edx, 1                  ;
        shl     eax, 1                  ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        and     eax, edi                ;
        and     edx, esi                ;
        /* Disc #4, flip direction 0. */
        /* Disc #4, flip direction 7. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        psllq   mm5, 9                  ;
        psrlq   mm7, 9                  ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        shl     edx, 1                  ;
        shl     eax, 1                  ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        and     eax, edi                ;
        and     edx, esi                ;
        /* Disc #5, flip direction 0. */
        /* Disc #5, flip direction 7. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        psllq   mm5, 9                  ;
        psrlq   mm7, 9                  ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        shl     edx, 1                  ;
        shl     eax, 1                  ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        and     eax, edi                ;
        and     edx, esi                ;
        /* Disc #6, flip direction 0. */
        /* Disc #6, flip direction 7. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        psrlq   mm7, 9                  ;
        psllq   mm5, 9                  ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        shl     edx, 1                  ;
        shl     eax, 1                  ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        and     eax, edi                ;
        and     edx, esi                ;
        psllq   mm4, 9                  ;
        psrlq   mm6, 9                  ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        por     mm2, mm4                ;
        por     mm2, mm6                ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        /* shift=-8   rowDelta=-1   colDelta=0 */
        /* shift=+8   rowDelta=1   colDelta=0 */
        /* Disc #1, flip direction 1. */
        /* Disc #1, flip direction 6. */
        movq    mm3, mm1                ;
        movq    mm4, mm0                ;
        movq    mm6, mm0                ;
        pand    mm3, dir_mask1          ; 0x00ffffffffffff00;
                                        ; dir_mask1 of value:
                                        ;   00000000
                                        ;   11111111
                                        ;   11111111
                                        ;   11111111
                                        ;   11111111
                                        ;   11111111
                                        ;   11111111
                                        ;   00000000
        psllq   mm4, 8                  ;
        psrlq   mm6, 8                  ;
        shl     edx, 1                  ;
        shl     eax, 1                  ;
        pand    mm4, mm3                ;
        pand    mm6, mm3                ;
        and     eax, edi                ;
        and     edx, esi                ;
        /* Disc #2, flip direction 1. */
        /* Disc #2, flip direction 6. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        psllq   mm5, 8                  ;
        psrlq   mm7, 8                  ;
        shl     ebx, 1                  ;
        shl     ecx, 1                  ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        /* serialize here: add horizontal shl flips. */
        movd    mm5, ebx                ;
        psllq   mm5, 32                 ;
        movd    mm7, ecx                ;
        por     mm5, mm7                ;
        por     mm2, mm5                ;
        /* Disc #3, flip direction 1. */
        /* Disc #3, flip direction 6. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        psllq   mm5, 8                  ;
        psrlq   mm7, 8                  ;
        pop     ebx                     ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        pop     ecx                     ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        push    ecx                     ;
        /* Disc #4, flip direction 1. */
        /* Disc #4, flip direction 6. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        push    ebx                     ;
        psllq   mm5, 8                  ;
        psrlq   mm7, 8                  ;
        shr     ebx, 1                  ;
        shr     ecx, 1                  ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        and     ebx, edi                ;
        and     ecx, esi                ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        /* Disc #5, flip direction 1. */
        /* Disc #5, flip direction 6. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        psllq   mm5, 8                  ;
        psrlq   mm7, 8                  ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        shr     eax, 1                  ;
        shr     edx, 1                  ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        and     eax, edi                ;
        and     edx, esi                ;
        /* Disc #6, flip direction 1. */
        /* Disc #6, flip direction 6. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        psllq   mm5, 8                  ;
        psrlq   mm7, 8                  ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        shr     eax, 1                  ;
        shr     edx, 1                  ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        and     eax, edi                ;
        and     edx, esi                ;
        psllq   mm4, 8                  ;
        psrlq   mm6, 8                  ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        por     mm2, mm4                ;
        por     mm2, mm6                ;
        /* shift=-7   rowDelta=-1   colDelta=1 */
        /* shift=+7   rowDelta=1   colDelta=-1 */
        /* Disc #1, flip direction 2. */
        /* Disc #1, flip direction 5. */
        movq    mm3, mm1                ;
        movq    mm4, mm0                ;
        movq    mm6, mm0                ;
        pand    mm3, dir_mask2          ; 0x007e7e7e7e7e7e00;
                                        ; dir_mask2 of value:
                                        ;   00000000
                                        ;   01111110
                                        ;   01111110
                                        ;   01111110
                                        ;   01111110
                                        ;   01111110
                                        ;   01111110
                                        ;   00000000
        psllq   mm4, 7                  ;
        psrlq   mm6, 7                  ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        pand    mm4, mm3                ;
        pand    mm6, mm3                ;
        shr     eax, 1                  ;
        shr     edx, 1                  ;
        /* Disc #2, flip direction 2. */
        /* Disc #2, flip direction 5. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        and     eax, edi                ;
        and     edx, esi                ;
        psllq   mm5, 7                  ;
        psrlq   mm7, 7                  ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        shr     eax, 1                  ;
        shr     edx, 1                  ;
        /* Disc #3, flip direction 2. */
        /* Disc #3, flip direction 5. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        and     eax, edi                ;
        and     edx, esi                ;
        psllq   mm5, 7                  ;
        psrlq   mm7, 7                  ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        shr     eax, 1                  ;
        shr     edx, 1                  ;
        /* Disc #4, flip direction 2. */
        /* Disc #4, flip direction 5. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        and     eax, edi                ;
        and     edx, esi                ;
        psllq   mm5, 7                  ;
        psrlq   mm7, 7                  ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        mov     eax, ebx                ;
        mov     edx, ecx                ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        shr     eax, 1                  ;
        shr     edx, 1                  ;
        /* Disc #5, flip direction 2. */
        /* Disc #5, flip direction 5. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        and     eax, edi                ;
        and     edx, esi                ;
        psllq   mm5, 7                  ;
        psrlq   mm7, 7                  ;
        or      ebx, eax                ;
        or      ecx, edx                ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        shr     ebx, 1                  ;
        shr     ecx, 1                  ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        /* serialize here: add horizontal shr flips. */
        movd    mm5, ebx                ;
        psllq   mm5, 32                 ;
        movd    mm7, ecx                ;
        por     mm5, mm7                ;
        por     mm2, mm5                ;
        pop     ebx                     ;
        /* Disc #6, flip direction 2. */
        /* Disc #6, flip direction 5. */
        movq    mm5, mm4                ;
        movq    mm7, mm6                ;
        psllq   mm5, 7                  ;
        psrlq   mm7, 7                  ;
        pop     ecx                     ;
        pand    mm5, mm3                ;
        pand    mm7, mm3                ;
        pop     edi                     ;
        por     mm4, mm5                ;
        por     mm6, mm7                ;
        pop     esi                     ;
        psllq   mm4, 7                  ;
        psrlq   mm6, 7                  ;
        por     mm2, mm4                ;
        por     mm2, mm6                ;
        /* mm2 is the pseudo-feasible moves at this point. */
        /* Let mm7 be the feasible moves, i.e., mm2 restricted to empty squares. */
        movq    mm7, mm0                ;
        por     mm7, mm1                ;
        pandn   mm7, mm2                ;
		movq    result, mm7             ;
        //
        emms                            ;
	}
	return result;
#endif
}

inline void Solver::putChess(int pos, BitBoard& b1, BitBoard& b2) {
	(*flipFunction[pos])(b1, b2);
}

inline bool Solver::checkedPutChess(int pos, BitBoard& b1, BitBoard& b2) {
	return (*flipFunction[pos])(b1, b2);
}

inline void Solver::makeMove(int pos, BitBoard& my, BitBoard& op) {
	bstack[stackptr] = black;
	wstack[stackptr] = white;
	stackptr++;
	putChess(pos, my, op);
	empties--;
}

inline bool Solver::checkedMakeMove(int pos, BitBoard& my, BitBoard& op) {
	if (!(op & neighborhood[pos])) return false;
	bstack[stackptr] = black;
	wstack[stackptr] = white;
	bool result = checkedPutChess(pos, my, op);
	if (result) {
		empties--;
		stackptr++;
	}
	return result;
}

inline void Solver::makeMove(int pos, int color) {
	if (color == BLACK)
		makeMove(pos, black, white);
	else makeMove(pos, white, black);
}

inline void Solver::unMakeMove() {
	stackptr--;
	black = bstack[stackptr];
	white = wstack[stackptr];
	empties++;
}

int Solver::getMobility(int color) const {
	if (color == BLACK)
		return bits(mobility(black, white));
	else return bits(mobility(white, black));
}

inline int Solver::getResult(const BitBoard& my) {
	int diff = (bits(my) << 1) + empties - MAXSTEP;
	if (diff > 0)
		diff += empties;
	else if (diff < 0)
		diff -= empties;
	evnum++;
	return diff;
}

inline int Solver::getResultNoEmpty(const BitBoard& my) {
	evnum++;
	return (bits(my) << 1) - MAXSTEP;
}

inline int Solver::getResultNoCount(const BitBoard& my) {
	int diff = (bits(my) << 1) + empties - MAXSTEP;
	if (diff > 0)
		diff += empties;
	else if (diff < 0)
		diff -= empties;
	return diff;
}

inline int Solver::getResultNoEmptyNoCount(const BitBoard& my) {
	return (bits(my) << 1) - MAXSTEP;
}

int Solver::strongEvaluate(BitBoard& my, BitBoard& op) {
	BitBoard mob = mobility(my, op);
	int result;
	if (mob == 0)
		if (mobility(op, my) == 0) {
			return endToMid(getResult(my));
		} else {
			return -evaluate(op, my);
		}
	int alpha = -INFINITE, beta = INFINITE, eval = evaluate(my, op);
	for (int i = 0; i < MAXSTEP; i++)
		if (posTable[i] & mob) {
			makeMove(i, my, op);
			if (mobility(op, my) == 0 && mobility(my, op) == 0) { // an immediate end
				result = endToMid(getResult(my));
			} else 
				result = eval;
			unMakeMove();
			if (result >= beta) {
				return beta;
			}
			if (result > alpha)
				alpha = result;
		}
	return alpha;
}

inline int Solver::evaluate(const BitBoard& my, const BitBoard& op) {
	return evaluate_diff(my, op);
}

int Solver::evaluate_diff(const BitBoard& my, const BitBoard& op) {
	if (my && op) { // wipeout detection
		DiffSet& set = diff_set[empties & 1];
		BitBoard plusOne = (my & set.diff_empty_last) | (op & set.diff_my_last); // 0 -> 1 && 1 -> 2
		BitBoard plusTwo = (op & set.diff_empty_last); // 0 -> 2
		BitBoard empty = ~(my | op);
		BitBoard minusOne = (my & set.diff_op_last) | (empty & set.diff_my_last); // 2 -> 1 && 1 -> 0
		BitBoard minusTwo = (empty & set.diff_op_last); // 2 -> 0

		set.diff_my_last = my;
		set.diff_op_last = op;
		set.diff_empty_last = empty;

		int* pattern = set.pattern;

		unsigned char* ucmy = (unsigned char*)&plusOne;
		unsigned char* ucop = (unsigned char*)&plusTwo;
		unsigned char* ucrmy = (unsigned char*)&minusOne;
		unsigned char* ucrop = (unsigned char*)&minusTwo;

		int stage = rstage[empties];
		int mybyte, opbyte, pCount, pCountM1;
		for (int i = 0; i < 8; i++) {
			mybyte = ucmy[i];
			if (mybyte) {
				pCount = pVCount[mybyte][i];
				pCountM1 = pCount - 1;
				unsigned char* pPtrMybyte = pPtr[mybyte][i];
				unsigned short* myPVMybyte = myPV[mybyte][i];
				for (int j = 0; j < pCountM1; j += 2) {
					pattern[pPtrMybyte[j]] += myPVMybyte[j];
					pattern[pPtrMybyte[j + 1]] += myPVMybyte[j + 1];
				}
				if (pCount & 1) {
					pattern[pPtrMybyte[pCountM1]] += myPVMybyte[pCountM1];
				}
			}
			opbyte = ucop[i];
			if (opbyte) {
				pCount = pVCount[opbyte][i];
				pCountM1 = pCount - 1;
				unsigned char* pPtrOpbyte = pPtr[opbyte][i];
				unsigned short* opPVOpbyte = myPV[opbyte][i];
				for (int j = 0; j < pCountM1; j += 2) {
					pattern[pPtrOpbyte[j]] += (opPVOpbyte[j] << 1);
					pattern[pPtrOpbyte[j + 1]] += (opPVOpbyte[j + 1] << 1);
				}
				if (pCount & 1) {
					pattern[pPtrOpbyte[pCountM1]] += (opPVOpbyte[pCountM1] << 1);
				}
			}

			// rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
			mybyte = ucrmy[i];
			if (mybyte) {
				pCount = pVCount[mybyte][i];
				pCountM1 = pCount - 1;
				unsigned char* pPtrMybyte = pPtr[mybyte][i];
				unsigned short* myPVMybyte = myPV[mybyte][i];
				for (int j = 0; j < pCountM1; j += 2) {
					pattern[pPtrMybyte[j]] -= myPVMybyte[j];
					pattern[pPtrMybyte[j + 1]] -= myPVMybyte[j + 1];
				}
				if (pCount & 1) {
					pattern[pPtrMybyte[pCountM1]] -= myPVMybyte[pCountM1];
				}
			}
			opbyte = ucrop[i];
			if (opbyte) {
				pCount = pVCount[opbyte][i];
				pCountM1 = pCount - 1;
				unsigned char* pPtrOpbyte = pPtr[opbyte][i];
				unsigned short* opPVOpbyte = myPV[opbyte][i];
				for (int j = 0; j < pCountM1; j += 2) {
					pattern[pPtrOpbyte[j]] -= (opPVOpbyte[j] << 1);
					pattern[pPtrOpbyte[j + 1]] -= (opPVOpbyte[j + 1] << 1);
				}
				if (pCount & 1) {
					pattern[pPtrOpbyte[pCountM1]] -= (opPVOpbyte[pCountM1] << 1);
				}
			}
		}
		int sum = 0;
		int *stageOffset = patternOffset[stage];
		for (int i = 0; i < ACTUAL_PATTERNS; i += 2) {
			sum += patternValues[pattern[i] + stageOffset[i]]
				+ patternValues[pattern[i + 1] + stageOffset[i + 1]];
		}
		if (empties & 1) sum += patternCorrection[stage];

		evnum++;
		return sum;
	} else {
		return endToMid(getResult(my));
	}
}

int Solver::evaluate_direct(const BitBoard& my, const BitBoard& op) {
	if (my && op) { // wipeout detection
		int pattern[ACTUAL_PATTERNS];
		unsigned char* ucmy = (unsigned char*)&my;
		unsigned char* ucop = (unsigned char*)&op;
		int stage = rstage[empties];
		memcpy(pattern, patternOffset[stage], sizeof(pattern));
		int mybyte, opbyte, pCount, pCountM1;
		for (int i = 0; i < 8; i++) {
			mybyte = ucmy[i];
			pCount = pVCount[mybyte][i];
			pCountM1 = pCount - 1;
			unsigned char* pPtrMybyte = pPtr[mybyte][i];
			unsigned short* myPVMybyte = myPV[mybyte][i];
			for (int j = 0; j < pCountM1; j += 2) {
				pattern[pPtrMybyte[j]] += myPVMybyte[j];
				pattern[pPtrMybyte[j + 1]] += myPVMybyte[j + 1];
			}
			if (pCount & 1) {
				pattern[pPtrMybyte[pCountM1]] += myPVMybyte[pCountM1];
			}
			opbyte = ucop[i];
			pCount = pVCount[opbyte][i];
			pCountM1 = pCount - 1;
			unsigned char* pPtrOpbyte = pPtr[opbyte][i];
			unsigned short* opPVOpbyte = myPV[opbyte][i];
			for (int j = 0; j < pCountM1; j += 2) {
				pattern[pPtrOpbyte[j]] += (opPVOpbyte[j] << 1);
				pattern[pPtrOpbyte[j + 1]] += (opPVOpbyte[j + 1] << 1);
			}
			if (pCount & 1) {
				pattern[pPtrOpbyte[pCountM1]] += (opPVOpbyte[pCountM1] << 1);
			}
		}
		int sum = 0;
		for (int i = 0; i < ACTUAL_PATTERNS; i += 2) {
			sum += patternValues[pattern[i]] + patternValues[pattern[i + 1]];
		}
		if (empties & 1) sum += patternCorrection[stage];
		evnum++;
		return sum;
	} else {
		return endToMid(getResult(my));
	}
}

inline int Solver::getEndSortSearchDepth() const {
	// return 1;
#if 1
	if (empties <= 13) return 1;
	if (empties <= 19) return 2 + (empties & 1);
	if (empties <= 25) return 4 + (empties & 1);
	return 6 + (empties & 1);
#else
	if (empties <= 15) return 1;
	if (empties <= 19) return 2 + (empties & 1);
	if (empties <= 25) return 4 + (empties & 1);
	return 6 + (empties & 1);
#endif
}

inline int Solver::getEndSortSearchLowerBound(int alpha) {
	return (alpha - 8) * RULER;
}

inline int Solver::getEndSortSearchLowerBound1(int alpha) {
	return -INFINITE + MAXSTEP + ((alpha >= 0) ? 0 : alpha);
}

inline int Solver::getEndSortSearchUpperBound(int beta) {
	return INFINITE - MAXSTEP + ((beta <= 0) ? 0 : beta);
}

int Solver::searchExact(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound) {
#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif
	int sortSearchDepth = getEndSortSearchDepth();
	int tabledepth = empties + EPC_STAGES;
	TPInfo* infoNewer;
	TPInfo* infoDeeper;
	int palpha = alpha, pbeta = beta;
	int result, maxresult = -INFINITE;
	int bestMove = -1, bestMove2 = -1, maxptr = -1;
	// using transposition table
	int zobPos = getZobKey(my, op) & currentTableMask;
	TPEntry* entry = &tp[zobPos];
	infoDeeper = &entry->deeper;
	if (infoDeeper->my == my && infoDeeper->op == op) {
		if (infoDeeper->depth == tabledepth) {
			if (infoDeeper->lower >= beta)
				return infoDeeper->lower;
			if (infoDeeper->upper <= alpha)
				return infoDeeper->upper;
			if (infoDeeper->lower > alpha) {
				alpha = infoDeeper->lower;
				maxresult = alpha;
				maxptr = infoDeeper->pos;
			}
			if (infoDeeper->upper < beta)
				beta = infoDeeper->upper;
		} else if (infoDeeper->depth < empties) {
			if (infoDeeper->lower >= getEndSortSearchUpperBound(beta))
				return infoDeeper->lower - INFINITE + MAXSTEP;
			if (infoDeeper->lower > getEndSortSearchUpperBound(alpha)) {
				alpha = infoDeeper->lower - INFINITE + MAXSTEP;
				maxresult = alpha;
				maxptr = infoDeeper->pos;
			}
			if (infoDeeper->upper <= getEndSortSearchLowerBound1(alpha))
				return infoDeeper->upper + INFINITE - MAXSTEP;
			if (infoDeeper->upper < getEndSortSearchLowerBound1(beta))
				beta = infoDeeper->upper + INFINITE - MAXSTEP;
		}
		if (infoDeeper->depth >= sortSearchDepth && infoDeeper->lower < END_WIPEOUT_THRESHOLD)
			bestMove = infoDeeper->pos;
	}

	infoNewer = &entry->newer;
	if (infoNewer->my == my && infoNewer->op == op) {
		if (infoNewer->depth == tabledepth) {
			if (infoNewer->lower >= beta)
				return infoNewer->lower;
			if (infoNewer->upper <= alpha)
				return infoNewer->upper;
			if (infoNewer->lower > alpha) {
				alpha = infoNewer->lower;
				maxresult = alpha;
				maxptr = infoNewer->pos;
			}
			if (infoNewer->upper < beta)
				beta = infoNewer->upper;
		} else if (infoNewer->depth < empties) {
			if (infoNewer->lower >= getEndSortSearchUpperBound(beta))
				return infoNewer->lower - INFINITE + MAXSTEP;
			if (infoNewer->lower > getEndSortSearchUpperBound(alpha)) {
				alpha = infoNewer->lower - INFINITE + MAXSTEP;
				maxresult = alpha;
				maxptr = infoNewer->pos;
			}
			if (infoNewer->upper <= getEndSortSearchLowerBound1(alpha))
				return infoNewer->upper + INFINITE - MAXSTEP;
			if (infoNewer->upper < getEndSortSearchLowerBound1(beta))
				beta = infoNewer->upper + INFINITE - MAXSTEP;
		}
		if (infoNewer->pos != bestMove && infoNewer->depth >= sortSearchDepth && infoNewer->lower < END_WIPEOUT_THRESHOLD)
			bestMove2 = infoNewer->pos;
	}

	BitBoard mob = mobility(my, op);

	if (mob == 0) {
		if (lastFound)
			return -searchExact(op, my, -beta, -alpha, false);
		else return 
#ifdef COUNT_INTERNAL_NODES
			getResultNoCount(my);
#else
			getResult(my);
#endif
	}

#ifdef USE_STABILITY
	int lower, upper;
	calcStabilityBound(my, op, lower, upper);
	if (lower > alpha) {
		alpha = lower;
		if (alpha >= beta) {
			return alpha;
		}
		// must look for a move to set maxptr
		if (bestMove != -1)
			maxptr = bestMove;
		else if (bestMove2 != -1)
			maxptr = bestMove2;
		else for (EmptyNode* i = emptyHead->succ; i != emptyTail; i = i->succ) {
			int current = i->pos;
			if (posTable[current] & mob) {
				maxptr = current;
				break;
			}
		}
		maxresult = alpha;
	}
	if (upper < beta) {
		beta = upper;
		if (beta <= alpha)
			return beta;
	}
#endif

	bool parityPrepare = false;
	int (Solver::*searchFunction)(BitBoard&, BitBoard&, int, int, bool);
	if (empties > MAX_USE_PARITY_DEPTH)
		searchFunction = &Solver::searchExact;
	else {
		searchFunction = &Solver::searchExact_parity;
		parityPrepare = true;
	}

	// check table move first
	int pSortStackPtr = sortStackPtr;
	if (bestMove != -1)
		emptySortStack[sortStackPtr++] = emptyPtr[bestMove];
	if (bestMove2 != -1)
		emptySortStack[sortStackPtr++] = emptyPtr[bestMove2];
	int sortStart = sortStackPtr;
	sortStackPtr = pSortStackPtr + bits(mob);

#ifdef USE_ETC
	// add the rest of the moves
	int ptr = sortStart;
	for (EmptyNode* enode = emptyHead->succ; enode != emptyTail; enode = enode->succ) {
		int current = enode->pos;
		if ((posTable[current] & mob) && (current != bestMove) && (current != bestMove2)) {
			emptySortStack[ptr++] = enode;
		}
	}
	// using enhanced transposition cutoff
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {
		EmptyNode* pos = emptySortStack[i];
		makeMove(pos->pos, my, op);
		// Enhanced Transpositon Cutoff
		int zobKey2 = getZobKey(op, my);
		TPEntry* entry = &tp[zobKey2 & currentTableMask];
		TPInfo* info = &entry->deeper;
		if (info->my == op && info->op == my && info->depth >= tabledepth - 1
			&& -info->upper > alpha) {
				alpha = -info->upper;
				maxresult = alpha;
				maxptr = pos->pos;
				if (alpha >= beta) {
					unMakeMove();
					sortStackPtr = pSortStackPtr;
					return alpha;
				}
		}
		info = &entry->newer;
		if (info->my == op && info->op == my && info->depth >= tabledepth - 1
			&& -info->upper > alpha) {
				alpha = -info->upper;
				maxresult = alpha;
				maxptr = pos->pos;
				if (alpha >= beta) {
					unMakeMove();
					sortStackPtr = pSortStackPtr;
					return alpha;
				}
		}
		unMakeMove();
	}
#endif

	// sort & search combined
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {
		if (i == sortStart) {
#ifndef USE_ETC
			// add the rest of the moves
			int ptr = sortStart;
			for (EmptyNode* enode = emptyHead->succ; enode != emptyTail; enode = enode->succ) {
				int current = enode->pos;
				if ((posTable[current] & mob) && (current != bestMove) && (current != bestMove2)) {
					emptySortStack[ptr++] = enode;
				}
			}
#endif
			for (int j = sortStart; j < sortStackPtr; j++) {
				int current = emptySortStack[j]->pos;
				makeMove(current, my, op);
				sortResultStack[j] = result = (sortSearchDepth == 1) ?
					(-evaluate(op, my))
					: ((sortSearchDepth <= MID_USE_SORT_DEPTH) ? 
					-fastSearch(op, my, sortSearchDepth - 1, -getEndSortSearchUpperBound(beta), -getEndSortSearchLowerBound(alpha), true)
					: -search(op, my, sortSearchDepth - 1, -getEndSortSearchUpperBound(beta), -getEndSortSearchLowerBound(alpha), true));
				/*-fastSearch(op, my, 1, -INFINITE, INFINITE, true);*/
				if (aborted) return 0;
				if (result >= END_WIPEOUT_THRESHOLD)
					sortResultStack[j] += END_VERY_HIGH_EVAL_BONUS;
				if (empties <= END_FASTEST_FIRST_DEPTH) {
					if (result < getEndSortSearchUpperBound(0) && result > getEndSortSearchLowerBound1(0)) {
						sortResultStack[j] -= weighted_mobility_bits(mobility(op, my)) * 
							(result >= END_WIPEOUT_THRESHOLD) ? END_WIPEOUT_MOBILITY_WEIGHT : END_MOBILITY_WEIGHT;
					}
				} else if (result >= END_WIPEOUT_THRESHOLD) {
					sortResultStack[j] -= weighted_mobility_bits(mobility(op, my)) * (END_WIPEOUT_MOBILITY_WEIGHT - END_MOBILITY_WEIGHT);
				}
				unMakeMove();
				if (sortSearchDepth == 1) continue;
				int tmpRes = result - INFINITE + MAXSTEP;
				if (result >= getEndSortSearchUpperBound(beta)) {
					if (beta == pbeta)
						saveTp(zobPos, my, op, tmpRes, MAXSTEP + 1, current, tabledepth, 0);
					else
						saveTp(zobPos, my, op, tmpRes, tmpRes, current, tabledepth, 0);
					sortStackPtr = pSortStackPtr;
					return tmpRes;
				}
				if (result > getEndSortSearchUpperBound(alpha)) {
					alpha = tmpRes;
					maxresult = alpha;
					maxptr = current;
				}
			}
		}

		if (i >= sortStart) {
			int p = i;
			for (int j = i + 1; j < sortStackPtr; j++) {
				if (sortResultStack[j] > sortResultStack[p])
					p = j;
			}
			if (p != i) {
				int tmp = sortResultStack[i];
				sortResultStack[i] = sortResultStack[p];
				sortResultStack[p] = tmp;
				EmptyNode* tmpNode = emptySortStack[i];
				emptySortStack[i] = emptySortStack[p];
				emptySortStack[p] = tmpNode;
			}
		}

		int current = emptySortStack[i]->pos;
		makeMoveAndSetEmpties(emptySortStack[i], my, op);
		if (parityPrepare) prepareParity();

#ifdef ENDGAME_USE_NEGASCOUT
		if (i != pSortStackPtr) { // it's not the first node
#else
		if ((alpha > palpha) && (beta - alpha >= END_WINDOW_SIZE)) { // it's a PV node
#endif
			// try a 0-window search
			result = -(this->*searchFunction)(op, my, -alpha - 1, -alpha, true);
			if (aborted) return 0;
			if (result <= alpha) { // no better evaluations, throw away the step
				unMakeMoveAndSetEmpties();
				if (result > maxresult) {
					maxresult = result;
				}
				continue;
			}
			if (result >= beta) {
				unMakeMoveAndSetEmpties();
				if (beta == pbeta)
					saveTp(zobPos, my, op, result, MAXSTEP + 1, current, tabledepth, 0);
				else
					saveTp(zobPos, my, op, result, result, current, tabledepth, 0);
				sortStackPtr = pSortStackPtr;
				return result;
			}
			maxresult = alpha = result;
			maxptr = current;
		}

		// otherwise, return to normal search
		result = -(this->*searchFunction)(op, my, -beta, -alpha, true);
		if (aborted) return 0;
		unMakeMoveAndSetEmpties();
		if (result >= beta) {
			if (beta == pbeta)
				saveTp(zobPos, my, op, result, MAXSTEP + 1, current, tabledepth, 0);
			else
				saveTp(zobPos, my, op, result, result, current, tabledepth, 0);
			sortStackPtr = pSortStackPtr;
			return result;
		}
		if (result > maxresult) {
			maxresult = result;
			if (result > alpha) {
				alpha = result;
				maxptr = current;
			}
		}
	}
	int lowerbound = 
#ifdef USE_STABILITY
		lower;
#else
		-MAXSTEP;
#endif
	if (maxresult == lowerbound) {
		palpha = alpha - 1;
		maxptr = emptySortStack[pSortStackPtr]->pos;
	}
	if (palpha == alpha)
		saveTp(zobPos, my, op, -MAXSTEP - 1, maxresult, maxptr, tabledepth, 0);
	else
		saveTp(zobPos, my, op, maxresult, maxresult, maxptr, tabledepth, 0);
	sortStackPtr = pSortStackPtr;
	return maxresult;
}

int Solver::fastSearchExact_TwoEmpty(BitBoard& my, BitBoard& op, int empty1, int empty2, int alpha, int beta) {
#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif
	int result = -INFINITE, maxresult = -INFINITE, ev, flipped;
	bstack[stackptr] = black; wstack[stackptr] = white;
	// try the first empty
	if (checkedPutChess(empty1, my, op)) {
		evnum++;
		ev = getResultNoEmptyNoCount(my);
		if (flipped = countFunction[empty2](op)) {
			maxresult = ev - flipped * 2;
			black = bstack[stackptr]; white = wstack[stackptr];
			if (maxresult >= beta) return maxresult;
		} else {
			// ev will remain unchanged or increase by at least 2(ev >= 0)
			maxresult = ev + 2 - (((unsigned int)(ev & 0x80000000UL)) >> 30);
			if (maxresult >= beta) {
				black = bstack[stackptr]; white = wstack[stackptr];
				return maxresult;
			}
			if (flipped = countFunction[empty2](my)) {
				maxresult = ev + (flipped + 1) * 2;
				black = bstack[stackptr]; white = wstack[stackptr];
				if (maxresult >= beta) return maxresult;
			} else {
				black = bstack[stackptr]; white = wstack[stackptr];
			}
		}
	}

	// and the second empty
	if (checkedPutChess(empty2, my, op)) {
		evnum++;
		ev = getResultNoEmptyNoCount(my);
		if (flipped = countFunction[empty1](op)) {
			result = ev - flipped * 2;
			black = bstack[stackptr]; white = wstack[stackptr];
		} else {
			// ev will remain unchanged or increase by at least 2(ev >= 0)
			result = ev + 2 - (((unsigned int)(ev & 0x80000000UL)) >> 30);
			if (result >= beta) {
				black = bstack[stackptr]; white = wstack[stackptr];
				return result;
			}
			if (flipped = countFunction[empty1](my)) {
				result = ev + (flipped + 1) * 2;
				black = bstack[stackptr]; white = wstack[stackptr];
			} else {
				black = bstack[stackptr]; white = wstack[stackptr];
			}
		}
	}

	if (result > maxresult)
		return result;

	if (maxresult == -INFINITE) {
		return -fastSearchExact_TwoEmpty_NoLastFound(op, my, empty1, empty2, -alpha);
	}

	return maxresult;

}

int Solver::fastSearchExact_TwoEmpty_NoLastFound(BitBoard& my, BitBoard& op, int empty1, int empty2, int beta) { 
// no need to pass alpha bound

#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif
	int result = -INFINITE, maxresult = -INFINITE, ev, flipped;
	//bstack[stackptr] = black; wstack[stackptr] = white; // already backed up by lastFound=true version
	// try the first empty
	if (checkedPutChess(empty1, my, op)) {
		evnum++;
		ev = getResultNoEmptyNoCount(my);
		if (flipped = countFunction[empty2](op)) {
			maxresult = ev - flipped * 2;
			black = bstack[stackptr]; white = wstack[stackptr];
			if (maxresult >= beta) return maxresult;
		} else {
			// ev will remain unchanged or increase by at least 2(ev >= 0)
			maxresult = ev + 2 - (((unsigned int)(ev & 0x80000000UL)) >> 30);
			if (maxresult >= beta) {
				black = bstack[stackptr]; white = wstack[stackptr];
				return maxresult;
			}
			if (flipped = countFunction[empty2](my)) {
				maxresult = ev + (flipped + 1) * 2;
				black = bstack[stackptr]; white = wstack[stackptr];
				if (maxresult >= beta) return maxresult;
			} else {
				black = bstack[stackptr]; white = wstack[stackptr];
			}
		}
	}

	// and the second empty
	if (checkedPutChess(empty2, my, op)) {
		evnum++;
		ev = getResultNoEmptyNoCount(my);
		if (flipped = countFunction[empty1](op)) {
			result = ev - flipped * 2;
			black = bstack[stackptr]; white = wstack[stackptr];
		} else {
			// ev will remain unchanged or increase by at least 2(ev >= 0)
			result = ev + 2 - (((unsigned int)(ev & 0x80000000UL)) >> 30);
			if (result >= beta) {
				black = bstack[stackptr]; white = wstack[stackptr];
				return result;
			}
			if (flipped = countFunction[empty1](my)) {
				result = ev + (flipped + 1) * 2;
				black = bstack[stackptr]; white = wstack[stackptr];
			} else {
				black = bstack[stackptr]; white = wstack[stackptr];
			}
		}
	}

	if (result > maxresult)
		return result;

	if (maxresult == -INFINITE) {
		// gameover when two empties left
		return 
#ifdef COUNT_INTERNAL_NODES
			getResultNoCount(my);
#else
			getResult(my);
#endif
	}

	return maxresult;

}

int Solver::fastSearchExact_ThreeEmpty(BitBoard& my, BitBoard& op, int empty1, int empty2, int empty3, int alpha, int beta, bool lastFound) {
#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif
	int maxresult = -INFINITE, result = -INFINITE;
	if (checkedMakeMove(empty1, my, op)) {
		maxresult = -fastSearchExact_TwoEmpty(op, my, empty2, empty3, -beta, -alpha);
		unMakeMove();
		if (maxresult >= beta) return maxresult;
		if (maxresult > alpha)
			alpha = maxresult;
	}
	if (checkedMakeMove(empty2, my, op)) {
		result = -fastSearchExact_TwoEmpty(op, my, empty1, empty3, -beta, -alpha);
		unMakeMove();
		if (result >= beta) return result;
		if (result > maxresult) {
			maxresult = result;
			if (maxresult > alpha)
				alpha = maxresult;
		}
	}
	if (checkedMakeMove(empty3, my, op)) {
		result = -fastSearchExact_TwoEmpty(op, my, empty1, empty2, -beta, -alpha);
		unMakeMove();
		if (result >= beta) return result;
		if (result > maxresult) {
			maxresult = result;
		}
	}
	if (maxresult == -INFINITE) {
		if (lastFound) return -fastSearchExact_ThreeEmpty(op, my, empty1, empty2, empty3, -beta, -alpha, false);
		return 
#ifdef COUNT_INTERNAL_NODES
			getResultNoCount(my);
#else
			getResult(my);
#endif
	}
	return maxresult;
}

int Solver::fastSearchExact_FourEmpty(BitBoard& my, BitBoard& op, int empty1, int empty2, int empty3, int empty4, int alpha, int beta, bool lastFound) {
#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif
	int maxresult = -INFINITE, result = -INFINITE;
	if (checkedMakeMove(empty1, my, op)) {
		maxresult = -fastSearchExact_ThreeEmpty(op, my, empty2, empty3, empty4, -beta, -alpha, true);
		unMakeMove();
		if (maxresult >= beta) return maxresult;
		if (maxresult > alpha)
			alpha = maxresult;
	}
	if (checkedMakeMove(empty2, my, op)) {
		result = -fastSearchExact_ThreeEmpty(op, my, empty1, empty3, empty4, -beta, -alpha, true);
		unMakeMove();
		if (result >= beta) return result;
		if (result > maxresult) {
			maxresult = result;
			if (maxresult > alpha)
				alpha = maxresult;
		}
	}
	if (checkedMakeMove(empty3, my, op)) {
		result = -fastSearchExact_ThreeEmpty(op, my, empty1, empty2, empty4, -beta, -alpha, true);
		unMakeMove();
		if (result >= beta) return result;
		if (result > maxresult) {
			maxresult = result;
			if (maxresult > alpha)
				alpha = maxresult;
		}
	}
	if (checkedMakeMove(empty4, my, op)) {
		result = -fastSearchExact_ThreeEmpty(op, my, empty1, empty2, empty3, -beta, -alpha, true);
		unMakeMove();
		if (result >= beta) return result;
		if (result > maxresult) {
			maxresult = result;
		}
	}
	if (maxresult == -INFINITE) {
		if (lastFound) return -fastSearchExact_FourEmpty(op, my, empty1, empty2, empty3, empty4, -beta, -alpha, false);
		return 
#ifdef COUNT_INTERNAL_NODES
			getResultNoCount(my);
#else
			getResult(my);
#endif
	}
	return maxresult;
}

int Solver::fastSearchExact(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound) {
#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif

	int result, maxresult = -MAXSTEP;
	bool found = false;

	if (empties == 5) {
		if (regionParity) {
			for (EmptyNode* i = emptyHead->succ; i != emptyTail; i = i->succ) {
				if ((regionParity & i->holeid)
					&& checkedMakeMoveAndSetEmpties(i, my, op)) {
					found = true;
					EmptyNode* current = emptyHead->succ;
					int empty1 = current->pos;
					current = current->succ;
					int empty2 = current->pos;
					current = current->succ;
					result = -fastSearchExact_FourEmpty(op, my, empty1, empty2, 
						current->pos, current->succ->pos, -beta, -alpha, true);
					unMakeMoveAndSetEmpties();
					if (result >= beta) {
						return result;
					}
					if (result > maxresult) {
						maxresult = result;
						if (result > alpha) {
							alpha = result;
						}
					}
				}
			}
			for (EmptyNode* i = emptyHead->succ; i != emptyTail; i = i->succ) {
				if (!(regionParity & i->holeid) 
					&& checkedMakeMoveAndSetEmpties(i, my, op)) {
					found = true;
					EmptyNode* current = emptyHead->succ;
					int empty1 = current->pos;
					current = current->succ;
					int empty2 = current->pos;
					current = current->succ;
					result = -fastSearchExact_FourEmpty(op, my, empty1, empty2, 
						current->pos, current->succ->pos, -beta, -alpha, true);
					unMakeMoveAndSetEmpties();
					if (result >= beta) {
						return result;
					}
					if (result > maxresult) {
						maxresult = result;
						if (result > alpha) {
							alpha = result;
						}
					}
				}
			}
		} else {
			for (EmptyNode* i = emptyHead->succ; i != emptyTail; i = i->succ) {
				if (checkedMakeMoveAndSetEmpties(i, my, op)) {
					found = true;
					EmptyNode* current = emptyHead->succ;
					int empty1 = current->pos;
					current = current->succ;
					int empty2 = current->pos;
					current = current->succ;
					result = -fastSearchExact_FourEmpty(op, my, empty1, empty2, 
						current->pos, current->succ->pos, -beta, -alpha, true);
					unMakeMoveAndSetEmpties();
					if (result >= beta) {
						return result;
					}
					if (result > maxresult) {
						maxresult = result;
						if (result > alpha) {
							alpha = result;
						}
					}
				}
			}
		}
	} else {
		if (regionParity) {
			for (EmptyNode* i = emptyHead->succ; i != emptyTail; i = i->succ) {
				if ((regionParity & i->holeid)
					&& checkedMakeMoveAndSetEmpties(i, my, op)) {
					setParity(i);
					found = true;
					result = (empties == 0) ? (getResultNoEmpty(my)) : (-fastSearchExact(op, my, -beta, -alpha, true));
					unSetParity(i);
					unMakeMoveAndSetEmpties();
					if (result >= beta) {
						return result;
					}
					if (result > maxresult) {
						maxresult = result;
						if (result > alpha) {
							alpha = result;
						}
					}
				}
			}
			for (EmptyNode* i = emptyHead->succ; i != emptyTail; i = i->succ) {
				if (!(regionParity & i->holeid)
					&& checkedMakeMoveAndSetEmpties(i, my, op)) {
					setParity(i);
					found = true;
					result = (empties == 0) ? (getResultNoEmpty(my)) : (-fastSearchExact(op, my, -beta, -alpha, true));
					unSetParity(i);
					unMakeMoveAndSetEmpties();
					if (result >= beta) {
						return result;
					}
					if (result > maxresult) {
						maxresult = result;
						if (result > alpha) {
							alpha = result;
						}
					}
				}
			}
		} else {
			for (EmptyNode* i = emptyHead->succ; i != emptyTail; i = i->succ) {
				if (checkedMakeMoveAndSetEmpties(i, my, op)) {
					setParity(i);
					found = true;
					result = (empties == 0) ? (getResultNoEmpty(my)) : (-fastSearchExact(op, my, -beta, -alpha, true));
					unSetParity(i);
					unMakeMoveAndSetEmpties();
					if (result >= beta) {
						return result;
					}
					if (result > maxresult) {
						maxresult = result;
						if (result > alpha) {
							alpha = result;
						}
					}
				}
			}
		}
	}
	if (!found) {
		if (lastFound)
			return -fastSearchExact(op, my, -beta, -alpha, false);
		else return 
#ifdef COUNT_INTERNAL_NODES
			getResultNoCount(my);
#else
			getResult(my);
#endif
	}

	return maxresult;
}

SolverResult Solver::solveExact(int color, bool winLoss) {
	// Just exactly solve it
	return solveExactInternal(color, winLoss, EPC_STAGES);
}

SolverResult Solver::solveExactInternal(int color, bool winLoss, int epcStage) {
	evnum = 0;
	percent = 0;
	partialResult = 0;
	partialDepth = 0;
	selectedMove = -1;
	focusedMove = -1;
	initPV();
	const int SORT_DEPTH = 4;
	const int MIN_ITER_START = 6;
	int tabledepth = empties + epcStage;
	int alpha, beta;
	if (winLoss) {
		alpha = -2; beta = 2;
	} else {
		alpha = -MAXSTEP; beta = MAXSTEP;
	}
	BitBoard& my = (color == BLACK) ? black : white;
	BitBoard& op = (color == BLACK) ? white : black;
	BitBoard mob = mobility(my, op);
	int pCount = bits(mob);
	if (pCount == 0) { // out-of-move check
		mob = mobility(op, my);
		if (mob == 0)
			return SolverResult(getResult(my), MAXSTEP);
		return SolverResult(
			-solveExactInternal(color == BLACK ? WHITE : BLACK, winLoss, epcStage).getResult(),
			MAXSTEP);
	}

#ifdef USE_STABILITY
	int lower, upper;
	calcStabilityBound(my, op, lower, upper);
	if (lower > alpha) {
		alpha = lower;
		if (alpha > beta)
			alpha = beta;
	}
	if (upper < beta)
		beta = upper;
#endif

	int palpha = alpha, pbeta = beta;
	int maxresult = -INFINITE, maxptr = -1;
	int adjustedIterStart = -1;
	int adjustedIterResult = 0;
	int choice = -1;
	int bestMove = -1/*, bestMove2 = -1*/;

	//using transposition table
	int zobPos = getZobKey(my, op) & currentTableMask;
	TPEntry* entry = &tp[zobPos];
	TPInfo* infoDeeper = &entry->deeper;
	if (infoDeeper->my == my && infoDeeper->op == op)
		if (infoDeeper->depth >= tabledepth) {
			if (infoDeeper->lower == infoDeeper->upper) {
				setPV(my, op, tabledepth, infoDeeper->pos);
				return SolverResult(infoDeeper->lower, infoDeeper->pos);
			}
			if (infoDeeper->upper < beta) beta = infoDeeper->upper;
			if (infoDeeper->lower >= beta) {
				setPV(my, op, tabledepth, infoDeeper->pos);
				return SolverResult(infoDeeper->lower, infoDeeper->pos);
			}
			if (infoDeeper->lower > alpha) {
				alpha = infoDeeper->lower;
				maxresult = alpha;
				maxptr = infoDeeper->pos;
				choice = maxptr;
			}
		} else if ((infoDeeper->depth >= MIN_ITER_START) 
			&& ((infoDeeper->lower == infoDeeper->upper) ||
				(infoDeeper->lower == MAXSTEP && infoDeeper->depth >= empties))) {
				//adjust iterative deepening
				bestMove = infoDeeper->pos;
				adjustedIterStart = infoDeeper->depth + 1;
				adjustedIterResult = infoDeeper->lower;
		}
	TPInfo* infoNewer = &entry->newer;
	if (infoNewer->my == my && infoNewer->op == op)
		if (infoNewer->depth >= tabledepth) {
			if (infoNewer->lower == infoNewer->upper) {
				setPV(my, op, tabledepth, infoNewer->pos);
				return SolverResult(infoNewer->lower, infoNewer->pos);
			}
			if (infoNewer->upper < beta) beta = infoNewer->upper;
			if (infoNewer->lower >= beta) {
				setPV(my, op, tabledepth, infoNewer->pos);
				return SolverResult(infoNewer->lower, infoNewer->pos);
			}
			if (infoNewer->lower > alpha) {
				alpha = infoNewer->lower;
				maxresult = alpha;
				maxptr = infoNewer->pos;
				choice = maxptr;
			}
		} else if ((infoNewer->depth >= MIN_ITER_START)
			&& ((infoNewer->lower == infoNewer->upper) ||
				(infoNewer->lower == MAXSTEP && infoNewer->depth >= empties))
			&& (infoNewer->depth >= adjustedIterStart)) { //adjust iterative deepening
				bestMove = infoNewer->pos;
				adjustedIterStart = infoNewer->depth + 1;
				adjustedIterResult = infoNewer->lower;
		}
	int pptr = 0;
	int opColor = BLACK + WHITE - color;
	setEmpties(); sortStackPtr = 0;
	EmptyNode* positions[MAXSTEP];
	memset(results, 0, sizeof(results));
	memset(positions, 0, sizeof(positions));

	int pre_stackptr = stackptr;

#ifdef SYMMETRIC_PRUNING
	bool is_symmetric[SYMMETRICS];
	// for symmetric positions, some moves are not searched at all
	for (int sym = 1; sym < SYMMETRICS; sym++) {
		is_symmetric[sym] = false;
		BitBoard work_my = my;
		BitBoard work_op = op;
		transformSingle(sym, work_my);
		if (work_my != my) continue;
		transformSingle(sym, work_op);
		if (work_op != op) continue;

		is_symmetric[sym] = true;
	}
#endif
	//sort the moves
	if (bestMove != -1) {
		positions[pptr++] = emptyPtr[bestMove];
	}
	for (EmptyNode* i = emptyHead->succ; i != emptyTail; i = i->succ)
		if (posTable[i->pos] & mob && i->pos != bestMove) {
#ifdef SYMMETRIC_PRUNING
			bool prunned = false;
			for (int sym = 1; sym < SYMMETRICS; sym++) {
				if (is_symmetric[sym]) {
					int sym_move = i->pos;
					transformPos(sym, sym_move);
					for (int j = 0; j < pptr; j++) {
						if (positions[j]->pos == sym_move) {
							prunned = true;
							// still keep track of the prunned move for ETC use,
							// move ordering here is not important
							positions[--pCount] = i;
							if (i->pos == maxptr) {
								maxptr = sym_move;
								choice = sym_move;
							}
							break;
						}
					}
					if (prunned) break;
				}
			}
			if (prunned) continue;
#endif
			positions[pptr] = i;
			if (empties <= SORT_DEPTH) {
				pptr++;
				continue;
			}
			makeMove(i->pos, my, op);
			results[pptr] = -search(op, my, SORT_DEPTH - 1, -INFINITE, INFINITE, true);
			if (aborted) {
				restoreStack(pre_stackptr);
				if (maxptr != -1)
					return SolverResult(maxresult, maxptr);
				else return SolverResult(0, positions[0]->pos);
			}
			unMakeMove();
			pptr++;
		}
	if (empties > SORT_DEPTH) {
		int tmp; EmptyNode* tmpNode;
		for (int i = (bestMove == -1) ? 0 : 1; i < pCount - 1; i++)
			for (int j = i + 1; j < pCount; j++)
				if (results[i] < results[j]) {
					tmp = results[i]; results[i] = results[j]; results[j] = tmp;
					tmpNode = positions[i]; positions[i] = positions[j]; positions[j] = tmpNode;
				}
	}

	partialDepth = SORT_DEPTH;
	partialResult = results[0];
	selectedMove = positions[0]->pos;

#ifdef USE_ETC_AT_ROOT
	// using enhanced transposition cutoff
#ifdef SYMMETRIC_PRUNING
	// access full range of moves to utilize most of the transposition table
	int totalCount = bits(mob);
	for (int i = 0; i < totalCount; i++) {
#else
	for (int i = 0; i < pCount; i++) {
#endif
		EmptyNode* pos = positions[i];
		makeMove(pos->pos, my, op);
		// Enhanced Transpositon Cutoff
		int zobKey2 = getZobKey(op, my);
		TPEntry* entry2 = &tp[zobKey2 & currentTableMask];
		TPInfo* info = &entry2->deeper;
		if (info->my == op && info->op == my && info->depth >= tabledepth - 1
			&& -info->upper > alpha) {
			alpha = -info->upper;
			maxresult = alpha;
			maxptr = pos->pos;
			choice = maxptr;
			if (alpha >= beta) {
				unMakeMove();
				setPV(my, op, tabledepth, choice);
				return SolverResult(alpha, choice);
			}
		}
		info = &entry2->newer;
		if (info->my == op && info->op == my && info->depth >= tabledepth - 1
			&& -info->upper > alpha) {
			alpha = -info->upper;
			maxresult = alpha;
			maxptr = pos->pos;
			choice = maxptr;
			if (alpha >= beta) {
				unMakeMove();
				setPV(my, op, tabledepth, choice);
				return SolverResult(alpha, choice);
			}
		}
		unMakeMove();
	}
#endif

	// using iterative deepening
	if (adjustedIterStart != -1 && adjustedIterStart <= empties) {
		partialDepth = adjustedIterStart - 1;
		partialResult = adjustedIterResult;
		selectedMove = positions[0]->pos;
	}
	setPV(my, op, partialDepth, positions[0]->pos);

#ifdef USE_EVEN_DEEPENING
	const int ITER_END = (empties > 26) ? (16 - (empties & 1)) : (empties - 10);
	const int ITER_START = (adjustedIterStart == -1) ? (MIN_ITER_START - ((MIN_ITER_START ^ ITER_END) & 1)) : (adjustedIterStart + ((adjustedIterStart ^ ITER_END) & 1));
#else
	const int ITER_START = (adjustedIterStart == -1) ? MIN_ITER_START : adjustedIterStart;
	const int ITER_END = (empties > 26) ? (16 - (empties & 1)) : (empties - 10);
#endif
	int iteralpha, iterbeta = MID_SEARCH_BOUND;
	int itermax;
#ifdef USE_EVEN_DEEPENING
	for (int iterdepth = ITER_START; iterdepth <= ITER_END; iterdepth += 2) {
#else
	for (int iterdepth = ITER_START; iterdepth <= ITER_END; iterdepth++) {
#endif
#ifdef USE_EVEN_DEEPENING
		percent = (iterdepth - ITER_START) * 10 / (ITER_END + 2 - ITER_START);
#else
		percent = (iterdepth - ITER_START) * 10 / (ITER_END + 1 - ITER_START);
#endif
		iteralpha = -MID_SEARCH_BOUND;
		itermax = -INFINITE;
		for (int i = 0; i < pCount; i++) {
			EmptyNode* pos = positions[i];
			focusedMove = pos->pos;
			makeMove(pos->pos, my, op);
#ifdef MIDGAME_USE_NEGASCOUT
			if ((i != 0) && alphacuttable(iteralpha) && (iterbeta - iteralpha >= MID_WINDOW_SIZE)) {
#else
			if ((itermax > -INFINITE) && (iterbeta - iteralpha >= MID_WINDOW_SIZE)) { // it's a PV node
#endif
				if (iterdepth > MPC_DEPTH_THRESHOLD) {
					//try a 0-window search, false cutoff fix
					results[i] = -search_mpc(op, my, iterdepth - 1, -iteralpha, -iteralpha + 1, true);
				} else {
					//try a 0-window search
					results[i] = -search(op, my, iterdepth - 1, -iteralpha - 1, -iteralpha, true);
				}
				if (aborted) {
					restoreStack(pre_stackptr);
					return SolverResult(partialResult, selectedMove);
				}
				if (results[i] < iteralpha
					|| (iterdepth <= MPC_DEPTH_THRESHOLD && results[i] == iteralpha)) { 
					// no better evaluations, throw away the step
					unMakeMove();
					if (results[i] > results[0]) {
						itermax = results[i];
						int tmp = results[i];
						EmptyNode* tmp2 = positions[i];
						for (int j = i - 1; j >= 0; j--) {
							results[j + 1] = results[j];
							positions[j + 1] = positions[j];
						}
						results[0] = tmp; positions[0] = tmp2;
						setPV(my, op, iterdepth, positions[0]->pos);
						partialResult = results[0];
						selectedMove = positions[0]->pos;
					}
					continue;
				}
#ifdef USE_STABILITY
				if (upper > 0 && results[i] >= endToMid(upper)) {
					unMakeMove();
					percent = 100;
					setPV(my, op, iterdepth, pos->pos);
					return SolverResult(endToMid(upper) - (INFINITE - MAXSTEP), pos->pos);
				}
#else
				if (results[i] >= INFINITE) {
					unMakeMove();
					percent = 100;
					setPV(my, op, iterdepth, pos->pos);
					return SolverResult(INFINITE - (INFINITE - MAXSTEP), pos->pos);
				}
#endif
				if (results[i] > iteralpha) { // if result == alpha, might be mistakenly cutoff by mpc
					itermax = iteralpha = results[i];
					selectedMove = focusedMove;
				}
			}

			// otherwise, return to normal search
			if (iteralpha < iterbeta) {
				results[i] = (iterdepth > MPC_DEPTH_THRESHOLD)
					?-search_mpc(op, my, iterdepth - 1, -iterbeta, -iteralpha, true)
					:-search(op, my, iterdepth - 1, -iterbeta, -iteralpha, true);

				if (aborted) {
					restoreStack(pre_stackptr);
					return SolverResult(partialResult, selectedMove);
				}
			}

			unMakeMove();
			if (results[i] >= INFINITE) {
				percent = 100;
				setPV(my, op, iterdepth, pos->pos);
				return SolverResult(INFINITE - (INFINITE - MAXSTEP), pos->pos);
			}
			if (results[i] > itermax) {
				itermax = results[i];
				if (results[i] > iteralpha)
					iteralpha = results[i];
			}
			if (results[i] > results[0]) {
				int tmp = results[i];
				EmptyNode* tmp2 = positions[i];
				for (int j = i - 1; j >= 0; j--) {
					results[j + 1] = results[j];
					positions[j + 1] = positions[j];
				}
				results[0] = tmp; positions[0] = tmp2;
				setPV(my, op, iterdepth, positions[0]->pos);
			}
			partialDepth = iterdepth;
			partialResult = results[0];
			selectedMove = positions[0]->pos;
			if (i == 0) {
				setPV(my, op, iterdepth, positions[0]->pos);
			}
			if (results[0] >= iterbeta) break;
		}
		if (itermax == -INFINITE) {
			percent = 100;
			setPV(my, op, iterdepth, positions[0]->pos);
			return SolverResult(itermax - (-INFINITE + MAXSTEP), positions[0]->pos);
		}
	}
	percent = 10;

	// using iterative widening
	if (adjustedIterStart > empties) {
		currentEpcPercentage = EPC_PERCENTAGE[adjustedIterStart - empties - 1];
		partialDepth = winLoss ? PARTIALDEPTH_WLD : PARTIALDEPTH_EXACT;
		partialResult = adjustedIterResult;
		selectedMove = positions[0]->pos;
		setPV(my, op, adjustedIterStart - 1, positions[0]->pos);
	}
	if (empties > MIN_EPC_DEPTH && alpha < beta) {// only use iter widening when there's possibility to cut
		const int EPC_START = (adjustedIterStart <= empties) ? 0 : (adjustedIterStart - empties);
		const int EPC_END = epcStage;
		for (currentEpcStage = EPC_START; currentEpcStage < EPC_END; currentEpcStage++) {
			iteralpha = alpha;
			int piteralpha = iteralpha;
			iterbeta = beta;
			for (int i = 0; i < pCount; i++) {
				percent = 10 + 40 * ((currentEpcStage - EPC_START) * pCount + i)
								/ (pCount * (EPC_END - EPC_START));
				EmptyNode* pos = positions[i];
				focusedMove = pos->pos;
				makeMoveAndSetEmpties(pos, my, op);
				results[i] = iteralpha;

#ifdef ENDGAME_USE_NEGASCOUT
				if (i != 0) { // it's not the first node
#else
				if (iteralpha > piteralpha && iterbeta - iteralpha >= END_WINDOW_SIZE) { // it's a PV node
#endif
					//try a 0-window search
					results[i] = -searchExact_epc(op, my, -iteralpha - 2, -iteralpha, true);
					if (aborted) {
						restoreStack(pre_stackptr);
						return SolverResult(partialResult, selectedMove);
					}
					if (results[i] <= iteralpha) { // no better evaluations, throw away the step
						unMakeMoveAndSetEmpties();
						// when no alpha improvement detected, table move is treated specially
						if (piteralpha == iteralpha && positions[i]->pos == choice) { 
							// the table move should be the choice so far
							for (int j = i - 1; j >= 0; j--) {
								results[j + 1] = results[j];
								positions[j + 1] = positions[j];
							}
							results[0] = iteralpha; positions[0] = emptyPtr[choice];
							setPV(my, op, empties + currentEpcStage, positions[0]->pos);
							currentEpcPercentage = EPC_PERCENTAGE[currentEpcStage];
							partialDepth = winLoss ? PARTIALDEPTH_WLD : PARTIALDEPTH_EXACT;
							partialResult = results[0];
							selectedMove = positions[0]->pos;
						}
						continue;
					}
//					if (results[i] > iteralpha + 1) { // if result == alpha + 1(odd number), might be mistakenly cutoff by epc
						// only tell the user when we are sure it's not a false cutoff
						if (results[i] > iteralpha + 2) selectedMove = focusedMove;
//					}
				}

				// otherwise, return to normal search
				if (results[i] < iterbeta) {
#ifdef USE_MTD_F
					results[i] = -mtdExact_epc(op, my, -iterbeta, -results[i], true);
#else
					results[i] = -searchExact_epc(op, my, -iterbeta, -results[i], true);
#endif
					if (aborted) {
						restoreStack(pre_stackptr);
						return SolverResult(partialResult, selectedMove);
					}
				}

				unMakeMoveAndSetEmpties();

				// when no alpha improvement(in WLD search), we should trust the mid search result
				if (results[i] > iteralpha) {
					iteralpha = results[i];
					int tmp = results[i];
					EmptyNode* tmp2 = positions[i];
					for (int j = i - 1; j >= 0; j--) {
						results[j + 1] = results[j];
						positions[j + 1] = positions[j];
					}
					results[0] = tmp; positions[0] = tmp2;
					setPV(my, op, empties + currentEpcStage, positions[0]->pos);
				}
				currentEpcPercentage = EPC_PERCENTAGE[currentEpcStage];
				partialDepth = winLoss ? PARTIALDEPTH_WLD : PARTIALDEPTH_EXACT;
				partialResult = results[0];
				selectedMove = positions[0]->pos;
				if (i == 0 && iteralpha == piteralpha) {
					setPV(my, op, empties + currentEpcStage, positions[0]->pos);
				}
				if (results[0] >= iterbeta) break;
			}
		}
	}

	percent = 50; currentEpcStage = epcStage;

	int (Solver::*searchFunction)(BitBoard&, BitBoard&, int, int, bool);
	searchFunction = //(empties > END_USE_SORT_DEPTH) ? (&Solver::searchExact) : (&Solver::fastSearchExact);
		(epcStage >= EPC_STAGES || empties <= MIN_EPC_DEPTH) 
		? (&Solver::searchExact) : (&Solver::searchExact_epc);

	if (choice == -1)
		choice = positions[0]->pos;
	else {
		currentEpcPercentage = EPC_PERCENTAGE[currentEpcStage];
		partialDepth = (winLoss) ? PARTIALDEPTH_WLD : PARTIALDEPTH_EXACT;
		partialResult = maxresult;
		selectedMove = choice;
		setPV(my, op, tabledepth, choice);
	}
	if (alpha >= beta) {
		percent = 100;
		return SolverResult(beta, choice); // only way to achieve alpha >= beta is to reduce beta
	}
	for (int i = 0; i < pptr; i++) {
		percent = 50 + i * 50 / pptr;
		EmptyNode* pos = positions[i];
		int current = pos->pos;
		focusedMove = current;
		makeMoveAndSetEmpties(pos, my, op);

		results[i] = alpha;

#ifdef ENDGAME_USE_NEGASCOUT
		if ((empties >= END_USE_SORT_DEPTH) && (i != 0)) { // it's not the first node
#else
		if ((empties >= END_USE_SORT_DEPTH) && (maxresult > palpha) && (beta - alpha >= END_WINDOW_SIZE)) { // it's a PV node
#endif
			// try a 0-window search
			results[i] = -(this->*searchFunction)(op, my, -alpha - 2, -alpha, true);
			if (aborted) {
				restoreStack(pre_stackptr);
				return SolverResult(partialResult, selectedMove);
			}
			if (results[i] <= alpha) { // no better evaluations, throw away the step
				unMakeMoveAndSetEmpties();
				if (results[i] > maxresult) {
					maxresult = results[i];
				}
				continue;
			}
			if (results[i] >= beta) {
				unMakeMoveAndSetEmpties();
				maxresult = results[i];
				maxptr = current;
				choice = maxptr;
				setPV(my, op, tabledepth, choice);
				break;
			}
			// only tell the user when we are sure it's not a false cutoff
			if (epcStage >= EPC_STAGES || results[i] > alpha + 2) {
				selectedMove = focusedMove; 
			}
		}

		// otherwise, return to normal search
#ifdef USE_MTD_F
		results[i] = (epcStage >= EPC_STAGES)
			? -mtdExact(op, my, -beta, -results[i], true)
			: -mtdExact_epc(op, my, -beta, -results[i], true);
#else
		results[i] = -(this->*searchFunction)(op, my, -beta, -results[i], true);
#endif
		if (aborted) {
			restoreStack(pre_stackptr);
			return SolverResult(partialResult, selectedMove);
		}
		unMakeMoveAndSetEmpties();
		if (results[i] > maxresult) {
			maxresult = results[i];
			if (maxresult >= beta) {
				maxptr = current;
				choice = maxptr;
				setPV(my, op, tabledepth, choice);
				break;
			}
			if (maxresult > alpha) {
				alpha = maxresult;
				maxptr = current;
				choice = maxptr;
				setPV(my, op, tabledepth, choice);
			}
		}
		currentEpcPercentage = EPC_PERCENTAGE[currentEpcStage];
		partialDepth = (winLoss) ? PARTIALDEPTH_WLD : PARTIALDEPTH_EXACT;
		partialResult = maxresult;
		selectedMove = choice;
		if (i == 0 && alpha == palpha) {
			setPV(my, op, tabledepth, choice);
		}
	}
	percent = 100;

	int lowerbound = 
#ifdef USE_STABILITY
		lower;
#else
		-MAXSTEP;
#endif
	if (maxresult == lowerbound) {
		palpha = alpha - 1;
		maxptr = choice;
	}
	if (maxresult >= beta && pbeta == beta) 
		saveTp(zobPos, my, op, maxresult, MAXSTEP + 1, maxptr, tabledepth, 0);
	else if (palpha == alpha) 
		saveTp(zobPos, my, op, -MAXSTEP - 1, maxresult, maxptr, tabledepth, 0);
	else 
		saveTp(zobPos, my, op, maxresult, maxresult, maxptr, tabledepth, 0);
	return SolverResult(maxresult, choice);
}

int Solver::searchBigEat(int color, int alpha, int beta, int depth, bool lastFound) {
	if (depth == 0) {
		int bCount = bits(black);
		int wCount = bits(white);
		int diff = bCount - wCount;
		return (color == BLACK) ? diff : -diff;
	}
	BitBoard mob = (color == BLACK) ? mobility(black, white) : mobility(white, black);
	int pCount = bits(mob);
	int opColor = BLACK + WHITE - color;
	if (pCount == 0) 
		if (lastFound)
			return -searchBigEat(opColor, -beta, -alpha, depth, false);
		else {
			return getResultNoCount((color == BLACK) ? black : white);
		}
	int result, pptr = 0;
	for (int i = 0; i < MAXSTEP; i++)
		if (posTable[i] & mob) {
			makeMove(i, color);
			result = -searchBigEat(opColor, -beta, -alpha, depth - 1, true);
			unMakeMove();
			if (result >= beta) return result;
			if (result > alpha)
				alpha = result;
			pptr++;
		}
	return alpha;
}

SolverResult Solver::solveBigEat(int color, int depth) {
	int alpha = -MAXSTEP, beta = MAXSTEP;
	BitBoard mob = (color == BLACK) ? mobility(black, white) : mobility(white, black);
	int pCount = bits(mob);
	if (pCount == 0) {
		mob = (color == BLACK) ? mobility(white, black) : mobility(black, white);
		if (mob == 0)
			return SolverResult(getResult((color == BLACK) ? black : white), MAXSTEP);
		return SolverResult(
			-solveBigEat(color == BLACK ? WHITE : BLACK, depth).getResult(),
			MAXSTEP);
	}
	int pptr = 0, maxptr = 0;
	int opColor = BLACK + WHITE - color;
	int shuffle[MAXSTEP]; // introduce randomness into bigeat search
	for (int i = 0; i < MAXSTEP; i++) {
		shuffle[i] = i;
	}
	for (int i = 0; i < MAXSTEP; i++) {
		unsigned int rnd;
		rand_s(&rnd);
		rnd = rnd % (MAXSTEP - i);
		if (rnd == 0) continue;
		int tmp = shuffle[i];
		shuffle[i] = shuffle[i + rnd];
		shuffle[i + rnd] = tmp;
	}
	for (int j = 0; j < MAXSTEP; j++) {
		int i = shuffle[j];
		if (posTable[i] & mob) {
			makeMove(i, color);
			int result = -searchBigEat(opColor, -beta, -alpha, depth - 1, true);
			unMakeMove();
			if (result >= beta) {
				alpha = beta;
				maxptr = i;
				break;
			}
			if (result > alpha) {
				alpha = result;
				maxptr = i;
			}
			pptr++;
		}
	}
	return SolverResult(alpha, maxptr);
}

unsigned long long Solver::getEvNum() const {
	return evnum;
}

void Solver::abortSearch() {
	aborted = true;
	staticSolver->aborted = true;
}

void Solver::abortSearchComplete() {
	aborted = false;
	staticSolver->aborted = false;
	sortStackPtr = 0;
	staticSolver->sortStackPtr = 0;
}

int Solver::getPercent() const {
	return percent;
}

int Solver::getPartialResult() const {
	return partialResult;
}

int Solver::getPartialDepth() const {
	return partialDepth;
}

int Solver::getFocusedMove() const {
	return focusedMove;
}

int Solver::getSelectedMove() const {
	return selectedMove;
}

int Solver::getBookPercent() const {
	return percent + (subPercent * currentBlock / 100);
}

bool Solver::loadPatterns(std::string patternFile) {
	std::ifstream in(patternFile.c_str(), std::ios::binary | std::ios::in);
	if (!in) return false;
	float value;
	for (int i = 0; i < ALL_STAGE_PATTERN_COUNT; i++) {
		in.read((char*)&value, sizeof(value));
		if (!in) {
			in.close();
			return false;
		}
		patternValues[i] = (int)(value * RULER + 0.5f);
	}
	for (int i = 0; i < STAGES; i++) {
		in.read((char*)&value, sizeof(value));
		if (!in) {
			in.close();
			return false;
		}
		patternCorrection[i] = (int)(value + 0.5f);
	}
	in.close();
	return true;
	//System::IO::FileStream^ fs;
	//try {
	//	fs = 
	//		gcnew System::IO::FileStream(patternFile, System::IO::FileMode::Open, System::IO::FileAccess::Read);
	//} catch (System::Exception^) {
	//	System::Diagnostics::Debug::WriteLine("Cannot access file " + patternFile);
	//	System::Windows::Forms::MessageBox::Show("无法打开 " + patternFile + " 文件。\n请确认此文件存在于游戏目录中，然后再试一次。",
	//		"找不到数据文件", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Stop);
	//	exit(-1);
	//	return;
	//}
	//
	//System::IO::BinaryReader^ br = gcnew System::IO::BinaryReader(fs);
	//try {
	//	for (int i = 0; i < ALL_STAGE_PATTERN_COUNT; i++) {
	//		patternValues[i] = br->ReadSingle();
	//	}
	//	for (int i = 0; i < STAGES; i++) {
	//		patternCorrection[i] = br->ReadSingle();
	//	}
	//} catch (System::Exception^) {
	//	System::Diagnostics::Debug::WriteLine("Cannot access file " + patternFile);
	//	br->Close();
	//	System::Windows::Forms::MessageBox::Show("读取 " + patternFile + " 文件出错。\n请确认此文件的完整性，然后再试一次。",
	//		"读取数据文件出错", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Stop);
	//	exit(-1);
	//	return;
	//}
	//System::Diagnostics::Debug::WriteLine("Patterns loaded from " + patternFile);
	//br->Close();
}

bool Solver::initPatterns(std::string patternPath) {
	deltaSum = 0;
	occurance = 0;
	patternValues = new int[ALL_STAGE_PATTERN_COUNT];

	memset(myPV, 0, sizeof(myPV));
	//memset(opPV, 0, sizeof(opPV));
	memset(pPtr, 0, sizeof(pPtr));
	memset(pVCount, 0, sizeof(pVCount));
	memset(posDepend, 0, sizeof(posDepend));
	memset(patternDependCount, 0, sizeof(patternDependCount));
	int tempOffset[PATTERNS];
	int offset = 0;
	for (int i = 0; i < PATTERNS; i++) {
		tempOffset[i] = offset;
		offset += patternCount[i];
	}
	for (int i = 0; i < STAGES; i++) {
		offset = i * TOTAL_PATTERN_COUNT;
		for (int j = 0; j < ACTUAL_PATTERNS; j++) {
			patternOffset[i][j] = offset + tempOffset[patternPtr[j]];
		}
	}
	if (!loadPatterns(patternPath)) {
		delete[] patternValues;
		patternValues = NULL;
		return false;
	};
	Solver::patternPath = patternPath;
	{
		// line 1 + 2X
		addDependency(9, 0);
		multipleAdd(0, 1, 8, 0);
		addDependency(14, 0);

		addDependency(49, 1);
		multipleAdd(56, -8, 8, 1);
		addDependency(9, 1);

		addDependency(54, 2);
		multipleAdd(63, -1, 8, 2);
		addDependency(49, 2);

		addDependency(14, 3);
		multipleAdd(7, 8, 8, 3);
		addDependency(54, 3);

		// line 2
		multipleAdd(8, 1, 8, 4);
		multipleAdd(57, -8, 8, 5);
		multipleAdd(55, -1, 8, 6);
		multipleAdd(6, 8, 8, 7);

		// line 3
		multipleAdd(16, 1, 8, 8);
		multipleAdd(58, -8, 8, 9);
		multipleAdd(47, -1, 8, 10);
		multipleAdd(5, 8, 8, 11);

		// line 4
		multipleAdd(24, 1, 8, 12);
		multipleAdd(59, -8, 8, 13);
		multipleAdd(39, -1, 8, 14);
		multipleAdd(4, 8, 8, 15);

		// diag 8
		multipleAdd(0, 9, 8, 16);
		multipleAdd(56, -7, 8, 17);

		// diag 7
		multipleAdd(8, 9, 7, 18);
		multipleAdd(57, -7, 7, 19);
		multipleAdd(55, -9, 7, 20);
		multipleAdd(6, 7, 7, 21);

		// diag 6
		multipleAdd(16, 9, 6, 22);
		multipleAdd(58, -7, 6, 23);
		multipleAdd(47, -9, 6, 24);
		multipleAdd(5, 7, 6, 25);

		// diag 5
		multipleAdd(24, 9, 5, 26);
		multipleAdd(59, -7, 5, 27);
		multipleAdd(39, -9, 5, 28);
		multipleAdd(4, 7, 5, 29);
		
		// diag 4
		multipleAdd(32, 9, 4, 30);
		multipleAdd(60, -7, 4, 31);
		multipleAdd(31, -9, 4, 32);
		multipleAdd(3, 7, 4, 33);

		// 2x5 square
		multipleAdd(0, 1, 5, 34);
		multipleAdd(8, 1, 5, 34);

		multipleAdd(56, -8, 5, 35);
		multipleAdd(57, -8, 5, 35);

		multipleAdd(63, -1, 5, 36);
		multipleAdd(55, -1, 5, 36);
		
		multipleAdd(7, 8, 5, 37);
		multipleAdd(6, 8, 5, 37);

		multipleAdd(0, 8, 5, 38);
		multipleAdd(1, 8, 5, 38);

		multipleAdd(56, 1, 5, 39);
		multipleAdd(48, 1, 5, 39);

		multipleAdd(63, -8, 5, 40);
		multipleAdd(62, -8, 5, 40);

		multipleAdd(7, -1, 5, 41);
		multipleAdd(15, -1, 5, 41);

		// 3x3 square
		multipleAdd(0, 1, 3, 42);
		multipleAdd(8, 1, 3, 42);
		multipleAdd(16, 1, 3, 42);

		multipleAdd(56, -8, 3, 43);
		multipleAdd(57, -8, 3, 43);
		multipleAdd(58, -8, 3, 43);

		multipleAdd(63, -1, 3, 44);
		multipleAdd(55, -1, 3, 44);
		multipleAdd(47, -1, 3, 44);

		multipleAdd(7, 8, 3, 45);
		multipleAdd(6, 8, 3, 45);
		multipleAdd(5, 8, 3, 45);
	}

	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 8; j++)
			if (i & (1 << (7 - j)))
				for (int k = 0; k < 8; k++)
					for (int l = 0; l < ACTUAL_PATTERNS; l++) {
						myPV[i][k][l] += (unsigned short)posDepend[k * HEIGHT + j][l];
					}
	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 8; j++)
			for (int k = 0; k < ACTUAL_PATTERNS; k++)
				if (myPV[i][j][k] != 0) {
					pPtr[i][j][pVCount[i][j]] = (unsigned char)k;
					//opPV[i][j][pVCount[i][j]] = myPV[i][j][k] << 1;
					myPV[i][j][pVCount[i][j]++] = myPV[i][j][k];
				}
	return true;
}

void Solver::addDependency(int pos, int pInd) {
	int value = pow3[patternDependCount[pInd]++];
	posDepend[pos][pInd] = value;
}

void Solver::multipleAdd(int pos, int step, int count, int pInd) {
	for (int i = 0; i < count; i++) {
		addDependency(pos, pInd);
		pos += step;
	}
}

void Solver::reverseSingle(BitBoard& bb) {
	const unsigned long long mask1 = 0x0f0f0f0f0f0f0f0full;
	const unsigned long long mask2 = 0xf0f0f0f0f0f0f0f0ull;
	const unsigned long long mask3 = 0x3333333333333333ull;
	const unsigned long long mask4 = 0xccccccccccccccccull;
	const unsigned long long mask5 = 0x5555555555555555ull;
	const unsigned long long mask6 = 0xaaaaaaaaaaaaaaaaull;
	bb = ((bb & mask1) << 4) | ((bb & mask2) >> 4);
	bb = ((bb & mask3) << 2) | ((bb & mask4) >> 2);
	bb = ((bb & mask5) << 1) | ((bb & mask6) >> 1);
}

void Solver::reverseBoard() {
	for (int i = 0; i < stackptr; i++) {
		reverseSingle(bstack[i]);
		reverseSingle(wstack[i]);
		reversePos(mstack[i]);
	}
	reverseSingle(black);
	reverseSingle(white);
}

void Solver::reverseColor() {
	BitBoard tmp;
	for (int i = 0; i < stackptr; i++) {
		tmp = bstack[i];
		bstack[i] = wstack[i];
		wstack[i] = tmp;
	}
	tmp = black;
	black = white;
	white = tmp;
}

void Solver::flipPos(int dir, int& pos) {
	switch (dir) {
	case 1:
		pos = ((pos & 7) << 3) | (7 - (pos >> 3));
		break;
	case 2:
		pos = MAXSTEP - 1 - pos;
		break;
	case 3:
		pos = ((7 - (pos & 7)) << 3) | (pos >> 3);
	}
}

void Solver::reversePos(int& pos) {
	pos = (pos & 0x38) | (7 - (pos & 7));
}

void Solver::flipSingle(int dir, BitBoard& bb) {
	BitBoard result = 0;
	switch (dir) {
	case 1:
		for (int i = 0; i < MAXSTEP; i++) {
			if (posTable[i] & bb) {
				result |= posTable[((i & 7) << 3) | (7 - (i >> 3))];
			}
		}
		bb = result;
		break;
	case 2:
		for (int i = 0; i < MAXSTEP; i++) {
			if (posTable[i] & bb) {
				result |= posTable[MAXSTEP - 1 - i];
			}
		}
		bb = result;
		break;
	case 3:
		for (int i = 0; i < MAXSTEP; i++) {
			if (posTable[i] & bb) {
				result |= posTable[((7 - (i & 7)) << 3) | (i >> 3)];
			}
		}
		bb = result;
	}
}

void Solver::transformSingle(int pattern, BitBoard& bb) {
	switch (pattern) {
	case 1:
		flipVertical(bb);
		break;
	case 2:
		flipDiagA1H8(bb);
		break;
	case 3:
		flipDiagA8H1(bb);
		break;
	case 4:
		flipHorizonal(bb);
		break;
	case 5:
		flipHorizonal(bb);
		flipVertical(bb);
		break;
	case 6:
		flipHorizonal(bb);
		flipDiagA1H8(bb);
		break;
	case 7:
		flipHorizonal(bb);
		flipDiagA8H1(bb);
		break;
	default:
		break;
	}
}

void Solver::transformPos(int pattern, int& pos) {
	switch (pattern) {
	case 1:
		posFlipVertical(pos);
		break;
	case 2:
		posFlipDiagA1H8(pos);
		break;
	case 3:
		posFlipDiagA8H1(pos);
		break;
	case 4:
		posFlipHorizonal(pos);
		break;
	case 5:
		posFlipHorizonal(pos);
		posFlipVertical(pos);
		break;
	case 6:
		posFlipHorizonal(pos);
		posFlipDiagA1H8(pos);
		break;
	case 7:
		posFlipHorizonal(pos);
		posFlipDiagA8H1(pos);
		break;
	default:
		break;
	}
}

void Solver::flipBoard(int dir) {
	for (int i = 0; i < stackptr; i++) {
		flipSingle(dir, bstack[i]);
		flipSingle(dir, wstack[i]);
		flipPos(dir, mstack[i]);
	}
	flipSingle(dir, black);
	flipSingle(dir, white);
}

void Solver::parseGame(int board[], int steps[], int totalSteps, int fp) {
	setBoard(board);
	int cp = fp, ptr = 0;
	BitBoard mob;
	while (ptr < totalSteps) {
		mob = (cp == BLACK) ? mobility(black, white) : mobility(white, black);
		if (mob == 0) {
			cp = BLACK + WHITE - cp;
			mob = (cp == BLACK) ? mobility(black, white) : mobility(white, black);
			if (mob == 0) return;
		}
		if (steps[ptr] < 0) return;
		pstack[ptr] = cp;
		mstack[ptr] = steps[ptr];
		if (!(posTable[mstack[ptr]] & mob)) {
			stackptr = 0;
			return;
		}
		makeMove(steps[ptr++], cp);
		cp = BLACK + WHITE - cp;
	}
	if ((empties + bits(black) + bits(white) != MAXSTEP) || (black & white)) {
		stackptr = 0;
		return;
	}
}

void Solver::clearGame() {
	stackptr = 0;
}

int min(int a, int b) {
	return (a > b) ? b : a;
}

float abs(float x) {
	return (x < 0) ? (-x) : (x);
}

double Solver::getSmallerRnd(int rate) {
	unsigned int rnd;
	rand_s(&rnd);
	double random = (double)rnd / ((double)UINT_MAX + 1.0);
	double res = random;
	for (int i = 1; i < rate; i++) {
		res *= random;
	}
	return res;
}

SolverResult Solver::particularSolve(int color, int depth, int move) {
	BitBoard& my = (color == BLACK) ? black : white;
	BitBoard& op = (color == BLACK) ? white : black;
	//lookup the book
	BookNode node = book->get(my, op);
	if (node)
		if (node.getMoveCount())
			for (int i = 0; i < node.getMoveCount(); i++)
				if (node.getMove(i) == move)
					return SolverResult(-node.getEval(i), -move - 1);
	SolverResult result(0, 0);
	makeMove(move, my, op);
	if (depth == 1) result = SolverResult(strongEvaluate(op, my), move);
	else if (depth == 2) result = SolverResult(checkedSearch(op, my, depth - 1, -INFINITE, INFINITE, true), move);
	else {
		if (depth <= MPC_DEPTH_THRESHOLD)
			result = SolverResult(search(op, my, depth - 1, -MID_SEARCH_BOUND, MID_SEARCH_BOUND, true), move);
		else
			result = SolverResult(search_mpc(op, my, depth - 1, -MID_SEARCH_BOUND, MID_SEARCH_BOUND, true), move);
	}
	unMakeMove();
	return result;
}

SolverResult Solver::solve(int color, int depth) {
	return solve(color, depth, true);
}

SolverResult Solver::solve(int color, int depth, bool useBook) {
	sortStackPtr = 0;
	evnum = 0;
	percent = 0;
	partialResult = 0;
	partialDepth = 0;
	selectedMove = -1;
	focusedMove = -1;
	initPV();

	if (depth >= empties) { // switch to endgame search
		SolverResult endResult = solveExact(color, false);
		return SolverResult(
			endToMid(endResult.getResult()),
			endResult.getBestMove());
	}

	const int SORT_DEPTH = 4;
	const int MIN_ITER_START = 6;
	int alpha = -MID_SEARCH_BOUND, beta = MID_SEARCH_BOUND;
	BitBoard& my = (color == BLACK) ? black : white;
	BitBoard& op = (color == BLACK) ? white : black;
	BitBoard mob = mobility(my, op);
	int pCount = bits(mob);
	if (pCount == 0) { // out-of-move check
		mob = mobility(op, my);
		if (mob == 0) {
			int result = getResult(my);
			if (result > 0)
				return SolverResult(INFINITE - MAXSTEP + result, MAXSTEP);
			else if (result < 0)
				return SolverResult(-INFINITE + MAXSTEP + result, MAXSTEP);
			else return SolverResult(0, MAXSTEP);
		}
		return SolverResult(
			-solve(color == BLACK ? WHITE : BLACK, depth, useBook).getResult(),
			MAXSTEP);
	}
	//lookup the book
	if (useBook) {
		BookNode node = book->get(my, op);
		if (node)
			if (node.getMoveCount()) {
				if (node.getEval(0) > maxEval)
					maxEval = node.getEval(0);
				int thisTolerance = (int)(tolerance * getSmallerRnd(1));
				int minEval = (maxEval < INFINITE - MAXSTEP && maxEval > -INFINITE + MAXSTEP) 
					? (maxEval - thisTolerance)
					: maxEval;
				if (minEval > node.getEval(0)) minEval = node.getEval(0);
				int randomRange = 1;
				while (randomRange < node.getMoveCount()) {
					if (node.getEval(randomRange) < minEval) break;
					randomRange++;
				}
				unsigned int rnd;
				rand_s(&rnd);
				int moveIndex = rnd % randomRange;
				setBookPV(my, op, node.getMove(moveIndex));
				return SolverResult(node.getEval(moveIndex), -node.getMove(moveIndex) - 1);
			}
	}
	int maxresult = -INFINITE - 1, maxptr = -1;
	int palpha = alpha, pbeta = beta;
	int bestMove = -1;
	int adjustedIterStart = -1;
	int adjustedIterResult = 0;

	//using transposition table
	unsigned int flags = (depth > MPC_DEPTH_THRESHOLD) ? TP_MPC : 0;
	int zobPos = getZobKey(my, op) & currentTableMask;
	TPEntry* entry = &tp[zobPos];
	TPInfo* infoDeeper = &entry->deeper;
	if (infoDeeper->my == my && infoDeeper->op == op)
		if (infoDeeper->depth == depth && ((infoDeeper->flags & TP_MPC) == 0 || (flags & TP_MPC))) {
			if (infoDeeper->lower == infoDeeper->upper) {
				setPV(my, op, depth, infoDeeper->pos);
				return SolverResult(infoDeeper->lower, infoDeeper->pos);
			}
			if (infoDeeper->upper < beta) beta = infoDeeper->upper;
			if (infoDeeper->lower >= beta) {
				setPV(my, op, depth, infoDeeper->pos);
				return SolverResult(infoDeeper->lower, infoDeeper->pos);
			}
			if (infoDeeper->lower > alpha) {
				alpha = infoDeeper->lower;
				maxresult = alpha;
				maxptr = infoDeeper->pos;
			}
		} else if ((infoDeeper->depth >= MIN_ITER_START) && (infoDeeper->depth < depth) 
			&& (infoDeeper->lower == infoDeeper->upper)) { //adjust iterative deepening
				bestMove = infoDeeper->pos;
				adjustedIterStart = infoDeeper->depth + 1;
				adjustedIterResult = infoDeeper->lower;
		}
	TPInfo* infoNewer = &entry->newer;
	if (infoNewer->my == my && infoNewer->op == op)
		if (infoNewer->depth == depth && ((infoNewer->flags & TP_MPC) == 0 || (flags & TP_MPC))) {
			if (infoNewer->lower == infoNewer->upper) {
				setPV(my, op, depth, infoNewer->pos);
				return SolverResult(infoNewer->lower, infoNewer->pos);
			}
			if (infoNewer->upper < beta) beta = infoNewer->upper;
			if (infoNewer->lower >= beta) {
				setPV(my, op, depth, infoNewer->pos);
				return SolverResult(infoNewer->lower, infoNewer->pos);
			}
			if (infoNewer->lower > alpha) {
				alpha = infoNewer->lower;
				maxresult = alpha;
				maxptr = infoNewer->pos;
			}
		} else if ((infoNewer->depth >= MIN_ITER_START) && (infoNewer->depth < depth) 
			&& (infoNewer->lower == infoNewer->upper) && (infoNewer->depth >= adjustedIterStart)) { //adjust iterative deepening
				bestMove = infoNewer->pos;
				adjustedIterStart = infoNewer->depth + 1;
				adjustedIterResult = infoNewer->lower;
		}
	int pptr = 0;
	int opColor = BLACK + WHITE - color;
	int pre_stackptr = stackptr;
	memset(results, 0, sizeof(results));
	memset(positions, 0, sizeof(positions));

#ifdef SYMMETRIC_PRUNING
	bool is_symmetric[SYMMETRICS];
	// for symmetric positions, some moves are not searched at all
	for (int sym = 1; sym < SYMMETRICS; sym++) {
		is_symmetric[sym] = false;
		BitBoard work_my = my;
		BitBoard work_op = op;
		transformSingle(sym, work_my);
		if (work_my != my) continue;
		transformSingle(sym, work_op);
		if (work_op != op) continue;

		is_symmetric[sym] = true;
	}
#endif

	// sort the moves
	if (bestMove != -1)
		positions[pptr++] = bestMove;
	for (int i = 0; i < MAXSTEP; i++)
		if ((orderTable[i] & mob) && (moveOrder[i] != bestMove)) {
#ifdef SYMMETRIC_PRUNING
			bool prunned = false;
			for (int sym = 1; sym < SYMMETRICS; sym++) {
				if (is_symmetric[sym]) {
					int sym_move = moveOrder[i];
					transformPos(sym, sym_move);
					for (int j = 0; j < pptr; j++) {
						if (positions[j] == sym_move) {
							prunned = true;
							// still keep track of the prunned move for ETC use,
							// move ordering here is not important
							positions[--pCount] = moveOrder[i];
							if (moveOrder[i] == maxptr) {
								maxptr = sym_move;
							}
							break;
						}
					}
					if (prunned) break;
				}
			}
			if (prunned) continue;
#endif
			positions[pptr] = moveOrder[i];
			if (depth <= SORT_DEPTH) {
				pptr++; continue;
			}
			makeMove(moveOrder[i], my, op);
			results[pptr] = -search(op, my, SORT_DEPTH - 1, -INFINITE, INFINITE, true);
			if (aborted) {
				restoreStack(pre_stackptr);
				if (maxptr != -1)
					return SolverResult(maxresult, maxptr);
				else
					return SolverResult(0, positions[0]);
			}
			unMakeMove();
			pptr++;
		}
	if (depth > SORT_DEPTH) {
		int tmp;
		for (int i = ((bestMove == -1) ? 0 : 1); i < pCount - 1; i++)
			for (int j = i + 1; j < pCount; j++)
				if (results[i] < results[j]) {
					tmp = results[i]; results[i] = results[j]; results[j] = tmp;
					tmp = positions[i]; positions[i] = positions[j]; positions[j] = tmp;
				}
	}

	partialDepth = SORT_DEPTH;
	partialResult = results[0];
	selectedMove = positions[0];

#ifdef USE_ETC_AT_ROOT
	// using enhanced transposition cutoff
#ifdef SYMMETRIC_PRUNING
	// access full range of moves to utilize most of the transposition table
	int totalCount = bits(mob);
	for (int i = 0; i < totalCount; i++) {
#else
	for (int i = 0; i < pCount; i++) {
#endif
		int pos = positions[i];
		makeMove(pos, my, op);
		// Enhanced Transpositon Cutoff
		int zobKey2 = getZobKey(op, my);
		TPEntry* entry2 = &tp[zobKey2 & currentTableMask];
		TPInfo* info = &entry2->deeper;
		if (info->my == op && info->op == my && info->depth == depth - 1
			&& ((info->flags & TP_MPC) == 0 || (flags & TP_MPC)) && -info->upper > alpha) {
			alpha = -info->upper;
			maxresult = alpha;
			maxptr = pos;
			if (alpha >= beta) {
				unMakeMove();
				setPV(my, op, depth, maxptr);
				return SolverResult(alpha, maxptr);
			}
		}
		info = &entry2->newer;
		if (info->my == op && info->op == my && info->depth == depth - 1
			&& ((info->flags & TP_MPC) == 0 || (flags & TP_MPC)) && -info->upper > alpha) {
			alpha = -info->upper;
			maxresult = alpha;
			maxptr = pos;
			if (alpha >= beta) {
				unMakeMove();
				setPV(my, op, depth, maxptr);
				return SolverResult(alpha, maxptr);
			}
		}
		unMakeMove();
	}
#endif

	//using iterative deepening
	if (adjustedIterStart != -1) {
		partialDepth = adjustedIterStart - 1;
		partialResult = adjustedIterResult;
		selectedMove = positions[0];
	}

	setPV(my, op, partialDepth, positions[0]);
#ifdef USE_EVEN_DEEPENING
	const int ITER_START = (adjustedIterStart == -1) ? (MIN_ITER_START - ((MIN_ITER_START ^ depth) & 1)) : (adjustedIterStart + ((adjustedIterStart ^ depth) & 1));
	const int ITER_END = depth - 2;
#else
	const int ITER_START = (adjustedIterStart == -1) ? MIN_ITER_START : adjustedIterStart;
	const int ITER_END = depth - 1;
#endif
	int iteralpha, iterbeta = MID_SEARCH_BOUND;
	int itermax;
#ifdef USE_EVEN_DEEPENING
	for (int iterdepth = ITER_START; iterdepth <= ITER_END; iterdepth += 2) {
#else
	for (int iterdepth = ITER_START; iterdepth <= ITER_END; iterdepth++) {
#endif
#ifdef USE_EVEN_DEEPENING
		percent = (iterdepth - ITER_START) * 30 / (ITER_END + 2 - ITER_START);
#else
		percent = (iterdepth - ITER_START) * 30 / (ITER_END + 1 - ITER_START);
#endif
		iteralpha = -MID_SEARCH_BOUND;
		itermax = -INFINITE;
		for (int i = 0; i < pptr; i++) {
#ifdef USE_EVEN_DEEPENING
			percent = (iterdepth - ITER_START) * 30 / (ITER_END + 2 - ITER_START)
				+ 2 * i * 30 / ((ITER_END + 2 - ITER_START) * pptr);
#else
			percent = (iterdepth - ITER_START) * 30 / (ITER_END + 1 - ITER_START)
				+ i * 30 / ((ITER_END + 1 - ITER_START) * pptr);
#endif
			int pos = positions[i];
			focusedMove = pos;
			makeMove(pos, my, op);
#ifdef MIDGAME_USE_NEGASCOUT
			if (i != 0 && alphacuttable(iteralpha) && (iterbeta - iteralpha >= MID_WINDOW_SIZE)) {
#else
			if ((itermax > -INFINITE) && (iterbeta - iteralpha >= MID_WINDOW_SIZE)) { // it's a PV node
#endif
				if (iterdepth > MPC_DEPTH_THRESHOLD) {
					//try a 0-window search, false cutoff fix
					results[i] = -search_mpc(op, my, iterdepth - 1, -iteralpha, -iteralpha + 1, true);
				} else {
					//try a 0-window search
					results[i] = -search(op, my, iterdepth - 1, -iteralpha - 1, -iteralpha, true);
				}
				if (aborted) {
					restoreStack(pre_stackptr);
					return SolverResult(partialResult, selectedMove);
				}
				if (results[i] < iteralpha
					|| (iterdepth <= MPC_DEPTH_THRESHOLD && results[i] == iteralpha)) { 
					// no better evaluations, throw away the step
					unMakeMove();
					if (results[i] > results[0]) {
						itermax = results[i];
						int tmp = results[i], tmp2 = positions[i];
						for (int j = i - 1; j >= 0; j--) {
							results[j + 1] = results[j];
							positions[j + 1] = positions[j];
						}
						results[0] = tmp; positions[0] = tmp2;
						setPV(my, op, iterdepth, positions[0]);
						partialResult = results[0];
						selectedMove = positions[0];
					}
					continue;
				}
				if (results[i] >= iterbeta) {
					unMakeMove();
					percent = 100;
					setPV(my, op, iterdepth, pos);
					return SolverResult(results[i], pos);
				}
				if (results[i] > iteralpha) { // if result == alpha, might be mistakenly cutoff by mpc
					itermax = iteralpha = results[i];
					selectedMove = focusedMove;
				}
			}

			// otherwise, return to normal search
			if (iteralpha < iterbeta) {
				results[i] = (iterdepth > MPC_DEPTH_THRESHOLD) 
					? -search_mpc(op, my, iterdepth - 1, -iterbeta, -iteralpha, true)
					: -search(op, my, iterdepth - 1, -iterbeta, -iteralpha, true);
				if (aborted) {
					restoreStack(pre_stackptr);
					return SolverResult(partialResult, selectedMove);
				}
			}

			unMakeMove();
			if (results[i] >= iterbeta) {
				percent = 100;
				setPV(my, op, iterdepth, pos);
				return SolverResult(results[i], pos);
			}
			if (results[i] > itermax) {
				itermax = results[i];
				if (results[i] > iteralpha)
					iteralpha = results[i];
			}
			if (results[i] > results[0]) {
				int tmp = results[i], tmp2 = positions[i];
				for (int j = i - 1; j >= 0; j--) {
					results[j + 1] = results[j];
					positions[j + 1] = positions[j];
				}
				results[0] = tmp; positions[0] = tmp2;
				setPV(my, op, iterdepth, positions[0]);
			}
			partialDepth = iterdepth;
			partialResult = results[0];
			selectedMove = positions[0];
			if (i == 0) {
				setPV(my, op, iterdepth, positions[0]);
			}
		}
		if (itermax <= -MID_SEARCH_BOUND) {
			percent = 100;
			return SolverResult(itermax, positions[0]);
		}
	}
	percent = 30;

	if (maxptr != -1) {
		partialDepth = depth;
		partialResult = maxresult;
		selectedMove = maxptr;
		setPV(my, op, depth, maxptr);
	} else {
		maxptr = positions[0];
	}
	if (alpha >= beta) {
		percent = 100;
		return SolverResult(beta, maxptr); // only way to achieve alpha >= beta is to reduce beta
	}
	for (int i = 0; i < pptr; i++) {
		percent = 30 + i * 70 / pptr;
		int pos = positions[i];
		focusedMove = pos;
		makeMove(pos, my, op);
		if (depth == 1)
			results[i] = -strongEvaluate(op, my);
		else if (depth == 2)
			results[i] = -checkedSearch(op, my, depth - 1, -beta, -alpha, true);
		else {
			results[i] = alpha;
#ifdef MIDGAME_USE_NEGASCOUT
			if ((depth >= 4) && (i != 0) && alphacuttable(alpha) && (beta - alpha >= MID_WINDOW_SIZE)) {
#else
			if ((depth >= 4) && (maxresult > palpha) && (beta - alpha >= MID_WINDOW_SIZE)) { // it's a PV node
#endif
				if (depth > MPC_DEPTH_THRESHOLD) {
					// try a 0-window search, false cutoff fix
					results[i] = -search_mpc(op, my, depth - 1, -alpha, -alpha + 1, true);
				} else {
					// try a 0-window search
					results[i] = -search(op, my, depth - 1, -alpha - 1, -alpha, true);
				}
				if (aborted) {
					restoreStack(pre_stackptr);
					return SolverResult(partialResult, selectedMove);
				}
				if (results[i] < alpha
					|| (depth <= MPC_DEPTH_THRESHOLD && results[i] == alpha)) { 
					// no better evaluations, throw away the step
					unMakeMove();
					if (results[i] > maxresult) {
						maxresult = results[i];
						maxptr = pos;
						partialResult = maxresult;
						selectedMove = maxptr;
						setPV(my, op, depth, maxptr);
					}
					continue;
				}
				if (results[i] >= beta) {
					unMakeMove();
					maxresult = results[i];
					maxptr = pos;
					setPV(my, op, depth, maxptr);
					break;
				}
				if (results[i] > alpha) { // if result == alpha, might be mistakenly cutoff by mpc
					selectedMove = maxptr;
				}
			}

			// otherwise, return to normal search
			results[i] = (depth > MPC_DEPTH_THRESHOLD)
				? -search_mpc(op, my, depth - 1, -beta, -results[i], true)
				: -search(op, my, depth - 1, -beta, -results[i], true);
		}
		if (aborted) {
			restoreStack(pre_stackptr);
			return SolverResult(partialResult, selectedMove);
		}
		unMakeMove();
		// maxresult will always be increased at least once
		if (results[i] > maxresult) {
			maxresult = results[i];
			maxptr = pos;
			setPV(my, op, depth, maxptr);
			if (maxresult >= beta) {
				break;
			}
			if (maxresult > alpha) {
				alpha = maxresult;
			}
		}
		partialDepth = depth;
		partialResult = maxresult;
		selectedMove = maxptr;
	}
	percent = 100;

	if (maxresult >= beta && pbeta == beta) 
		saveTp(zobPos, my, op, maxresult, INFINITE + 1, maxptr, depth, flags);
	else if (palpha == alpha) 
		saveTp(zobPos, my, op, -INFINITE - 1, maxresult, maxptr, depth, flags);
	else 
		saveTp(zobPos, my, op, maxresult, maxresult, maxptr, depth, flags);
	return SolverResult(maxresult, maxptr);
}

int Solver::checkedSearch(BitBoard &my, BitBoard &op, int depth, int alpha, int beta, bool lastFound) {
	int palpha = alpha, pbeta = beta;
	int result, maxresult = -INFINITE - 1;
	int maxptr = -1;
	BitBoard mob = mobility(my, op);

#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif

	if (mob == 0) {
		if (lastFound)
			return -checkedSearch(op, my, depth, -beta, -alpha, false);
		else {
#ifdef COUNT_INTERNAL_NODES
			return endToMid(getResultNoCount(my));
#else
			return endToMid(getResult(my));
#endif
		}
	}

	if (depth == 0) {
		return evaluate(my, op);
	}


	int pSortStackPtr = sortStackPtr;
	for (int i = 0; i < MAXSTEP; i++) {
		if (orderTable[i] & mob) {
			sortStack[sortStackPtr++] = moveOrder[i];
		}
	}
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {
		int current = sortStack[i];
		makeMove(current, my, op);
		result = -checkedSearch(op, my, depth - 1, -beta, -alpha, true);
		if (aborted) return 0;
		unMakeMove();
		if (result >= beta) {
			sortStackPtr = pSortStackPtr;
			return result;
		}
		if (result > maxresult) {
			maxresult = result;
			maxptr = current;
			if (result > alpha)
				alpha = result;
		}
	}
	sortStackPtr = pSortStackPtr;
	return maxresult;
}

int Solver::fastSearch(BitBoard& my, BitBoard& op, int depth, int alpha, int beta, bool lastFound) {
#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif
	int result, maxresult = -INFINITE - 1;
	BitBoard mob = mobility(my, op);

	if (mob == 0) {
		if (lastFound)
			return -fastSearch(op, my, depth, -beta, -alpha, false);
		else {
#ifdef COUNT_INTERNAL_NODES
			return endToMid(getResultNoCount(my));
#else
			return endToMid(getResult(my));
#endif
		}
	}


	for (int i = 0; i < MAXSTEP; i++) if (orderTable[i] & mob) {
		int current = moveOrder[i];
		makeMove(current, my, op);
		result = (depth > 1) ? (-fastSearch(op, my, depth - 1, -beta, -alpha, true))
			: -evaluate(op, my);
		unMakeMove();
		if (result >= beta) {
			return result;
		}
		if (result > maxresult) {
			maxresult = result;
			if (result > alpha)
				alpha = result;
		}
	}
	return maxresult;
}

inline int Solver::endToMid(int value) {
	if (value > 0) return INFINITE - MAXSTEP + value;
	else if (value < 0) return -INFINITE + MAXSTEP + value;
	else return 0;
}

int Solver::getMidSortSearchDepth(int depth) {
	return 1;
	//if (depth <= 8) return 1;
	//int d = depth - 8;
	//if (d > 6) d = 6 - (depth & 1);
	//return d;
}

int Solver::search(BitBoard& my, BitBoard& op, int depth, int alpha, int beta, bool lastFound) {
	if (depth == 0) {
		return evaluate(my, op);
	}
#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif
	TPInfo* infoNewer;
	TPInfo* infoDeeper;
	int palpha = alpha, pbeta = beta;
	int result, maxresult = -INFINITE - 1;
	int bestMove = -1, bestMove2 = -1, maxptr = -1;
	int (Solver::*searchFunction)(BitBoard&, BitBoard&, int, int, int, bool);
	searchFunction = (depth > MID_USE_SORT_DEPTH) ? (&Solver::search) : (&Solver::fastSearch);
	// using transposition table
	int zobPos = getZobKey(my, op) & currentTableMask;
	TPEntry* entry = &tp[zobPos];
	infoDeeper = &entry->deeper;
	if (infoDeeper->my == my && infoDeeper->op == op) {
		if (infoDeeper->depth == depth && !(infoDeeper->flags & TP_MPC)) { // force no mpc
			if (infoDeeper->lower >= beta)
				return infoDeeper->lower;
			if (infoDeeper->upper <= alpha)
				return infoDeeper->upper;
			if (infoDeeper->lower > alpha) {
				alpha = infoDeeper->lower;
				maxresult = alpha;
				maxptr = infoDeeper->pos;
			}
			if (infoDeeper->upper < beta) {
				beta = infoDeeper->upper;
			}
		}
		if (infoDeeper->lower < MID_WIPEOUT_THRESHOLD)
			bestMove = infoDeeper->pos;
	}

	infoNewer = &entry->newer;
	if (infoNewer->my == my && infoNewer->op == op) {
		if (infoNewer->depth == depth && !(infoNewer->flags & TP_MPC)) { // force no mpc
			if (infoNewer->lower >= beta)
				return infoNewer->lower;
			if (infoNewer->upper <= alpha)
				return infoNewer->upper;
			if (infoNewer->lower > alpha) {
				alpha = infoNewer->lower;
				maxresult = alpha;
				maxptr = infoNewer->pos;
			}
			if (infoNewer->upper < beta) {
				beta = infoNewer->upper;
			}
		}
		if (infoNewer->pos != bestMove && infoNewer->lower < MID_WIPEOUT_THRESHOLD) {
			bestMove2 = infoNewer->pos;
		}
	}

	BitBoard mob = mobility(my, op);

	if (mob == 0) {
		if (lastFound)
			return -search(op, my, depth, -beta, -alpha, false);
		else {
#ifdef COUNT_INTERNAL_NODES
			return endToMid(getResultNoCount(my));
#else
			return endToMid(getResult(my));
#endif
		}
	}

	// check table moves first
	int pSortStackPtr = sortStackPtr;
	if (bestMove != -1) {
		sortStack[sortStackPtr++] = bestMove;
	}
	if (bestMove2 != -1) {
		sortStack[sortStackPtr++] = bestMove2;
	}
	int sortStart = sortStackPtr;
	sortStackPtr = pSortStackPtr + bits(mob);

#ifdef USE_ETC
	// add the rest of the moves
	int ptr = sortStart;
	for (int i = 0; i < MAXSTEP; i++) {
		if ((orderTable[i] & mob) && (moveOrder[i] != bestMove) && (moveOrder[i] != bestMove2)) {
			sortStack[ptr++] = moveOrder[i];
		}
	}
	// using enhanced transposition cutoff
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {
		int pos = sortStack[i];
		makeMove(pos, my, op);
		// Enhanced Transpositon Cutoff
		int zobKey2 = getZobKey(op, my);
		TPEntry* entry = &tp[zobKey2 & currentTableMask];
		TPInfo* info = &entry->deeper;
		if (info->my == op && info->op == my && info->depth == depth - 1
			&& !(info->flags & TP_MPC) && -info->upper > alpha) {
			alpha = -info->upper;
			maxresult = alpha;
			maxptr = pos;
			if (alpha >= beta) {
				unMakeMove();
				sortStackPtr = pSortStackPtr;
				return alpha;
			}
		}
		info = &entry->newer;
		if (info->my == op && info->op == my && info->depth == depth - 1
			&& !(info->flags & TP_MPC) && -info->upper > alpha) {
			alpha = -info->upper;
			maxresult = alpha;
			maxptr = pos;
			if (alpha >= beta) {
				unMakeMove();
				sortStackPtr = pSortStackPtr;
				return alpha;
			}
		}
		unMakeMove();
	}
#endif

	// sort & search combined
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {

		if (i == sortStart) {
#ifndef USE_ETC
			// add the rest of the moves
			int ptr = sortStart;
			for (int j = 0; j < MAXSTEP; j++) {
				if ((orderTable[j] & mob) && (moveOrder[j] != bestMove) && (moveOrder[j] != bestMove2)) {
					sortStack[ptr++] = moveOrder[j];
				}
			}
#endif
			for (int j = sortStart; j < sortStackPtr; j++) {
				makeMove(sortStack[j], my, op);
				sortResultStack[j] = -evaluate(op, my);
				if (sortResultStack[j] >= MID_WIPEOUT_THRESHOLD)
					sortResultStack[j] += MID_VERY_HIGH_EVAL_BONUS - weighted_mobility_bits(mobility(op, my)) * MID_WIPEOUT_MOBILITY_WEIGHT;
				unMakeMove();
			}
		}

		if (i >= sortStart) {
			int p = i;
			for (int j = i + 1; j < sortStackPtr; j++) {
				if (sortResultStack[j] > sortResultStack[p])
					p = j;
			}
			if (p != i) {
				int tmp = sortResultStack[i];
				sortResultStack[i] = sortResultStack[p];
				sortResultStack[p] = tmp;
				tmp = sortStack[i];
				sortStack[i] = sortStack[p];
				sortStack[p] = tmp;
			}
		}

		int current = sortStack[i];
		makeMove(current, my, op);

#ifdef MIDGAME_USE_NEGASCOUT
		if (i != pSortStackPtr && alphacuttable(alpha) && (beta - alpha >= MID_WINDOW_SIZE)) {
#else
		if ((alpha > palpha) && (beta - alpha >= MID_WINDOW_SIZE)) { // it's a PV node
#endif
			// try a 0-window search
			result = -(this->*searchFunction)(op, my, depth - 1, -alpha - 1, -alpha, true);
			if (aborted) return 0;
			if (result <= alpha) { // no better evaluations, throw away the step
				unMakeMove();
				if (result > maxresult) {
					maxresult = result;
				}
				continue;
			}
			if (result >= beta) {
				unMakeMove();
				if (beta == pbeta)
					saveTp(zobPos, my, op, result, INFINITE + 1, current, depth, 0);
				else
					saveTp(zobPos, my, op, result, result, current, depth, 0);
				sortStackPtr = pSortStackPtr;
				return result;
			}
			maxresult = alpha = result;
			maxptr = current;
		}

		// otherwise, return to normal search
		result = -(this->*searchFunction)(op, my, depth - 1, -beta, -alpha, true);
		if (aborted) return 0;
		unMakeMove();
		if (result >= beta) {
			if (beta == pbeta)
				saveTp(zobPos, my, op, result, INFINITE + 1, current, depth, 0);
			else
				saveTp(zobPos, my, op, result, result, current, depth, 0);
			sortStackPtr = pSortStackPtr;
			return result;
		}
		if (result > maxresult) {
			maxresult = result;
			if (result > alpha) {
				maxptr = current;
				alpha = result;
			}
		}
	}
	if (palpha == alpha)
		saveTp(zobPos, my, op, -INFINITE - 1, maxresult, maxptr, depth, 0);
	else
		saveTp(zobPos, my, op, maxresult, maxresult, maxptr, depth, 0);
	sortStackPtr = pSortStackPtr;
	return maxresult;
}

void Solver::setEmpties() {
	emptyHead = emptyList;
	BitBoard e = ~(black | white);
	int ptr = 0;
	emptyList[ptr].prev = NULL;
	for (int i = 0; i < MAXSTEP; i++)
		if (orderTable[i] & e) {
			ptr++;
			emptyList[ptr].prev = &emptyList[ptr - 1];
			emptyList[ptr].pos = moveOrder[i];
			emptyList[ptr - 1].succ = &emptyList[ptr];
			emptyPtr[moveOrder[i]] = &emptyList[ptr];
		}
	ptr++;
	emptyList[ptr].prev = &emptyList[ptr - 1];
	emptyList[ptr - 1].succ = &emptyList[ptr];
	emptyTail = &emptyList[ptr];
}

inline void Solver::makeMoveAndSetEmpties(EmptyNode* emptyPos, BitBoard &my, BitBoard &op) {
	emptyStack[stackptr] = emptyPos;
	makeMove(emptyPos->pos, my, op);
	emptyPos->succ->prev = emptyPos->prev;
	emptyPos->prev->succ = emptyPos->succ;
}

inline bool Solver::checkedMakeMoveAndSetEmpties(EmptyNode* emptyPos, BitBoard& my, BitBoard& op) {
	if (!(op & neighborhood[emptyPos->pos])) return false;
	bstack[stackptr] = black;
	wstack[stackptr] = white;
	bool result = checkedPutChess(emptyPos->pos, my, op);
	if (result) {
		empties--;
		emptyStack[stackptr] = emptyPos;
		stackptr++;
		emptyPos->succ->prev = emptyPos->prev;
		emptyPos->prev->succ = emptyPos->succ;
	}
	return result;
}

inline void Solver::unMakeMoveAndSetEmpties() {
	EmptyNode* emptyPos = emptyStack[stackptr - 1];
	emptyPos->succ->prev = emptyPos;
	emptyPos->prev->succ = emptyPos;
	unMakeMove();
}

void Solver::clearCache() {
	memset(tp, 0, sizeof(TPEntry) * currentTableSize);
}

bool Solver::checkTableSize(size_t tableSize) {
	return bitGet1s((unsigned int)tableSize) == 1;
}

void Solver::setCacheSize(size_t newCacheSize) {
	if (!checkTableSize(newCacheSize) || (currentTableSize == newCacheSize)) return;
	delete[] tp;
	tp = NULL;
	tp = new TPEntry[newCacheSize];
	currentTableSize = newCacheSize;
	currentTableMask = ((unsigned int)newCacheSize) - 1;
	clearCache();
}

size_t Solver::getCacheSize() {
	return currentTableSize;
}

bool Solver::loadBook(std::string bookFile) {
	std::ifstream in(bookFile.c_str(), std::ios::binary | std::ios::in);
	if (!in) {
		book = new Book();
		return true;
	}

	int bookSize;
	in.read((char*)&bookSize, sizeof(bookSize));
	if (!in || bookSize <= 0) {
		book = new Book();
		in.close();
		return true;
	}
	book = new Book(bookSize * 3 / 2);
	for (int i = 0; i < bookSize; i++) {
		BookNode node;
		BitBoard bb;
		char ch;
		int I;
		in.read((char*)&bb, sizeof(bb));
		node.setMy(bb);
		in.read((char*)&bb, sizeof(bb));
		node.setOp(bb);
		in.read(&ch, sizeof(ch));
		node.setMoveCount(ch);
		for (int j = 0; j < node.getMoveCount(); j++) {
			in.read(&ch, sizeof(ch));
			in.read((char*)&I, sizeof(I));
			node.setMove(j, ch);
			node.setEval(j, I);
		}
		if (!in) {
			delete book;
			book = NULL;
			in.close();
			return false;
		}
		book->insert(node);
	}
	in.close();
	return true;
	//System::IO::FileStream^ fs;
	//try {
	//	fs = 
	//		gcnew System::IO::FileStream(bookFile, System::IO::FileMode::Open, System::IO::FileAccess::Read);
	//} catch (System::Exception^) {
	//	book = new Book();
	//	return;
	//}
	//
	//System::IO::BinaryReader^ br = gcnew System::IO::BinaryReader(fs);
	//try {
	//	int bookSize = br->ReadInt32();
	//	if (bookSize <= 0) {
	//		book = new Book();
	//		return;
	//	}
	//	book = new Book(bookSize * 3 / 2);
	//	for (int i = 0; i < bookSize; i++) {
	//		BookNode node;
	//		node.setMy(br->ReadUInt64());
	//		node.setOp(br->ReadUInt64());
	//		node.setMoveCount(br->ReadSByte());
	//		for (int j = 0; j < node.getMoveCount(); j++) {
	//			node.setMove(j, br->ReadSByte());
	//			node.setEval(j, br->ReadInt32());
	//		}
	//		book->insert(node);
	//	}
	//} catch (System::Exception^) {
	//	br->Close();
	//	System::Windows::Forms::MessageBox::Show("读取 " + bookFile + " 文件出错。\n请确认此文件的完整性，然后再试一次。",
	//		"读取棋谱文件出错", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Stop);
	//	exit(-1);
	//	return;
	//}
	//System::Diagnostics::Debug::WriteLine("Book loaded from " + bookFile);
	//br->Close();
}

bool Solver::saveBook(std::string bookFile) {
	std::ofstream out(bookFile.c_str(), std::ios::binary | std::ios::out);
	if (!out) return false;
	int bookNodes = 0;
	for (int i = 0; i < book->getCapacity(); i++) {
		BookNode node = book->get(i);
		if (node) if (node.getMoveCount())
			bookNodes++;
	}
	out.write((char*)&bookNodes, sizeof(bookNodes));
	if (!out) {
		out.close();
		return false;
	}
	for (int i = 0; i < book->getCapacity(); i++) {
		BookNode node = book->get(i);
		if (node) if (node.getMoveCount()) {
			BitBoard bb = node.getMy();
			out.write((char*)&bb, sizeof(BitBoard));
			bb = node.getOp();
			out.write((char*)&bb, sizeof(BitBoard));
			char ch = node.getMoveCount();
			out.write((char*)&ch, sizeof(ch));
			for (int j = 0; j < node.getMoveCount(); j++) {
				ch = node.getMove(j);
				out.write((char*)&ch, sizeof(ch));
				int I = node.getEval(j);
				out.write((char*)&I, sizeof(int));
			}
			if (!out) {
				out.close();
				return false;
			}
		}
	}
	out.close();
	return true;

	//System::IO::FileStream^ fs;
	//try {
	//	fs = 
	//		gcnew System::IO::FileStream(bookFile, System::IO::FileMode::Create, System::IO::FileAccess::Write);
	//} catch (System::Exception^) {
	//	System::Windows::Forms::MessageBox::Show("写入 " + bookFile + " 文件出错。\n请确认您对其有写入权限。",
	//		"写入棋谱文件出错", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Exclamation);
	//	return;
	//}
	//System::IO::BinaryWriter^ bw = gcnew System::IO::BinaryWriter(fs);
	//try {
	//	int bookNodes = 0;
	//	for (int i = 0; i < book->getCapacity(); i++) {
	//		BookNode node = book->get(i);
	//		if (node) if (node.getMoveCount())
	//			bookNodes++;
	//	}
	//	bw->Write(bookNodes);
	//	for (int i = 0; i < book->getCapacity(); i++) {
	//		BookNode node = book->get(i);
	//		if (node) if (node.getMoveCount()) {
	//			bw->Write(node.getMy());
	//			bw->Write(node.getOp());
	//			bw->Write(System::SByte(node.getMoveCount()));
	//			for (int j = 0; j < node.getMoveCount(); j++) {
	//				bw->Write(System::SByte(node.getMove(j)));
	//				bw->Write(node.getEval(j));
	//			}
	//		}
	//	}
	//} catch (System::Exception^) {
	//	System::Windows::Forms::MessageBox::Show("写入 " + bookFile + " 文件出错。\n请确认您对其有写入权限。",
	//		"写入棋谱文件出错", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Exclamation);
	//	bw->Close();
	//	return;
	//}
	//System::Diagnostics::Debug::WriteLine("Book written to " + bookFile);
	//bw->Close();
	//isBookChanged = false;
}

bool Solver::saveBook() {
	if (!isBookChanged) return true;
	if (!saveBook(bookPath)) return false;
	isBookChanged = false;
	return true;
}

void Solver::setBookDepth(int depth) {
	bookDepth = depth;
}

void Solver::setBookEndDepth(int depth) {
	bookEndDepth = depth;
}

void Solver::expandNode(BookNode& node) {
	if (node.getMoveCount()) return;
	BitBoard mob = mobility(node.getMy(), node.getOp());
	if (mob == 0) return;
	int mobCount = bits(mob);
	int* result = new int[mobCount];
	int* moves = new int[mobCount];
	int pptr = 0;
	int localmax = -INFINITE;
	black = node.getMy();
	white = node.getOp();
	stackptr = 0;
	sortStackPtr = 0;
	empties = MAXSTEP - bits(black) - bits(white);
	if (empties > bookEndDepth) {
		int middepth = bookDepth - (empties & 1); // Make book evals look more stable
		for (int i = 0; i < MAXSTEP; i++)
			if (posTable[i] & mob) {
				makeMove(i, black, white);
				int eval;
				BookNode child = book->get(white, black);
				int value = (localmax > 0) ? localmax - EVAL_RANGE : -EVAL_RANGE;
				if (child) {
					if (child.getMoveCount())
						result[pptr] = eval = -child.getEval(0);
					else
						result[pptr] = eval = -search(white, black, middepth - 1, -INFINITE, 
							-value, true);
				}
				else
					result[pptr] = eval = -search(white, black, middepth - 1, -INFINITE, 
						-value, true);
				unMakeMove();
				moves[pptr] = i;
				if (eval > value) pptr++;
				if (eval > localmax) {
					localmax = eval;
					if (localmax > 0) {
						int old_pptr = pptr;
						pptr = 0;
						// reserve those nodes with evals > localmax - eval_range
						for (int i = 0; i < old_pptr; i++) {
							if (result[i] > localmax - EVAL_RANGE) {
								result[pptr] = result[i];
								moves[pptr] = moves[i];
								pptr++;
							}
						}
					}
				}
			}
	} else if (empties >= BOOK_ENDGAME_THRESHOLD) { //end game stuff
		setEmpties();
		int alpha = -MAXSTEP;
		result[0] = alpha;
		for (int i = 0; i < MAXSTEP; i++) {
			if (posTable[i] & mob) {
				makeMoveAndSetEmpties(emptyPtr[i], black, white);
				int eval = -searchExact(white, black, -MAXSTEP, min(-alpha + 1, MAXSTEP), true);
				unMakeMoveAndSetEmpties();
				if (eval > alpha) {
					result[0] = endToMid(eval);
					moves[0] = i;
					pptr = 1;
					alpha = eval;
				} else if (eval == alpha) {
					result[pptr] = result[0];
					moves[pptr] = i;
					pptr++;
				}
			}
		}
	}
	node.setMoveCount(pptr);
	for (int i = 0; i < pptr; i++) {
		node.setMove(i, moves[i]);
		node.setEval(i, result[i]);
	}
	if (pptr) {
		isBookChanged = true;
		connectChilds(node);
		node.sortByEval();
		propagateEval(node);
	}
	delete[] result;
	delete[] moves;
}

void Solver::connectChilds(BookNode& node) {
	int pptr = 0;
	for (int i = 0; i < node.getMoveCount(); i++) {
		BitBoard my = node.getMy();
		BitBoard op = node.getOp();
		putChess(node.getMove(i), my, op);
		BookNode child = book->get(op, my);
		if (child) {
			int oldFatherCount = child.getFatherCount();
			bool exist = false;
			for (int j = 0; j < oldFatherCount; j++)
				if (child.getFatherMy(j) == node.getMy() 
				&& child.getFatherOp(j) == node.getOp())
					exist = true;
			if (exist) continue;
			int newFatherCount = oldFatherCount + 1;
			child.setFatherCount(newFatherCount);
			child.setFatherMy(oldFatherCount, node.getMy());
			child.setFatherOp(oldFatherCount, node.getOp());
			child.setFatherMove(oldFatherCount, node.getMove(i));
			if (child.getMoveCount()) {
				node.setEvalSymmetric(i, -child.getEval(0));
			}
		}
	}
}

void Solver::propagateEval(const BookNode& from) {
	int eval = -from.getEval(0);
	for (int i = 0; i < from.getFatherCount(); i++) {
		BookNode father = book->get(from.getFatherMy(i), from.getFatherOp(i));
		int preEval = father.getEval(0);
		int j = 0;
		while (j < father.getMoveCount()) {
			if (father.getMove(j) == from.getFatherMove(i)) break;
			j++;
		}
		if (father.getEval(j) == eval) continue;
		father.setEvalSymmetric(j, eval);
		father.sortByEval();
		if (preEval != father.getEval(0))
			propagateEval(father);
	}
}

void Solver::addFather(const BookNode& node) {
	int moves[MAXSTEP], evals[MAXSTEP];
	// potential buffer overflow when input with malicious book.craft - fixed
	for (int i = 0; i < min(node.getMoveCount(), MAXSTEP); i++) {
		moves[i] = node.getMove(i);
		evals[i] = node.getEval(i);
	}
	BitBoard nodeMy = node.getMy();
	BitBoard nodeOp = node.getOp();
	for (int i = 0; i < min(node.getMoveCount(), MAXSTEP); i++) {
		BitBoard my = nodeMy;
		BitBoard op = nodeOp;
		putChess(moves[i], my, op);
		BookNode nextNode = book->get(op, my);
		if (nextNode) {
			int oldFatherCount = nextNode.getFatherCount();
			bool exist = false;
			for (int j = 0; j < oldFatherCount; j++)
				if (nextNode.getFatherMy(j) == nodeMy 
					&& nextNode.getFatherOp(j) == nodeOp) {
					exist = true;
					break;
				}
			if (exist) continue;
			int newFatherCount = oldFatherCount + 1;
			nextNode.setFatherCount(newFatherCount);
			nextNode.setFatherMy(oldFatherCount, nodeMy);
			nextNode.setFatherOp(oldFatherCount, nodeOp);
			nextNode.setFatherMove(oldFatherCount, moves[i]);
			if (nextNode.getMoveCount() && evals[i] != -nextNode.getEval(0)) {
				propagateEval(nextNode);
				isBookChanged = true;
			}
			if (newFatherCount == 1) addFather(nextNode);
		}
	}
	fatherAdded = true;
}

void Solver::setDefaultNode() {
	BookNode result;
	result.setMy(defaultMy);
	result.setOp(defaultOp);
	book->insert(result);
}

Solver::BookNode::BookNode() {
	node = new BasicBookNode();
	memset(node, 0, sizeof(BasicBookNode));
	transformState = 0;
}

Solver::BookNode::BookNode(BasicBookNode* node, int transformState) {
	this->node = node;
	this->transformState = transformState;
}

Solver::BookNode::operator void*() {
	return node;
}

BitBoard Solver::BookNode::getMy() const {
	return getTransformed(node->my);
}

BitBoard Solver::BookNode::getOp() const {
	return getTransformed(node->op);
}

int Solver::BookNode::getMoveCount() const {
	return node->moveCount;
}

int Solver::BookNode::getMove(int index) const {
	return getTransformed(node->moves[index]);
}

int Solver::BookNode::getEval(int index) const {
	return node->evals[index];
}

int Solver::BookNode::getFatherCount() const {
	return node->fatherCount;
}

BitBoard Solver::BookNode::getFatherMy(int index) const {
	return getTransformed(node->fatherMy[index]);
}

BitBoard Solver::BookNode::getFatherOp(int index) const {
	return getTransformed(node->fatherOp[index]);
}

int Solver::BookNode::getFatherMove(int index) const {
	return getTransformed(node->fatherMove[index]);
}

void Solver::BookNode::setMy(const BitBoard& my) {
	node->my = setTransformed(my);
}

void Solver::BookNode::setOp(const BitBoard& op) {
	node->op = setTransformed(op);
}

void Solver::BookNode::setMove(int index, int move) {
	node->moves[index] = setTransformed(move);
}

void Solver::BookNode::setEval(int index, int eval) {
	node->evals[index] = eval;
}

void Solver::BookNode::setFatherMy(int index, const BitBoard& fatherMy) {
	node->fatherMy[index] = setTransformed(fatherMy);
}

void Solver::BookNode::setFatherOp(int index, const BitBoard& fatherOp) {
	node->fatherOp[index] = setTransformed(fatherOp);
}

void Solver::BookNode::setFatherMove(int index, int fatherMove) {
	node->fatherMove[index] = setTransformed(fatherMove);
}

BitBoard Solver::BookNode::getTransformed(BitBoard bb) const {
	transformSingle(transformState, bb);
	return bb;
}

int Solver::BookNode::getTransformed(int pos) const {
	transformPos(transformState, pos);
	return pos;
}

BitBoard Solver::BookNode::setTransformed(BitBoard bb) const {
	transformSingle(antiPattern[transformState], bb);
	return bb;
}

int Solver::BookNode::setTransformed(int pos) const {
	transformPos(antiPattern[transformState], pos);
	return pos;
}

void Solver::BookNode::setMoveCount(int moveCount) {
	if (node->moveCount < moveCount) {
		char* newMoves = new char[moveCount];
		int* newEvals = new int[moveCount];
		for (int i = 0; i < node->moveCount; i++) {
			newMoves[i] = node->moves[i];
			newEvals[i] = node->evals[i];
		}
		delete[] node->moves;
		delete[] node->evals;
		node->moves = newMoves;
		node->evals = newEvals;
		node->moveCount = moveCount;
	}
}

void Solver::BookNode::setEvalSymmetric(int index, int eval) {
	setEval(index, eval);
	for (int i = 1; i < SYMMETRICS; i++) {
		BitBoard my = node->my;
		BitBoard op = node->op;
		transformSingle(i, my);
		transformSingle(i, op);
		if ((my == node->my) && (op == node->op)) {
			int move = node->moves[index];
			transformPos(i, move);
			for (int j = 0; j < node->moveCount; j++)
				if (node->moves[j] == move) {
					node->evals[j] = eval;
					break;
				}
		}
	}
}

void Solver::BookNode::sortByEval() {
	for (int i = 0; i < node->moveCount - 1; i++) {
		int maxptr = i;
		for (int j = i + 1; j < node->moveCount; j++)
			if (node->evals[j] > node->evals[maxptr])
				maxptr = j;
		if (maxptr == i) continue;
		int tmp = node->evals[i];
		node->evals[i] = node->evals[maxptr];
		node->evals[maxptr] = tmp;
		tmp = node->moves[i];
		node->moves[i] = node->moves[maxptr];
		node->moves[maxptr] = tmp;
	}
}

void Solver::BookNode::setFatherCount(int fatherCount) {
	if (node->fatherCount < fatherCount) {
		char* newFatherMove = new char[fatherCount];
		BitBoard* newFatherMy = new BitBoard[fatherCount];
		BitBoard* newFatherOp = new BitBoard[fatherCount];
		for (int i = 0; i < node->fatherCount; i++) {
			newFatherMove[i] = node->fatherMove[i];
			newFatherMy[i] = node->fatherMy[i];
			newFatherOp[i] = node->fatherOp[i];
		}
		delete[] node->fatherMove;
		delete[] node->fatherMy;
		delete[] node->fatherOp;
		node->fatherMove = newFatherMove;
		node->fatherMy = newFatherMy;
		node->fatherOp = newFatherOp;
		node->fatherCount = fatherCount;
	}
}

void Solver::Book::clear() {
	for (int i = 0; i < capacity; i++)
		if (nodes[i])
			delete nodes[i];
	delete[] nodes;
	nodes = new BasicBookNode*[capacity];
	memset(nodes, 0, sizeof(BasicBookNode*) * capacity);
	size = 0;
}

Solver::Book::~Book() {
	for (int i = 0; i < capacity; i++)
		if (nodes[i])
			delete nodes[i];
	delete[] nodes;
}

int Solver::Book::getSize() const {
	return size;
}

int Solver::Book::getCapacity() const {
	return capacity;
}

Solver::BookNode Solver::Book::get(int index) const {
	return BookNode(nodes[index]);
}

void Solver::Book::inflate() {
	int newCapacity = capacity * 2;
	BasicBookNode** newNodes = new BasicBookNode*[newCapacity];
	memset(newNodes, 0, sizeof(BasicBookNode*) * newCapacity);
	for (int i = 0; i < capacity; i++) {
		BasicBookNode* node = nodes[i];
		if (node) {
			unsigned int key = Solver::getZobKey(node->my, node->op);
			int pos = key % newCapacity;
			BasicBookNode* destNode = newNodes[pos];
			while (destNode) {
				pos++;
				if (pos == newCapacity) pos = 0;
				destNode = newNodes[pos];
			}
			newNodes[pos] = node;
		}
	}
	delete[] nodes;
	nodes = newNodes;
	capacity = newCapacity;
}

void Solver::Book::insert(const BookNode& node) {
	if (get(node.getMy(), node.getOp())) return;
	BasicBookNode* baseNode = node.node;
	unsigned int key = getZobKey(baseNode->my, baseNode->op);
	int pos = key % capacity;
	BasicBookNode* destNode = nodes[pos];
	while (destNode) {
		if (destNode->my == baseNode->my && destNode->op == baseNode->op) {
			nodes[pos] = baseNode;
			return;
		}
		pos++; if (pos == capacity) pos = 0;
		destNode = nodes[pos];
	}
	nodes[pos] = baseNode;
	size++;
	if (size > capacity * LOAD_FACTOR / 100)
		inflate();
}

Solver::BasicBookNode* Solver::Book::simpleGet(const BitBoard& my, const BitBoard& op) const {
	unsigned int key = getZobKey(my, op);
	int pos = key % capacity;
	BasicBookNode* node = nodes[pos];
	while (node) {
		if (my == node->my && op == node->op)
			return node;
		pos++; if (pos == capacity) pos = 0;
		node = nodes[pos];
	}
	return NULL;
}

Solver::BookNode Solver::Book::get(BitBoard my, BitBoard op) {
	BasicBookNode* res = simpleGet(my, op);
	if (res) return BookNode(res);
	for (int i = 1; i < SYMMETRICS; i++) {
		BitBoard newMy = my;
		BitBoard newOp = op;
		transformSingle(antiPattern[i], newMy);
		transformSingle(antiPattern[i], newOp);
		res = simpleGet(newMy, newOp);
		if (res) return BookNode(res, i);
	}
	return BookNode(NULL);
}

Solver::Book::Book(int capacity) {
	nodes = new BasicBookNode*[capacity];
	memset(nodes, 0, sizeof(BookNode*) * capacity);
	size = 0;
	this->capacity = capacity;
}

Solver::Book::Book() {
	this->Book::Book(DEFAULT_CAPACITY);
}

void Solver::extendBook() {
	extendingBook = true;

	percent = 0; currentBlock = 10; subPercent = 0;
	if (!fatherAdded)
		addFather(book->get(defaultMy, defaultOp));

	percent = 10; currentBlock = 90; subPercent = 0;
	extendSingle();

	percent = 100; currentBlock = 0; subPercent = 0;
	extendingBook = false;
}

void Solver::extendSingle() {
	subPercent = 0;
	int ptr = 0;
	while (ptr < stackptr - 1) {
		BookNode node = (pstack[ptr] == BLACK) ? book->get(bstack[ptr], wstack[ptr]) 
			: book->get(wstack[ptr], bstack[ptr]);
		if (node) {
			if (!node.getMoveCount())
				staticSolver->expandNode(node);
			int moveIndex = 0;
			while (moveIndex < node.getMoveCount()) {
				if (node.getMove(moveIndex) == mstack[ptr]) break;
				moveIndex++;
			}
			if (moveIndex == node.getMoveCount()) {
				ptr++;
				subPercent = ptr * 100 / (stackptr - 1);
				continue;
			}
			BitBoard tempMy = (pstack[ptr] == BLACK) ? wstack[ptr + 1] : bstack[ptr + 1];
			BitBoard tempOp = (pstack[ptr] == BLACK) ? bstack[ptr + 1] : wstack[ptr + 1];
			BookNode nextNode = book->get(tempMy, tempOp);
			if (!nextNode) {
				nextNode = BookNode();
				nextNode.setMy(tempMy);
				nextNode.setOp(tempOp);
				nextNode.setFatherCount(1);
				nextNode.setFatherMy(0, node.getMy());
				nextNode.setFatherOp(0, node.getOp());
				nextNode.setFatherMove(0, mstack[ptr]);
				book->insert(nextNode);
				staticSolver->expandNode(nextNode);
			} else {
				int fatherIndex = 0;
				while (fatherIndex < nextNode.getFatherCount()) {
					if (nextNode.getFatherMy(fatherIndex) == node.getMy() 
					&& nextNode.getFatherOp(fatherIndex) == node.getOp())
						break;
					fatherIndex++;
				}
				if (fatherIndex == nextNode.getFatherCount()) {
					int oldFatherCount = nextNode.getFatherCount();
					int newFatherCount = oldFatherCount + 1;
					nextNode.setFatherCount(newFatherCount);
					nextNode.setFatherMy(oldFatherCount, node.getMy());
					nextNode.setFatherOp(oldFatherCount, node.getOp());
					nextNode.setFatherMove(oldFatherCount, mstack[ptr]);
					if (nextNode.getMoveCount()) {
						isBookChanged = true;
						propagateEval(nextNode);
					}
				}
			}
		}
		ptr++;
		subPercent = ptr * 100 / (stackptr - 1);
	}
	subPercent = 100;
}

AnalyzeResult* Solver::getAnalyzeResult() {
	if (!analyzeResult) analyzeResult = new AnalyzeResult();
	return analyzeResult;
}

void Solver::analyzeGame(int midDepth, int wlStep, int endStep) {
	if (!analyzeResult) analyzeResult = new AnalyzeResult();
	analyzeResult->clear();
	analyzeResult->setFirstEmpty(empties + stackptr);
	bstack[stackptr] = black;
	wstack[stackptr] = white;
	staticSolver->setBookTolerance(0);
	percent = 0;
	for (int i = 0; i < stackptr; i++) {
		staticSolver->setBitBoard(bstack[i], wstack[i]);
		SolverResult res(0, 0), pres(0, 0);
		int empties = staticSolver->empties;
		if (empties > wlStep) {
			res = staticSolver->solve(pstack[i], midDepth);
		} else if (empties > endStep) {
			res = staticSolver->solveExact(pstack[i], true);
		} else {
			res = staticSolver->solveExact(pstack[i], false);
		}
		if (aborted) {
			return;
		}
		int playedMove;
		staticSolver->sortStackPtr = 0;
		if (empties > wlStep) {
			// To avoid the 'wrong cutoff' problem, first see if the player made the best(Craft thinks) move
			if (mstack[i] == res.getBestMove()) {
				pres = SolverResult(-res.getResult(), res.getBestMove());
			} else pres = staticSolver->particularSolve(pstack[i], midDepth, mstack[i]);
		} else {
			playedMove = mstack[i];
			staticSolver->setBitBoard(bstack[i + 1], wstack[i + 1]);
			//staticSolver->setEmpties();
			//BitBoard& my = (pstack[i] == WHITE) ? staticSolver->black : staticSolver->white;
			//BitBoard& op = (pstack[i] == WHITE) ? staticSolver->white : staticSolver->black;
			int op = BLACK + WHITE - pstack[i];
			if (empties > endStep) {
//#ifdef USE_MTD_F
//				pres = SolverResult(staticSolver->mtdExact(my, op, -1, 1, true), playedMove);
//#else
				pres = SolverResult(staticSolver->solveExact(op, true).getResult(), playedMove);
//#endif
			} else {
//#ifdef USE_MTD_F
//				pres = SolverResult(staticSolver->mtdExact(my, op, -MAXSTEP, MAXSTEP, true), playedMove);
//#else
				pres = SolverResult(staticSolver->solveExact(op, false).getResult(), playedMove);
//#endif
			}
		}
		if (aborted) {
			return;
		}
		percent = 100 * (i + 1) / stackptr;
		analyzeResult->addResult(res.getResult(), res.getBestMove(), -pres.getResult(), pres.getBestMove(), pstack[i]);
	}
	percent = 100;
}

inline AnalyzeResult::AnalyzeResult() { 
	moves = 0; 
}

inline void AnalyzeResult::addResult(int best, int bestMove, int played, int playedMove, int color) {
	bestEvals[moves] = best;
	bestMoves[moves] = bestMove;
	playedEvals[moves] = played;
	playedMoves[moves] = playedMove;
	player[moves] = color;
	moves++;
}

inline void AnalyzeResult::setFirstEmpty(int empty) {
	firstEmpty = empty;
}

inline void AnalyzeResult::clear() {
	moves = 0;
	firstEmpty = 0;
}

inline SolverResult::SolverResult(int r, int bm) : result(r), bestMove(bm) {}

void Solver::clearSearchStats() {
	evnum = 0;
	percent = 0;
	partialResult = 0;
	partialDepth = 0;
	selectedMove = -1;
	focusedMove = -1;
	pvLength = 0;
}

int Solver::search_mpc(BitBoard &my, BitBoard &op, int depth, int alpha, int beta, bool lastFound) {
	if (depth == 0) {
		return evaluate(my, op);
	}
#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif
	TPInfo* infoDeeper;
	TPInfo* infoNewer;
	int palpha = alpha, pbeta = beta;
	int result, maxresult = -INFINITE - 1;
	int bestMove = -1, bestMove2 = -1, maxptr = -1;
	int (Solver::*searchFunction)(BitBoard&, BitBoard&, int, int, int, bool);
	if (depth > MIN_MPC_DEPTH)
		searchFunction = &Solver::search_mpc;
	else if (depth > MID_USE_SORT_DEPTH)
		searchFunction = &Solver::search;
	else searchFunction = &Solver::fastSearch;
	// using transposition table
	int zobPos = getZobKey(my, op) & currentTableMask;
	TPEntry* entry = &tp[zobPos];
	infoDeeper = &entry->deeper;
	if (infoDeeper->my == my && infoDeeper->op == op) {
		if (infoDeeper->depth == depth) {
			if (infoDeeper->lower >= beta)
				return infoDeeper->lower;
			if (infoDeeper->upper <= alpha)
				return infoDeeper->upper;
			if (infoDeeper->lower > alpha) {
				alpha = infoDeeper->lower;
				maxresult = alpha;
				maxptr = infoDeeper->pos;
			}
			if (infoDeeper->upper < beta) {
				beta = infoDeeper->upper;
			}
		}
		if (infoDeeper->lower < MID_WIPEOUT_THRESHOLD)
			bestMove = infoDeeper->pos;
	}

	infoNewer = &entry->newer;
	if (infoNewer->my == my && infoNewer->op == op) {
		if (infoNewer->depth == depth) {
			if (infoNewer->lower >= beta)
				return infoNewer->lower;
			if (infoNewer->upper <= alpha)
				return infoNewer->upper;
			if (infoNewer->lower > alpha) {
				alpha = infoNewer->lower;
				maxresult = alpha;
				maxptr = infoNewer->pos;
			}
			if (infoNewer->upper < beta) {
				beta = infoNewer->upper;
			}
		}
		if (infoNewer->pos != bestMove && infoNewer->lower < MID_WIPEOUT_THRESHOLD) {
			bestMove2 = infoNewer->pos;
		}
	}

	BitBoard mob = mobility(my, op);

	if (mob == 0) {
		if (lastFound)
			return -search_mpc(op, my, depth, -beta, -alpha, false);
		else {
#ifdef COUNT_INTERNAL_NODES
			return endToMid(getResultNoCount(my));
#else
			return endToMid(getResult(my));
#endif
		}
	}

	// mpc happens here
	if (depth <= MAX_MPC_DEPTH) {
		int mpc_stage = MPC_STAGE[empties];
		for (int mpc_try = 0; mpc_try < MPC_TRIES; mpc_try++) {
			CutPair& pair = mpcThreshold[mpc_stage][depth][mpc_try];
			if (pair.shallowDepth == 0) break;
			if (betacuttable(beta)) {
				int shallow_beta = (int)((MPC_PERCENTILE[mpc_stage] * pair.sigma + beta - pair.b) * pair.a_r);
				int shallow_result = search(my, op, pair.shallowDepth, shallow_beta - 1, shallow_beta, lastFound);
				if (aborted) return 0;
				if (shallow_result >= shallow_beta)
					return beta;
			}
			if (alphacuttable(alpha)) {
				int shallow_alpha = (int)((-MPC_PERCENTILE[mpc_stage] * pair.sigma + alpha - pair.b) * pair.a_r);
				int shallow_result = search(my, op, pair.shallowDepth, shallow_alpha, shallow_alpha + 1, lastFound);
				if (aborted) return 0;
				if (shallow_result <= shallow_alpha)
					return alpha;
			}
		}
	}

	// check table moves first
	int pSortStackPtr = sortStackPtr;
	if (bestMove != -1) {
		sortStack[sortStackPtr++] = bestMove;
	}
	if (bestMove2 != -1) {
		sortStack[sortStackPtr++] = bestMove2;
	}
	int sortStart = sortStackPtr;
	sortStackPtr = pSortStackPtr + bits(mob);

#ifdef USE_ETC
	// add the rest of the moves
	int ptr = sortStart;
	for (int i = 0; i < MAXSTEP; i++) {
		if ((orderTable[i] & mob) && (moveOrder[i] != bestMove) && (moveOrder[i] != bestMove2)) {
			sortStack[ptr++] = moveOrder[i];
		}
	}
	// using enhanced transposition cutoff
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {
		int pos = sortStack[i];
		makeMove(pos, my, op);
		// Enhanced Transpositon Cutoff
		int zobKey2 = getZobKey(op, my);
		TPEntry* entry = &tp[zobKey2 & currentTableMask];
		TPInfo* info = &entry->deeper;
		if (info->my == op && info->op == my && info->depth == depth - 1
			&& -info->upper > alpha) {
			alpha = -info->upper;
			maxresult = alpha;
			maxptr = pos;
			if (alpha >= beta) {
				unMakeMove();
				sortStackPtr = pSortStackPtr;
				return alpha;
			}
		}
		info = &entry->newer;
		if (info->my == op && info->op == my && info->depth == depth - 1
			&& -info->upper > alpha) {
			alpha = -info->upper;
			maxresult = alpha;
			maxptr = pos;
			if (alpha >= beta) {
				unMakeMove();
				sortStackPtr = pSortStackPtr;
				return alpha;
			}
		}
		unMakeMove();
	}
#endif

	// sort & search combined
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {

		if (i == sortStart) {
#ifndef USE_ETC
			// add the rest of the moves
			int ptr = sortStart;
			for (int j = 0; j < MAXSTEP; j++) {
				if ((orderTable[j] & mob) && (moveOrder[j] != bestMove) && (moveOrder[j] != bestMove2)) {
					sortStack[ptr++] = moveOrder[j];
				}
			}
#endif
			for (int j = sortStart; j < sortStackPtr; j++) {
				makeMove(sortStack[j], my, op);
				sortResultStack[j] = -evaluate(op, my);
				if (sortResultStack[j] >= MID_WIPEOUT_THRESHOLD)
					sortResultStack[j] += MID_VERY_HIGH_EVAL_BONUS - weighted_mobility_bits(mobility(op, my)) * MID_WIPEOUT_MOBILITY_WEIGHT;
				unMakeMove();
			}
		}

		if (i >= sortStart) {
			int p = i;
			for (int j = i + 1; j < sortStackPtr; j++) {
				if (sortResultStack[j] > sortResultStack[p])
					p = j;
			}
			if (p != i) {
				int tmp = sortResultStack[i];
				sortResultStack[i] = sortResultStack[p];
				sortResultStack[p] = tmp;
				tmp = sortStack[i];
				sortStack[i] = sortStack[p];
				sortStack[p] = tmp;
			}
		}

		int current = sortStack[i];
		makeMove(current, my, op);
#ifdef MIDGAME_USE_NEGASCOUT
		if (i != pSortStackPtr && alphacuttable(alpha) && (beta - alpha >= MID_WINDOW_SIZE)) {
#else
		if ((alpha > palpha) && (beta - alpha >= MID_WINDOW_SIZE)) { // it's a PV node
#endif
			// try a 0-window search, false cutoff fix
			result = -(this->*searchFunction)(op, my, depth - 1, -alpha, -alpha + 1, true);
			if (aborted) return 0;
			if (result < alpha) { // no better evaluations, throw away the step
				unMakeMove();
				if (result > maxresult) {
					maxresult = result;
				}
				continue;
			}
			if (result >= beta) {
				unMakeMove();
				if (beta == pbeta)
					saveTp(zobPos, my, op, result, INFINITE + 1, current, depth, TP_MPC);
				else
					saveTp(zobPos, my, op, result, result, current, depth, TP_MPC);
				sortStackPtr = pSortStackPtr;
				return result;
			}
			if (result > alpha) { // if result == alpha, might be mistakenly cutoff by mpc
				maxresult = alpha = result;
				maxptr = current;
			}
		}

		// otherwise, return to normal search
		result = -(this->*searchFunction)(op, my, depth - 1, -beta, -alpha, true);
		if (aborted) return 0;
		unMakeMove();
		if (result >= beta) {
			if (beta == pbeta)
				saveTp(zobPos, my, op, result, INFINITE + 1, current, depth, TP_MPC);
			else
				saveTp(zobPos, my, op, result, result, current, depth, TP_MPC);
			sortStackPtr = pSortStackPtr;
			return result;
		}
		if (result > maxresult) {
			maxresult = result;
			if (result > alpha) {
				maxptr = current;
				alpha = result;
			}
		}
	}
	if (palpha == alpha)
		saveTp(zobPos, my, op, -INFINITE - 1, maxresult, maxptr, depth, TP_MPC);
	else
		saveTp(zobPos, my, op, maxresult, maxresult, maxptr, depth, TP_MPC);
	sortStackPtr = pSortStackPtr;
	return maxresult;
}

int Solver::searchExact_epc(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound) {
#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif
	int sortSearchDepth = getEndSortSearchDepth();
	// IN TRANSPOSITION TABLE no cuts happen when empties <= MIN_EPC_DEPTH
	int tabledepth = empties + ((empties <= MIN_EPC_DEPTH) ? EPC_STAGES : currentEpcStage);
	TPInfo* infoNewer;
	TPInfo* infoDeeper;
	int palpha = alpha, pbeta = beta;
	int result, maxresult = -INFINITE;
	int bestMove = -1, bestMove2 = -1, maxptr = -1;
	// using transposition table
	int zobPos = getZobKey(my, op) & currentTableMask;
	TPEntry* entry = &tp[zobPos];
	infoDeeper = &entry->deeper;
	if (infoDeeper->my == my && infoDeeper->op == op) {
		if (infoDeeper->depth >= tabledepth) {
			if (infoDeeper->lower >= beta)
				return infoDeeper->lower;
			if (infoDeeper->upper <= alpha)
				return infoDeeper->upper;
			if (infoDeeper->lower > alpha) {
				alpha = infoDeeper->lower;
				maxresult = alpha;
				maxptr = infoDeeper->pos;
			}
			if (infoDeeper->upper < beta)
				beta = infoDeeper->upper;
		} else if (infoDeeper->depth < empties) {
			if (infoDeeper->lower >= getEndSortSearchUpperBound(beta))
				return infoDeeper->lower - INFINITE + MAXSTEP;
			if (infoDeeper->lower > getEndSortSearchUpperBound(alpha)) {
				alpha = infoDeeper->lower - INFINITE + MAXSTEP;
				maxresult = alpha;
				maxptr = infoDeeper->pos;
			}
			if (infoDeeper->upper <= getEndSortSearchLowerBound1(alpha))
				return infoDeeper->upper + INFINITE - MAXSTEP;
			if (infoDeeper->upper < getEndSortSearchLowerBound1(beta))
				beta = infoDeeper->upper + INFINITE - MAXSTEP;
		}
		if (infoDeeper->depth >= sortSearchDepth && infoDeeper->lower < END_WIPEOUT_THRESHOLD)
			bestMove = infoDeeper->pos;
	}

	infoNewer = &entry->newer;
	if (infoNewer->my == my && infoNewer->op == op) {
		if (infoNewer->depth >= tabledepth) {
			if (infoNewer->lower >= beta)
				return infoNewer->lower;
			if (infoNewer->upper <= alpha)
				return infoNewer->upper;
			if (infoNewer->lower > alpha) {
				alpha = infoNewer->lower;
				maxresult = alpha;
				maxptr = infoNewer->pos;
			}
			if (infoNewer->upper < beta)
				beta = infoNewer->upper;
		} else if (infoNewer->depth < empties) {
			if (infoNewer->lower >= getEndSortSearchUpperBound(beta))
				return infoNewer->lower - INFINITE + MAXSTEP;
			if (infoNewer->lower > getEndSortSearchUpperBound(alpha)) {
				alpha = infoNewer->lower - INFINITE + MAXSTEP;
				maxresult = alpha;
				maxptr = infoNewer->pos;
			}
			if (infoNewer->upper <= getEndSortSearchLowerBound1(alpha))
				return infoNewer->upper + INFINITE - MAXSTEP;
			if (infoNewer->upper < getEndSortSearchLowerBound1(beta))
				beta = infoNewer->upper + INFINITE - MAXSTEP;
		}
		if (infoNewer->pos != bestMove && infoNewer->depth >= sortSearchDepth && infoNewer->lower < END_WIPEOUT_THRESHOLD)
			bestMove2 = infoNewer->pos;
	}

	BitBoard mob = mobility(my, op);

	if (mob == 0) {
		if (lastFound)
			return -searchExact_epc(op, my, -beta, -alpha, false);
		else return 
#ifdef COUNT_INTERNAL_NODES
			getResultNoCount(my);
#else
			getResult(my);
#endif
	}

#ifdef USE_STABILITY
	int lower, upper;
	calcStabilityBound(my, op, lower, upper);
	if (lower > alpha) {
		alpha = lower;
		if (alpha >= beta) {
			return alpha;
		}
		// must look for a move to set maxptr
		if (bestMove != -1)
			maxptr = bestMove;
		else if (bestMove2 != -1)
			maxptr = bestMove2;
		else for (EmptyNode* i = emptyHead->succ; i != emptyTail; i = i->succ) {
			int current = i->pos;
			if (posTable[current] & mob) {
				maxptr = current;
				break;
			}
		}
		maxresult = alpha;
	}
	if (upper < beta) {
		beta = upper;
		if (beta <= alpha)
			return beta;
	}
#endif

	// epc happens here
	if (empties <= MAX_EPC_DEPTH) {
		for (int epc_try = 0; epc_try < EPC_TRIES; epc_try++) {
			CutPair& pair = epcThreshold[empties][epc_try];
			if (pair.shallowDepth == 0) break;

			if (beta < MAXSTEP) {
				int shallow_beta = (int)((EPC_PERCENTILE[currentEpcStage] * pair.sigma + beta - pair.b) * pair.a_r);
				int shallow_result = search(my, op, pair.shallowDepth, shallow_beta - 1, shallow_beta, lastFound);
				if (aborted) return 0;
				if (shallow_result >= shallow_beta)
					return beta;
			}
			if (alpha > -MAXSTEP) {
				int shallow_alpha = (int)((-EPC_PERCENTILE[currentEpcStage] * pair.sigma + alpha - pair.b) * pair.a_r);
				int shallow_result = search(my, op, pair.shallowDepth, shallow_alpha, shallow_alpha + 1, lastFound);
				if (aborted) return 0;
				if (shallow_result <= shallow_alpha)
					return alpha;
			}
		}
	}

	int (Solver::*searchFunction)(BitBoard&, BitBoard&, int, int, bool);
	if (empties > MIN_EPC_DEPTH)
		searchFunction = &Solver::searchExact_epc;
	else
		searchFunction = &Solver::searchExact;

	// check table move first
	int pSortStackPtr = sortStackPtr;
	if (bestMove != -1)
		emptySortStack[sortStackPtr++] = emptyPtr[bestMove];
	if (bestMove2 != -1)
		emptySortStack[sortStackPtr++] = emptyPtr[bestMove2];
	int sortStart = sortStackPtr;
	sortStackPtr = pSortStackPtr + bits(mob);

#ifdef USE_ETC
	// add the rest of the moves
	int ptr = sortStart;
	for (EmptyNode* enode = emptyHead->succ; enode != emptyTail; enode = enode->succ) {
		int current = enode->pos;
		if ((posTable[current] & mob) && (current != bestMove) && (current != bestMove2)) {
			emptySortStack[ptr++] = enode;
		}
	}
	// using enhanced transposition cutoff
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {
		EmptyNode* pos = emptySortStack[i];
		makeMove(pos->pos, my, op);
		// Enhanced Transpositon Cutoff
		int zobKey2 = getZobKey(op, my);
		TPEntry* entry = &tp[zobKey2 & currentTableMask];
		TPInfo* info = &entry->deeper;
		if (info->my == op && info->op == my && info->depth >= tabledepth - 1
			&& -info->upper > alpha) {
				alpha = -info->upper;
				maxresult = alpha;
				maxptr = pos->pos;
				if (alpha >= beta) {
					unMakeMove();
					sortStackPtr = pSortStackPtr;
					return alpha;
				}
		}
		info = &entry->newer;
		if (info->my == op && info->op == my && info->depth >= tabledepth - 1
			&& -info->upper > alpha) {
				alpha = -info->upper;
				maxresult = alpha;
				maxptr = pos->pos;
				if (alpha >= beta) {
					unMakeMove();
					sortStackPtr = pSortStackPtr;
					return alpha;
				}
		}
		unMakeMove();
	}
#endif

	// sort & search combined
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {
		if (i == sortStart) {
#ifndef USE_ETC
			// add the rest of the moves
			int ptr = sortStart;
			for (EmptyNode* enode = emptyHead->succ; enode != emptyTail; enode = enode->succ) {
				int current = enode->pos;
				if ((posTable[current] & mob) && (current != bestMove) && (current != bestMove2)) {
					emptySortStack[ptr++] = enode;
				}
			}
#endif
			for (int j = sortStart; j < sortStackPtr; j++) {
				int current = emptySortStack[j]->pos;
				makeMove(current, my, op);
				sortResultStack[j] = result = (sortSearchDepth == 1) ?
					(-evaluate(op, my))
					: ((sortSearchDepth <= MID_USE_SORT_DEPTH) ? 
					-fastSearch(op, my, sortSearchDepth - 1, -getEndSortSearchUpperBound(beta), -getEndSortSearchLowerBound(alpha), true)
					: -search(op, my, sortSearchDepth - 1, -getEndSortSearchUpperBound(beta), -getEndSortSearchLowerBound(alpha), true));
				/*-fastSearch(op, my, 1, -INFINITE, INFINITE, true);*/
				if (aborted) return 0;
				if (result >= END_WIPEOUT_THRESHOLD)
					sortResultStack[j] += END_VERY_HIGH_EVAL_BONUS;
				if (empties <= END_FASTEST_FIRST_DEPTH) {
					if (result < getEndSortSearchUpperBound(0) && result > getEndSortSearchLowerBound1(0)) {
						sortResultStack[j] -= weighted_mobility_bits(mobility(op, my)) * 
							(result >= END_WIPEOUT_THRESHOLD) ? END_WIPEOUT_MOBILITY_WEIGHT : END_MOBILITY_WEIGHT;
					}
				} else if (result >= END_WIPEOUT_THRESHOLD) {
					sortResultStack[j] -= weighted_mobility_bits(mobility(op, my)) * (END_WIPEOUT_MOBILITY_WEIGHT - END_MOBILITY_WEIGHT);
				}
				unMakeMove();
				if (sortSearchDepth == 1) continue;
				int tmpRes = result - INFINITE + MAXSTEP;
				if (result >= getEndSortSearchUpperBound(beta)) {
					if (beta == pbeta)
						saveTp(zobPos, my, op, tmpRes, MAXSTEP + 1, current, tabledepth, 0);
					else
						saveTp(zobPos, my, op, tmpRes, tmpRes, current, tabledepth, 0);
					sortStackPtr = pSortStackPtr;
					return tmpRes;
				}
				if (result > getEndSortSearchUpperBound(alpha)) {
					alpha = tmpRes;
					maxresult = alpha;
					maxptr = current;
				}
			}
		}

		if (i >= sortStart) {
			int p = i;
			for (int j = i + 1; j < sortStackPtr; j++) {
				if (sortResultStack[j] > sortResultStack[p])
					p = j;
			}
			if (p != i) {
				int tmp = sortResultStack[i];
				sortResultStack[i] = sortResultStack[p];
				sortResultStack[p] = tmp;
				EmptyNode* tmpNode = emptySortStack[i];
				emptySortStack[i] = emptySortStack[p];
				emptySortStack[p] = tmpNode;
			}
		}

		int current = emptySortStack[i]->pos;
		makeMoveAndSetEmpties(emptySortStack[i], my, op);

		result = alpha;
#ifdef ENDGAME_USE_NEGASCOUT
		if (i != pSortStackPtr) { // it's not the first node
#else
		if ((alpha > palpha) && (beta - alpha >= END_WINDOW_SIZE)) { // it's a PV node
#endif
			// try a 0-window search
			result = -(this->*searchFunction)(op, my, -alpha - 2, -alpha, true);
			if (aborted) return 0;
			if (result <= alpha) { // no better evaluations, throw away the step
				unMakeMoveAndSetEmpties();
				if (result > maxresult) {
					maxresult = result;
				}
				continue;
			}
			if (result >= beta) {
				unMakeMoveAndSetEmpties();
				if (beta == pbeta)
					saveTp(zobPos, my, op, result, MAXSTEP + 1, current, tabledepth, 0);
				else
					saveTp(zobPos, my, op, result, result, current, tabledepth, 0);
				sortStackPtr = pSortStackPtr;
				return result;
			}
			//maxresult = alpha = result;
			//maxptr = current;
		}

		// otherwise, return to normal search
		result = -(this->*searchFunction)(op, my, -beta, -result, true);
		if (aborted) return 0;
		unMakeMoveAndSetEmpties();
		if (result >= beta) {
			if (beta == pbeta)
				saveTp(zobPos, my, op, result, MAXSTEP + 1, current, tabledepth, 0);
			else
				saveTp(zobPos, my, op, result, result, current, tabledepth, 0);
			sortStackPtr = pSortStackPtr;
			return result;
		}
		if (result > maxresult) {
			maxresult = result;
			if (result > alpha) {
				alpha = result;
				maxptr = current;
			}
		}
	}
	int lowerbound = 
#ifdef USE_STABILITY
		lower;
#else
		-MAXSTEP;
#endif
	if (maxresult == lowerbound) {
		palpha = alpha - 1;
		maxptr = emptySortStack[pSortStackPtr]->pos;
	}
	if (palpha == alpha)
		saveTp(zobPos, my, op, -MAXSTEP - 1, maxresult, maxptr, tabledepth, 0);
	else
		saveTp(zobPos, my, op, maxresult, maxresult, maxptr, tabledepth, 0);
	sortStackPtr = pSortStackPtr;
	return maxresult;
}

inline bool Solver::notAccrossBorder(int i, int j, BitBoard mask) {
	return (posTable[i] & mask) || (posTable[j] & mask);
}

void Solver::prepareParity() {
	const unsigned long long mask1 = 0x7e7e7e7e7e7e7e7eull;
	const unsigned long long mask79 = 0x7e7e7e7e7e7e7e7eull;

	// find hole IDs:
	unsigned int k = 1;
	BitBoard empti = ~(black | white);
	for (int i = 0; i < MAXSTEP; i++){
		if (empti & posTable[i]){
			if (i >= 9 && (empti & posTable[i - 9]) && notAccrossBorder(i, i - 9, mask79)) holeids[i] = holeids[i - 9];
			else
				if (i >= 8 && (empti & posTable[i - 8])) holeids[i] = holeids[i - 8];
				else 
					if (i >= 7 && (empti & posTable[i - 7]) && notAccrossBorder(i, i - 7, mask79)) holeids[i] = holeids[i - 7];
					else 
						if (i >= 1 && (empti & posTable[i - 1]) && notAccrossBorder(i, i - 1, mask1)) holeids[i] = holeids[i - 1];
						else { holeids[i] = k; k <<= 1; }
		}
		else holeids[i] = 0;
	}
#define MAXITERS 1
#define minu(a, b) ((a) < (b) ? (a) : (b))
	for (int j = 0; j < MAXITERS; j++){
		for (int i = MAXSTEP - 2; i >= 0; i--){
			if (empti & posTable[i]){
				k = holeids[i];
				if (i < MAXSTEP - 9 && (empti & posTable[i + 9]) && notAccrossBorder(i, i + 9, mask79)) 
					holeids[i] = minu(k, holeids[i + 9]);
				if (i < MAXSTEP - 8 && (empti & posTable[i + 8])) 
					holeids[i] = minu(k, holeids[i + 8]);
				if (i < MAXSTEP - 7 && (empti & posTable[i + 7]) && notAccrossBorder(i, i + 7, mask79)) 
					holeids[i] = minu(k, holeids[i + 7]);
				if (empti & posTable[i + 1] && notAccrossBorder(i, i + 1, mask1))
					holeids[i] = minu(k, holeids[i + 1]);
			}
		}
		for (int i = 1; i < MAXSTEP; i++){
			if (empti & posTable[i]){
				k = holeids[i];
				if (i >= 9 && (empti & posTable[i - 9]) && notAccrossBorder(i, i - 9, mask79)) 
					holeids[i] = minu(k, holeids[i - 9]);
				if (i >= 8 && (empti & posTable[i - 8])) 
					holeids[i] = minu(k, holeids[i - 8]);
				if (i >= 7 && (empti & posTable[i - 7]) && notAccrossBorder(i, i - 7, mask79)) 
					holeids[i] = minu(k, holeids[i - 7]);
				if (empti & posTable[i - 1] && notAccrossBorder(i, i - 1, mask1)) 
					holeids[i] = minu(k, holeids[i - 1]);
			}
		}
	}
	// find parity of holes:
	regionParity = 0;
	for (int i = 0; i < MAXSTEP; i++){
		regionParity ^= holeids[i];
	}
	// assign holeid to empty list:
	for (EmptyNode* node = emptyHead->succ; node != emptyTail; node = node->succ) {
		node->holeid = holeids[node->pos];
	}
}

int Solver::searchExact_parity(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound) {
#ifdef COUNT_INTERNAL_NODES
	evnum++;
#endif
	int sortSearchDepth = getEndSortSearchDepth();
	int tabledepth = empties + EPC_STAGES;
	TPInfo* infoNewer;
	TPInfo* infoDeeper;
	int palpha = alpha, pbeta = beta;
	int result, maxresult = -INFINITE;
	int bestMove = -1, bestMove2 = -1, maxptr = -1;
	// using transposition table
	int zobPos = getZobKey(my, op) & currentTableMask;
	TPEntry* entry = &tp[zobPos];
	infoDeeper = &entry->deeper;
	if (infoDeeper->my == my && infoDeeper->op == op) {
		if (infoDeeper->depth == tabledepth) {
			if (infoDeeper->lower >= beta)
				return infoDeeper->lower;
			if (infoDeeper->upper <= alpha)
				return infoDeeper->upper;
			if (infoDeeper->lower > alpha) {
				alpha = infoDeeper->lower;
				maxresult = alpha;
				maxptr = infoDeeper->pos;
			}
			if (infoDeeper->upper < beta)
				beta = infoDeeper->upper;
		} else if (infoDeeper->depth < empties) {
			if (infoDeeper->lower >= getEndSortSearchUpperBound(beta))
				return infoDeeper->lower - INFINITE + MAXSTEP;
			if (infoDeeper->lower > getEndSortSearchUpperBound(alpha)) {
				alpha = infoDeeper->lower - INFINITE + MAXSTEP;
				maxresult = alpha;
				maxptr = infoDeeper->pos;
			}
			if (infoDeeper->upper <= getEndSortSearchLowerBound1(alpha))
				return infoDeeper->upper + INFINITE - MAXSTEP;
			if (infoDeeper->upper < getEndSortSearchLowerBound1(beta))
				beta = infoDeeper->upper + INFINITE - MAXSTEP;
		}
		if (infoDeeper->depth >= sortSearchDepth && infoDeeper->lower < END_WIPEOUT_THRESHOLD)
			bestMove = infoDeeper->pos;
	}

	infoNewer = &entry->newer;
	if (infoNewer->my == my && infoNewer->op == op) {
		if (infoNewer->depth == tabledepth) {
			if (infoNewer->lower >= beta)
				return infoNewer->lower;
			if (infoNewer->upper <= alpha)
				return infoNewer->upper;
			if (infoNewer->lower > alpha) {
				alpha = infoNewer->lower;
				maxresult = alpha;
				maxptr = infoNewer->pos;
			}
			if (infoNewer->upper < beta)
				beta = infoNewer->upper;
		} else if (infoNewer->depth < empties) {
			if (infoNewer->lower >= getEndSortSearchUpperBound(beta))
				return infoNewer->lower - INFINITE + MAXSTEP;
			if (infoNewer->lower > getEndSortSearchUpperBound(alpha)) {
				alpha = infoNewer->lower - INFINITE + MAXSTEP;
				maxresult = alpha;
				maxptr = infoNewer->pos;
			}
			if (infoNewer->upper <= getEndSortSearchLowerBound1(alpha))
				return infoNewer->upper + INFINITE - MAXSTEP;
			if (infoNewer->upper < getEndSortSearchLowerBound1(beta))
				beta = infoNewer->upper + INFINITE - MAXSTEP;
		}
		if (infoNewer->pos != bestMove && infoNewer->depth >= sortSearchDepth && infoNewer->lower < END_WIPEOUT_THRESHOLD)
			bestMove2 = infoNewer->pos;
	}

	BitBoard mob = mobility(my, op);

	if (mob == 0) {
		if (lastFound)
			return -searchExact_parity(op, my, -beta, -alpha, false);
		else return 
#ifdef COUNT_INTERNAL_NODES
			getResultNoCount(my);
#else
			getResult(my);
#endif
	}

#ifdef USE_STABILITY
	int lower, upper;
	calcStabilityBound(my, op, lower, upper);
	if (lower > alpha) {
		alpha = lower;
		if (alpha >= beta) {
			return alpha;
		}
		// must look for a move to set maxptr
		if (bestMove != -1)
			maxptr = bestMove;
		else if (bestMove2 != -1)
			maxptr = bestMove2;
		else for (EmptyNode* i = emptyHead->succ; i != emptyTail; i = i->succ) {
			int current = i->pos;
			if (posTable[current] & mob) {
				maxptr = current;
				break;
			}
		}
		maxresult = alpha;
	}
	if (upper < beta) {
		beta = upper;
		if (beta <= alpha)
			return beta;
	}
#endif

	int (Solver::*searchFunction)(BitBoard&, BitBoard&, int, int, bool);
	searchFunction = (empties > END_USE_SORT_DEPTH) ? (&Solver::searchExact_parity) : (&Solver::fastSearchExact);

	// check table move first
	int pSortStackPtr = sortStackPtr;
	if (bestMove != -1)
		emptySortStack[sortStackPtr++] = emptyPtr[bestMove];
	if (bestMove2 != -1)
		emptySortStack[sortStackPtr++] = emptyPtr[bestMove2];
	int sortStart = sortStackPtr;
	sortStackPtr = pSortStackPtr + bits(mob);

#ifdef USE_ETC
	// add the rest of the moves
	int ptr = sortStart;
	for (EmptyNode* enode = emptyHead->succ; enode != emptyTail; enode = enode->succ) {
		int current = enode->pos;
		if ((posTable[current] & mob) && (current != bestMove) && (current != bestMove2)) {
			emptySortStack[ptr++] = enode;
		}
	}
	// using enhanced transposition cutoff
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {
		EmptyNode* pos = emptySortStack[i];
		makeMove(pos->pos, my, op);
		// Enhanced Transpositon Cutoff
		int zobKey2 = getZobKey(op, my);
		TPEntry* entry = &tp[zobKey2 & currentTableMask];
		TPInfo* info = &entry->deeper;
		if (info->my == op && info->op == my && info->depth >= tabledepth - 1
			&& -info->upper > alpha) {
				alpha = -info->upper;
				maxresult = alpha;
				maxptr = pos->pos;
				if (alpha >= beta) {
					unMakeMove();
					sortStackPtr = pSortStackPtr;
					return alpha;
				}
		}
		info = &entry->newer;
		if (info->my == op && info->op == my && info->depth >= tabledepth - 1
			&& -info->upper > alpha) {
				alpha = -info->upper;
				maxresult = alpha;
				maxptr = pos->pos;
				if (alpha >= beta) {
					unMakeMove();
					sortStackPtr = pSortStackPtr;
					return alpha;
				}
		}
		unMakeMove();
	}
#endif

	// sort & search combined
	for (int i = pSortStackPtr; i < sortStackPtr; i++) {
		if (i == sortStart) {
#ifndef USE_ETC
			// add the rest of the moves
			int ptr = sortStart;
			for (EmptyNode* enode = emptyHead->succ; enode != emptyTail; enode = enode->succ) {
				int current = enode->pos;
				if ((posTable[current] & mob) && (current != bestMove) && (current != bestMove2)) {
					emptySortStack[ptr++] = enode;
				}
			}
#endif
			for (int j = sortStart; j < sortStackPtr; j++) {
				int current = emptySortStack[j]->pos;
				makeMove(current, my, op);
				sortResultStack[j] = result = (sortSearchDepth == 1) ?
					(-evaluate(op, my))
					: ((sortSearchDepth <= MID_USE_SORT_DEPTH) ? 
					-fastSearch(op, my, sortSearchDepth - 1, -getEndSortSearchUpperBound(beta), -getEndSortSearchLowerBound(alpha), true)
					: -search(op, my, sortSearchDepth - 1, -getEndSortSearchUpperBound(beta), -getEndSortSearchLowerBound(alpha), true));
				/*-fastSearch(op, my, 1, -INFINITE, INFINITE, true);*/
				if (aborted) return 0;
				if (result >= END_WIPEOUT_THRESHOLD)
					sortResultStack[j] += END_VERY_HIGH_EVAL_BONUS;
				if (result < getEndSortSearchUpperBound(0) && result > getEndSortSearchLowerBound1(0)) {
					sortResultStack[j] -= weighted_mobility_bits(mobility(op, my)) *
						(result >= END_WIPEOUT_THRESHOLD) ? END_WIPEOUT_MOBILITY_WEIGHT : END_MOBILITY_WEIGHT;
				}
				unMakeMove();
				// Parity bonus
				if (regionParity & emptySortStack[j]->holeid) {
					sortResultStack[j] += END_PARITY_WEIGHT[current];
				}
				if (sortSearchDepth == 1) continue;
				int tmpRes = result - INFINITE + MAXSTEP;
				if (result >= getEndSortSearchUpperBound(beta)) {
					if (beta == pbeta)
						saveTp(zobPos, my, op, tmpRes, MAXSTEP + 1, current, tabledepth, 0);
					else
						saveTp(zobPos, my, op, tmpRes, tmpRes, current, tabledepth, 0);
					sortStackPtr = pSortStackPtr;
					return tmpRes;
				}
				if (result > getEndSortSearchUpperBound(alpha)) {
					alpha = tmpRes;
					maxresult = alpha;
					maxptr = current;
				}
			}
		}

		if (i >= sortStart) {
			int p = i;
			for (int j = i + 1; j < sortStackPtr; j++) {
				if (sortResultStack[j] > sortResultStack[p])
					p = j;
			}
			if (p != i) {
				int tmp = sortResultStack[i];
				sortResultStack[i] = sortResultStack[p];
				sortResultStack[p] = tmp;
				EmptyNode* tmpNode = emptySortStack[i];
				emptySortStack[i] = emptySortStack[p];
				emptySortStack[p] = tmpNode;
			}
		}

		int current = emptySortStack[i]->pos;
		makeMoveAndSetEmpties(emptySortStack[i], my, op);
		setParity(emptySortStack[i]);

#ifdef ENDGAME_USE_NEGASCOUT
		if (i != pSortStackPtr) { // it's not the first node
#else
		if ((alpha > palpha) && (beta - alpha >= END_WINDOW_SIZE)) { // it's a PV node
#endif
			// try a 0-window search
			result = -(this->*searchFunction)(op, my, -alpha - 1, -alpha, true);
			if (aborted) return 0;
			if (result <= alpha) { // no better evaluations, throw away the step
				unSetParity(emptySortStack[i]);
				unMakeMoveAndSetEmpties();
				if (result > maxresult) {
					maxresult = result;
				}
				continue;
			}
			if (result >= beta) {
				unSetParity(emptySortStack[i]);
				unMakeMoveAndSetEmpties();
				if (beta == pbeta)
					saveTp(zobPos, my, op, result, MAXSTEP + 1, current, tabledepth, 0);
				else
					saveTp(zobPos, my, op, result, result, current, tabledepth, 0);
				sortStackPtr = pSortStackPtr;
				return result;
			}
			maxresult = alpha = result;
			maxptr = current;
		}

		// otherwise, return to normal search
		result = -(this->*searchFunction)(op, my, -beta, -alpha, true);
		if (aborted) return 0;
		unSetParity(emptySortStack[i]);
		unMakeMoveAndSetEmpties();
		if (result >= beta) {
			if (beta == pbeta)
				saveTp(zobPos, my, op, result, MAXSTEP + 1, current, tabledepth, 0);
			else
				saveTp(zobPos, my, op, result, result, current, tabledepth, 0);
			sortStackPtr = pSortStackPtr;
			return result;
		}
		if (result > maxresult) {
			maxresult = result;
			if (result > alpha) {
				alpha = result;
				maxptr = current;
			}
		}
	}
	int lowerbound = 
#ifdef USE_STABILITY
		lower;
#else
		-MAXSTEP;
#endif
	if (maxresult == lowerbound) {
		palpha = alpha - 1;
		maxptr = emptySortStack[pSortStackPtr]->pos;
	}
	if (palpha == alpha)
		saveTp(zobPos, my, op, -MAXSTEP - 1, maxresult, maxptr, tabledepth, 0);
	else
		saveTp(zobPos, my, op, maxresult, maxresult, maxptr, tabledepth, 0);
	sortStackPtr = pSortStackPtr;
	return maxresult;
}

inline void Solver::setParity(EmptyNode* emptyPos) {
	regionParity ^= emptyPos->holeid;
}

inline void Solver::unSetParity(EmptyNode* emptyPos) {
	regionParity ^= emptyPos->holeid;
}

inline int Solver::weighted_mobility_bits(const BitBoard& mobility) {
	const BitBoard CORNER_MASK = 0x8100000000000081ull;
	return bits(mobility) + bits(mobility & CORNER_MASK);
}

#ifdef USE_MTD_F
int Solver::mtdExact(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound) {
	return mtdExact_main(my, op, alpha, beta, lastFound, &Solver::searchExact);
}

int Solver::mtdExact_epc(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound) {
	return mtdExact_main(my, op, alpha, beta, lastFound, &Solver::searchExact_epc);
}

int Solver::mtdExact_main(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound,
		int (Solver::*mainSearchFunction)(BitBoard& my, BitBoard& op, int alpha, int beta, bool lastFound)) {
	int lower = alpha;
	int upper = beta;
	int eval;

	// use transposition table to predict evaluation
	int zobPos = getZobKey(my, op) & currentTableMask;
	TPEntry* entry = &tp[zobPos];
	TPInfo* info2 = &entry->deeper;
	if (info2->my == my && info2->op == op && info2->depth >= empties) {
		eval = info2->lower;
		if (eval < -MAXSTEP) eval = info2->upper;
	} else {
		TPInfo* info = &entry->newer;
		if (info->my == my && info->op == op && info->depth >= empties)
			eval = info->lower;
			if (eval < -MAXSTEP) eval = info->upper;
		else {
			if (info2->my == my && info2->op == op) {
				int value = (info2->lower < -INFINITE) ? info2->upper : info2->lower;
				eval = (value + RULER) / RULER / 2 * 2;
			} else if (info->my == my && info->op == op) {
				int value = (info->lower < -INFINITE) ? info->upper : info->lower;
				eval = (value + RULER) / RULER / 2 * 2;
			} else
				eval = (lower + upper) / 4 * 2;
		}
	}

	if (eval > upper) eval = upper;
	if (eval < lower) eval = lower;

	int window_lower, window_upper;
	do {
		window_upper = eval + 2;
		window_lower = eval - 2;
		if (window_upper > upper) window_upper = upper;
		if (window_lower < lower) window_lower = lower;
		eval = (this->*mainSearchFunction)(my, op, window_lower, window_upper, lastFound);
		if (eval <= window_lower)
			upper = eval;
		else if (eval >= window_upper)
			lower = eval;
		else return eval;
	} while (lower < upper);
	return eval;
}

#endif

#ifdef REEVALUATE

#include <windows.h>

using namespace std;

static int expanded = 0;

void Solver::Book::reevaluate() {
	for (int i = 0; i < getCapacity(); i++) {
		BookNode node = get(i);
		BasicBookNode* basic_node = node.node;
		if (basic_node) {
			basic_node->fatherCount = 0;
			basic_node->moveCount = 0;
		}
	}
	{
		wstringstream wss;
		wss << L"Nodes total: " << this->getSize();
		MessageBox(NULL, wss.str().c_str(), L"", MB_OK);
	}

	setCacheSize(0x200000);
	BookNode node = get(defaultMy, defaultOp);
	reevaluateNode(node);


	//for (int i = 0; i < getCapacity(); i++) {
	//	BookNode node = get(i);
	//	if (node) {
	//		staticSolver->expandNode(node);
	//	}
	//	initPercent = (i + 1) * 100 / getCapacity();
	//	if ((i + 1) % 100 == 0) clearCache();
	//}
	//for (int i = 0; i < getCapacity(); i++) {
	//	BookNode node = get(i);
	//	if (node) {
	//		if (node.getMoveCount())
	//			staticSolver->propagateEval(node);
	//	}
	//}
	saveBook("book_reevaluated.craft");
	isBookChanged = false;

	{
		wstringstream wss;
		wss << L"Nodes expanded: " << expanded;
		MessageBox(NULL, wss.str().c_str(), L"", MB_OK);
	}
}

void Solver::Book::reevaluateNode(BookNode node) {
	if (node) if (!node.getMoveCount()) {
		staticSolver->expandNode(node);
		if (node.getMoveCount()) {
			expanded++;
			initPercent = expanded * 100 / getSize();
			if (expanded % 200 == 0) clearCache();
			int* moves = new int[node.getMoveCount()];
			// in case of re-ordering issues, we copy the moves out first
			for (int i = 0; i < node.getMoveCount(); i++) {
				moves[i] = node.getMove(i);
			}
			for (int i = 0; i < node.getMoveCount(); i++) {
				BitBoard my = node.getMy();
				BitBoard op = node.getOp();
				putChess(moves[i], my, op);
				BookNode nextNode = get(op, my);
				reevaluateNode(nextNode);
			}
			delete [] moves;
		}
	}
}

#endif

SolverResult Solver::partialSolveExact(int color, bool winLoss, int percentage, int& actualPercentage) {
	if (empties <= MIN_EPC_DEPTH) {
		actualPercentage = 100;
		return solveExactInternal(color, winLoss, EPC_STAGES);
	}
	int epcStage = 0;
	while (epcStage < EPC_STAGES && EPC_PERCENTAGE[epcStage] < percentage) {
		epcStage++;
	}
	actualPercentage = EPC_PERCENTAGE[epcStage];
	return solveExactInternal(color, winLoss, epcStage);
}

#ifdef STABILITY
int Solver::bits16(unsigned int num) {
	return bitTable[num];
}

void Solver::calcStabilityBound(const BitBoard& my, const BitBoard& op, int& lower, int& upper) {
	int stmy = 0, stop = 0;
	const unsigned int CENTER_MASK = 0x7e;
	// A1-A8
	unsigned int line_my = (unsigned int)(my >> 56);
	unsigned int line_op = (unsigned int)(op >> 56);
	int index = twoTo3Base[line_my] + (twoTo3Base[line_op] << 1);
	stmy += bits16(stab_my[index]);
	stop += bits16(stab_op[index]);
	// A1-H1
	line_my = (unsigned int)(((my & 0x8080808080808080ull) * 0x2040810204081ull) >> 56);
	line_op = (unsigned int)(((op & 0x8080808080808080ull) * 0x2040810204081ull) >> 56);
	index = twoTo3Base[line_my] + (twoTo3Base[line_op] << 1);
	stmy += bits16(CENTER_MASK & stab_my[index]);
	stop += bits16(CENTER_MASK & stab_op[index]);
	// H1-H8
	line_my = (unsigned int)(my & 0xffull);
	line_op = (unsigned int)(op & 0xffull);
	index = twoTo3Base[line_my] + (twoTo3Base[line_op] << 1);
	stmy += bits16(stab_my[index]);
	stop += bits16(stab_op[index]);
	// A8-H8
	line_my = (unsigned int)(((my & 0x0101010101010101ull) * 0x102040810204080ull) >> 56);
	line_op = (unsigned int)(((op & 0x0101010101010101ull) * 0x102040810204080ull) >> 56);
	index = twoTo3Base[line_my] + (twoTo3Base[line_op] << 1);
	stmy += bits16(CENTER_MASK & stab_my[index]);
	stop += bits16(CENTER_MASK & stab_op[index]);

	lower = (stmy << 1) - MAXSTEP;
	upper = MAXSTEP - (stop << 1);
}
#endif

int Solver::getPV(int pv[], int length) {
	int len = min(length, pvLength);
	memcpy(pv, principalVariation, len * sizeof(int));
	return len;
}

void Solver::initPV() {
	pvLength = 0;
}

void Solver::setPV(BitBoard& my, BitBoard& op, int depth, int firstMove) {
	principalVariation[0] = firstMove;
	if (checkedMakeMove(firstMove, my, op)) {
		pvLength = 1 + searchPV(op, my, depth - 1, true, principalVariation + 1, principalVariation + MAX_PV_LENGTH);
		unMakeMove();
	} else {
		pvLength = 1;
	}
}

int Solver::searchPV(BitBoard& my, BitBoard& op, int depth, bool lastFound, int* pvStart, int* pvEnd) {
	if (pvStart == pvEnd) return 0;

	BitBoard mob = mobility(my, op);
	if (mob == 0) {
		if (lastFound)
			return searchPV(op, my, depth, false, pvStart, pvEnd);
		else return 0;
	}

	int lower, upper;
	calcStabilityBound(my, op, lower, upper);

	if (depth < empties) {
		lower = endToMid(lower);
		upper = endToMid(upper);
	}

	int bestMove = -1;

	int zobPos = getZobKey(my, op) & currentTableMask;
	TPEntry* entry = &tp[zobPos];
	TPInfo* info = &entry->deeper;
	if (((depth >= empties && info->depth >= depth) || info->depth == depth)
			&& info->my == my && info->op == op) {
		if (info->lower == info->upper) {
			*pvStart = info->pos;
			if (!checkedMakeMove(info->pos, my, op)) return 0;
			int len = 1 + searchPV(op, my, depth - 1, true, pvStart + 1, pvEnd);
			unMakeMove();
			return len;
		}
		if (info->lower >= upper) {
			*pvStart = info->pos;
			if (!checkedMakeMove(info->pos, my, op)) return 0;
			int len = 1 + searchPV(op, my, depth - 1, true, pvStart + 1, pvEnd);
			unMakeMove();
			return len;
		}
		if (info->lower > lower) {
			lower = info->lower;
			bestMove = info->pos;
		}
		if (info->upper < upper) {
			upper = info->upper;
		}
	}

	info = &entry->newer;
	if (((depth >= empties && info->depth >= depth) || info->depth == depth)
			&& info->my == my && info->op == op) {
		if (info->lower == info->upper) {
			*pvStart = info->pos;
			if (!checkedMakeMove(info->pos, my, op)) return 0;
			int len = 1 + searchPV(op, my, depth - 1, true, pvStart + 1, pvEnd);
			unMakeMove();
			return len;
		}
		if (info->lower >= upper) {
			*pvStart = info->pos;
			if (!checkedMakeMove(info->pos, my, op)) return 0;
			int len = 1 + searchPV(op, my, depth - 1, true, pvStart + 1, pvEnd);
			unMakeMove();
			return len;
		}
		if (info->lower > lower) {
			lower = info->lower;
			bestMove = info->pos;
		}
		if (info->upper < upper) {
			upper = info->upper;
		}
	}

	if (lower < upper)
		return 0;
	if (bestMove == -1) {
		bestMove = 0;
		while ((orderTable[bestMove] & mob) == 0) {
			bestMove++;
		}
		bestMove = moveOrder[bestMove];
	}

	*pvStart = bestMove;
	if (!checkedMakeMove(bestMove, my, op)) return 0;
	int len = 1 + searchPV(op, my, depth - 1, true, pvStart + 1, pvEnd);
	unMakeMove();
	return len;
}

void Solver::setBookPV(BitBoard& my, BitBoard& op, int firstMove) {
	BitBoard work_my = my, work_op = op;
	int pvLength = 1;
	principalVariation[0] = firstMove;
	BookNode node;
	putChess(firstMove, work_my, work_op);
	BitBoard tmp = work_my; work_my = work_op; work_op = tmp;
	while (pvLength < MAX_PV_LENGTH && (node = book->get(work_my, work_op)) && node.getMoveCount()) {
		principalVariation[pvLength++] = node.getMove(0);
		putChess(node.getMove(0), work_my, work_op);
		tmp = work_my; work_my = work_op; work_op = tmp;
	}
	this->pvLength = pvLength;
}

void Solver::saveTp(int zobPos, const BitBoard& my, const BitBoard& op, 
					int lower, int upper, int pos, int depth, unsigned int flags) {
	TPEntry* entry = &tp[zobPos];
	TPInfo* info = &entry->deeper;
	if (info->depth == depth && info->my == my && info->op == op && info->flags == flags) {
		if (lower > info->lower) {
			info->lower = lower;
			info->pos = pos;
		}
		if (upper < info->upper) {
			info->upper = upper;
		}
		return;
	}
	info = &entry->newer;
	if (info->depth == depth && info->my == my && info->op == op && info->flags == flags) {
		if (lower > info->lower) {
			info->lower = lower;
			info->pos = pos;
		}
		if (upper < info->upper) {
			info->upper = upper;
		}
		return;
	}
	if (depth >= entry->deeper.depth - DEEP_COVER) {
		entry->newer = entry->deeper;
		info = &entry->deeper;
	}
	info->lower = lower;
	info->upper = upper;
	info->depth = depth;
	info->pos = pos;
	info->flags = flags;
	info->my = my;
	info->op = op;
}

void Solver::restoreStack(int pre_stackptr) {
	while (stackptr != pre_stackptr) {
		unMakeMove();
	}
}

void Solver::flipVertical(BitBoard& bb) {
	const BitBoard k1 = 0x5555555555555555ull;
	const BitBoard k2 = 0x3333333333333333ull;
	const BitBoard k4 = 0x0f0f0f0f0f0f0f0full;
	/* taking advantage of x86 LEA instruction */
	bb = ((bb >> 1) & k1) +  2 * (bb & k1);
	bb = ((bb >> 2) & k2) +  4 * (bb & k2);
	bb = ((bb >> 4) & k4) + 16 * (bb & k4);
}

void Solver::flipHorizonal(BitBoard& bb) {
#ifdef MACHINE_X64
	bb = _byteswap_uint64(bb);
#else
	const BitBoard k1 = 0x00FF00FF00FF00FFull;
	const BitBoard k2 = 0x0000FFFF0000FFFFull;
	bb = ((bb >>  8) & k1) | ((bb & k1) <<  8);
	bb = ((bb >> 16) & k2) | ((bb & k2) << 16);
	bb = ( bb >> 32)       | ( bb       << 32);
#endif
}

void Solver::flipDiagA1H8(BitBoard& bb) {
   BitBoard t;
   const BitBoard k1 = 0x5500550055005500ull;
   const BitBoard k2 = 0x3333000033330000ull;
   const BitBoard k4 = 0x0f0f0f0f00000000ull;
   t   = k4 & (bb ^ (bb << 28));
   bb ^=       t ^ (t >> 28) ;
   t   = k2 & (bb ^ (bb << 14));
   bb ^=       t ^ (t >> 14) ;
   t   = k1 & (bb ^ (bb <<  7));
   bb ^=       t ^ (t >>  7) ;
}

void Solver::flipDiagA8H1(BitBoard& bb) {
   BitBoard t;
   const BitBoard k1 = 0xaa00aa00aa00aa00ull;
   const BitBoard k2 = 0xcccc0000cccc0000ull;
   const BitBoard k4 = 0xf0f0f0f00f0f0f0full;
   t   =       bb ^ (bb << 36) ;
   bb ^= k4 & (t ^ (bb >> 36));
   t   = k2 & (bb ^ (bb << 18));
   bb ^=       t ^ (t >> 18) ;
   t   = k1 & (bb ^ (bb <<  9));
   bb ^=       t ^ (t >>  9) ;
}

void Solver::posFlipVertical(int& pos) {
	pos = pos ^ 7;
}

void Solver::posFlipHorizonal(int& pos) {
	pos = pos ^ 56;
}

void Solver::posFlipDiagA1H8(int& pos) {
	pos = ((pos >> 3) | (pos << 3)) & 63;
}

void Solver::posFlipDiagA8H1(int& pos) {
	pos = (((pos >> 3) | (pos << 3)) & 63) ^ 63;
}


} // namespace CraftEngine
