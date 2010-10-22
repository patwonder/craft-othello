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
								Solver_putChess.cpp
		作者：Patrick
		概述：包含类 Solver 走棋函数的定义。
			Solver 是 Craft 的核心 AI 引擎。

********************************************************************************
*/
#include "StdAfx.h"
#include "Solver.h"

namespace CraftEngine {

bool CALLSPEC Solver::putChess_A1(BitBoard& b1, BitBoard& b2) {
	const int pos = 0;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[(b1 >> 56) | ((b2 & 0xff00000000000000ull) >> 48)][1];
	toFlip = lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		| ((((b2 & 0x8080808080808080ull) * 0x2040810204081ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x8040201008040201ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_A8(BitBoard& b1, BitBoard& b2) {
	const int pos = 7;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(7 << 16)
		| (b1 >> 56) | ((b2 & 0xff00000000000000ull) >> 48)][0];
	toFlip = lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		| ((((b2 & 0x0101010101010101ull) * 0x102040810204080ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x0102040810204080ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];
	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_H1(BitBoard& b1, BitBoard& b2) {
	const int pos = 56;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[(b1 & 0xffull) | ((b2 & 0xffull) << 8)][1];
	toFlip = lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		| ((((b2 & 0x8080808080808080ull) * 0x2040810204081ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-right
	delta = fastFlipPattern[(((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x0102040810204080ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_H8(BitBoard& b1, BitBoard& b2) {
	const int pos = 63;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(7 << 16)
		| (b1 & 0xffull) | ((b2 & 0xffull) << 8)][0];
	toFlip = lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		| ((((b2 & 0x0101010101010101ull) * 0x102040810204080ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x8040201008040201ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_A2(BitBoard& b1, BitBoard& b2) {
	const int pos = 1;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[(1 << 16)
		| (b1 >> 56) | ((b2 & 0xff00000000000000ull) >> 48)][1];
	toFlip = lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		| ((((b2 & 0x4040404040404040ull) * 0x4081020408102ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x4020100804020100ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_A7(BitBoard& b1, BitBoard& b2) {
	const int pos = 6;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(6 << 16)
		| (b1 >> 56) | ((b2 & 0xff00000000000000ull) >> 48)][0];
	toFlip = lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		| ((((b2 & 0x0202020202020202ull) * 0x081020408102040ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x0204081020408000ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];
	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_B1(BitBoard& b1, BitBoard& b2) {
	const int pos = 8;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[((b1 & 0x00ff000000000000ull) >> 48) 
		| ((b2 & 0x00ff000000000000ull) >> 40)][1];
	toFlip = lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		| ((((b2 & 0x8080808080808080ull) * 0x2040810204081ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0080402010080402ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_B8(BitBoard& b1, BitBoard& b2) {
	const int pos = 15;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(7 << 16)
		| ((b1 & 0x00ff000000000000ull) >> 48) 
		| ((b2 & 0x00ff000000000000ull) >> 40)][0];
	toFlip = lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		| ((((b2 & 0x0101010101010101ull) * 0x102040810204080ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0001020408102040ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_G1(BitBoard& b1, BitBoard& b2) {
	const int pos = 48;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[((b1 & 0x0000ff00ull) >> 8)
		| (b2 & 0x0000ff00ull)][1];
	toFlip = lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		| ((((b2 & 0x8080808080808080ull) * 0x2040810204081ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-right
	delta = fastFlipPattern[(((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x0204081020408000ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_G8(BitBoard& b1, BitBoard& b2) {
	const int pos = 55;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(7 << 16)
		| ((b1 & 0x0000ff00ull) >> 8)
		| (b2 & 0x0000ff00ull)][0];
	toFlip = lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		| ((((b2 & 0x0101010101010101ull) * 0x102040810204080ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x4020100804020100ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_H2(BitBoard& b1, BitBoard& b2) {
	const int pos = 57;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[(1 << 16)
		| (b1 & 0x000000ffull)
		| ((b2 & 0x000000ffull) << 8)][1];
	toFlip = lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		| ((((b2 & 0x4040404040404040ull) * 0x4081020408102ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0001020408102040ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_H7(BitBoard& b1, BitBoard& b2) {
	const int pos = 62;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(6 << 16)
		| (b1 & 0x000000ffull)
		| ((b2 & 0x000000ffull) << 8)][0];
	toFlip = lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		| ((((b2 & 0x0202020202020202ull) * 0x081020408102040ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0080402010080402ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_A3(BitBoard& b1, BitBoard& b2) {
	const int pos = 2;
	BitBoard toFlip;
	// right & left
	int placement = (2 << 16)
		| (b1 >> 56) | ((b2 & 0xff00000000000000ull) >> 48);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		| ((((b2 & 0x2020202020202020ull) * 0x8102040810204ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(2 << 16)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x2010080402010000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(2 << 16)
		| (((b1 & 0x2040800000000000ull) * 0x10101ull) >> 56)
		| ((((b2 & 0x2040800000000000ull) * 0x10101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_A6(BitBoard& b1, BitBoard& b2) {
	const int pos = 5;
	BitBoard toFlip;
	// right & left
	int placement = (5 << 16)
		| (b1 >> 56) | ((b2 & 0xff00000000000000ull) >> 48);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		| ((((b2 & 0x0404040404040404ull) * 0x40810204081020ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(5 << 16)
		| (((b1 & 0x0402010000000000ull) * 0x10101ull) >> 56)
		| ((((b2 & 0x0402010000000000ull) * 0x10101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(5 << 16)
		| (((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x0408102040800000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_C1(BitBoard& b1, BitBoard& b2) {
	const int pos = 16;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[((b1 & 0x0000ff0000000000ull) >> 40)
		| ((b2 & 0x0000ff0000000000ull) >> 32)][1];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (2 << 16)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		| ((((b2 & 0x8080808080808080ull) * 0x2040810204081ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000804020100804ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(((b1 & 0x2040800000000000ull) * 0x10101ull) >> 56)
		| ((((b2 & 0x2040800000000000ull) * 0x10101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_C8(BitBoard& b1, BitBoard& b2) {
	const int pos = 23;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(7 << 16)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		| ((b2 & 0x0000ff0000000000ull) >> 32)][0];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (2 << 16)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		| ((((b2 & 0x0101010101010101ull) * 0x102040810204080ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0402010000000000ull) * 0x10101ull) >> 56)
		| ((((b2 & 0x0402010000000000ull) * 0x10101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000010204081020ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_F1(BitBoard& b1, BitBoard& b2) {
	const int pos = 40;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[((b1 & 0x00ff0000ull) >> 16)
		| ((b2 & 0x00ff0000ull) >> 8)][1];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (5 << 16)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		| ((((b2 & 0x8080808080808080ull) * 0x2040810204081ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(((b1 & 0x0000000000804020ull) * 0x101010000000000ull) >> 56)
		| ((((b2 & 0x0000000000804020ull) * 0x101010000000000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x0408102040800000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_F8(BitBoard& b1, BitBoard& b2) {
	const int pos = 47;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(7 << 16)
		| ((b1 & 0x00ff0000ull) >> 16)
		| ((b2 & 0x00ff0000ull) >> 8)][0];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (5 << 16)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		| ((((b2 & 0x0101010101010101ull) * 0x102040810204080ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x2010080402010000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0000000000010204ull) * 0x101010000000000ull) >> 56)
		| ((((b2 & 0x0000000000010204ull) * 0x101010000000000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_H3(BitBoard& b1, BitBoard& b2) {
	const int pos = 58;
	BitBoard toFlip;
	// right & left
	int placement = (2 << 16)
		| (b1 & 0x000000ffull)
		| ((b2 & 0x000000ffull) << 8);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		| ((((b2 & 0x2020202020202020ull) * 0x8102040810204ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(2 << 16)
		| (((b1 & 0x0000000000804020ull) * 0x101010000000000ull) >> 56)
		| ((((b2 & 0x0000000000804020ull) * 0x101010000000000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(2 << 16)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000010204081020ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_H6(BitBoard& b1, BitBoard& b2) {
	const int pos = 61;
	BitBoard toFlip;
	// right & left
	int placement = (5 << 16)
		| (b1 & 0x000000ffull)
		| ((b2 & 0x000000ffull) << 8);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		| ((((b2 & 0x0404040404040404ull) * 0x40810204081020ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(5 << 16)
		| (((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000804020100804ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(5 << 16)
		| (((b1 & 0x0000000000010204ull) * 0x101010000000000ull) >> 56)
		| ((((b2 & 0x0000000000010204ull) * 0x101010000000000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_A4(BitBoard& b1, BitBoard& b2) {
	const int pos = 3;
	BitBoard toFlip;
	// right & left
	int placement = (3 << 16)
		| (b1 >> 56) | ((b2 & 0xff00000000000000ull) >> 48);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		| ((((b2 & 0x1010101010101010ull) * 0x10204081020408ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(3 << 16)
		| (((b1 & 0x1008040201000000ull) * 0x101010101ull) >> 56)
		| ((((b2 & 0x1008040201000000ull) * 0x101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(3 << 16)
		| (((b1 & 0x1020408000000000ull) * 0x1010101ull) >> 56)
		| ((((b2 & 0x1020408000000000ull) * 0x1010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_A5(BitBoard& b1, BitBoard& b2) {
	const int pos = 4;
	BitBoard toFlip;
	// right & left
	int placement = (4 << 16)
		| (b1 >> 56) | ((b2 & 0xff00000000000000ull) >> 48);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		| ((((b2 & 0x0808080808080808ull) * 0x20408102040810ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(4 << 16)
		| (((b1 & 0x0804020100000000ull) * 0x1010101ull) >> 56)
		| ((((b2 & 0x0804020100000000ull) * 0x1010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(4 << 16)
		| (((b1 & 0x0810204080000000ull) * 0x101010101ull) >> 56)
		| ((((b2 & 0x0810204080000000ull) * 0x101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_D1(BitBoard& b1, BitBoard& b2) {
	const int pos = 24;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[((b1 & 0x000000ff00000000ull) >> 32)
		| ((b2 & 0x000000ff00000000ull) >> 24)][1];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (3 << 16)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		| ((((b2 & 0x8080808080808080ull) * 0x2040810204081ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(((b1 & 0x0000008040201008ull) * 0x101010101000000ull) >> 56)
		| ((((b2 & 0x0000008040201008ull) * 0x101010101000000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(((b1 & 0x1020408000000000ull) * 0x1010101ull) >> 56)
		| ((((b2 & 0x1020408000000000ull) * 0x1010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_D8(BitBoard& b1, BitBoard& b2) {
	const int pos = 31;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(7 << 16)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		| ((b2 & 0x000000ff00000000ull) >> 24)][0];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (3 << 16)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		| ((((b2 & 0x0101010101010101ull) * 0x102040810204080ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0804020100000000ull) * 0x1010101ull) >> 56)
		| ((((b2 & 0x0804020100000000ull) * 0x1010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0000000102040810ull) * 0x101010101000000ull) >> 56)
		| ((((b2 & 0x0000000102040810ull) * 0x101010101000000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_E1(BitBoard& b1, BitBoard& b2) {
	const int pos = 32;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[((b1 & 0xff000000ull) >> 24)
		| ((b2 & 0xff000000ull) >> 16)][1];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (4 << 16)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		| ((((b2 & 0x8080808080808080ull) * 0x2040810204081ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(((b1 & 0x0000000080402010ull) * 0x101010100000000ull) >> 56)
		| ((((b2 & 0x0000000080402010ull) * 0x101010100000000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(((b1 & 0x0810204080000000ull) * 0x101010101ull) >> 56)
		| ((((b2 & 0x0810204080000000ull) * 0x101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_E8(BitBoard& b1, BitBoard& b2) {
	const int pos = 39;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(7 << 16)
		| ((b1 & 0xff000000ull) >> 24)
		| ((b2 & 0xff000000ull) >> 16)][0];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (4 << 16)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		| ((((b2 & 0x0101010101010101ull) * 0x102040810204080ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x1008040201000000ull) * 0x101010101ull) >> 56)
		| ((((b2 & 0x1008040201000000ull) * 0x101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0000000001020408ull) * 0x101010100000000ull) >> 56)
		| ((((b2 & 0x0000000001020408ull) * 0x101010100000000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_H4(BitBoard& b1, BitBoard& b2) {
	const int pos = 59;
	BitBoard toFlip;
	// right & left
	int placement = (3 << 16)
		| (b1 & 0x000000ffull)
		| ((b2 & 0x000000ffull) << 8);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		| ((((b2 & 0x1010101010101010ull) * 0x10204081020408ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(3 << 16)
		| (((b1 & 0x0000000080402010ull) * 0x101010100000000ull) >> 56)
		| ((((b2 & 0x0000000080402010ull) * 0x101010100000000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(3 << 16)
		| (((b1 & 0x0000000102040810ull) * 0x101010101000000ull) >> 56)
		| ((((b2 & 0x0000000102040810ull) * 0x101010101000000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_H5(BitBoard& b1, BitBoard& b2) {
	const int pos = 60;
	BitBoard toFlip;
	// right & left
	int placement = (4 << 16)
		| (b1 & 0x000000ffull)
		| ((b2 & 0x000000ffull) << 8);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(7 << 16)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		| ((((b2 & 0x0808080808080808ull) * 0x20408102040810ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(4 << 16)
		| (((b1 & 0x0000008040201008ull) * 0x101010101000000ull) >> 56)
		| ((((b2 & 0x0000008040201008ull) * 0x101010101000000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(4 << 16)
		| (((b1 & 0x0000000001020408ull) * 0x101010100000000ull) >> 56)
		| ((((b2 & 0x0000000001020408ull) * 0x101010100000000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_B2(BitBoard& b1, BitBoard& b2) {
	const int pos = 9;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[(1 << 16)
		| ((b1 & 0x00ff000000000000ull) >> 48) 
		| ((b2 & 0x00ff000000000000ull) >> 40)][1];
	toFlip = lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		| ((((b2 & 0x4040404040404040ull) * 0x4081020408102ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x8040201008040201ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_B7(BitBoard& b1, BitBoard& b2) {
	const int pos = 14;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(6 << 16)
		| ((b1 & 0x00ff000000000000ull) >> 48) 
		| ((b2 & 0x00ff000000000000ull) >> 40)][0];
	toFlip = lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		| ((((b2 & 0x0202020202020202ull) * 0x081020408102040ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x0102040810204080ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_G2(BitBoard& b1, BitBoard& b2) {
	const int pos = 49;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[(1 << 16)
		| ((b1 & 0x0000ff00ull) >> 8)
		| (b2 & 0x0000ff00ull)][1];
	toFlip = lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		| ((((b2 & 0x4040404040404040ull) * 0x4081020408102ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x0102040810204080ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_G7(BitBoard& b1, BitBoard& b2) {
	const int pos = 54;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(6 << 16)
		| ((b1 & 0x0000ff00ull) >> 8)
		| (b2 & 0x0000ff00ull)][0];
	toFlip = lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		| ((((b2 & 0x0202020202020202ull) * 0x081020408102040ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x8040201008040201ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_B3(BitBoard& b1, BitBoard& b2) {
	const int pos = 10;
	BitBoard toFlip;
	// right & left
	int placement = (2 << 16)
		| ((b1 & 0x00ff000000000000ull) >> 48) 
		| ((b2 & 0x00ff000000000000ull) >> 40);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		| ((((b2 & 0x2020202020202020ull) * 0x8102040810204ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(2 << 16)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x4020100804020100ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(2 << 16)
		| (((b1 & 0x1020408000000000ull) * 0x1010101ull) >> 56)
		| ((((b2 & 0x1020408000000000ull) * 0x1010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_B6(BitBoard& b1, BitBoard& b2) {
	const int pos = 13;
	BitBoard toFlip;
	// right & left
	int placement = (5 << 16)
		| ((b1 & 0x00ff000000000000ull) >> 48) 
		| ((b2 & 0x00ff000000000000ull) >> 40);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		| ((((b2 & 0x0404040404040404ull) * 0x40810204081020ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(5 << 16)
		| (((b1 & 0x0804020100000000ull) * 0x1010101ull) >> 56)
		| ((((b2 & 0x0804020100000000ull) * 0x1010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(5 << 16)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x0204081020408000ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_C2(BitBoard& b1, BitBoard& b2) {
	const int pos = 17;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[(1 << 16)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		| ((b2 & 0x0000ff0000000000ull) >> 32)][1];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (2 << 16)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		| ((((b2 & 0x4040404040404040ull) * 0x4081020408102ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0080402010080402ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x1020408000000000ull) * 0x1010101ull) >> 56)
		| ((((b2 & 0x1020408000000000ull) * 0x1010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_C7(BitBoard& b1, BitBoard& b2) {
	const int pos = 22;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(6 << 16)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		| ((b2 & 0x0000ff0000000000ull) >> 32)][0];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (2 << 16)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		| ((((b2 & 0x0202020202020202ull) * 0x081020408102040ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0804020100000000ull) * 0x1010101ull) >> 56)
		| ((((b2 & 0x0804020100000000ull) * 0x1010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0001020408102040ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_F2(BitBoard& b1, BitBoard& b2) {
	const int pos = 41;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[(1 << 16)
		| ((b1 & 0x00ff0000ull) >> 16)
		| ((b2 & 0x00ff0000ull) >> 8)][1];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (5 << 16)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		| ((((b2 & 0x4040404040404040ull) * 0x4081020408102ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0000000080402010ull) * 0x101010100000000ull) >> 56)
		| ((((b2 & 0x0000000080402010ull) * 0x101010100000000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x0204081020408000ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_F7(BitBoard& b1, BitBoard& b2) {
	const int pos = 46;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(6 << 16)
		| ((b1 & 0x00ff0000ull) >> 16)
		| ((b2 & 0x00ff0000ull) >> 8)][0];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (5 << 16)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		| ((((b2 & 0x0202020202020202ull) * 0x081020408102040ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x4020100804020100ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0000000001020408ull) * 0x101010100000000ull) >> 56)
		| ((((b2 & 0x0000000001020408ull) * 0x101010100000000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_G3(BitBoard& b1, BitBoard& b2) {
	const int pos = 50;
	BitBoard toFlip;
	// right & left
	int placement = (2 << 16)
		| ((b1 & 0x0000ff00ull) >> 8)
		| (b2 & 0x0000ff00ull);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		| ((((b2 & 0x2020202020202020ull) * 0x8102040810204ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(2 << 16)
		| (((b1 & 0x0000000080402010ull) * 0x101010100000000ull) >> 56)
		| ((((b2 & 0x0000000080402010ull) * 0x101010100000000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(2 << 16)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0001020408102040ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_G6(BitBoard& b1, BitBoard& b2) {
	const int pos = 53;
	BitBoard toFlip;
	// right & left
	int placement = (5 << 16)
		| ((b1 & 0x0000ff00ull) >> 8)
		| (b2 & 0x0000ff00ull);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		| ((((b2 & 0x0404040404040404ull) * 0x40810204081020ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(5 << 16)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0080402010080402ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(5 << 16)
		| (((b1 & 0x0000000001020408ull) * 0x101010100000000ull) >> 56)
		| ((((b2 & 0x0000000001020408ull) * 0x101010100000000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_B4(BitBoard& b1, BitBoard& b2) {
	const int pos = 11;
	BitBoard toFlip;
	// right & left
	int placement = (3 << 16)
		| ((b1 & 0x00ff000000000000ull) >> 48) 
		| ((b2 & 0x00ff000000000000ull) >> 40);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		| ((((b2 & 0x1010101010101010ull) * 0x10204081020408ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(3 << 16)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x2010080402010000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(3 << 16)
		| (((b1 & 0x0810204080000000ull) * 0x101010101ull) >> 56)
		| ((((b2 & 0x0810204080000000ull) * 0x101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_B5(BitBoard& b1, BitBoard& b2) {
	const int pos = 12;
	BitBoard toFlip;
	// right & left
	int placement = (4 << 16)
		| ((b1 & 0x00ff000000000000ull) >> 48) 
		| ((b2 & 0x00ff000000000000ull) >> 40);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		| ((((b2 & 0x0808080808080808ull) * 0x20408102040810ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(4 << 16)
		| (((b1 & 0x1008040201000000ull) * 0x101010101ull) >> 56)
		| ((((b2 & 0x1008040201000000ull) * 0x101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(4 << 16)
		| (((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x0408102040800000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_D2(BitBoard& b1, BitBoard& b2) {
	const int pos = 25;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[(1 << 16)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		| ((b2 & 0x000000ff00000000ull) >> 24)][1];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (3 << 16)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		| ((((b2 & 0x4040404040404040ull) * 0x4081020408102ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000804020100804ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0810204080000000ull) * 0x101010101ull) >> 56)
		| ((((b2 & 0x0810204080000000ull) * 0x101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_D7(BitBoard& b1, BitBoard& b2) {
	const int pos = 30;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(6 << 16)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		| ((b2 & 0x000000ff00000000ull) >> 24)][0];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (3 << 16)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		| ((((b2 & 0x0202020202020202ull) * 0x081020408102040ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x1008040201000000ull) * 0x101010101ull) >> 56)
		| ((((b2 & 0x1008040201000000ull) * 0x101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000010204081020ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_E2(BitBoard& b1, BitBoard& b2) {
	const int pos = 33;
	BitBoard toFlip;
	// right
	int delta = fastFlipPattern[(1 << 16)
		| ((b1 & 0xff000000ull) >> 24)
		| ((b2 & 0xff000000ull) >> 16)][1];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (4 << 16)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		| ((((b2 & 0x4040404040404040ull) * 0x4081020408102ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0000008040201008ull) * 0x101010101000000ull) >> 56)
		| ((((b2 & 0x0000008040201008ull) * 0x101010101000000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(1 << 16)
		| (((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x0408102040800000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_E7(BitBoard& b1, BitBoard& b2) {
	const int pos = 38;
	BitBoard toFlip;
	// left
	int delta = fastFlipPattern[(6 << 16)
		| ((b1 & 0xff000000ull) >> 24)
		| ((b2 & 0xff000000ull) >> 16)][0];
	toFlip = lineTable[pos][pos + delta];
	// down & up
	int placement = (4 << 16)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		| ((((b2 & 0x0202020202020202ull) * 0x081020408102040ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x2010080402010000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// down-left
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0000000102040810ull) * 0x101010101000000ull) >> 56)
		| ((((b2 & 0x0000000102040810ull) * 0x101010101000000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_G4(BitBoard& b1, BitBoard& b2) {
	const int pos = 51;
	BitBoard toFlip;
	// right & left
	int placement = (3 << 16)
		| ((b1 & 0x0000ff00ull) >> 8)
		| (b2 & 0x0000ff00ull);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		| ((((b2 & 0x1010101010101010ull) * 0x10204081020408ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(3 << 16)
		| (((b1 & 0x0000008040201008ull) * 0x101010101000000ull) >> 56)
		| ((((b2 & 0x0000008040201008ull) * 0x101010101000000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(3 << 16)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000010204081020ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_G5(BitBoard& b1, BitBoard& b2) {
	const int pos = 52;
	BitBoard toFlip;
	// right & left
	int placement = (4 << 16)
		| ((b1 & 0x0000ff00ull) >> 8)
		| (b2 & 0x0000ff00ull);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// up
	delta = fastFlipPattern[(6 << 16)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		| ((((b2 & 0x0808080808080808ull) * 0x20408102040810ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// up-left
	delta = fastFlipPattern[(4 << 16)
		| (((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000804020100804ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48)][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right
	delta = fastFlipPattern[(4 << 16)
		| (((b1 & 0x0000000102040810ull) * 0x101010101000000ull) >> 56)
		| ((((b2 & 0x0000000102040810ull) * 0x101010101000000ull) & 0xff00000000000000ull) >> 48)][1];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_C3(BitBoard& b1, BitBoard& b2) {
	const int pos = 18;
	BitBoard toFlip;
	// right & left
	int placement = (2 << 16)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		| ((b2 & 0x0000ff0000000000ull) >> 32);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (2 << 16)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		| ((((b2 & 0x2020202020202020ull) * 0x8102040810204ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (2 << 16)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x8040201008040201ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (2 << 16)
		| (((b1 & 0x0810204080000000ull) * 0x101010101ull) >> 56)
		| ((((b2 & 0x0810204080000000ull) * 0x101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_C6(BitBoard& b1, BitBoard& b2) {
	const int pos = 21;
	BitBoard toFlip;
	// right & left
	int placement = (5 << 16)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		| ((b2 & 0x0000ff0000000000ull) >> 32);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (2 << 16)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		| ((((b2 & 0x0404040404040404ull) * 0x40810204081020ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (5 << 16)
		| (((b1 & 0x1008040201000000ull) * 0x101010101ull) >> 56)
		| ((((b2 & 0x1008040201000000ull) * 0x101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (5 << 16)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x0102040810204080ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_F3(BitBoard& b1, BitBoard& b2) {
	const int pos = 42;
	BitBoard toFlip;
	// right & left
	int placement = (2 << 16)
		| ((b1 & 0x00ff0000ull) >> 16)
		| ((b2 & 0x00ff0000ull) >> 8);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (5 << 16)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		| ((((b2 & 0x2020202020202020ull) * 0x8102040810204ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (2 << 16)
		| (((b1 & 0x0000008040201008ull) * 0x101010101000000ull) >> 56)
		| ((((b2 & 0x0000008040201008ull) * 0x101010101000000ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (2 << 16)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x0102040810204080ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_F6(BitBoard& b1, BitBoard& b2) {
	const int pos = 45;
	BitBoard toFlip;
	// right & left
	int placement = (5 << 16)
		| ((b1 & 0x00ff0000ull) >> 16)
		| ((b2 & 0x00ff0000ull) >> 8);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (5 << 16)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		| ((((b2 & 0x0404040404040404ull) * 0x40810204081020ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (5 << 16)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x8040201008040201ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (5 << 16)
		| (((b1 & 0x0000000102040810ull) * 0x101010101000000ull) >> 56)
		| ((((b2 & 0x0000000102040810ull) * 0x101010101000000ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_C4(BitBoard& b1, BitBoard& b2) {
	const int pos = 19;
	BitBoard toFlip;
	// right & left
	int placement = (3 << 16)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		| ((b2 & 0x0000ff0000000000ull) >> 32);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (2 << 16)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		| ((((b2 & 0x1010101010101010ull) * 0x10204081020408ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (3 << 16)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x4020100804020100ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (3 << 16)
		| (((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x0408102040800000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_C5(BitBoard& b1, BitBoard& b2) {
	const int pos = 20;
	BitBoard toFlip;
	// right & left
	int placement = (4 << 16)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		| ((b2 & 0x0000ff0000000000ull) >> 32);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (2 << 16)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		| ((((b2 & 0x0808080808080808ull) * 0x20408102040810ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (4 << 16)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x2010080402010000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (4 << 16)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x0204081020408000ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_D3(BitBoard& b1, BitBoard& b2) {
	const int pos = 26;
	BitBoard toFlip;
	// right & left
	int placement = (2 << 16)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		| ((b2 & 0x000000ff00000000ull) >> 24);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (3 << 16)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		| ((((b2 & 0x2020202020202020ull) * 0x8102040810204ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (2 << 16)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0080402010080402ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (2 << 16)
		| (((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x0408102040800000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_D6(BitBoard& b1, BitBoard& b2) {
	const int pos = 29;
	BitBoard toFlip;
	// right & left
	int placement = (5 << 16)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		| ((b2 & 0x000000ff00000000ull) >> 24);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (3 << 16)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		| ((((b2 & 0x0404040404040404ull) * 0x40810204081020ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (5 << 16)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		| ((((b2 & 0x2010080402010000ull) * 0x10101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (5 << 16)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0001020408102040ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_E3(BitBoard& b1, BitBoard& b2) {
	const int pos = 34;
	BitBoard toFlip;
	// right & left
	int placement = (2 << 16)
		| ((b1 & 0xff000000ull) >> 24)
		| ((b2 & 0xff000000ull) >> 16);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (4 << 16)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		| ((((b2 & 0x2020202020202020ull) * 0x8102040810204ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (2 << 16)
		| (((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000804020100804ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (2 << 16)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x0204081020408000ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_E6(BitBoard& b1, BitBoard& b2) {
	const int pos = 37;
	BitBoard toFlip;
	// right & left
	int placement = (5 << 16)
		| ((b1 & 0xff000000ull) >> 24)
		| ((b2 & 0xff000000ull) >> 16);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (4 << 16)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		| ((((b2 & 0x0404040404040404ull) * 0x40810204081020ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (5 << 16)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x4020100804020100ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (5 << 16)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000010204081020ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_F4(BitBoard& b1, BitBoard& b2) {
	const int pos = 43;
	BitBoard toFlip;
	// right & left
	int placement = (3 << 16)
		| ((b1 & 0x00ff0000ull) >> 16)
		| ((b2 & 0x00ff0000ull) >> 8);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (5 << 16)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		| ((((b2 & 0x1010101010101010ull) * 0x10204081020408ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (3 << 16)
		| (((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000804020100804ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (3 << 16)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0001020408102040ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_F5(BitBoard& b1, BitBoard& b2) {
	const int pos = 44;
	BitBoard toFlip;
	// right & left
	int placement = (4 << 16)
		| ((b1 & 0x00ff0000ull) >> 16)
		| ((b2 & 0x00ff0000ull) >> 8);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (5 << 16)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		| ((((b2 & 0x0808080808080808ull) * 0x20408102040810ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (4 << 16)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0080402010080402ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (4 << 16)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		| ((((b2 & 0x0000010204081020ull) * 0x101010101010000ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_D4(BitBoard& b1, BitBoard& b2) {
	const int pos = 27;
	BitBoard toFlip;
	// right & left
	int placement = (3 << 16)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		| ((b2 & 0x000000ff00000000ull) >> 24);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (3 << 16)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		| ((((b2 & 0x1010101010101010ull) * 0x10204081020408ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (3 << 16)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x8040201008040201ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (3 << 16)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x0204081020408000ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_D5(BitBoard& b1, BitBoard& b2) {
	const int pos = 28;
	BitBoard toFlip;
	// right & left
	int placement = (4 << 16)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		| ((b2 & 0x000000ff00000000ull) >> 24);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (3 << 16)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		| ((((b2 & 0x0808080808080808ull) * 0x20408102040810ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (4 << 16)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		| ((((b2 & 0x4020100804020100ull) * 0x1010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (4 << 16)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x0102040810204080ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_E4(BitBoard& b1, BitBoard& b2) {
	const int pos = 35;
	BitBoard toFlip;
	// right & left
	int placement = (3 << 16)
		| ((b1 & 0xff000000ull) >> 24)
		| ((b2 & 0xff000000ull) >> 16);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (4 << 16)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		| ((((b2 & 0x1010101010101010ull) * 0x10204081020408ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (3 << 16)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0080402010080402ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (3 << 16)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x0102040810204080ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

bool CALLSPEC Solver::putChess_E5(BitBoard& b1, BitBoard& b2) {
	const int pos = 36;
	BitBoard toFlip;
	// right & left
	int placement = (4 << 16)
		| ((b1 & 0xff000000ull) >> 24)
		| ((b2 & 0xff000000ull) >> 16);
	int delta = fastFlipPattern[placement][1];
	toFlip = lineTable[pos][pos + delta];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta];
	// down & up
	placement = (4 << 16)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		| ((((b2 & 0x0808080808080808ull) * 0x20408102040810ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 8];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 8];
	// down-right & up-left
	placement = (4 << 16)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		| ((((b2 & 0x8040201008040201ull) * 0x101010101010101ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos + delta * 9];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos + delta * 9];
	// up-right & down-left
	placement = (4 << 16)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		| ((((b2 & 0x0001020408102040ull) * 0x101010101010100ull) & 0xff00000000000000ull) >> 48);
	delta = fastFlipPattern[placement][1];
	toFlip |= lineTable[pos][pos - delta * 7];
	delta = fastFlipPattern[placement][0];
	toFlip |= lineTable[pos][pos - delta * 7];

	if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false;
}

} // namespace Othello
