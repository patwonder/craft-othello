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
								Solver_countFlips.cpp
		作者：Patrick
		概述：包含类 Solver 数最后一步翻棋数目的函数的定义。
			Solver 是 Craft 的核心 AI 引擎。

********************************************************************************
*/
#include "StdAfx.h"
#include "Solver.h"
#include "Bitboard.h"

namespace CraftEngine {

int CALLSPEC Solver::countFlips_A1(const BitBoard& b1) {
	//const int pos = 0;
	int flipped;
	// right
	flipped = fastCountPattern[(b1 >> 56) ];
	
	// down
	flipped += fastCountPattern[(((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		];
	
	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_A8(const BitBoard& b1) {
	//const int pos = 7;
	int flipped;
	// left
	flipped = fastCountPattern[(7 << 8)
		| (b1 >> 56) ];
	
	// down
	flipped += fastCountPattern[(((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		];
	
	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_H1(const BitBoard& b1) {
	//const int pos = 56;
	int flipped;
	// right
	flipped = fastCountPattern[(b1 & 0xffull) ];
	
	// up
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		];
	
	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_H8(const BitBoard& b1) {
	//const int pos = 63;
	int flipped;
	// left
	flipped = fastCountPattern[(7 << 8)
		| (b1 & 0xffull) ];
	
	// up
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		];
	
	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_A2(const BitBoard& b1) {
	//const int pos = 1;
	int flipped;
	// right
	flipped = fastCountPattern[(1 << 8)
		| (b1 >> 56) ];
	
	// down
	flipped += fastCountPattern[(((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		];
	
	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_A7(const BitBoard& b1) {
	//const int pos = 6;
	int flipped;
	// left
	flipped = fastCountPattern[(6 << 8)
		| (b1 >> 56) ];
	
	// down
	flipped += fastCountPattern[(((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		];
	
	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_B1(const BitBoard& b1) {
	//const int pos = 8;
	int flipped;
	// right
	flipped = fastCountPattern[((b1 & 0x00ff000000000000ull) >> 48) 
		];
	
	// down
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		];
	
	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_B8(const BitBoard& b1) {
	//const int pos = 15;
	int flipped;
	// left
	flipped = fastCountPattern[(7 << 8)
		| ((b1 & 0x00ff000000000000ull) >> 48) 
		];
	
	// down
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_G1(const BitBoard& b1) {
	//const int pos = 48;
	int flipped;
	// right
	flipped = fastCountPattern[((b1 & 0x0000ff00ull) >> 8)
		];
	
	// up
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_G8(const BitBoard& b1) {
	//const int pos = 55;
	int flipped;
	// left
	flipped = fastCountPattern[(7 << 8)
		| ((b1 & 0x0000ff00ull) >> 8)
		];
	
	// up
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_H2(const BitBoard& b1) {
	//const int pos = 57;
	int flipped;
	// right
	flipped = fastCountPattern[(1 << 8)
		| (b1 & 0x000000ffull)
		];
	
	// up
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_H7(const BitBoard& b1) {
	//const int pos = 62;
	int flipped;
	// left
	flipped = fastCountPattern[(6 << 8)
		| (b1 & 0x000000ffull)
		];
	
	// up
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_A3(const BitBoard& b1) {
	//const int pos = 2;
	int flipped;
	// right & left
	flipped = fastCountPattern[(2 << 8)
		| (b1 >> 56)
		];
	
	// down
	flipped += fastCountPattern[(((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x2040800000000000ull) * 0x10101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_A6(const BitBoard& b1) {
	//const int pos = 5;
	int flipped;
	// right & left	
	flipped = fastCountPattern[(5 << 8)
		| (b1 >> 56)
		];
	
	// down
	flipped += fastCountPattern[(((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0402010000000000ull) * 0x10101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_C1(const BitBoard& b1) {
	//const int pos = 16;
	int flipped;
	// right
	flipped = fastCountPattern[((b1 & 0x0000ff0000000000ull) >> 40)
		];
	
	// down & up
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(((b1 & 0x2040800000000000ull) * 0x10101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_C8(const BitBoard& b1) {
	//const int pos = 23;
	int flipped;
	// left
	flipped = fastCountPattern[(7 << 8)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		];
	
	// down & up
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0402010000000000ull) * 0x10101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_F1(const BitBoard& b1) {
	//const int pos = 40;
	int flipped;
	// right
	flipped = fastCountPattern[((b1 & 0x00ff0000ull) >> 16)
		];
	
	// down & up
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(((b1 & 0x0000000000804020ull) * 0x101010000000000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_F8(const BitBoard& b1) {
	//const int pos = 47;
	int flipped;
	// left
	flipped = fastCountPattern[(7 << 8)
		| ((b1 & 0x00ff0000ull) >> 16)
		];
	
	// down & up
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0000000000010204ull) * 0x101010000000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_H3(const BitBoard& b1) {
	//const int pos = 58;
	int flipped;
	// right & left
	flipped = fastCountPattern[(2 << 8)
		| (b1 & 0x000000ffull)
		];
	
	// up
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0000000000804020ull) * 0x101010000000000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_H6(const BitBoard& b1) {
	//const int pos = 61;
	int flipped;
	// right & left
	flipped = fastCountPattern[(5 << 8)
		| (b1 & 0x000000ffull)
		];
	
	// up
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0000000000010204ull) * 0x101010000000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_A4(const BitBoard& b1) {
	//const int pos = 3;
	int flipped;
	// right & left
	flipped = fastCountPattern[(3 << 8)
		| (b1 >> 56)
		];
	
	// down
	flipped += fastCountPattern[(((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x1008040201000000ull) * 0x101010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x1020408000000000ull) * 0x1010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_A5(const BitBoard& b1) {
	//const int pos = 4;
	int flipped;
	// right & left
	flipped = fastCountPattern[(4 << 8)
		| (b1 >> 56)
		];
	
	// down
	flipped += fastCountPattern[(((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0804020100000000ull) * 0x1010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0810204080000000ull) * 0x101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_D1(const BitBoard& b1) {
	//const int pos = 24;
	int flipped;
	// right
	flipped = fastCountPattern[((b1 & 0x000000ff00000000ull) >> 32)
		];
	
	// down & up
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(((b1 & 0x0000008040201008ull) * 0x101010101000000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(((b1 & 0x1020408000000000ull) * 0x1010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_D8(const BitBoard& b1) {
	//const int pos = 31;
	int flipped;
	// left
	flipped = fastCountPattern[(7 << 8)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		];
	
	// down & up
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0804020100000000ull) * 0x1010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0000000102040810ull) * 0x101010101000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_E1(const BitBoard& b1) {
	//const int pos = 32;
	int flipped;
	// right
	flipped = fastCountPattern[((b1 & 0xff000000ull) >> 24)
		];
	
	// down & up
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x8080808080808080ull) * 0x2040810204081ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(((b1 & 0x0000000080402010ull) * 0x101010100000000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(((b1 & 0x0810204080000000ull) * 0x101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_E8(const BitBoard& b1) {
	//const int pos = 39;
	int flipped;
	// left
	flipped = fastCountPattern[(7 << 8)
		| ((b1 & 0xff000000ull) >> 24)
		];
	
	// down & up
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0101010101010101ull) * 0x102040810204080ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x1008040201000000ull) * 0x101010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0000000001020408ull) * 0x101010100000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_H4(const BitBoard& b1) {
	//const int pos = 59;
	int flipped;
	// right & left
	flipped = fastCountPattern[(3 << 8)
		| (b1 & 0x000000ffull)
		];
	
	// up
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0000000080402010ull) * 0x101010100000000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0000000102040810ull) * 0x101010101000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_H5(const BitBoard& b1) {
	//const int pos = 60;
	int flipped;
	// right & left
	flipped = fastCountPattern[(4 << 8)
		| (b1 & 0x000000ffull)
		];
	
	// up
	flipped += fastCountPattern[(7 << 8)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0000008040201008ull) * 0x101010101000000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0000000001020408ull) * 0x101010100000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_B2(const BitBoard& b1) {
	//const int pos = 9;
	int flipped;
	// right
	flipped = fastCountPattern[(1 << 8)
		| ((b1 & 0x00ff000000000000ull) >> 48) 
		];
	
	// down
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_B7(const BitBoard& b1) {
	//const int pos = 14;
	int flipped;
	// left
	flipped = fastCountPattern[(6 << 8)
		| ((b1 & 0x00ff000000000000ull) >> 48) 
		];
	
	// down
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_G2(const BitBoard& b1) {
	//const int pos = 49;
	int flipped;
	// right
	flipped = fastCountPattern[(1 << 8)
		| ((b1 & 0x0000ff00ull) >> 8)
		];
	
	// up
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_G7(const BitBoard& b1) {
	//const int pos = 54;
	int flipped;
	// left
	flipped = fastCountPattern[(6 << 8)
		| ((b1 & 0x0000ff00ull) >> 8)
		];
	
	// up
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_B3(const BitBoard& b1) {
	//const int pos = 10;
	int flipped;
	// right & left
	flipped = fastCountPattern[(2 << 8)
		| ((b1 & 0x00ff000000000000ull) >> 48)
		];
	
	// down
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x1020408000000000ull) * 0x1010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_B6(const BitBoard& b1) {
	//const int pos = 13;
	int flipped;
	// right & left
	flipped = fastCountPattern[(5 << 8)
		| ((b1 & 0x00ff000000000000ull) >> 48)
		];
	
	// down
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0804020100000000ull) * 0x1010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_C2(const BitBoard& b1) {
	//const int pos = 17;
	int flipped;
	// right
	flipped = fastCountPattern[(1 << 8)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		];
	
	// down & up
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x1020408000000000ull) * 0x1010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_C7(const BitBoard& b1) {
	//const int pos = 22;
	int flipped;
	// left
	flipped = fastCountPattern[(6 << 8)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		];
	
	// down & up
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0804020100000000ull) * 0x1010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_F2(const BitBoard& b1) {
	//const int pos = 41;
	int flipped;
	// right
	flipped = fastCountPattern[(1 << 8)
		| ((b1 & 0x00ff0000ull) >> 16)
		];
	
	// down & up
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0000000080402010ull) * 0x101010100000000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_F7(const BitBoard& b1) {
	//const int pos = 46;
	int flipped;
	// left
	flipped = fastCountPattern[(6 << 8)
		| ((b1 & 0x00ff0000ull) >> 16)
		];
	
	// down & up
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0000000001020408ull) * 0x101010100000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_G3(const BitBoard& b1) {
	//const int pos = 50;
	int flipped;
	// right & left
	flipped = fastCountPattern[(2 << 8)
		| ((b1 & 0x0000ff00ull) >> 8)
		];
	
	// up
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0000000080402010ull) * 0x101010100000000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_G6(const BitBoard& b1) {
	//const int pos = 53;
	int flipped;
	// right & left
	flipped = fastCountPattern[(5 << 8)
		| ((b1 & 0x0000ff00ull) >> 8)
		];
	
	// up
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0000000001020408ull) * 0x101010100000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_B4(const BitBoard& b1) {
	//const int pos = 11;
	int flipped;
	// right & left
	flipped = fastCountPattern[(3 << 8)
		| ((b1 & 0x00ff000000000000ull) >> 48)
		];
	
	// down
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0810204080000000ull) * 0x101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_B5(const BitBoard& b1) {
	//const int pos = 12;
	int flipped;
	// right & left
	flipped = fastCountPattern[(4 << 8)
		| ((b1 & 0x00ff000000000000ull) >> 48)
		];
	
	// down
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x1008040201000000ull) * 0x101010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_D2(const BitBoard& b1) {
	//const int pos = 25;
	int flipped;
	// right
	flipped = fastCountPattern[(1 << 8)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		];
	
	// down & up
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0810204080000000ull) * 0x101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_D7(const BitBoard& b1) {
	//const int pos = 30;
	int flipped;
	// left
	flipped = fastCountPattern[(6 << 8)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		];
	
	// down & up
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x1008040201000000ull) * 0x101010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_E2(const BitBoard& b1) {
	//const int pos = 33;
	int flipped;
	// right
	flipped = fastCountPattern[(1 << 8)
		| ((b1 & 0xff000000ull) >> 24)
		];
	
	// down & up
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x4040404040404040ull) * 0x4081020408102ull) >> 56)
		];
	
	// down-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0000008040201008ull) * 0x101010101000000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(1 << 8)
		| (((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_E7(const BitBoard& b1) {
	//const int pos = 38;
	int flipped;
	// left
	flipped = fastCountPattern[(6 << 8)
		| ((b1 & 0xff000000ull) >> 24)
		];
	
	// down & up
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0202020202020202ull) * 0x081020408102040ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		];
	
	// down-left
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0000000102040810ull) * 0x101010101000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_G4(const BitBoard& b1) {
	//const int pos = 51;
	int flipped;
	// right & left
	flipped = fastCountPattern[(3 << 8)
		| ((b1 & 0x0000ff00ull) >> 8)
		];
	
	// up
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0000008040201008ull) * 0x101010101000000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_G5(const BitBoard& b1) {
	//const int pos = 52;
	int flipped;
	// right & left
	flipped = fastCountPattern[(4 << 8)
		| ((b1 & 0x0000ff00ull) >> 8)
		];
	
	// up
	flipped += fastCountPattern[(6 << 8)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		];
	
	// up-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		];
	
	// up-right
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0000000102040810ull) * 0x101010101000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_C3(const BitBoard& b1) {
	//const int pos = 18;
	int flipped;
	// right & left
	flipped = fastCountPattern[(2 << 8)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		];
	
	// down & up
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0810204080000000ull) * 0x101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_C6(const BitBoard& b1) {
	//const int pos = 21;
	int flipped;
	// right & left
	flipped = fastCountPattern[(5 << 8)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		];
	
	// down & up
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x1008040201000000ull) * 0x101010101ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_F3(const BitBoard& b1) {
	//const int pos = 42;
	int flipped;
	// right & left
	flipped = fastCountPattern[(2 << 8)
		| ((b1 & 0x00ff0000ull) >> 16)
		];
	
	// down & up
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0000008040201008ull) * 0x101010101000000ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_F6(const BitBoard& b1) {
	//const int pos = 45;
	int flipped;
	// right & left
	flipped = fastCountPattern[(5 << 8)
		| ((b1 & 0x00ff0000ull) >> 16)
		];
	
	// down & up
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0000000102040810ull) * 0x101010101000000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_C4(const BitBoard& b1) {
	//const int pos = 19;
	int flipped;
	// right & left
	flipped = fastCountPattern[(3 << 8)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		];
	
	// down & up
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_C5(const BitBoard& b1) {
	//const int pos = 20;
	int flipped;
	// right & left
	flipped = fastCountPattern[(4 << 8)
		| ((b1 & 0x0000ff0000000000ull) >> 40)
		];
	
	// down & up
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_D3(const BitBoard& b1) {
	//const int pos = 26;
	int flipped;
	// right & left
	flipped = fastCountPattern[(2 << 8)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		];
	
	// down & up
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0408102040800000ull) * 0x10101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_D6(const BitBoard& b1) {
	//const int pos = 29;
	int flipped;
	// right & left
	flipped = fastCountPattern[(5 << 8)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		];
	
	// down & up
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x2010080402010000ull) * 0x10101010101ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_E3(const BitBoard& b1) {
	//const int pos = 34;
	int flipped;
	// right & left
	flipped = fastCountPattern[(2 << 8)
		| ((b1 & 0xff000000ull) >> 24)
		];
	
	// down & up
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x2020202020202020ull) * 0x8102040810204ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(2 << 8)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_E6(const BitBoard& b1) {
	//const int pos = 37;
	int flipped;
	// right & left
	flipped = fastCountPattern[(5 << 8)
		| ((b1 & 0xff000000ull) >> 24)
		];
	
	// down & up
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0404040404040404ull) * 0x40810204081020ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_F4(const BitBoard& b1) {
	//const int pos = 43;
	int flipped;
	// right & left
	flipped = fastCountPattern[(3 << 8)
		| ((b1 & 0x00ff0000ull) >> 16)
		];
	
	// down & up
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0000804020100804ull) * 0x101010101010000ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_F5(const BitBoard& b1) {
	//const int pos = 44;
	int flipped;
	// right & left
	flipped = fastCountPattern[(4 << 8)
		| ((b1 & 0x00ff0000ull) >> 16)
		];
	
	// down & up
	flipped += fastCountPattern[(5 << 8)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0000010204081020ull) * 0x101010101010000ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_D4(const BitBoard& b1) {
	//const int pos = 27;
	int flipped;
	// right & left
	flipped = fastCountPattern[(3 << 8)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		];
	
	// down & up
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0204081020408000ull) * 0x1010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_D5(const BitBoard& b1) {
	//const int pos = 28;
	int flipped;
	// right & left
	flipped = fastCountPattern[(4 << 8)
		| ((b1 & 0x000000ff00000000ull) >> 32)
		];
	
	// down & up
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x4020100804020100ull) * 0x1010101010101ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_E4(const BitBoard& b1) {
	//const int pos = 35;
	int flipped;
	// right & left
	flipped = fastCountPattern[(3 << 8)
		| ((b1 & 0xff000000ull) >> 24)
		];
	
	// down & up
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x1010101010101010ull) * 0x10204081020408ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0080402010080402ull) * 0x101010101010100ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(3 << 8)
		| (((b1 & 0x0102040810204080ull) * 0x101010101010101ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

int CALLSPEC Solver::countFlips_E5(const BitBoard& b1) {
	//const int pos = 36;
	int flipped;
	// right & left
	flipped = fastCountPattern[(4 << 8)
		| ((b1 & 0xff000000ull) >> 24)
		];
	
	// down & up
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0808080808080808ull) * 0x20408102040810ull) >> 56)
		];
	
	// down-right & up-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x8040201008040201ull) * 0x101010101010101ull) >> 56)
		];
	
	// up-right & down-left
	flipped += fastCountPattern[(4 << 8)
		| (((b1 & 0x0001020408102040ull) * 0x101010101010100ull) >> 56)
		];
	

	/* if (toFlip) {
		b1 |= toFlip | posTable[pos];
		b2 ^= toFlip;
		return true;
	} else return false; */
	return flipped;
}

} // namespace Othello
