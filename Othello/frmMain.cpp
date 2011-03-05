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
								frmMain.cpp
		作者：Patrick
		概述：包含类 frmMain 的定义。
			frmMain 类实现 Craft 程序主窗体。

********************************************************************************
*/

#include "StdAfx.h"
#include "frmMain.h"
#include "BoardSetup.h"
#include "frmAbout.h"
#include "SecondInstance.h"
#include "BoardResolver.h"

using namespace System;
using namespace Othello;
using namespace CraftEngine;

void frmMain::playerMoved(Othello::Move move, 
		GameContext^ gcBlack, GameContext^ gcWhite) {
	addMove(gcBlack->getCurrentStep() - 1, move);
	setBoard();
	continueGame = true;
	if (userInfo->PlaySound && move.getColor() != Chess::AVAILABLE)
		playSound(goSound);
}

void frmMain::gameStarted(Chess firstPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	this->gcBlack = gcBlack;
	this->gcWhite = gcWhite;
	addMoves();
	setBoard();
	gameRunning = true;
	setGameInfo();
	tsbtnLearn->Enabled = false;
	mnuLearn->Enabled = false;
}

void frmMain::gameReseted(Chess firstPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	gameRunning = false;
	restoreState();
}

String^ frmMain::getComment(PlayerType type, int result) {
	String^ msg = Players::getAIPlayerName(type) + ": ";
	switch (type) {
	case PlayerType::RANDOM:
	case PlayerType::BIG_EAT:
		if (result < -32) {
			msg += "帮助中的战术指南看了吗？";
		} else if (result < -8) {
			msg += "多试试，你会找到诀窍的。";
		} else if (result < 0) {
			msg += "还差一点。继续努力！";
		} else if (result == 0) {
			msg += "要再加把劲啊！";
		} else if (result <= 8) {
			msg += "恭喜！你已经入门了。";
		} else if (result <= 32) {
			msg += "加油！你可以赢更多的。";
		} else {
			msg += "很好，你可以试着挑战更高级别的棋手了。";
		}
		break;
	case PlayerType::EARTH:
	case PlayerType::CLOUDS:
	case PlayerType::ORBIT:
	case PlayerType::MOON:
		if (result < -32) {
			msg += "也许你应该找低级别的棋手下棋。";
		} else if (result < -8) {
			msg += "下的还可以，不要灰心！";
		} else if (result < 0) {
			msg += "好棋。你可以下得更好的。";
		} else if (result == 0) {
			msg += "握手言和。下得不错！";
		} else if (result <= 8) {
			msg += "嗯，下得非常不错！";
		} else if (result <= 32) {
			msg += "下得太好啦！";
		} else {
			msg += "下得太好了，你应该找个高手下棋！";
		}
		break;
	case PlayerType::STAR:
	case PlayerType::GALAXY:
		if (result < -32) {
			msg += "别浪费时间了。找个低手多练练吧。";
		} else if (result < -8) {
			msg += "不错不错，有前途。";
		} else if (result < 0) {
			msg += "下得不错，好棋。";
		} else if (result == 0) {
			msg += "加油，胜利就在眼前！";
		} else if (result <= 8) {
			msg += "下得太好啦！";
		} else if (result <= 32) {
			msg += "你赢了，我甘拜下风！";
		} else {
			msg += "也许你应该找个高手下棋！";
		}
		break;
	case PlayerType::UNIVERSE:
		if (result < -32) {
			msg += "别浪费时间了。找个低手多练练吧。";
		} else if (result < -8) {
			msg += "不错不错，有前途。";
		} else if (result < 0) {
			msg += "下得不错，好棋。";
		} else if (result == 0) {
			msg += "加油，胜利就在眼前！";
		} else if (result <= 8) {
			msg += "下得太好啦！";
		} else if (result <= 32) {
			msg += "你赢了，我甘拜下风！";
		} else {
			msg += "你无可阻挡。尝试一下更优秀的黑白棋程序吧~~";
		}
		break;
	default:
		msg = "未知对手。";
	}
	return msg;
}

System::String^ frmMain::getEndGameResultDescription(int result) {
	System::String^ res;
	if (bestResult != 0) {
		res = (result / 2 + 32).ToString() + ":" + (32 - result / 2).ToString();
		res += (result > 0) ? " 胜" : " 负";
	} else {
		res = "平局";
	}
	return res;
}

void frmMain::gameEnded(Chess winner, int nBlack, int nWhite, 
		GameContext^ gcBlack, GameContext^ gcWhite) {
	if (!gameRunning) return;

	notifyUser();

	gameRunning = false;
	guiTipper->reset();
	String^ msg;
	if (endGameMode) {
		msg = "最佳结果: ";
		if (bestResult != 0) {
			msg += (bestResult / 2 + 32).ToString() + ":" + (32 - bestResult / 2).ToString();
			msg += (bestResult > 0) ? " 胜" : " 负";
		} else {
			msg += "平局";
		}
		msg += "\n实际结果: ";
		int realResult = (blackType == PlayerType::GUI) ? nBlack - nWhite : nWhite - nBlack;
		if (realResult != 0) {
			msg += (realResult / 2 + 32).ToString() + ":" + (32 - realResult / 2).ToString();
			msg += (realResult > 0) ? " 胜" : " 负";
		} else {
			msg += "平局";
		}
		if (realResult == bestResult) {
			msg += "\n恭喜您找到了最佳走法！\n再玩一局？";
			if (userInfo->PlaySound) playSound(winSound);
			Windows::Forms::DialogResult res = MessageBox::Show(this, msg,
				"残局结果", MessageBoxButtons::YesNo, MessageBoxIcon::Information,
				MessageBoxDefaultButton::Button1);
			leaveEndGameMode();
			if (res == Windows::Forms::DialogResult::Yes)
				startNewEndGame(currentEndGameDifficulty);
		} else {
			msg += "\n您总共损失了" + ((bestResult - realResult) / 2).ToString() + "子";
			msg += "\n要再试一次吗？选择否将进入棋局分析。";
			if (userInfo->PlaySound) playSound(loseSound);
			Windows::Forms::DialogResult res = MessageBox::Show(this, msg,
				"残局结果", MessageBoxButtons::YesNo, MessageBoxIcon::Information,
				MessageBoxDefaultButton::Button1);
			if (res == Windows::Forms::DialogResult::Yes) {
				restartGame();
			} else {
				leaveEndGameMode();
				analyzer = gcnew frmAnalyzer(this, gcBlack, PlayerType::UNIVERSE);
				analyzer->Show(this);
				analyzer->startAnalyze();
			}
		}
	} else {
		msg = "终局比分 " + nBlack + " : " + nWhite + "\n";
		switch (winner) {
			case Chess::BLACK :
				msg += "黑方胜";
				break;
			case Chess::WHITE :
				msg += "白方胜";
				break;
			case Chess::AVAILABLE :
				msg += "平局";
		}
		if (blackType == PlayerType::GUI && whiteType != PlayerType::GUI) {
			msg += "\n" + getComment(whiteType, nBlack - nWhite);
			if (fairGame)
				userInfo->getStatistics(whiteType)->recordGame(nBlack - nWhite);
			if (userInfo->PlaySound) {
				switch (winner) {
					case Chess::BLACK :
						playSound(winSound);
						break;
					case Chess::WHITE :
						playSound(loseSound);
						break;
					case Chess::AVAILABLE :
						playSound(drawSound);
				}
			}
		} else if (blackType != PlayerType::GUI && whiteType == PlayerType::GUI) {
			msg += "\n" + getComment(blackType, nWhite - nBlack);
			if (fairGame)
				userInfo->getStatistics(blackType)->recordGame(nWhite - nBlack);
			if (userInfo->PlaySound) {
				switch (winner) {
					case Chess::WHITE :
						playSound(winSound);
						break;
					case Chess::BLACK :
						playSound(loseSound);
						break;
					case Chess::AVAILABLE :
						playSound(drawSound);
				}
			}
		}
		MessageBox::Show(this, msg, "游戏结束", MessageBoxButtons::OK, MessageBoxIcon::Information);
		fairGame = false;
		setGameInfo();
		if (game->isLearnable()) {
			if (userInfo->AutoLearn) {
				learnGame();
			} else {
				tsbtnLearn->Enabled = true;
				mnuLearn->Enabled = true;
			}
		}
	}
}

void frmMain::boardChanged(Chess firstPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	gameRunning = false;
	restoreState();
}

void frmMain::playerChanged(Chess currentPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	gameRunning = false;
	if (gcBlack->isGameEnded())
		restartGame();
	else restoreState();
}

void frmMain::timeLimitChanged(int timeLimit,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	gameRunning = false;
	restoreState();
}

void frmMain::playerOverTimed(Chess overTimer,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	gameRunning = false;
	restoreState();
}

void frmMain::startGameForGUIPlayer() {
	if (((gcBlack->getCurrentPlayer() == Chess::BLACK) ? blackType : whiteType)
		== PlayerType::GUI && !analyzeMode) {
			startGame();
	}
}

void frmMain::gameGoneBack(int step,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	gameRunning = false;
	restoreState();
	startGameForGUIPlayer();
}

void frmMain::gameGoneForward(int step,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	gameRunning = false;
	restoreState();
	startGameForGUIPlayer();
}

void frmMain::gameHasBeenPaused(Chess currentPlayer,
		GameContext^ gcBlack, GameContext^ gcWhite) {
	gameRunning = false;
	restoreState();
}

bool frmMain::OnIdle() {
	if (!continueGame) return false;

	if (gamePaused)
		return false;
	if (!game->canGoOn())
		return false;
	continueGame = false;
	if (gameRunning) {
		game->goOn();
	}
	
	return continueGame;
}

System::Void frmMain::mnu2GB_Click(System::Object^  sender, System::EventArgs^  e) {
#ifdef MACHINE_X64
	setTableSize(0x2000000);
#endif
}

System::Void frmMain::mnu1GB_Click(System::Object^  sender, System::EventArgs^  e) {
	setTableSize(0x1000000);
}

System::Void frmMain::mnu512MB_Click(System::Object ^sender, System::EventArgs ^e) {
	setTableSize(0x800000);
}

System::Void frmMain::mnu256MB_Click(System::Object ^sender, System::EventArgs ^e) {
	setTableSize(0x400000);
}

System::Void frmMain::mnu128MB_Click(System::Object ^sender, System::EventArgs ^e) {
	setTableSize(0x200000);
}

System::Void frmMain::mnu64MB_Click(System::Object ^sender, System::EventArgs ^e) {
	setTableSize(0x100000);
}

System::Void frmMain::mnu32MB_Click(System::Object ^sender, System::EventArgs ^e) {
	setTableSize(0x80000);
}

System::Void frmMain::mnu16MB_Click(System::Object ^sender, System::EventArgs ^e) {
	setTableSize(0x40000);
}

System::Void frmMain::mnu8MB_Click(System::Object ^sender, System::EventArgs ^e) {
	setTableSize(0x20000);
}

System::Void frmMain::mnu4MB_Click(System::Object ^sender, System::EventArgs ^e) {
	setTableSize(0x10000);
}

void frmMain::setTableSize(int size) {
	if (searching) return;
	const int TABLE_ENTRY_SIZE = 64;
	if (!checkMem(((unsigned long long)size) * TABLE_ENTRY_SIZE)) return;
	if (userInfo->TableSize != size) {
		try {
			Solver::setCacheSize(size);
			userInfo->TableSize = (int)Solver::getCacheSize();
			setMnuTableSize();
		} catch (...) {
			if (size != 0x10000) {
				MessageBox::Show(this, "分配内存时出现异常，将重设置换表大小为最小。\n单击“确定”继续。",
					"警告", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				setTableSize(0x10000);
			} else {
				MessageBox::Show(this, "分配内存时出现异常，\n单击“确定”退出应用程序。",
					"错误", MessageBoxButtons::OK, MessageBoxIcon::Error);
				exitProgram();
			}
		}
	}
}

void frmMain::addMoves() {
	lstSteps->Items->Clear();
	if (gcBlack->getCurrentStep() > 1) {
		array<String^> ^moves = gcnew array<String^>(gcBlack->getCurrentStep() / 2);
		for (int i = 1; i < gcBlack->getCurrentStep(); i++) {
			String ^msg = getMoveDescription(i, gcBlack->getMove(i));
			if (i % 2 == 1) moves[i / 2] = msg;
			else moves[i / 2 - 1] += " " + msg;
		}
		lstSteps->Items->AddRange(moves);
		lstSteps->SetSelected(lstSteps->Items->Count - 1, true);
	}
}

void frmMain::goForward() {
	if (endGameMode) return;
	if (analyzeMode) {
		if (game->getMaxStep() > gcBlack->getCurrentStep()) game->goForward();
		return;
	}
	if (game->getMaxStep() > gcBlack->getCurrentStep()) {
		int goForwardCount;
		if ((((gcBlack->getCurrentPlayer() == Chess::BLACK) ? blackType : whiteType) 
			== PlayerType::GUI) && blackType != whiteType) {
				goForwardCount = 2;
				while (game->getMaxStep() > gcBlack->getCurrentStep() + goForwardCount) {
					if (gcBlack->getMove(gcBlack->getCurrentStep() + goForwardCount).getColor() != Chess::AVAILABLE)
						break;
					goForwardCount += 2;
				}
		} else if ((((gcBlack->getCurrentPlayer() == Chess::BLACK) ? whiteType : blackType) 
			== PlayerType::GUI) && blackType != whiteType) {
				goForwardCount = 1;
				while (game->getMaxStep() > gcBlack->getCurrentStep() + goForwardCount) {
					if (gcBlack->getMove(gcBlack->getCurrentStep() + goForwardCount).getColor() != Chess::AVAILABLE)
						break;
					goForwardCount += 2;
				}
		} else {
			goForwardCount = 1;
			while (game->getMaxStep() > gcBlack->getCurrentStep() + goForwardCount) {
				if (gcBlack->getMove(gcBlack->getCurrentStep() + goForwardCount).getColor() != Chess::AVAILABLE)
					break;
				goForwardCount += 1;
			}
		}
		if (game->getMaxStep() < gcBlack->getCurrentStep() + goForwardCount)
			goForwardCount = game->getMaxStep() - gcBlack->getCurrentStep();
		game->goForward(goForwardCount);

		if (fairGame) {
			goBackChance++;
			tsbtnBack->ToolTipText = tsbtnBack->Text + "(剩余 " + goBackChance + " 次)";
			if (goBackChance <= 3) {
				prompt("您还剩下 " + goBackChance + " 次悔棋机会！", iconWarning);
			}
		}
	}
}

System::Void frmMain::tsbtnContinue_Click(System::Object ^sender, System::EventArgs ^e) {
	goForward();
}

void frmMain::goBack() {
	if (endGameMode) return;
	if (analyzeMode) {
		if (gcBlack->getCurrentStep() > 1) game->goBack();
		return;
	}
	if (gcBlack->getCurrentStep() > 1) {
		if (fairGame && (goBackChance == 0)) {
			setPaused(true);
			bool ifbreak = breakFairness("您的悔棋次数已达到限制，继续悔棋将记您本次对局为负。\n确实要悔棋吗？");
			setPaused(false);
			if (!ifbreak) return;
		}

		int goBackCount;
		if ((((gcBlack->getCurrentPlayer() == Chess::BLACK) ? blackType : whiteType) 
			== PlayerType::GUI) && blackType != whiteType) {
				goBackCount = 2;
				while (gcBlack->getCurrentStep() > goBackCount) {
					if (gcBlack->getMove(gcBlack->getCurrentStep() - goBackCount).getColor() != Chess::AVAILABLE)
						break;
					goBackCount += 2;
				}
		} else if ((((gcBlack->getCurrentPlayer() == Chess::BLACK) ? whiteType : blackType) 
			== PlayerType::GUI) && blackType != whiteType) {
				goBackCount = 1;
				while (gcBlack->getCurrentStep() > goBackCount) {
					if (gcBlack->getMove(gcBlack->getCurrentStep() - goBackCount).getColor() != Chess::AVAILABLE)
						break;
					goBackCount += 2;
				}
		} else {
			goBackCount = 1;
			while (gcBlack->getCurrentStep() > goBackCount) {
				if (gcBlack->getMove(gcBlack->getCurrentStep() - goBackCount).getColor() != Chess::AVAILABLE)
					break;
				goBackCount += 1;
			}
		}
		if (goBackCount >= gcBlack->getCurrentStep())
			goBackCount = gcBlack->getCurrentStep() - 1;
		game->goBack(goBackCount);

		if (fairGame) {
			goBackChance--;
			tsbtnBack->ToolTipText = tsbtnBack->Text + "(剩余 " + goBackChance + " 次)";
			if (goBackChance <= 3) {
				//setPaused(true);
				//MessageBox::Show(this, "您还剩下 " + goBackChance + " 次悔棋机会！", "悔棋", MessageBoxButtons::OK,
				//	MessageBoxIcon::Information);
				//setPaused(false);
				prompt("您还剩下 " + goBackChance + " 次悔棋机会！", iconWarning);
			}
		}
	}
}

System::Void frmMain::tsbtnBack_Click(System::Object ^sender, System::EventArgs ^e) {
	goBack();
}

void frmMain::restoreState() {
	setGUIPlay(false);
	guiTipper->reset();
	try {
		game->startGame();
		btnStart->Text = gameJustStarted ? "开始游戏" : "继续游戏";
		btnStart->Visible = true;
	} catch (IllegalGameStateException^) {
		btnStart->Visible = false;
		gcBlack = game->getContext(Chess::BLACK);
		gcWhite = game->getContext(Chess::WHITE);
		addMoves();
		setBoard();
		tsbtnLearn->Enabled = false;
		mnuLearn->Enabled = false;
	}
	continueGame = false;
}

System::Void frmMain::tsmnuWhiteUniverse_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::UNIVERSE, Chess::WHITE);
}

System::Void frmMain::tsmnuWhiteGalaxy_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::GALAXY, Chess::WHITE);
}

System::Void frmMain::tsmnuWhiteStar_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::STAR, Chess::WHITE);
}

System::Void frmMain::tsmnuWhiteMoon_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::MOON, Chess::WHITE);
}

System::Void frmMain::tsmnuWhiteOrbit_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::ORBIT, Chess::WHITE);
}

System::Void frmMain::tsmnuWhiteClouds_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::CLOUDS, Chess::WHITE);
}

System::Void frmMain::tsmnuWhiteEarth_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::EARTH, Chess::WHITE);
}

System::Void frmMain::tsmnuWhiteBigEat_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::BIG_EAT, Chess::WHITE);
}

System::Void frmMain::tsmnuBlackUniverse_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::UNIVERSE, Chess::BLACK);
}

System::Void frmMain::tsmnuBlackGalaxy_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::GALAXY, Chess::BLACK);
}

System::Void frmMain::tsmnuBlackStar_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::STAR, Chess::BLACK);
}

System::Void frmMain::tsmnuBlackMoon_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::MOON, Chess::BLACK);
}

System::Void frmMain::tsmnuBlackOrbit_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::ORBIT, Chess::BLACK);
}

System::Void frmMain::tsmnuBlackClouds_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::CLOUDS, Chess::BLACK);
}

System::Void frmMain::tsmnuBlackEarth_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::EARTH, Chess::BLACK);
}

System::Void frmMain::tsmnuBlackBigEat_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::BIG_EAT, Chess::BLACK);
}

void frmMain::setBlackType(PlayerType type) {
	setTsmnuBlack(blackType, false);
	blackType = type;
	userInfo->BlackType = type;
	setTsmnuBlack(blackType, true);
}

void frmMain::setWhiteType(PlayerType type) {
	setTsmnuWhite(whiteType, false);
	whiteType = type;
	userInfo->WhiteType = type;
	setTsmnuWhite(whiteType, true);
}

void frmMain::changePlayer(PlayerType type, Chess color) {
	if (endGameMode) return;
	if (analyzeMode) return;
	bool gameChanged = false;
	if (color == Chess::BLACK) {
		if (blackType != type) {
			setPaused(true);
			bool ifbreak = breakFairness("继续切换选手将记您本次对局为负。\n确实要切换选手吗？");
			setPaused(false);
			if (!ifbreak) return;
			blackPlayer = createPlayer(type, color);
			setBlackType(type);
			gameChanged = true;
		}
	} else if (whiteType != type) {
		setPaused(true);
		bool ifbreak = breakFairness("继续切换选手将记您本次对局为负。\n确实要切换选手吗？");
		setPaused(false);
		if (!ifbreak) return;
		whitePlayer = createPlayer(type, color);
		setWhiteType(type);
		gameChanged = true;
	}
	if (gameChanged) {
		game->changeGame(blackPlayer, whitePlayer);
		if (gameJustStarted) setFairness();
	}
}

System::Void frmMain::tsmnuWhiteRandom_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::RANDOM, Chess::WHITE);
}

System::Void frmMain::tsmnuWhiteMan_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::GUI, Chess::WHITE);
}

System::Void frmMain::tsmnuBlackRandom_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::RANDOM, Chess::BLACK);
}

System::Void frmMain::tsmnuBlackMan_Click(System::Object ^sender, System::EventArgs ^e) {
	changePlayer(PlayerType::GUI, Chess::BLACK);
}

System::Void frmMain::mnuRestart_Click(System::Object ^sender, System::EventArgs ^e) {
	restartGame();
}

System::Void frmMain::mnuMachines_Click(System::Object ^sender, System::EventArgs ^e) {
	machines();
}

System::Void frmMain::mnuManToMan_Click(System::Object ^sender, System::EventArgs ^e) {
	manToMan();
}

System::Void frmMain::mnuManWhiteToMachine_Click(System::Object ^sender, System::EventArgs ^e) {
	manWhiteToMachine();
}

System::Void frmMain::mnuManBlackToMachine_Click(System::Object ^sender, System::EventArgs ^e) {
	manBlackToMachine();
}

System::Void frmMain::tsbtnRestart_Click(System::Object ^sender, System::EventArgs ^e) {
	restartGame();
}

void frmMain::restartGame() {
	if (analyzeMode) return;
	setPaused(true);
	bool ifbreak = breakFairness("重新开始游戏将记您本次对局为负。\n确实要重新开始吗？");
	setPaused(false);
	if (!ifbreak) return;
	setGameJustStarted(true);
	tsbtnAnalyze->Enabled = false;
	mnuAnalyze->Enabled = false;
	game->resetGame();
	setFairness();
	if (endGameMode) startGame();
	else autoClear();
}

System::Void frmMain::tsmnuMachines_Click(System::Object ^sender, System::EventArgs ^e) {
	machines();
}

void frmMain::machines() {
	GameOption option;
	option.black = chooseMachineType(Chess::BLACK);
	if (option.black == PlayerType::GUI)
		return;
	option.white = chooseMachineType(Chess::WHITE);
	if (option.white == PlayerType::GUI)
		return;
	option.firstPlayer = Chess::BLACK;
	option.board = nullptr;
	option.totalTimeLimit = 0;
	startNewGame(option);
}

System::Void frmMain::tsmnuManToMan_Click(System::Object ^sender, System::EventArgs ^e) {
	manToMan();
}

void frmMain::manToMan() {
	GameOption option;
	option.white = PlayerType::GUI;
	option.black = PlayerType::GUI;
	option.firstPlayer = Chess::BLACK;
	option.board = nullptr;
	option.totalTimeLimit = 0;
	startNewGame(option);
}

System::Void frmMain::tsmnuManWhiteToMachine_Click(System::Object ^sender, System::EventArgs ^e) {
	manWhiteToMachine();
}

void frmMain::manWhiteToMachine() {
	GameOption option;
	option.white = PlayerType::GUI;
	option.black = chooseMachineType(Chess::BLACK);
	if (option.black == PlayerType::GUI)
		return;
	option.firstPlayer = Chess::BLACK;
	option.board = nullptr;
	option.totalTimeLimit = 0;
	startNewGame(option);
}

System::Void frmMain::tsmnuManBlackToMachine_Click(System::Object ^sender, System::EventArgs ^e) {
	manBlackToMachine();
}

void frmMain::manBlackToMachine() {
	GameOption option;
	option.black = PlayerType::GUI;
	option.white = chooseMachineType(Chess::WHITE);
	if (option.white == PlayerType::GUI)
		return;
	option.firstPlayer = Chess::BLACK;
	option.board = nullptr;
	option.totalTimeLimit = 0;
	startNewGame(option);
}

PlayerType frmMain::chooseMachineType(Chess color) {
	PlayerType currentPlayerType = (color == Chess::BLACK) ? blackType : whiteType;
	setPaused(true);
	dlgChooseMachineType^ dialog = gcnew dlgChooseMachineType(color, currentPlayerType);

	try {
		Windows::Forms::DialogResult res = dialog->ShowDialog(this);
		setPaused(false);
		
		if (res != Windows::Forms::DialogResult::OK) return PlayerType::GUI;
		return dialog->MachineType;
	} finally {
		delete dialog;
	}
}

void frmMain::setPaused(bool paused) {
	gamePaused = paused;
	if (peekMode) leavePeekMode();
}

void frmMain::setTsmnuWhite(PlayerType type, bool state) {
	switch (type) {
	case PlayerType::GUI:
		tsmnuWhiteMan->Checked = state;
		break;
	case PlayerType::RANDOM:
		tsmnuWhiteRandom->Checked = state;
		break;
	case PlayerType::BIG_EAT:
		tsmnuWhiteBigEat->Checked = state;
		break;
	case PlayerType::EARTH:
		tsmnuWhiteEarth->Checked = state;
		break;
	case PlayerType::CLOUDS:
		tsmnuWhiteClouds->Checked = state;
		break;
	case PlayerType::ORBIT:
		tsmnuWhiteOrbit->Checked = state;
		break;
	case PlayerType::MOON:
		tsmnuWhiteMoon->Checked = state;
		break;
	case PlayerType::STAR:
		tsmnuWhiteStar->Checked = state;
		break;
	case PlayerType::GALAXY:
		tsmnuWhiteGalaxy->Checked = state;
		break;
	case PlayerType::UNIVERSE:
		tsmnuWhiteUniverse->Checked = state;
		break;
	}
}

void frmMain::setTsmnuBlack(PlayerType type, bool state) {
	switch (type) {
	case PlayerType::GUI:
		tsmnuBlackMan->Checked = state;
		break;
	case PlayerType::RANDOM:
		tsmnuBlackRandom->Checked = state;
		break;
	case PlayerType::BIG_EAT:
		tsmnuBlackBigEat->Checked = state;
		break;
	case PlayerType::EARTH:
		tsmnuBlackEarth->Checked = state;
		break;
	case PlayerType::CLOUDS:
		tsmnuBlackClouds->Checked = state;
		break;
	case PlayerType::ORBIT:
		tsmnuBlackOrbit->Checked = state;
		break;
	case PlayerType::MOON:
		tsmnuBlackMoon->Checked = state;
		break;
	case PlayerType::STAR:
		tsmnuBlackStar->Checked = state;
		break;
	case PlayerType::GALAXY:
		tsmnuBlackGalaxy->Checked = state;
		break;
	case PlayerType::UNIVERSE:
		tsmnuBlackUniverse->Checked = state;
	}
}

void frmMain::addMove(int step, Othello::Move move) {
	String ^msg = getMoveDescription(step, move);
	if (step & 1) // odd numbers
		lstSteps->Items->Add(msg);
	else {
		//even numbers
		int itemCount = lstSteps->Items->Count;
		String ^last = safe_cast<String^>(lstSteps->Items[itemCount - 1]);
		lstSteps->Items[itemCount - 1] = last + " " + msg;
	}
	lstSteps->SetSelected(lstSteps->Items->Count - 1, true);
}

String^ frmMain::getMoveDescription(int step, Othello::Move move) {
	String ^msg = (step < 10) ? "0" + step.ToString() : step.ToString();
	switch (move.getColor()) {
	case Chess::BLACK:
		msg += "." + column[move.getX()] + row[move.getY()] + "(黑)";
		break;
	case Chess::WHITE:
		msg += "." + column[move.getX()] + row[move.getY()] + "(白)";
		break;
	case Chess::AVAILABLE:
		msg += ".--(欠)";
	}
	return msg;
}

void frmMain::startGame() {
	if (analyzing) return;
	if (analyzeMode) {
		previousStep = gcBlack->getCurrentStep();
		analyzer->Close();
		return;
	}
	btnStart->Visible = false;
	continueGame = true;
	if (userInfo->PlaySound && gameJustStarted) 
		playSound(startSound);
	setGameJustStarted(false);
}

System::Void frmMain::btnStart_Click(System::Object ^sender, System::EventArgs ^e) {
	startGame();
}

System::Void frmMain::frmMain_FormClosing(System::Object ^sender, System::Windows::Forms::FormClosingEventArgs ^e) {
	if (analyzing) {
		e->Cancel = true;
		try {
			System::Media::SystemSounds::Beep->Play();
		} catch (Exception^) {
			Diagnostics::Debug::WriteLine("Cannot beep!");
		}
		return;
	}
	setPaused(true);
	bool ifbreak = breakFairness("退出游戏将记您本次对局为负。\n确实要退出游戏吗？");
	setPaused(false);
	if (!ifbreak) {
		e->Cancel = true;
		return;
	}
}

void frmMain::saveBook() {
	if (!Solver::saveBook()) {
		setPaused(true);
		System::Windows::Forms::MessageBox::Show(this, "保存棋谱时出错。\n请确认您对棋谱文件有写入权限。",
			"写入棋谱文件出错", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Exclamation);
		setPaused(false);
	}
}

System::Void frmMain::frmMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
	if (analyzeMode) analyzer->Close();
	if (statisticsShowing) statisticsWindow->Close();
	endCurrentGame();
	saveBook();
	saveConfig();
}

System::Void frmMain::tsbtnExit_Click(System::Object ^sender, System::EventArgs ^e) {
	exitProgram();
}

System::Void frmMain::mnuExit_Click(System::Object ^sender, System::EventArgs ^e) {
	exitProgram();
}

void frmMain::endCurrentGame() {
	if (gameRunning) {
		guiTipper->reset();
		gameRunning = false;
		game->endGame();
		game->delObserver(this);
	}
}

void frmMain::exitProgram() {
	this->Close();
}

void frmMain::setGameJustStarted(bool state) {
	gameJustStarted = state;
	tsbtnAnalyze->Enabled = !state && !endGameMode;
	mnuAnalyze->Enabled = tsbtnAnalyze->Enabled;
}

void frmMain::startNewGame(GameOption option) {
	if (analyzeMode) return;
	if (endGameMode) leaveEndGameMode();
	setPaused(true);
	bool ifbreak = breakFairness("开始新游戏将记您本次对局为负。\n确实要开始新游戏吗？");
	setPaused(false);
	if (!ifbreak) return;
	endCurrentGame();
	createGame(option);
	game->addObserver(this);
	setGameJustStarted(true);
	restoreState();
	setFairness();
	autoClear();
}

void frmMain::startNewGame() {
	startNewGame(defaultOption);
}

void frmMain::setQuestionLevel(QuestionLevel ql) {
	switch (ql) {
		case QuestionLevel::NONE:
			ssPlayers->ForeColor = ::SystemColors::ControlText;
			ssPlayers->Font = gcnew System::Drawing::Font(ssPlayers->Font, FontStyle::Regular);
			break;
		case QuestionLevel::NORMAL:
			ssPlayers->ForeColor = ::Color::Blue;
			ssPlayers->Font = gcnew System::Drawing::Font(ssPlayers->Font, FontStyle::Bold);
			break;
		case QuestionLevel::BAD:
			ssPlayers->ForeColor = ::Color::Red;
			ssPlayers->Font = gcnew System::Drawing::Font(ssPlayers->Font, FontStyle::Bold);
			break;
	}
}

void frmMain::setBoard() {
	setSelectedMove(-1, -1);
	setFocusedMove(-1, -1);
	setTipPos(-1, -1);
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			switch (gcBlack->get(i, j)) {
			case Chess::BLACK:
				imageBoard[i, j].color = Chess::BLACK;
				break;
			case Chess::WHITE:
				imageBoard[i, j].color = Chess::WHITE;
				break;
			case Chess::AVAILABLE:
				imageBoard[i, j].color = Chess::AVAILABLE;
				break;
			}
			imageBoard[i, j].available = false;
			imageBoard[i, j].bordered = false;
			imageBoard[i, j].focused = false;
			imageBoard[i, j].selected = false;
			imageBoard[i, j].tip = false;
			imageBoard[i, j].next = false;
		}
	Othello::Move lastMove = gcBlack->getLastMove();
	if (lastMove.getColor() != Chess::AVAILABLE)
		imageBoard[lastMove.getX(), lastMove.getY()].bordered = true;
	Chess currentPlayer = gcBlack->getCurrentPlayer();
	GameContext ^currentContext = (currentPlayer == Chess::BLACK) ? gcBlack : gcWhite;
	for (int i = 0; i < currentContext->getAvailableCount(); i++) {
		Point pos = currentContext->getAvailableMove(i);
		imageBoard[pos.X, pos.Y].color = currentPlayer;
		imageBoard[pos.X, pos.Y].available = true;
	}
	if (game->getMaxStep() > currentContext->getCurrentStep()) {
		Othello::Move move = currentContext->getMove(gcBlack->getCurrentStep());
		if (move.getColor() != Chess::AVAILABLE)
			imageBoard[move.getX(), move.getY()].next = true;
	}
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			board[i, j]->setColor(imageBoard[i, j].color, imageBoard[i, j].selected, imageBoard[i, j].bordered, 
				imageBoard[i, j].focused, imageBoard[i, j].available, imageBoard[i, j].tip, imageBoard[i, j].next);
		}
	lblBNum->Text = gcBlack->getNumber().ToString();
	lblWNum->Text = gcWhite->getNumber().ToString();
	lblBCount->Text = gcBlack->getAvailableCount().ToString();
	lblWCount->Text = gcWhite->getAvailableCount().ToString();
	lblEmpty->Text = gcBlack->getEmptyNumber().ToString();
	if (!gcBlack->isGameEnded()) {
		lblCStep->Text = gcBlack->getCurrentStep().ToString();
		if (currentPlayer == Chess::BLACK) {
			picWPointer->Visible = false;
			picBPointer->Visible = true;
		}
		else {
			picBPointer->Visible = false;
			picWPointer->Visible = true;
		}
	}
	else {
		lblCStep->Text = (gcBlack->getCurrentStep() - 1).ToString();
		picBPointer->Visible = false;
		picWPointer->Visible = false;
	}
	tsbtnBack->Enabled = (gcBlack->getCurrentStep() > 1) && !endGameMode;
	mnuBack->Enabled = tsbtnBack->Enabled;
	tsbtnContinue->Enabled = (game->getMaxStep() > gcBlack->getCurrentStep()) && !endGameMode;
	mnuForward->Enabled = tsbtnContinue->Enabled;
	if (analyzeMode) {
		int step = gcBlack->getCurrentStep();
		if (step < analyzer->getSteps()) {
			AnalyzedMove move = analyzer->getAnalyzedResult(step);
			if (move.getPlayer() != Chess::AVAILABLE) {
				System::Drawing::Point playedMove = move.getPPlayedMove();
				imageBoard[playedMove.X, playedMove.Y].next = true;
				board[playedMove.X, playedMove.Y]->setNext(true);
				setQuestionLevel(move.getQuestionLevel());
				if (move.getQuestionLevel() != QuestionLevel::NONE) {
					System::Drawing::Point bestMove = move.getPBestMove();
					imageBoard[bestMove.X, bestMove.Y].tip = true;
					board[bestMove.X, bestMove.Y]->setTip(true);
				}
			} else {
				setQuestionLevel(QuestionLevel::NONE);
			}
			ssPlayers->Text = move.getGeneralDescription();
		} else {
			setQuestionLevel(QuestionLevel::NONE);
			ssPlayers->Text = "分析模式";
		}
	}
}

System::Void frmMain::backBoard_MouseClick(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e) {
	// check the situation when new users don't click "Start" button before making the first move
	if (!analyzeMode && btnStart->Visible && e->Button == System::Windows::Forms::MouseButtons::Left) {
		prompt("请单击右侧面板上的\"" + btnStart->Text + "\"按钮。", iconInfo);
	}
	// In case people make an illegal move
	if (GUIPlay && e->Button == System::Windows::Forms::MouseButtons::Left)
		prompt("根据规则，请在\"*\"标示处落子。", iconInfo);
}

System::Void frmMain::backBoard_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
	picBoard_MouseLeft();
}

System::Void frmMain::backBoard_MouseHover(System::Object^  sender, System::EventArgs^  e) {
	picBoard_MouseLeft();
}

System::Void frmMain::tmrLayout_Tick(System::Object ^sender, System::EventArgs ^e) {
	tmrLayout->Enabled = false;
	if (this->WindowState != FormWindowState::Minimized) {
		setLayout();
		picBoard->Visible = true;
	}
}

System::Void frmMain::frmMain_SizeChanged(System::Object ^sender, System::EventArgs ^e) {
	tmrLayout->Enabled = false;
	picBoard->Visible = false;
	tmrLayout->Enabled = true;
}
System::Void frmMain::frmMain_ResizeBegin(System::Object^  sender, System::EventArgs^  e) {
}
System::Void frmMain::frmMain_ResizeEnd(System::Object^  sender, System::EventArgs^  e) {
}

bool frmMain::needShowPV() {
	return userInfo->ShowPrincipleVariation && !endGameMode && !analyzeMode && !fairGame;
}

void frmMain::setLayout() {
	static int const listOffSet = -15;
	//infoPanel->SetBounds(this->ClientSize.Width - infoPanel->Width, infoPanel->Top, infoPanel->Width, statusBar->Top - infoPanel->Top);
	lstSteps->Height = infoPanel->Height - lblCurrentStep->Bottom + listOffSet;
	picBoard->Bounds = Rectangle(0, toolBar->Bottom, infoPanel->Left, 
		(userInfo->ShowPrincipleVariation ? statusBar2 : statusBar)->Top - toolBar->Bottom);
	int width = picBoard->ClientSize.Width;
	int height = picBoard->ClientSize.Height;
	double widthOffSet;
	double heightOffSet;
	double rate = (double)picBoard->Image->Width / picBoard->Image->Height;
	double bSizeWidth, bSizeHeight;
	if (((double)width / height) >= rate) {
		widthOffSet = (double)(width - height * rate) / 2;
		heightOffSet = 0.0;
		bSizeHeight = height;
		bSizeWidth = height * rate;
	}
	else {
		widthOffSet = 0.0;
		heightOffSet = (double)(height - width / rate) / 2;
		bSizeWidth = width;
		bSizeHeight = width / rate;
	}
	double boardWidth = (double)(lrCorner.X - ulCorner.X) * bSizeWidth / picBoard->Image->Width;
	double boardHeight = (double)(lrCorner.Y - ulCorner.Y) * bSizeHeight / picBoard->Image->Height;
	double boardULX = (double)ulCorner.X * bSizeWidth / picBoard->Image->Width;
	double boardULY = (double)ulCorner.Y * bSizeHeight / picBoard->Image->Height;
	double chessWidth = boardWidth / WIDTH;
	double chessHeight = boardHeight / HEIGHT;
	double borderSize = chessWidth / 25;
	chessWidth -= borderSize * 2;
	chessHeight -= borderSize * 2;
	chessWidth += 0.5;
	chessHeight += 0.5;
	ChessPicBox::setSize(static_cast<int>(chessWidth), static_cast<int>(chessHeight));
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			board[i, j]->SetBounds(
				static_cast<int>(widthOffSet + boardULX + i * boardWidth / WIDTH + borderSize + 0.5), 
				static_cast<int>(heightOffSet + boardULY + j * boardHeight / HEIGHT + borderSize + 0.5), 
				static_cast<int>(chessWidth), 
				static_cast<int>(chessHeight)
			);
			board[i, j]->performChange();
		}
	if (analyzeMode) ssPlayers->Width = statusBar->Width - 15;
	else ssPlayers->Width = statusBar->Width - ssPlayersOffset;
	ssPrompt->Width = ssPlayers->Width;
	ssPV->Width = statusBar2->Width - ssPVOffset;
}

void frmMain::setMnuTableSize() {
	for each (ToolStripMenuItem ^tsmnu in mnuTableSize->DropDownItems)
		tsmnu->Checked = false;
	switch (userInfo->TableSize) {
	case 0x2000000:
		mnu2GB->Checked = true;
		break;
	case 0x1000000:
		mnu1GB->Checked = true;
		break;
	case 0x800000:
		mnu512MB->Checked = true;
		break;
	case 0x400000:
		mnu256MB->Checked = true;
		break;
	case 0x200000:
		mnu128MB->Checked = true;
		break;
	case 0x100000:
		mnu64MB->Checked = true;
		break;
	case 0x80000:
		mnu32MB->Checked = true;
		break;
	case 0x40000:
		mnu16MB->Checked = true;
		break;
	case 0x20000:
		mnu8MB->Checked = true;
		break;
	case 0x10000:
		mnu4MB->Checked = true;
		break;
	}
}

bool frmMain::imageAbort() {
	return false;
}

void frmMain::enterPeekMode() {
	if (peekMode 
	|| selection.X >= WIDTH || selection.X < 0 || selection.Y >= HEIGHT || selection.Y < 0)
		return;
	peekMode = true;
	Board^ bd = gcnew Board(WIDTH, HEIGHT);
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			bd[i, j] = gcBlack->get(i, j);
		}
	ChessBoard^ temp = gcnew ChessBoard(bd);
	array<ChessOption, 2>^ imageBoard = gcnew array<ChessOption, 2>(WIDTH, HEIGHT);
	temp->putChess(selection.X, selection.Y, gcBlack->getCurrentPlayer());
	// set the display
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			switch (temp->get(i, j)) {
			case Chess::BLACK:
				imageBoard[i, j].color = Chess::BLACK;
				break;
			case Chess::WHITE:
				imageBoard[i, j].color = Chess::WHITE;
				break;
			case Chess::AVAILABLE:
				imageBoard[i, j].color = Chess::AVAILABLE;
				break;
			}
			imageBoard[i, j].available = false;
			imageBoard[i, j].bordered = false;
			imageBoard[i, j].focused = false;
			imageBoard[i, j].selected = false;
			imageBoard[i, j].tip = false;
			imageBoard[i, j].next = false;
		}
	Othello::Move lastMove = temp->getLastMove();
	if (lastMove.getColor() != Chess::AVAILABLE)
		imageBoard[lastMove.getX(), lastMove.getY()].bordered = true;
	Chess currentPlayer = 
		(gcBlack->getCurrentPlayer() == Chess::BLACK) ? Chess::WHITE : Chess::BLACK;
	for (int i = 0; i < temp->getAvailableCount(currentPlayer); i++) {
		Point pos = temp->getAvailableMove(currentPlayer, i);
		imageBoard[pos.X, pos.Y].color = currentPlayer;
		imageBoard[pos.X, pos.Y].available = true;
	}
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			board[i, j]->setColor(imageBoard[i, j].color, imageBoard[i, j].selected, imageBoard[i, j].bordered, 
				imageBoard[i, j].focused, imageBoard[i, j].available, imageBoard[i, j].tip, imageBoard[i, j].next);
		}
	lblBNum->Text = temp->getNumber(Chess::BLACK).ToString();
	lblWNum->Text = temp->getNumber(Chess::WHITE).ToString();
	lblBCount->Text = temp->getAvailableCount(Chess::BLACK).ToString();
	lblWCount->Text = temp->getAvailableCount(Chess::WHITE).ToString();
	lblEmpty->Text = temp->getNumber(Chess::AVAILABLE).ToString();
	this->Cursor = ::Cursors::Default;
}

void frmMain::leavePeekMode() {
	if (!peekMode) return;
	peekMode = false;
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			board[i, j]->setColor(imageBoard[i, j].color, imageBoard[i, j].selected, imageBoard[i, j].bordered, 
				imageBoard[i, j].focused, imageBoard[i, j].available, imageBoard[i, j].tip, imageBoard[i, j].next);
		}
	lblBNum->Text = gcBlack->getNumber().ToString();
	lblWNum->Text = gcWhite->getNumber().ToString();
	lblBCount->Text = gcBlack->getAvailableCount().ToString();
	lblWCount->Text = gcWhite->getAvailableCount().ToString();
	lblEmpty->Text = gcBlack->getEmptyNumber().ToString();
	int selX = selection.X, selY = selection.Y;
	setSelection(-1, -1);
	setSelection(selX, selY);
}

System::Void frmMain::picBoard_MouseDown(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e) {
	if (!GUIPlay || peekMode)
		return;
	if (e->Button == System::Windows::Forms::MouseButtons::Right) {
		ChessPicBox ^c = safe_cast<ChessPicBox^>(sender);
		int x = c->getXIndex(), y = c->getYIndex();
		if (imageBoard[x, y].available) {
			setSelection(x, y);
			enterPeekMode();
		}
	}
}

System::Void frmMain::picBoard_MouseUp(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e) {
	if (!GUIPlay || !peekMode)
		return;
	if (e->Button == System::Windows::Forms::MouseButtons::Right) {
		leavePeekMode();
	}
}

System::Void frmMain::picBoard_MouseClick(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e) {
	// check the situation when new users don't click "Start" button before making the first move
	if (!analyzeMode && btnStart->Visible && e->Button == System::Windows::Forms::MouseButtons::Left) {
		prompt("请单击右侧面板上的\"" + btnStart->Text + "\"按钮。", iconInfo);
	}
	// In case people make an illegal move
	if (GUIPlay && e->Button == System::Windows::Forms::MouseButtons::Left
		&& !safe_cast<ChessPicBox^>(sender)->isAvailable()) {
		prompt("根据规则，请在\"*\"标示处落子。", iconInfo);
	}
	if (!GUIPlay || peekMode)
		return;
	if (e->Button == System::Windows::Forms::MouseButtons::Left) {
		ChessPicBox ^c = safe_cast<ChessPicBox^>(sender);
		GameContext ^gc = (gcBlack->getCurrentPlayer() == Chess::BLACK) ? gcBlack : gcWhite;
		for (int i = 0; i < gc->getAvailableCount(); i++)
			if (c->getXIndex() == gc->getAvailableMove(i).X && c->getYIndex() == gc->getAvailableMove(i).Y) {
				GUIResult = i;
				Cursor = Cursors::Default;
				setGUIPlay(false);
				return;
			}
	}
}

System::Void frmMain::frmMain_Load(System::Object ^sender, System::EventArgs ^e) {
	fairGame = false;
	startNewGame();
}

AbstractPlayer^ frmMain::createPlayer(PlayerType type, Chess colorReserved) {
	switch (type) {
	case PlayerType::GUI:
		return gcnew GUIPlayer(this, userInfo->UserName);
	case PlayerType::RANDOM:
		return gcnew RandomPlayer(userInfo->StepDelay);
	case PlayerType::BIG_EAT:
		return gcnew BigEater(userInfo->StepDelay);
	default:
		return gcnew Craft(this, Players::getAISearchOptions(type), 
			Players::getAIPlayerName(type), userInfo);
	}
	return nullptr;
}

void frmMain::createGame(GameOption option) {
	setBlackType(option.black);
	setWhiteType(option.white);
	blackPlayer = createPlayer(blackType, Chess::BLACK);
	whitePlayer = createPlayer(whiteType, Chess::WHITE);
	if (option.board == nullptr)
		game = gcnew Game(blackPlayer, whitePlayer, option.totalTimeLimit);
	else
		game = gcnew Game(blackPlayer, whitePlayer, option.board, option.firstPlayer, option.totalTimeLimit);
}

void frmMain::setGUIPlay(bool state) {
	if (state) {
		GUIResult = -1;
		peekMode = false;
	} else {
		if (peekMode) leavePeekMode();
	}
	GUIPlay = state;
	tsbtnTip->Enabled = state && !endGameMode;
	mnuTip->Enabled = tsbtnTip->Enabled;
	mnuForceEndSolve->Enabled = state & !endGameMode;
	tsbtnForceEndSolve->Enabled = mnuForceEndSolve->Enabled;
	setSelection(-1, -1);
}

int frmMain::getGUIPlay() {
	return GUIResult;
}

void frmMain::loadDefaultTheme() {
	backgroundName = "Background";
	bChessOrigin = safe_cast<Image^>(resources->GetObject("BlackChess"));
	wChessOrigin = safe_cast<Image^>(resources->GetObject("WhiteChess"));
	abChessOrigin = safe_cast<Image^>(resources->GetObject("AvBlack"));
	awChessOrigin = safe_cast<Image^>(resources->GetObject("AvWhite"));
	bBChessOrigin = safe_cast<Image^>(resources->GetObject("BBlackChess"));
	bWChessOrigin = safe_cast<Image^>(resources->GetObject("BWhiteChess"));
	fBChessOrigin = safe_cast<Image^>(resources->GetObject("FocusedAvBlack"));
	fWChessOrigin = safe_cast<Image^>(resources->GetObject("FocusedAvWhite"));
	nBChessOrigin = safe_cast<Image^>(resources->GetObject("NextAvBlack"));
	nWChessOrigin = safe_cast<Image^>(resources->GetObject("NextAvWhite"));
	sabChessOrigin = safe_cast<Image^>(resources->GetObject("SelectedAvBlack"));
	sawChessOrigin = safe_cast<Image^>(resources->GetObject("SelectedAvWhite"));
	fsbChessOrigin = safe_cast<Image^>(resources->GetObject("FocusedSelectedAvBlack"));
	fswChessOrigin = safe_cast<Image^>(resources->GetObject("FocusedSelectedAvWhite"));
	nsbChessOrigin = safe_cast<Image^>(resources->GetObject("NextSelectedAvBlack"));
	nswChessOrigin = safe_cast<Image^>(resources->GetObject("NextSelectedAvWhite"));
	ChessPicBox::setBlackChess(bChessOrigin);
	ChessPicBox::setWhiteChess(wChessOrigin);
	ChessPicBox::setAvBlackChess(abChessOrigin);
	ChessPicBox::setAvWhiteChess(awChessOrigin);
	ChessPicBox::setBdBlackChess(bBChessOrigin);
	ChessPicBox::setBdWhiteChess(bWChessOrigin);
	ChessPicBox::setFocusBlackChess(fBChessOrigin);
	ChessPicBox::setFocusWhiteChess(fWChessOrigin);
	ChessPicBox::setNextBlackChess(nBChessOrigin);
	ChessPicBox::setNextWhiteChess(nWChessOrigin);
	ChessPicBox::setSelAvBlackChess(sabChessOrigin);
	ChessPicBox::setSelAvWhiteChess(sawChessOrigin);
	ChessPicBox::setFocusSelBlackChess(fsbChessOrigin);
	ChessPicBox::setFocusSelWhiteChess(fswChessOrigin);
	ChessPicBox::setNextSelBlackChess(nsbChessOrigin);
	ChessPicBox::setNextSelWhiteChess(nswChessOrigin);
	bgImage = safe_cast<Image^>(resources->GetObject(backgroundName));
	cbImage = safe_cast<Image^>(resources->GetObject("ChessBoard"));
	iconBusy = safe_cast<Image^>(resources->GetObject("iconBusy"));
	iconRest = safe_cast<Image^>(resources->GetObject("iconRest"));
	iconInfo = safe_cast<Image^>(resources->GetObject("IconInfo"));
	iconWarning = safe_cast<Image^>(resources->GetObject("IconWarning"));
	iconError = safe_cast<Image^>(resources->GetObject("IconError"));
	bdSetupBlack = safe_cast<Image^>(resources->GetObject("BoardSetupBlack"));
	bdSetupWhite = safe_cast<Image^>(resources->GetObject("BoardSetupWhite"));
	bdSetupAv = safe_cast<Image^>(resources->GetObject("BoardSetupAv"));
	ulCorner = DEFAULT_ULCORNER;
	lrCorner = DEFAULT_LRCORNER;
	try {
		startSound = gcnew SoundPlayer(startSoundLocation);
		goSound = gcnew SoundPlayer(goSoundLocation);
		winSound = gcnew SoundPlayer(winSoundLocation);
		drawSound = gcnew SoundPlayer(drawSoundLocation);
		loseSound = gcnew SoundPlayer(loseSoundLocation);
		startSound->Load();
		goSound->Load();
		winSound->Load();
		drawSound->Load();
		loseSound->Load();
		canPlaySound = true;
	} catch (Exception^) {
		canPlaySound = false;
	}
	userInfo->Theme = DEFAULT_THEME_NAME;
}

void frmMain::playSound(SoundPlayer^ player) {
	try {
		player->Play();
	} catch (Exception^) {
		System::Diagnostics::Debug::WriteLine("Cannot play the sound " + player->SoundLocation);
	}
}

void frmMain::initEndGameMode() {
	endGameInfoShowing = false;
	currentEndGameDifficulty = EndGameDifficulty::CUSTOM;
	currentEndGame = nullptr;
	currentEndGameEmpties = 0;
	currentEndGamePlayer = Chess::AVAILABLE;
	tsmnuShowEndGameInfo->Enabled = false;
	mnuShowEndGameInfo->Enabled = false;
	wthorReader = gcnew WThorReader();
	wthorReader->loadPlayers(jouFileLocation);
	wthorReader->loadTours(trnFileLocation);
	wthorFileList = gcnew Generic::List<WThorFileInfo>();
	totalWThorGames = 0;
	array<String^>^ wtbFiles;
	try {
		wtbFiles = IO::Directory::GetFiles(wtbFilePath, "*.wtb", 
			IO::SearchOption::TopDirectoryOnly);
	} catch (Exception^) {
		tsmnuNewEndGame->Enabled = false;
		mnuNewEndGame->Enabled = false;
		return;
	}
	for (int i = 0; i < wtbFiles->Length; i++) {
		if (wthorReader->loadGameFile(wtbFiles[i])) {
			WThorFileInfo info;
			info.fileLocation = wtbFiles[i];
			info.numberOfGames = wthorReader->getGameCount();
			totalWThorGames += info.numberOfGames;
			info.yearOfGames = wthorReader->getTourYear();
			wthorFileList->Add(info);
			wthorReader->unloadGameFile();
		}
	}
	tsmnuNewEndGame->Enabled = (wthorFileList->Count > 0);
	mnuNewEndGame->Enabled = tsmnuNewEndGame->Enabled;
}

void frmMain::initThemeList() {
#ifdef CHRISTMAS
	bool afterChristmas = (DateTime::Today >= XmasEve);
#endif
	themeList = gcnew Generic::SortedList<String^, String^>();
	themeMenuList = gcnew Generic::List<ThemeMenuItem^>();
	ConfigReader^ reader = gcnew ConfigReader();
	if (!reader->readConfigFromFile(themeConfigFile)) return;
	Catagory^ catagory = reader->getCatagory("Theme");
	if (!catagory) return;
	Generic::SortedDictionary<String^, String^>::Enumerator enumerator
		= catagory->getEnumerator();
	while (enumerator.MoveNext()) {
#ifdef CHRISTMAS
		if (enumerator.Current.Key->Equals(CHRISTMAS_THEME) && !afterChristmas) continue;
#endif
		themeList[enumerator.Current.Key] = enumerator.Current.Value;
		ThemeMenuItem^ themeMnu = gcnew ThemeMenuItem(enumerator.Current.Key);
		themeMnu->Name = L"mnu" + enumerator.Current.Key + L"Theme";
		themeMnu->Text = enumerator.Current.Value;
		themeMnu->Checked = false;
		themeMnu->Click += gcnew EventHandler(this, &frmMain::mnuSomeTheme_Click);
		//Size = System::Drawing::Size(152, 22);
		themeMenuList->Add(themeMnu);
		mnuTheme->DropDownItems->Add(themeMnu);
	}
	reader->clearConfig();
}

Image^ frmMain::loadImage(String^ path, String^ defaultResourceName) {
	if (!path) return safe_cast<Image^>(resources->GetObject(defaultResourceName));
	try {
		Image^ image = Image::FromFile(path);
		return image;
	} catch (...) {
		return safe_cast<Image^>(resources->GetObject(defaultResourceName));
	}
}

void frmMain::setMnuTheme() {
	if (userInfo->Theme->Equals(DEFAULT_THEME_NAME)) {
		mnuDefaultTheme->Checked = true;
		for each (ThemeMenuItem^ themeMnu in themeMenuList) {
			themeMnu->Checked = false;
		}
	} else {
		mnuDefaultTheme->Checked = false;
		for each (ThemeMenuItem^ themeMnu in themeMenuList) {
			themeMnu->Checked = userInfo->Theme->Equals(themeMnu->getThemeName());
		}
	}
}

void frmMain::applyTheme() {
	this->picBoard->BackgroundImage = bgImage;
	this->picBoard->Image = cbImage;
	if (!canPlaySound)
		userInfo->PlaySound = false;
	mnuPlaySound->Enabled = canPlaySound;
	mnuPlaySound->Checked = userInfo->PlaySound;
	setLayout();
	setMnuTheme();
}

void frmMain::loadTheme(System::String^ theme) {
	if (theme->Equals(DEFAULT_THEME_NAME) || !themeList->ContainsKey(theme))
		loadDefaultTheme();
	else {
		String^ themeDir = themePath + theme + "\\";
		ConfigReader^ reader = gcnew ConfigReader();
		if (!reader->readConfigFromFile(themeDir + theme + CONFIG_FILE_EXTENSION)) {
			loadDefaultTheme();
			setPaused(true);
			MessageBox::Show(this, "无法正常载入主题：" + themeList[theme] + "\n建议重新安装此主题。",
				"载入主题失败", MessageBoxButtons::OK, MessageBoxIcon::Error);
			setPaused(false);
			return;
		}
		// pictures
		Catagory^ catagory = reader->getCatagory("Picture");
		if (!catagory)
			catagory = gcnew Catagory("");
		bChessOrigin = loadImage(themeDir + catagory->getAttribute("BlackChess", nullptr), "BlackChess");
		wChessOrigin = loadImage(themeDir + catagory->getAttribute("WhiteChess", nullptr), "WhiteChess");
		abChessOrigin = loadImage(themeDir + catagory->getAttribute("AvBlack", nullptr), "AvBlack");
		awChessOrigin = loadImage(themeDir + catagory->getAttribute("AvWhite", nullptr), "AvWhite");
		bBChessOrigin = loadImage(themeDir + catagory->getAttribute("BBlackChess", nullptr), "BBlackChess");
		bWChessOrigin = loadImage(themeDir + catagory->getAttribute("BWhiteChess", nullptr), "BWhiteChess");
		fBChessOrigin = loadImage(themeDir + catagory->getAttribute("FocusedAvBlack", nullptr), "FocusedAvBlack");
		fWChessOrigin = loadImage(themeDir + catagory->getAttribute("FocusedAvWhite", nullptr), "FocusedAvWhite");
		nBChessOrigin = loadImage(themeDir + catagory->getAttribute("NextAvBlack", nullptr), "NextAvBlack");
		nWChessOrigin = loadImage(themeDir + catagory->getAttribute("NextAvWhite", nullptr), "NextAvWhite");
		sabChessOrigin = loadImage(themeDir + catagory->getAttribute("SelectedAvBlack", nullptr), "SelectedAvBlack");
		sawChessOrigin = loadImage(themeDir + catagory->getAttribute("SelectedAvWhite", nullptr), "SelectedAvWhite");
		fsbChessOrigin = loadImage(themeDir + catagory->getAttribute("FocusedSelectedAvBlack", nullptr), "FocusedSelectedAvBlack");
		fswChessOrigin = loadImage(themeDir + catagory->getAttribute("FocusedSelectedAvWhite", nullptr), "FocusedSelectedAvWhite");
		nsbChessOrigin = loadImage(themeDir + catagory->getAttribute("NextSelectedAvBlack", nullptr), "NextSelectedAvBlack");
		nswChessOrigin = loadImage(themeDir + catagory->getAttribute("NextSelectedAvWhite", nullptr), "NextSelectedAvWhite");
		bgImage = loadImage(themeDir + catagory->getAttribute("Background", nullptr), "Background");
		cbImage = loadImage(themeDir + catagory->getAttribute("ChessBoard", nullptr), "ChessBoard");
		iconBusy = loadImage(themeDir + catagory->getAttribute("IconBusy", nullptr), "iconBusy");
		iconRest = loadImage(themeDir + catagory->getAttribute("IconRest", nullptr), "iconRest");
		iconInfo = loadImage(themeDir + catagory->getAttribute("IconInfo", nullptr), "IconInfo");
		iconWarning = loadImage(themeDir + catagory->getAttribute("IconWarning", nullptr), "IconWarning");
		iconError = loadImage(themeDir + catagory->getAttribute("IconError", nullptr), "IconError");
		ChessPicBox::setBlackChess(bChessOrigin);
		ChessPicBox::setWhiteChess(wChessOrigin);
		ChessPicBox::setAvBlackChess(abChessOrigin);
		ChessPicBox::setAvWhiteChess(awChessOrigin);
		ChessPicBox::setBdBlackChess(bBChessOrigin);
		ChessPicBox::setBdWhiteChess(bWChessOrigin);
		ChessPicBox::setFocusBlackChess(fBChessOrigin);
		ChessPicBox::setFocusWhiteChess(fWChessOrigin);
		ChessPicBox::setNextBlackChess(nBChessOrigin);
		ChessPicBox::setNextWhiteChess(nWChessOrigin);
		ChessPicBox::setSelAvBlackChess(sabChessOrigin);
		ChessPicBox::setSelAvWhiteChess(sawChessOrigin);
		ChessPicBox::setFocusSelBlackChess(fsbChessOrigin);
		ChessPicBox::setFocusSelWhiteChess(fswChessOrigin);
		ChessPicBox::setNextSelBlackChess(nsbChessOrigin);
		ChessPicBox::setNextSelWhiteChess(nswChessOrigin);
		bdSetupBlack = loadImage(themeDir + catagory->getAttribute("BoardSetupBlack", nullptr), "BoardSetupBlack");
		bdSetupWhite = loadImage(themeDir + catagory->getAttribute("BoardSetupWhite", nullptr), "BoardSetupWhite");
		bdSetupAv = loadImage(themeDir + catagory->getAttribute("BoardSetupAv", nullptr), "BoardSetupAv");
		// coodinates
		if (catagory->getAttribute("ChessBoard", nullptr)) {
			catagory = reader->getCatagory("Coordinates");
			try {
				ulCorner = Point(catagory->getIntAttribute("BoardULX", 0), 
					catagory->getIntAttribute("BoardULY", 0));
				lrCorner = Point(catagory->getIntAttribute("BoardLRX", cbImage->Width), 
					catagory->getIntAttribute("BoardLRY", cbImage->Height));
			} catch (...) {
				ulCorner = Point(0, 0);
				lrCorner = Point(cbImage->Width, cbImage->Height);
			}
		} else {
			ulCorner = DEFAULT_ULCORNER;
			lrCorner = DEFAULT_LRCORNER;
		}
		// sounds
		catagory = reader->getCatagory("Sound");
		if (!catagory)
			catagory = gcnew Catagory("");
		String^ start = catagory->getAttribute("Start", nullptr);
		String^ go = catagory->getAttribute("Go", nullptr);
		String^ win = catagory->getAttribute("Win", nullptr);
		String^ draw = catagory->getAttribute("Draw", nullptr);
		String^ lose = catagory->getAttribute("Lose", nullptr);
		if (start)
			start = themeDir + start;
		else start = startSoundLocation;
		if (go)
			go = themeDir + go;
		else go = goSoundLocation;
		if (win)
			win = themeDir + win;
		else win = winSoundLocation;
		if (draw)
			draw = themeDir + draw;
		else draw = drawSoundLocation;
		if (lose)
			lose = themeDir + lose;
		else lose = loseSoundLocation;
		try {
			startSound = gcnew SoundPlayer(start);
			goSound = gcnew SoundPlayer(go);
			winSound = gcnew SoundPlayer(win);
			drawSound = gcnew SoundPlayer(draw);
			loseSound = gcnew SoundPlayer(lose);
			startSound->Load();
			goSound->Load();
			winSound->Load();
			drawSound->Load();
			loseSound->Load();
			canPlaySound = true;
		} catch (Exception^) {
			canPlaySound = false;
		}
		reader->clearConfig();
		userInfo->Theme = theme;
	}
}

void frmMain::changeTheme(String^ newTheme) {
	if (newTheme != userInfo->Theme) {
		loadTheme(newTheme);
		applyTheme();
		GC::Collect();
	}
}

frmMain::frmMain() {
	InitializeComponent();

#ifndef MACHINE_X64
	mnu2GB->Enabled = false;
	mnu2GB->Text += " (仅64位)";
	mnu2GB->ToolTipText = "仅在64位版本中可用";
#endif

	initThemeList();
	loadConfig();
	resources = (gcnew System::ComponentModel::ComponentResourceManager(frmMain::typeid));

#ifdef CHRISTMAS
	wishShowing = false;
	bool duringChristmas = (DateTime::Today >= XmasEve && DateTime::Today <= XmasEnd);
	loadTheme(duringChristmas ? CHRISTMAS_THEME : userInfo->Theme);
	if (duringChristmas) {
		showChristmasWish();
	}
#else
	loadTheme(userInfo->Theme);
#endif
	selection.X = -1;
	selection.Y = -1;
	tipPos.X = -1;
	tipPos.Y = -1;
	searching = false;
	imageBoard = gcnew array<ChessOption, 2>(WIDTH, HEIGHT);
	this->picBoard->BackgroundImage = bgImage;
	this->picBoard->Image = cbImage;
	this->picBoard->BackgroundImageLayout = ImageLayout::Tile;
	this->picBoard->SizeMode = PictureBoxSizeMode::Zoom;
	// Christmas special below:
	//ulCorner = Point(222, 144);
	//lrCorner = Point(1118, 1040);
	ssPlayersOffset = statusBar->Width - ssPlayers->Width;
	ssPVOffset = statusBar2->Width - ssPV->Width;
	board = gcnew array<ChessPicBox^, 2>(WIDTH, HEIGHT);
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			board[i, j] = gcnew ChessPicBox(i, j);
			picBoard->Controls->Add(board[i, j]);
			board[i, j]->Visible = false;
			//this->picBoard->Location = System::Drawing::Point(0, 49);
			board[i, j]->Name = L"picBoard(" + i + "," + j + ")";
			//this->picBoard->Size = System::Drawing::Size(400, 400);
			board[i, j]->BorderStyle = System::Windows::Forms::BorderStyle::None;
			//board[i, j]->BackColor = Color::Transparent;
			board[i, j]->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Normal;
			board[i, j]->TabStop = false;
			board[i, j]->MouseClick += gcnew MouseEventHandler(this, & frmMain::picBoard_MouseClick);
			board[i, j]->MouseMove += gcnew MouseEventHandler(this, & frmMain::picBoard_MouseMove);
			board[i, j]->MouseLeave += gcnew EventHandler(this, & frmMain::picBoard_MouseLeave);
			board[i, j]->MouseDown += gcnew MouseEventHandler(this, & frmMain::picBoard_MouseDown);
			board[i, j]->MouseUp += gcnew MouseEventHandler(this, & frmMain::picBoard_MouseUp);
			board[i, j]->MouseHover += gcnew EventHandler(this, & frmMain::picBoard_MouseHover);
		}
	imageBoard = gcnew array<ChessOption, 2>(WIDTH, HEIGHT);
	setMnuTheme();
	picBoard->Visible = true;
	if (!canPlaySound) {
		userInfo->PlaySound = false;
		mnuPlaySound->Enabled = false;
	}
	defaultOption.black = userInfo->BlackType;
	defaultOption.white = userInfo->WhiteType;
	defaultOption.firstPlayer = Chess::BLACK;
	defaultOption.board = nullptr;
	defaultOption.totalTimeLimit = 0;
	setMnuDelay();
	setMnuTableSize();
	mnuLearnImmediately->Checked = userInfo->AutoLearn;
	mnuPlaySound->Checked = userInfo->PlaySound;
	mnuShowEvaluation->Checked = userInfo->ShowEvaluation;
	mnuShowProgress->Checked = userInfo->ShowProgress;
	mnuShowSpeed->Checked = userInfo->ShowSpeed;
	mnuShowPV->Checked = userInfo->ShowPrincipleVariation;
	statusBar2->Visible = userInfo->ShowPrincipleVariation;
	setLayout();
	mnuUseBook->Checked = userInfo->UseBook;
	if (!userInfo->UseBook) userInfo->FreeMode = true;
	mnuFreeMode->Checked = userInfo->FreeMode;
	mnuAutoClean->Checked = userInfo->AutoCleanTable;
	tspb1->Visible = userInfo->ShowProgress;
	tsbtnLearn->Enabled = false;
	mnuLearn->Enabled = false;
	this->Text = applicationName + " " + applicationVersion;
	memorySize = getMemorySize();
	guiTipper = createPlayer(PlayerType::STAR, Chess::AVAILABLE);
	searchState = false;
	focusedMove.X = -1; focusedMove.Y = -1;
	selectedMove.X = -1; selectedMove.Y = -1;
	resetComponents();
	analyzeMode = false;
	analyzing = false;
	rnd = gcnew System::Random();
	tsbtnStopSearch->Enabled = false;
	mnuStopSearch->Enabled = false;
	tipping = false;
	continueGame = false;
	initEndGameMode();
}

frmMain::~frmMain() {
	if (components)
		delete components;
}

void frmMain::setStepDelay(int delay) {
	userInfo->StepDelay = delay;
	blackPlayer->setStepDelay(delay);
	whitePlayer->setStepDelay(delay);
}

void frmMain::setMnuDelay() {
	switch (userInfo->StepDelay) {
	case 0:
		mnuDelayClose->Checked = true;
		mnuDelay05s->Checked = false;
		mnuDelay1s->Checked = false;
		mnuDelay2s->Checked = false;
		break;
	case 500:
		mnuDelayClose->Checked = false;
		mnuDelay05s->Checked = true;
		mnuDelay1s->Checked = false;
		mnuDelay2s->Checked = false;
		break;
	case 2000:
		mnuDelayClose->Checked = false;
		mnuDelay05s->Checked = false;
		mnuDelay1s->Checked = false;
		mnuDelay2s->Checked = true;
		break;
	default:
		mnuDelayClose->Checked = false;
		mnuDelay05s->Checked = false;
		mnuDelay1s->Checked = true;
		mnuDelay2s->Checked = false;
		break;
	}
}

System::Void frmMain::mnuDelayClose_Click(System::Object ^sender, System::EventArgs ^e) {
	this->setStepDelay(0);
	mnuDelayClose->Checked = true;
	mnuDelay05s->Checked = false;
	mnuDelay1s->Checked = false;
	mnuDelay2s->Checked = false;
	if (userInfo->PlaySound) {
		userInfo->PlaySound = false;
		mnuPlaySound->Checked = false;
	}
}

System::Void frmMain::mnuDelay1s_Click(System::Object ^sender, System::EventArgs ^e) {
	this->setStepDelay(1000);
	mnuDelayClose->Checked = false;
	mnuDelay05s->Checked = false;
	mnuDelay1s->Checked = true;
	mnuDelay2s->Checked = false;
}

System::Void frmMain::mnuDelay05s_Click(System::Object ^sender, System::EventArgs ^e) {
	this->setStepDelay(500);
	mnuDelayClose->Checked = false;
	mnuDelay05s->Checked = true;
	mnuDelay1s->Checked = false;
	mnuDelay2s->Checked = false;
}

System::Void frmMain::mnuDelay2s_Click(System::Object ^sender, System::EventArgs ^e) {
	this->setStepDelay(2000);
	mnuDelayClose->Checked = false;
	mnuDelay05s->Checked = false;
	mnuDelay1s->Checked = false;
	mnuDelay2s->Checked = true;
}


System::Void frmMain::mnuPlaySound_Click(System::Object ^sender, System::EventArgs ^e) {
	userInfo->PlaySound = !userInfo->PlaySound;
	mnuPlaySound->Checked = userInfo->PlaySound;
	if (userInfo->PlaySound && userInfo->StepDelay == 0) {
		this->setStepDelay(500);
		mnuDelay05s->Checked = true;
		mnuDelayClose->Checked = false;
	}
}

void frmMain::picBoard_MouseMoved(System::Object ^sender) {
	if (!GUIPlay || peekMode) return;
	ChessPicBox^ picBox = safe_cast<ChessPicBox^>(sender);
	int x = picBox->getXIndex();
	int y = picBox->getYIndex();
	if (imageBoard[x, y].available) {
		setSelection(x, y);
		Cursor = Cursors::Hand;
	} else {
		setSelection(-1, -1);
		Cursor = Cursors::Default;
	}
}

System::Void frmMain::picBoard_MouseHover(System::Object ^sender, System::EventArgs ^e) {
	picBoard_MouseMoved(sender);
}

System::Void frmMain::picBoard_MouseMove(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e) {
	picBoard_MouseMoved(sender);
}

void frmMain::picBoard_MouseLeft() {
	if (!GUIPlay) return;
	if (peekMode) leavePeekMode();
	setSelection(-1, -1);
	Cursor = Cursors::Default;
}

System::Void frmMain::picBoard_MouseLeave(System::Object ^sender, System::EventArgs ^e) {
	picBoard_MouseLeft();
}

void frmMain::openHelp() {
	//execute("hh.exe \"" + helpFile + "\"");
	try {
		System::Diagnostics::Process::Start(helpFile);
	} catch (Exception^) {
	}
}

System::Void frmMain::frmMain_KeyDown(System::Object ^sender, System::Windows::Forms::KeyEventArgs ^e) {
	e->Handled = true;
	switch (e->KeyCode) {
	case Keys::E:
		if (!GUIPlay || peekMode) return;
		enterPeekMode();
		return;
	}
	if (e->Modifiers == Keys::None) {
		int x, y;
		switch (e->KeyCode) {
		case Keys::Down:
			if (!GUIPlay || peekMode) return;
			x = selection.X;
			y = selection.Y;
			if (x == -1 && y == -1) {
				x = 0; y = 0;
			}
			y++; if (y == WIDTH) {
				y = 0; x++;
				if (x == HEIGHT) x = 0;
			}
			while (!board[x, y]->isAvailable()) {
				y++; if (y == WIDTH) {
					y = 0; x++;
					if (x == HEIGHT) x = 0;
				}
			}
			setSelection(x, y);
			::Cursor::Position = board[x, y]->PointToScreen(Point(board[x, y]->Width / 2, board[x, y]->Height / 2));
			break;
		case Keys::Up:
			if (!GUIPlay || peekMode) return;
			x = selection.X;
			y = selection.Y;
			if (x == -1 && y == -1) {
				x = HEIGHT - 1; y = WIDTH - 1;
			}
			y--;
			if (y == -1) {
				y = WIDTH - 1; x--;
				if (x == -1) x = HEIGHT - 1;
			}
			while (!board[x, y]->isAvailable()) {
				y--;
				if (y == -1) {
					y = WIDTH - 1; x--;
					if (x == -1) x = HEIGHT - 1;
				}
			}
			setSelection(x, y);
			::Cursor::Position = board[x, y]->PointToScreen(Point(board[x, y]->Width / 2, board[x, y]->Height / 2));
			break;
		case Keys::Left:
			if (!GUIPlay || peekMode) return;
			x = selection.X;
			y = selection.Y;
			if (x == -1 && y == -1) {
				x = HEIGHT - 1; y = WIDTH - 1;
			}
			x--; 
			if (x == -1) {
				x = HEIGHT - 1; y--;
				if (y == -1) y = WIDTH - 1;
			}
			while (!board[x, y]->isAvailable()) {
				x--; 
				if (x == -1) {
					x = HEIGHT - 1; y--;
					if (y == -1) y = WIDTH - 1;
				}
			}
			setSelection(x, y);
			::Cursor::Position = board[x, y]->PointToScreen(Point(board[x, y]->Width / 2, board[x, y]->Height / 2));
			break;
		case Keys::Right:
			if (!GUIPlay || peekMode) return;
			x = selection.X;
			y = selection.Y;
			if (x == -1 && y == -1) {
				x = 0; y = 0;
			}
			x++;
			if (x == HEIGHT) {
				x = 0; y++;
				if (y == WIDTH) y = 0;
			}
			while (!board[x, y]->isAvailable()) {
				x++;
				if (x == HEIGHT) {
					x = 0; y++;
					if (y == WIDTH) y = 0;
				}
			}
			setSelection(x, y);
			::Cursor::Position = board[x, y]->PointToScreen(Point(board[x, y]->Width / 2, board[x, y]->Height / 2));
			break;
		case Keys::Enter:
		case Keys::Space:
			if (btnStart->Visible) {
				startGame();
			} else {
				if (!GUIPlay || peekMode) return;
				int x = selection.X, y = selection.Y;
				if (x != -1 || y != -1) {
					GameContext ^gc = (gcBlack->getCurrentPlayer() == Chess::BLACK) ? gcBlack : gcWhite;
					selection.X = -1; selection.Y = -1;
					for (int i = 0; i < gc->getAvailableCount(); i++)
						if (x == gc->getAvailableMove(i).X && y == gc->getAvailableMove(i).Y) {
							GUIResult = i;
							setGUIPlay(false);
							Cursor = Cursors::Default;
							return;
						}
				}
			}
			break;
		case Keys::Back:
			goBack();
			break;
		case Keys::F1:
			openHelp();
			break;
		case Keys::T:
			if (mnuTip->Enabled)
				getGUITip();
			break;
		case Keys::F:
			if (mnuForceEndSolve->Enabled)
				forceEndSolve();
			break;
		case Keys::P:
			if (mnuStopSearch->Enabled)
				stopSearch();
			break;
		default:
			e->Handled = false;
		}
	} else if (e->Modifiers == Keys::Control) {
		switch (e->KeyCode) {
		case Keys::N:
			if (analyzeMode) return;
			if (tsbtnNew->Enabled)
				tsbtnNew->ShowDropDown();
			break;
		case Keys::R:
			restartGame();
			break;
		case Keys::B:
			if (analyzeMode) return;
			if (tsbtnBlack->Enabled)
				tsbtnBlack->ShowDropDown();
			break;
		case Keys::W:
			if (analyzeMode) return;
			if (tsbtnWhite->Enabled)
				tsbtnWhite->ShowDropDown();
			break;
		case Keys::L:
			if (mnuLearn->Enabled)
				learnGame();
			break;
		case Keys::Left:
		case Keys::Z:
			goBack();
			break;
		case Keys::Right:
		case Keys::Y:
			goForward();
			break;
		case Keys::X:
			exitProgram();
			break;
		case Keys::P:
			setupBoard();
			break;
		case Keys::O:
			openGame();
			break;
		case Keys::S:
			saveGame();
			break;
		case Keys::I:
			showStatistics();
			break;
		case Keys::A:
			showAnalyzer();
			break;
		case Keys::M:
			if (analyzeMode) return;
			if (tsmnuNewEndGame->Enabled)
				tsmnuNewEndGame->ShowDropDown();
			break;
		case Keys::K:
			reshowEndGameInfo();
			break;
		case Keys::C:
			copyBoard();
			break;
		case Keys::V:
			pasteBoard();
			break;
		//case Keys::T:
		//	changeTheme("xmas");
		//	break;
		//case Keys::Q:
		//	changeTheme("default");
		//	break;
		//case Keys::Enter: {
		//	Solver* solver = Solver::newInstance();
		//	GameContext^ gc = gcBlack;
		//	int board[Solver::MAXSTEP];
		//	for (int i = 0; i < Solver::MAXSTEP; i++)
		//		switch (gc->get(i / HEIGHT, i % HEIGHT)) {
		//		case Chess::BLACK :
		//			board[i] = Solver::BLACK;
		//			break;
		//		case Chess::WHITE :
		//			board[i] = Solver::WHITE;
		//			break;
		//		case Chess::AVAILABLE :
		//			board[i] = Solver::AV;
		//	}
		//	solver->setBoard(board);
		//	int lower, upper;
		//	solver->getStabilityBound(lower, upper);
		//	delete solver;
		//	MessageBox::Show("Lower: " + lower + ", Upper: " + upper);
		//	break;
		//				  }
		default:
			e->Handled = false;
		}
	} else {
		e->Handled = false;
	}
}

System::Void frmMain::frmMain_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^  e) {
	e->Handled = true;
	switch (e->KeyCode) {
	case Keys::E:
		if (!peekMode) return;
		leavePeekMode();
		return;
	}
	e->Handled = false;
}

void frmMain::learnGame() {
	if (analyzeMode) return;
	if (game->isLearnable() && gcBlack->isGameEnded()) {
		tsbtnLearn->Enabled = false;
		mnuLearn->Enabled = false;
		BookLearning^ bookLearningForm = gcnew BookLearning(gcBlack);
		try {
			bookLearningForm->ShowDialog(this);
		} finally {
			delete bookLearningForm;
		}
		saveBook();
	}
}

void frmMain::loadConfig() {
	userInfo = gcnew UserInfo();
	System::IO::FileStream^ fs;
	try {
		fs = 
			gcnew System::IO::FileStream(configPath, System::IO::FileMode::Open, System::IO::FileAccess::Read);
	} catch (System::Exception^) {
		return;
	}

	System::IO::BinaryReader^ br = gcnew System::IO::BinaryReader(fs);
	//int hasValue = 0;
	//int noValue = 0;
	try {
		int tTableSize;
		int tStepDelay;
		bool tPlaySound;
		userInfo->UserName = br->ReadString();
		tTableSize = br->ReadInt32();
		if (
#ifdef MACHINE_X64
			tTableSize == 0x2000000 ||
#endif
			tTableSize == 0x1000000 || tTableSize == 0x800000 || tTableSize == 0x400000
			|| tTableSize == 0x200000 || tTableSize == 0x100000 || tTableSize == 0x80000
			|| tTableSize == 0x40000 || tTableSize == 0x20000 || tTableSize == 0x10000) {
				userInfo->TableSize = tTableSize;
				Solver::setCacheSize(tTableSize);
		}
		tStepDelay = br->ReadInt32();
		if (tStepDelay == 0 || tStepDelay == 500 || tStepDelay == 1000 || tStepDelay == 2000)
			userInfo->StepDelay = tStepDelay;
		tPlaySound = br->ReadBoolean();
		userInfo->PlaySound = tPlaySound;
		userInfo->AutoLearn = br->ReadBoolean();
		userInfo->ShowEvaluation = br->ReadBoolean();
		userInfo->ShowProgress = br->ReadBoolean();
		userInfo->ShowSpeed = br->ReadBoolean();
		userInfo->BlackType = readPlayerType(br);
		userInfo->WhiteType = readPlayerType(br);
		userInfo->addStatistics(PlayerType::RANDOM, readStatistics(br));
		userInfo->addStatistics(PlayerType::BIG_EAT, readStatistics(br));
		userInfo->addStatistics(PlayerType::EARTH, readStatistics(br));
		userInfo->addStatistics(PlayerType::CLOUDS, readStatistics(br));
		userInfo->addStatistics(PlayerType::ORBIT, readStatistics(br));
		userInfo->addStatistics(PlayerType::MOON, readStatistics(br));
		userInfo->addStatistics(PlayerType::STAR, readStatistics(br));
		userInfo->addStatistics(PlayerType::GALAXY, readStatistics(br));
		userInfo->addStatistics(PlayerType::UNIVERSE, readStatistics(br));
		userInfo->FreeMode = br->ReadBoolean();
		int tBookTolerance = br->ReadInt32();
		if (tBookTolerance == 3 * RULER)
			userInfo->BookTolerance = tBookTolerance;
		userInfo->Analyzer = readPlayerType(br);
		userInfo->CustomEmpties = br->ReadInt32();
		userInfo->Theme = br->ReadString();
		userInfo->UseBook = br->ReadBoolean();
		userInfo->AutoCleanTable = br->ReadBoolean();
		userInfo->ShowPrincipleVariation = br->ReadBoolean();
	} catch (System::Exception^) {
		br->Close();
		return;
	}
	br->Close();
}

PlayerType frmMain::readPlayerType(IO::BinaryReader^ br) {
	switch (br->ReadInt32()) {
	case 1:
		return PlayerType::RANDOM;
		break;
	case 2:
		return PlayerType::BIG_EAT;
		break;
	case 3:
		return PlayerType::EARTH;
		break;
	case 4:
		return PlayerType::CLOUDS;
		break;
	case 5:
		return PlayerType::ORBIT;
		break;
	case 6:
		return PlayerType::MOON;
		break;
	case 7:
		return PlayerType::STAR;
		break;
	case 8:
		return PlayerType::GALAXY;
		break;
	case 9:
		return PlayerType::UNIVERSE;
		break;
	default:
		return PlayerType::GUI;
	}
}

Statistics^ frmMain::readStatistics(IO::BinaryReader^ br) {
	int win = br->ReadInt32();
	int draw = br->ReadInt32();
	int loss = br->ReadInt32();
	int series = br->ReadInt32();
	GameResult seriesState;
	switch (br->ReadInt32()) {
	case 2:
		seriesState = GameResult::WIN;
		break;
	case 1:
		seriesState = GameResult::DRAW;
		break;
	case 0:
		seriesState = GameResult::LOSS;
		break;
	default:
		seriesState = GameResult::LOSS;
	}
	int longestWin = br->ReadInt32();
	int longestDraw = br->ReadInt32();
	int longestLoss = br->ReadInt32();
	int bestScore = br->ReadInt32();
	return gcnew Statistics(win, draw, loss, series, seriesState,
		longestWin, longestDraw, longestLoss, bestScore);
}

void frmMain::saveConfig() {
	System::IO::FileStream^ fs;
	try {
		fs = 
			gcnew System::IO::FileStream(configPath, System::IO::FileMode::Create, System::IO::FileAccess::Write);
	} catch (System::Exception^) {
		return;
	}
	System::IO::BinaryWriter^ bw = gcnew System::IO::BinaryWriter(fs);
	try {
		bw->Write(userInfo->UserName);
		bw->Write(userInfo->TableSize);
		bw->Write(userInfo->StepDelay);
		bw->Write(userInfo->PlaySound);
		bw->Write(userInfo->AutoLearn);
		bw->Write(userInfo->ShowEvaluation);
		bw->Write(userInfo->ShowProgress);
		bw->Write(userInfo->ShowSpeed);
		writePlayerType(bw, userInfo->BlackType);
		writePlayerType(bw, userInfo->WhiteType);
		writeStatistics(bw, userInfo->getStatistics(PlayerType::RANDOM));
		writeStatistics(bw, userInfo->getStatistics(PlayerType::BIG_EAT));
		writeStatistics(bw, userInfo->getStatistics(PlayerType::EARTH));
		writeStatistics(bw, userInfo->getStatistics(PlayerType::CLOUDS));
		writeStatistics(bw, userInfo->getStatistics(PlayerType::ORBIT));
		writeStatistics(bw, userInfo->getStatistics(PlayerType::MOON));
		writeStatistics(bw, userInfo->getStatistics(PlayerType::STAR));
		writeStatistics(bw, userInfo->getStatistics(PlayerType::GALAXY));
		writeStatistics(bw, userInfo->getStatistics(PlayerType::UNIVERSE));
		bw->Write(userInfo->FreeMode);
		bw->Write(userInfo->BookTolerance);
		writePlayerType(bw, userInfo->Analyzer);
		bw->Write(userInfo->CustomEmpties);
		bw->Write(userInfo->Theme);
		bw->Write(userInfo->UseBook);
		bw->Write(userInfo->AutoCleanTable);
		bw->Write(userInfo->ShowPrincipleVariation);
	} catch (System::Exception^) {
		bw->Close();
		return;
	}
	bw->Close();
}

void frmMain::writePlayerType(IO::BinaryWriter^ bw, PlayerType type) {
	switch (type) {
	case PlayerType::RANDOM:
		bw->Write(1);
		break;
	case PlayerType::BIG_EAT:
		bw->Write(2);
		break;
	case PlayerType::EARTH:
		bw->Write(3);
		break;
	case PlayerType::CLOUDS:
		bw->Write(4);
		break;
	case PlayerType::ORBIT:
		bw->Write(5);
		break;
	case PlayerType::MOON:
		bw->Write(6);
		break;
	case PlayerType::STAR:
		bw->Write(7);
		break;
	case PlayerType::GALAXY:
		bw->Write(8);
		break;
	case PlayerType::UNIVERSE:
		bw->Write(9);
		break;
	default:
		bw->Write(0);
		break;
	}
}

void frmMain::writeStatistics(IO::BinaryWriter^ bw, Statistics^ stat) {
	bw->Write(stat->getWin());
	bw->Write(stat->getDraw());
	bw->Write(stat->getLoss());
	bw->Write(stat->getSeries());
	switch (stat->getSeriesState()) {
	case GameResult::WIN:
		bw->Write(2);
		break;
	case GameResult::DRAW:
		bw->Write(1);
		break;
	case GameResult::LOSS:
		bw->Write(0);
	}
	bw->Write(stat->getLongestWin());
	bw->Write(stat->getLongestDraw());
	bw->Write(stat->getLongestLoss());
	bw->Write(stat->getBestScore());
}

System::Void frmMain::tsbtnLearn_Click(System::Object^ sender, System::EventArgs^ e) {
	learnGame();
}

System::Void frmMain::mnuLearnImmediately_Click(System::Object^ sender, System::EventArgs^ e) {
	userInfo->AutoLearn = !userInfo->AutoLearn;
	mnuLearnImmediately->Checked = userInfo->AutoLearn;
}

Board^ frmMain::getCurrentBoard() {
	Board^ board = gcnew Board(WIDTH, HEIGHT);
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
			board[i, j] = gcBlack->get(i, j);
	return board;
}

void frmMain::setupBoard() {
	if (analyzeMode) return;
	setupBoard(getCurrentBoard(), gcBlack->getCurrentPlayer());
}

void frmMain::setupBoard(Board^ board, Chess firstPlayer) {
	setPaused(true);
	BoardSetup^ boardSetupForm =
		gcnew BoardSetup(board, firstPlayer, bdSetupBlack,
		bdSetupWhite, bdSetupAv);
	try {
		Windows::Forms::DialogResult res = boardSetupForm->ShowDialog(this);
		setPaused(false);
		if (res != Windows::Forms::DialogResult::OK) return;

		board = boardSetupForm->InitBoard;
		GameOption option;
		option.black = blackType;
		option.white = whiteType;
		option.board = board;
		option.firstPlayer = boardSetupForm->FirstPlayer;
		option.totalTimeLimit = 0;
		this->startNewGame(option);
	} finally {
		delete boardSetupForm;
	}
}
void frmMain::setSelection(int x, int y) {
	if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0) {
		//disable
		if (selection.X < WIDTH && selection.X >= 0 
			&& selection.Y < HEIGHT && selection.Y >= 0) {
				board[selection.X, selection.Y]->setSelected(false);
		}
	} else {
		//enable
		if (selection.X < WIDTH && selection.X >= 0 
			&& selection.Y < HEIGHT && selection.Y >= 0
			&& (selection.X != x || selection.Y != y)) {
				board[selection.X, selection.Y]->setSelected(false);
		}
		if (selection.X != x || selection.Y != y)
			board[x, y]->setSelected(true);
	}
	selection.X = x;
	selection.Y = y;
}

void frmMain::showAbout() {
	//String^ message = applicationName + " " + applicationVersion + "\r\n"
	//	+ "程序设计: Patrick\r\n"
	//	+ "E-mail: patrick880905@sina.com\r\n"
	//	+ "QQ: 511754081\r\n"
	//	+ "如有什么意见或建议, 欢迎与我联系.";
	setPaused(true);
	//MessageBox::Show(this, message, "关于", MessageBoxButtons::OK, MessageBoxIcon::Information);
	frmAbout^ aboutDlg = gcnew frmAbout();
	aboutDlg->ShowDialog(this);
	delete aboutDlg;
	setPaused(false);
}

System::Void frmMain::mnuAbout_Click(System::Object^ sender, System::EventArgs^ e) {
	showAbout();
}

System::Void frmMain::mnuHelpContents_Click(System::Object^ sender, System::EventArgs^ e) {
	openHelp();
}

System::Void frmMain::mnuShowEvaluation_Click(System::Object^ sender, System::EventArgs^ e) {
	userInfo->ShowEvaluation = !userInfo->ShowEvaluation;
	mnuShowEvaluation->Checked = userInfo->ShowEvaluation;
	if (searchState && (!userInfo->ShowEvaluation || endGameMode)) {
		ssResult->Text = "思考中";
		setSelectedMove(-1, -1);
		setFocusedMove(-1, -1);
		setTipPos(-1, -1);
	}
}

System::Void frmMain::mnuShowSpeed_Click(System::Object^ sender, System::EventArgs^ e) {
	userInfo->ShowSpeed = !userInfo->ShowSpeed;
	mnuShowSpeed->Checked = userInfo->ShowSpeed;
	if (searchState && !userInfo->ShowSpeed) {
		ssSpeed->Text = "";
		ssNodes->Text = "";
	}
}

System::Void frmMain::mnuShowProgress_Click(System::Object^ sender, System::EventArgs^ e) {
	userInfo->ShowProgress = !userInfo->ShowProgress;
	mnuShowProgress->Checked = userInfo->ShowProgress;
	if (!analyzeMode) tspb1->Visible = userInfo->ShowProgress;
}

System::Void frmMain::mnuLearn_Click(System::Object^ sender, System::EventArgs^ e) {
	learnGame();
}

System::Void frmMain::mnuSetupBoard_Click(System::Object^ sender, System::EventArgs^ e) {
	setupBoard();
}

System::Void frmMain::tsbtnSetupBoard_Click(System::Object^ sender, System::EventArgs^ e) {
	setupBoard();
}

bool frmMain::checkMem(unsigned long long tableSize) {
	const unsigned long long fixedMem = 64 * 1048576;
#ifdef MACHINE_X64
	const unsigned long long ARCH_MAX = 1ull << 63; // 8EB for x64
#else
	const unsigned long long ARCH_MAX = 1ull << 31; // 2GB for x86
#endif
	if ((tableSize + fixedMem) > ARCH_MAX) {
		setPaused(true);
		int tbSize = (int)((double)tableSize / 1048576 + 0.5);
		MessageBox::Show(this, tbSize + " MB 的置换表超过了体系结构容许的大小。",
			"错误", MessageBoxButtons::OK, MessageBoxIcon::Error);
		setPaused(false);
		return false;
	}
	if ((tableSize + fixedMem) >= memorySize / 2) {
		setPaused(true);
		int phyMem = (int)((double)memorySize / 1048576 + 0.5);
		int tbSize = (int)((double)tableSize / 1048576 + 0.5);
		Windows::Forms::DialogResult res = MessageBox::Show(this, "您的物理内存为 " + phyMem + " MB，"
			+ "使用 " + tbSize + " MB 的置换表可能会导致过多的页面交换，影响系统运行速度。\n确实要继续吗？",
			"警告", MessageBoxButtons::YesNo, MessageBoxIcon::Exclamation, MessageBoxDefaultButton::Button2);
		setPaused(false);
		if (res == Windows::Forms::DialogResult::Yes) return true;
		else return false;
	}
	return true;
}

System::Void frmMain::mnuBack_Click(System::Object^  sender, System::EventArgs^  e) {
	goBack();
}
System::Void frmMain::mnuForward_Click(System::Object^  sender, System::EventArgs^  e) {
	goForward();
}

System::Void frmMain::mnuOpenGame_Click(System::Object^  sender, System::EventArgs^  e) {
	openGame();
}
System::Void frmMain::mnuSaveGame_Click(System::Object^  sender, System::EventArgs^  e) {
	saveGame();
}
System::Void frmMain::tsbtnOpenGame_Click(System::Object^  sender, System::EventArgs^  e) {
	openGame();
}
System::Void frmMain::tsbtnSaveGame_Click(System::Object^  sender, System::EventArgs^  e) {
	saveGame();
}

void frmMain::openGameFile(String^ fileName) {
	if (endGameMode) leaveEndGameMode();
	if (!breakFairness("继续打开游戏存档将记您本次对局为负。\n确实要打开游戏存档吗？")) {
		return;
	}
	IO::FileStream^ fs;
	try {
		fs = gcnew IO::FileStream(fileName, IO::FileMode::Open, IO::FileAccess::Read);
	} catch (Exception^) {
		System::Windows::Forms::MessageBox::Show(this, "无法打开 " + fileName + " 文件。",
			"无法打开游戏存档", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Stop);
		return;
	}
	IO::StreamReader^ reader = gcnew IO::StreamReader(fs, System::Text::Encoding::UTF8, true);
	Board^ board;
	Chess first;
	array<Othello::Move>^ moveSequence;
	int moveCount;
	try {
		String^ versionInfo = reader->ReadLine(); // reserved for further detection
		board = gcnew Board(WIDTH, HEIGHT);
		for (int i = 0; i < HEIGHT; i++) {
			String^ current = reader->ReadLine();
			for (int j = 0; j < WIDTH; j++) {
				switch (current[j]) {
				case 'X':
					board[j, i] = Chess::BLACK;
					break;
				case 'O':
					board[j, i] = Chess::WHITE;
					break;
				default:
					board[j, i] = Chess::AVAILABLE;
				}
			}
		}
		String^ fp = reader->ReadLine();
		first = fp->ToUpper()->Equals("WHITE") ? Chess::WHITE
			: (fp->ToUpper()->Equals("BLACK") ? Chess::BLACK : Chess::AVAILABLE);
		String^ ms = reader->ReadLine();
		moveSequence = gcnew array<Othello::Move>(WIDTH * HEIGHT);
		moveCount = 0;
		if (ms != nullptr) {
			for (int i = 0; i < ms->Length; i += 2) {
				Chess color;
				int xIndex = String("ABCDEFGH").IndexOf(ms[i]);
				if (xIndex < 0) {
					xIndex = String("abcdefgh").IndexOf(ms[i]);
					if (xIndex < 0) color = Chess::AVAILABLE;
					else color = Chess::WHITE;
				} else color = Chess::BLACK;
				int yIndex = String("12345678").IndexOf(ms[i + 1]);
				if (yIndex < 0) color = Chess::AVAILABLE;
				moveSequence[moveCount++] = Othello::Move(xIndex, yIndex, color);
			}
		}
	} catch (Exception^) {
		System::Windows::Forms::MessageBox::Show(this, "读取 " + fileName + " 文件出错。\n文件格式不正确。",
			"打开游戏存档出错", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Stop);
		reader->Close();
		return;
	}
	reader->Close();
	bool pGameJustStarted = gameJustStarted;
	setGameJustStarted(false);
	if (!game->parseGame(board, moveSequence, moveCount, first)) {
		setGameJustStarted(pGameJustStarted);
		System::Windows::Forms::MessageBox::Show(this, "无效游戏存档：\n" + fileName,
			"游戏存档无效", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Stop);
		return;
	}
	fairGame = false;
	setGameInfo();
	autoClear();
}

void frmMain::openGame() {
	if (analyzeMode) return;
	OpenFileDialog^ dialog = gcnew OpenFileDialog();
	dialog->Title = "选择游戏存档";
	dialog->Filter = "Craft 游戏存档(*.CraftSave)|*.CraftSave";
	dialog->CheckFileExists = true;
	dialog->Multiselect = false;
	dialog->FileName = "";
	setPaused(true);
	try {
		Windows::Forms::DialogResult res = dialog->ShowDialog(this);
		if (res == Windows::Forms::DialogResult::Cancel) {
			return;
		}
		openGameFile(dialog->FileName);
	} finally {
		delete dialog;
		setPaused(false);
	}
}

void frmMain::saveGameFile(String^ fileName) {
	IO::FileStream^ fs;
	try {
		fs = gcnew IO::FileStream(fileName, IO::FileMode::Create, IO::FileAccess::Write);
	} catch (Exception^) {
		System::Windows::Forms::MessageBox::Show(this, "无法写入 " + fileName + " 文件。",
			"无法保存游戏存档", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Stop);
		return;
	}
	IO::StreamWriter^ writer = gcnew IO::StreamWriter(fs, System::Text::Encoding::UTF8);
	try {
		String^ versionInfo = __APP_NAME__ + " " + __APP_VERSION__; // reserved for further detection
		writer->WriteLine(versionInfo);
		Board^ board = game->getInitialBoard();
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				switch (board[j, i]) {
				case Chess::BLACK:
					writer->Write('X');
					break;
				case Chess::WHITE:
					writer->Write('O');
					break;
				default:
					writer->Write('.');
				}
			}
			writer->WriteLine();
		}
		writer->WriteLine((gcBlack->getFirstPlayer() == Chess::BLACK) ? "BLACK" : "WHITE");
		String^ const bColumn = "ABCDEFGH";
		String^ const wColumn = "abcdefgh";
		String^ const row = "12345678";
		int maxStep = analyzeMode ? previousStep : gcBlack->getCurrentStep();
		for (int i = 1; i < maxStep; i++) {
			Othello::Move currentMove = gcBlack->getMove(i);
			if (currentMove.getColor() == Chess::AVAILABLE) continue;
			writer->Write((currentMove.getColor() == Chess::BLACK) ? bColumn[currentMove.getX()] : wColumn[currentMove.getX()]);
			writer->Write(row[currentMove.getY()]);
		}
	} catch (Exception^) {
		System::Windows::Forms::MessageBox::Show(this, "写入 " + fileName + " 文件出错。",
			"保存游戏存档出错", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Stop);
		writer->Close();
		return;
	}
	writer->Close();
	System::Windows::Forms::MessageBox::Show(this, "游戏已保存在：\n" + fileName,
		"游戏已保存", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
}

void frmMain::saveGame() {
	SaveFileDialog^ dialog = gcnew SaveFileDialog();
	dialog->Title = "保存游戏存档";
	dialog->Filter = "Craft 游戏存档(*.CraftSave)|*.CraftSave";
	dialog->CheckFileExists = false;
	dialog->FileName = "";
	setPaused(true);
	try {
		Windows::Forms::DialogResult res = dialog->ShowDialog(this);
		if (res == Windows::Forms::DialogResult::Cancel) {
			return;
		}
		saveGameFile(dialog->FileName);
	} finally {
		delete dialog;
		setPaused(false);
	}
}

void frmMain::setGameInfo() {
	if (analyzeMode) return;
	if (!endGameMode)
		ssPlayers->Text = gcWhite->getOpponentName() + " VS " + gcBlack->getOpponentName()
			+ ((fairGame) ? "(正式)" : "");
	else ssPlayers->Text = "执" + ((currentEndGamePlayer == Chess::BLACK) ? "黑" : "白")
		+ " 最佳结果: " + getEndGameResultDescription(bestResult);
	tsbtnBack->ToolTipText = tsbtnBack->Text 
		+ ((fairGame) ? ("(剩余 " + goBackChance + " 次)") : "");
	tsbtnTip->ToolTipText = tsbtnTip->Text 
		+ ((fairGame) ? ("(剩余 " + tipChance + " 次)") : "");
	showPVChanged();
}

void frmMain::setFairness() {
	fairGame = false;
	if (userInfo->FreeMode) {
		setGameInfo();
		return;
	}
	if (blackType == PlayerType::GUI) {
		if (whiteType != PlayerType::GUI && whiteType != PlayerType::NETWORK) {
			fairGame = game->isLearnable();
			if (fairGame) {
				goBackChance = getGoBackChance(whiteType);
				tipChance = getTipChance(whiteType);
			}
		}
	} else if (whiteType == PlayerType::GUI) {
		if (blackType != PlayerType::NETWORK) {
			fairGame = game->isLearnable();
			if (fairGame) {
				goBackChance = getGoBackChance(blackType);
				tipChance = getTipChance(blackType);
			}
		}
	}
	setGameInfo();
}

int frmMain::getGoBackChance(PlayerType type) {
	switch (type) {
	case PlayerType::RANDOM:
		return 3;
		break;
	case PlayerType::BIG_EAT:
		return 3;
		break;
	case PlayerType::EARTH:
		return 5;
		break;
	case PlayerType::CLOUDS:
		return 8;
		break;
	case PlayerType::ORBIT:
		return 8;
		break;
	case PlayerType::MOON:
		return 10;
		break;
	case PlayerType::STAR:
		return 12;
		break;
	case PlayerType::GALAXY:
		return 12;
		break;
	case PlayerType::UNIVERSE:
		return 15;
		break;
	default:
		return 0;
	}
}

int frmMain::getTipChance(PlayerType type) {
	switch (type) {
	case PlayerType::RANDOM:
		return 0;
		break;
	case PlayerType::BIG_EAT:
		return 0;
		break;
	case PlayerType::EARTH:
		return 1;
		break;
	case PlayerType::CLOUDS:
		return 2;
		break;
	case PlayerType::ORBIT:
		return 2;
		break;
	case PlayerType::MOON:
		return 3;
		break;
	case PlayerType::STAR:
		return 4;
		break;
	case PlayerType::GALAXY:
		return 4;
		break;
	case PlayerType::UNIVERSE:
		return 5;
		break;
	default:
		return 0;
	}
}

bool frmMain::breakFairness(String ^prompt) {
	if (userInfo->FreeMode) return true;
	if (!fairGame) return true;
	if (gameJustStarted) return true;
	Windows::Forms::DialogResult res = MessageBox::Show(this, prompt, "提示", MessageBoxButtons::YesNo, 
		MessageBoxIcon::Question, MessageBoxDefaultButton::Button2);
	if (res == Windows::Forms::DialogResult::No) return false;
	fairGame = false;
	PlayerType opponentType = (blackType == PlayerType::GUI) ? whiteType : blackType;
	userInfo->getStatistics(opponentType)->recordGame(-WIDTH * HEIGHT);
	setGameInfo();
	return true;
}

void frmMain::changeMode() {
	setPaused(true);
	bool ifbreak = breakFairness("继续切换模式将记您本次对局为负。\n确实要切换模式吗？");
	setPaused(false);
	if (!ifbreak) return;
	userInfo->FreeMode = !userInfo->FreeMode;
	if (!userInfo->FreeMode) userInfo->UseBook = true;
	mnuFreeMode->Checked = userInfo->FreeMode;
	mnuUseBook->Checked = userInfo->UseBook;
	if (gameJustStarted) setFairness();
}

System::Void frmMain::mnuFreeMode_Click(System::Object^  sender, System::EventArgs^  e) {
	changeMode();
}

System::Void frmMain::mnuShowStatistics_Click(System::Object^  sender, System::EventArgs^  e) {
	showStatistics();
}

void frmMain::showStatistics() {
	if (statisticsShowing)
		statisticsWindow->Activate();
	else {
		statisticsWindow = gcnew frmStatistics(userInfo, this);
		statisticsWindow->Show(this);
	}
}

System::Void frmMain::mnuTip_Click(System::Object^  sender, System::EventArgs^  e) {
	getGUITip();
}
System::Void frmMain::tsbtnTip_Click(System::Object^  sender, System::EventArgs^  e) {
	getGUITip();
}

void frmMain::forceEndSolve() {
	if (endGameMode) return;
	if (!GUIPlay) return;
	Chess guiColor = gcBlack->getCurrentPlayer();
	GameContext^ guiContext = (guiColor == Chess::BLACK) ? gcBlack : gcWhite;
	if (guiContext->getAvailableCount() == 0) {
		return;
	}

	setPaused(true);

	if (guiContext->getEmptyNumber() > END_SOLVE_WARNING_DEPTH) {
		String^ warning = "此时进行终局求解可能花费较长时间，\n建议在剩余空格数小于或等于"
			+ END_SOLVE_WARNING_DEPTH + "时求解。\n要继续吗？";
		Windows::Forms::DialogResult result = MessageBox::Show(this, warning, "警告",
			MessageBoxButtons::YesNo, MessageBoxIcon::Warning, MessageBoxDefaultButton::Button2);
		if (result == Windows::Forms::DialogResult::No) {
			setPaused(false);
			return;
		}
	}

	bool ifbreak = breakFairness("继续进行终局求解将记您本次对局为负。\n确实要进行终局求解吗？");
	setPaused(false);
	if (!ifbreak) return;
	
	setGUIPlay(false);

	setTipPos(-1, -1);
	guiTipper->init(guiContext, guiColor);
	int res = safe_cast<Craft^>(guiTipper)->getTip(guiContext, Othello::Move::invalidMove, true);
	if (res < 0 || res >= guiContext->getAvailableCount()) return;
	Point pos = guiContext->getAvailableMove(res);
	setSelectedMove(-1, -1);
	setTipPos(pos.X, pos.Y);

	setGUIPlay(true);

	mnuForceEndSolve->Enabled = false;
	tsbtnForceEndSolve->Enabled = false;

	tsbtnTip->Enabled = false;
	mnuTip->Enabled = false;

}

void frmMain::getGUITip() {
	if (endGameMode) return;
	if (!GUIPlay) return;
	Chess guiColor = gcBlack->getCurrentPlayer();
	GameContext^ guiContext = (guiColor == Chess::BLACK) ? gcBlack : gcWhite;
	if (guiContext->getAvailableCount() == 0) {
		return;
	}
	if (fairGame && (guiContext->getAvailableCount() == 1)) {
		tipChance++;
	}

	if (fairGame && (tipChance == 0)) {
		setPaused(true);
		bool ifbreak = breakFairness("您的提示次数已达到限制，继续提示将记您本次对局为负。\n确实要提示吗？");
		setPaused(false);
		if (!ifbreak) return;
	}
	
	if (fairGame) {
		tipChance--;
		tsbtnTip->ToolTipText = tsbtnTip->Text + "(剩余 " + tipChance + " 次)";
		prompt("您还剩下 " + tipChance + " 次提示机会！", iconWarning);
	}

	setGUIPlay(false);
	tipping = true;

	setTipPos(-1, -1);
	guiTipper->init(guiContext, guiColor);
	int res = safe_cast<Craft^>(guiTipper)->getTip(guiContext, Othello::Move::invalidMove, false);
	if (res < 0 || res >= guiContext->getAvailableCount()) return;
	Point pos = guiContext->getAvailableMove(res);
	setSelectedMove(-1, -1);
	setTipPos(pos.X, pos.Y);

	tipping = false;
	setGUIPlay(true);

	tsbtnTip->Enabled = false;
	mnuTip->Enabled = false;

}

void frmMain::setTipPos(int x, int y) {
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
		if (x != tipPos.X || y != tipPos.Y) {
			if (tipPos.X >= 0 && tipPos.X < WIDTH 
			&& tipPos.Y >= 0 && tipPos.Y < HEIGHT) {
				imageBoard[tipPos.X, tipPos.Y].tip = false;
				board[tipPos.X, tipPos.Y]->setTip(false);
			}
			imageBoard[x, y].tip = true;
			board[x, y]->setTip(true);
			tipPos.X = x; tipPos.Y = y;
		}
	} else {
		if (tipPos.X >= 0 && tipPos.X < WIDTH 
		&& tipPos.Y >= 0 && tipPos.Y < HEIGHT) {
			imageBoard[tipPos.X, tipPos.Y].tip = false;
			board[tipPos.X, tipPos.Y]->setTip(false);
		}
		tipPos.X = -1; tipPos.Y = -1;
	}
}

void frmMain::showShortKeys() {
	setPaused(true);
	MessageBox::Show(this, shortKeys, "键盘快捷键", MessageBoxButtons::OK, MessageBoxIcon::Information);
	setPaused(false);
}

System::Void frmMain::mnuShortkeys_Click(System::Object^  sender, System::EventArgs^  e) {
	showShortKeys();
}

void frmMain::setSearchState(bool searching) {
	if (searchState = searching) { /* !! Not '==' !! */
		resetComponents();
		ssResult->Image = iconBusy;
		ssResult->Text = "思考中";
		picBoard->Cursor = ::Cursors::WaitCursor;
		//Cursor = ::Cursors::WaitCursor;
		if (!endGameMode) {
			tsbtnStopSearch->Enabled = true;
			mnuStopSearch->Enabled = true;
		}
	} else {
		ssResult->Image = iconRest;
		picBoard->Cursor = ::Cursors::Default;
		//Cursor = ::Cursors::Default;
		if (!userInfo->ShowEvaluation || endGameMode) {
			ssResult->Text = "空闲";
		}
		tsbtnStopSearch->Enabled = false;
		mnuStopSearch->Enabled = false;

		if (!userInfo->ShowSpeed || endGameMode) {
			ssNodes->Text = "";
			ssSpeed->Text = "";
		}
		if (!needShowPV()) {
			ssPV->Text = "最优着法序列: (仅自由模式下进行搜索时显示)";
		}
	}
}

void frmMain::setResult(System::String^ result) {
	if (userInfo->ShowEvaluation && !endGameMode)
		ssResult->Text = result;
}
void frmMain::setSpeed(System::String^ speed) {
	if (userInfo->ShowSpeed && !endGameMode)
		ssSpeed->Text = speed;
}
void frmMain::setTotalNum(System::String^ totalNum) {
	if (userInfo->ShowSpeed && !endGameMode)
		ssNodes->Text = totalNum;
}
void frmMain::setProgress(int percent) {
	//if (userInfo->ShowProgress) {
		tspb1->Value = percent;
		if (percent == 0) {
			if (tspb1->Style != ProgressBarStyle::Marquee)
				tspb1->Style = ProgressBarStyle::Marquee;
		} else {
			if (tspb1->Style != ProgressBarStyle::Continuous)
				tspb1->Style = ProgressBarStyle::Continuous;
		}
	//}
}
void frmMain::setFocusedMove(int x, int y) {
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
		if (x != focusedMove.X || y != focusedMove.Y) {
			if (focusedMove.X >= 0 && focusedMove.X < WIDTH 
			&& focusedMove.Y >= 0 && focusedMove.Y < HEIGHT)
				board[focusedMove.X, focusedMove.Y]->setFocused(false);
			board[x, y]->setFocused(true);
			focusedMove.X = x; focusedMove.Y = y;
		}
	} else {
		if (focusedMove.X >= 0 && focusedMove.X < WIDTH 
		&& focusedMove.Y >= 0 && focusedMove.Y < HEIGHT)
			board[focusedMove.X, focusedMove.Y]->setFocused(false);
		focusedMove.X = -1; focusedMove.Y = -1;
	}
}
void frmMain::setSelectedMove(int x, int y) {
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
		if (x != selectedMove.X || y != selectedMove.Y) {
			if (selectedMove.X >= 0 && selectedMove.X < WIDTH 
			&& selectedMove.Y >= 0 && selectedMove.Y < HEIGHT)
				board[selectedMove.X, selectedMove.Y]->setSelected(false);
			board[x, y]->setSelected(true);
			selectedMove.X = x; selectedMove.Y = y;
		}
	} else {
		if (selectedMove.X >= 0 && selectedMove.X < WIDTH 
		&& selectedMove.Y >= 0 && selectedMove.Y < HEIGHT)
			board[selectedMove.X, selectedMove.Y]->setSelected(false);
		selectedMove.X = -1; selectedMove.Y = -1;
	}
}

void frmMain::showPrincipleVariation(System::String^ pv) {
	if (needShowPV())
		ssPV->Text = "最优着法序列: " + pv;
}

void frmMain::resetComponents() {
	picBoard->Cursor = ::Cursors::Default;
	ssResult->Image = iconRest;
	ssResult->Text = "空闲";
	ssSpeed->Text = "";
	ssNodes->Text = "";
	ssPV->Text = "最优着法序列: (仅自由模式下进行搜索时显示)";
	tspb1->Value = 0;
	tspb1->Style = ProgressBarStyle::Continuous;
	setSelectedMove(-1, -1);
	setFocusedMove(-1, -1);
	setTipPos(-1, -1);
	tsbtnStopSearch->Enabled = false;
	mnuStopSearch->Enabled = false;
}

void frmMain::searchStarted() {
	searching = true;
	setTranspositionTableAllowed(false);
}

void frmMain::searchEnded() {
	searching = false;
	setTranspositionTableAllowed(true);
}

void frmMain::showAnalyzer() {
	if (endGameMode) return;
	if (analyzeMode) {
		if (analyzer->WindowState == FormWindowState::Minimized)
			analyzer->WindowState = FormWindowState::Normal;
		analyzer->Activate();
	} else {
		if (gameJustStarted) return;
		setPaused(true);
		bool ifbreak = breakFairness("继续分析棋局将计您本次对局为负。\n确实要进行分析吗？");
		setPaused(false);
		if (!ifbreak) return;
		analyzer = gcnew frmAnalyzer(this, gcBlack, userInfo->Analyzer);
		analyzer->Show(this);
	}
}

void frmMain::enterAnalyzeMode() {
	if (analyzeMode) return;
	if (gameRunning) game->pauseGame();
	analyzeMode = true;
	previousStep = gcBlack->getCurrentStep();
	tsbtnNew->Enabled = false;
	mnuNew->Enabled = false;
	tsbtnRestart->Enabled = false;
	mnuRestart->Enabled = false;
	tsbtnSetupBoard->Enabled = false;
	mnuSetupBoard->Enabled = false;
	previousLearnEnabled = mnuLearn->Enabled;
	tsbtnLearn->Enabled = false;
	mnuLearn->Enabled = false;
	tsbtnBlack->Enabled = false;
	tsbtnWhite->Enabled = false;
	tsmnuNewEndGame->Enabled = false;
	mnuNewEndGame->Enabled = false;
	tsbtnOpenGame->Enabled = false;
	mnuOpenGame->Enabled = false;
	mnuPaste->Enabled = false;
	tspb1->Visible = false;
	ssSpace->Visible = false;
	ssNodes->Visible = false;
	ssSpeed->Visible = false;
	ssResult->Visible = false;
	ssPlayers->Width = statusBar->Width - 15;
	ssPrompt->Width = ssPlayers->Width;
	ssPlayers->Text = "分析模式";
	showPVChanged();
}

void frmMain::leaveAnalyzeMode() {
	if (!analyzeMode) return;
	analyzeMode = false;
	setQuestionLevel(QuestionLevel::NONE);
	showMove(previousStep);
	if (!gcBlack->isGameEnded()) startGameForGUIPlayer();
	tsbtnNew->Enabled = true;
	mnuNew->Enabled = true;
	tsbtnRestart->Enabled = true;
	mnuRestart->Enabled = true;
	tsbtnSetupBoard->Enabled = true;
	mnuSetupBoard->Enabled = true;
	mnuLearn->Enabled = previousLearnEnabled && gcBlack->isGameEnded();
	tsbtnLearn->Enabled = previousLearnEnabled && gcBlack->isGameEnded();
	tsbtnBlack->Enabled = true;
	tsbtnWhite->Enabled = true;
	tsmnuNewEndGame->Enabled = (wthorFileList->Count > 0);
	mnuNewEndGame->Enabled = tsmnuNewEndGame->Enabled;
	tsbtnOpenGame->Enabled = true;
	mnuOpenGame->Enabled = true;
	mnuPaste->Enabled = true;
	ssPlayers->Width = statusBar->Width - ssPlayersOffset;
	ssPrompt->Width = ssPlayers->Width;
	ssResult->Visible = true;
	ssSpeed->Visible = true;
	ssNodes->Visible = true;
	ssSpace->Visible = true;
	tspb1->Visible = userInfo->ShowProgress;
	userInfo->Analyzer = analyzer->getAnalyzerType();
	setGameInfo();
	analyzer = nullptr;
	showPVChanged();
}

void frmMain::setTranspositionTableAllowed(bool allowed) {
	mnuClearCache->Enabled = allowed;
	for each (ToolStripMenuItem ^tsmnu in mnuTableSize->DropDownItems) {
#ifndef MACHINE_X64
		if (tsmnu == mnu2GB) continue;
#endif
		tsmnu->Enabled = allowed;
	}
}

void frmMain::analyzeStarted() {
	searching = true;
	analyzing = true;
	//showMove(previousStep);
	tsbtnExit->Enabled = false;
	mnuExit->Enabled = false;
	btnStart->Enabled = false;
	setTranspositionTableAllowed(false);
}

void frmMain::analyzeEnded() {
	searching = false;
	analyzing = false;
	tsbtnExit->Enabled = true;
	mnuExit->Enabled = true;
	btnStart->Enabled = true;
	setTranspositionTableAllowed(true);
}

void frmMain::analyzerOpened() {
	enterAnalyzeMode();
}
void frmMain::analyzerClosed() {
	leaveAnalyzeMode();
}

void frmMain::showMove(int step) {
	int currentStep = gcBlack->getCurrentStep();
	if (step > currentStep) 
		game->goForward(step - currentStep);
	else if (step < currentStep)
		game->goBack(currentStep - step);
	else {
		if (!analyzeMode) setBoard();
		//if (!gcBlack->isGameEnded()) startGameForGUIPlayer();
	}
}

void frmMain::startFromCurrentStep() {
	previousStep = gcBlack->getCurrentStep();
	analyzer->Close();
}

void frmMain::statisticsWindowShown() {
	statisticsShowing = true;
}

void frmMain::statisticsWindowClosed() {
	statisticsShowing = false;
}

void frmMain::acquirePause(bool paused) {
	setPaused(paused);
}

System::Void frmMain::mnuAnalyze_Click(System::Object^  sender, System::EventArgs^  e) {
	showAnalyzer();
}

System::Void frmMain::tsbtnAnalyze_Click(System::Object^  sender, System::EventArgs^  e) {
	showAnalyzer();
}

void frmMain::startNewEndGame(EndGameDifficulty difficulty) {
	if (analyzeMode) return;
	if (totalWThorGames == 0) {
		setPaused(true);
		MessageBox::Show(this, "找不到 WThor 棋局文件。\n请确认游戏目录下 thor 文件夹内容的完整性。",
			"找不到棋局文件", MessageBoxButtons::OK, MessageBoxIcon::Stop);
		setPaused(false);
		return;
	}
	setPaused(true);
	bool ifBreak = breakFairness("开始新的残局将记您本次对局为负。\n确实要开始新的残局吗？");
	setPaused(false);
	if (!ifBreak) return;
	game->pauseGame();
	int startFrom = rnd->Next(totalWThorGames);
	int empties;
	switch (difficulty) {
	case EndGameDifficulty::BEGINNER:
		empties = rnd->Next(5, 9); // 5 ~ 8 empties
		break;
	case EndGameDifficulty::AMATEUR:
		empties = rnd->Next(9, 13); // 9 ~ 12 empties
		break;
	case EndGameDifficulty::PROFESSIONAL:
		empties = rnd->Next(13, 17); // 13 ~ 16 empties
		break;
	case EndGameDifficulty::MASTER:
		empties = rnd->Next(17, 19); // 17 ~ 18 empties
		break;
	default:
		empties = userInfo->CustomEmpties;
		break;
	}
	int i = startFrom;
	do {
		if (startNewEndGame(i, empties)) {
			currentEndGameDifficulty = difficulty;
			return;
		}
		i++; if (i == totalWThorGames) i = 0;
	} while (i != startFrom);
	setPaused(true);
	MessageBox::Show(this, "棋局文件中找不到有效对局。\n请确认游戏目录下 thor 文件夹内容的完整性。",
		"找不到有效对局", MessageBoxButtons::OK, MessageBoxIcon::Stop);
	setPaused(false);
}

bool frmMain::startNewEndGame(int gameIndex, int empties) {
	if (gameIndex >= totalWThorGames) return false;
	int fileIndex = 0;
	while (gameIndex >= wthorFileList[fileIndex].numberOfGames) {
		gameIndex -= wthorFileList[fileIndex].numberOfGames;
		fileIndex++;
	}
	if (!wthorFileList[fileIndex].fileLocation) return false;
	if (!wthorReader->loadGameFile(wthorFileList[fileIndex].fileLocation)) {
		wthorFileList[fileIndex].fileLocation = nullptr;
		return false;
	}
	WThorGame^ thisGame = gcnew WThorGame();
	if (!wthorReader->readGame(gameIndex, thisGame)) {
		return false;	
	}
	int moveCount = 60 - empties;
	if (thisGame->moveSequence->Length < moveCount)
		return false;
	if (!Game::checkGame(ChessBoard::getDefaultBoard(1), thisGame->moveSequence,
		moveCount, Chess::BLACK)) {
			return false;
	}

	ChessBoard^ temp = gcnew ChessBoard();
	Chess currentPlayer = Chess::BLACK;
	for (int i = 0; i < moveCount; i++) {
		if (temp->getAvailableCount(currentPlayer) == 0) {
			temp->recordInvalidMove(currentPlayer);
			currentPlayer = (currentPlayer == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
		}
		temp->putChess(thisGame->moveSequence[i].X, thisGame->moveSequence[i].Y, currentPlayer);
		currentPlayer = (currentPlayer == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
	}
	if (temp->isGameEnded()) return false;
	if (temp->getAvailableCount(currentPlayer) == 0) {
		temp->recordInvalidMove(currentPlayer);
		currentPlayer = (currentPlayer == Chess::BLACK ? Chess::WHITE : Chess::BLACK);
	}
	Board^ board = gcnew Board(WIDTH, HEIGHT);
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			board[i, j] = temp->get(i, j);
		}
	GameOption option;
	if (currentPlayer == Chess::BLACK) {
		option.black = PlayerType::GUI;
		option.white = PlayerType::UNIVERSE;
	} else {
		option.black = PlayerType::UNIVERSE;
		option.white = PlayerType::GUI;
	}
	option.board = board;
	option.firstPlayer = currentPlayer;
	startNewGame(option);
	
	this->Cursor = Cursors::WaitCursor;
	if (endGameInfoShowing) endGameInfoWindow->Cursor = Cursors::WaitCursor;
	Solver* solver = Solver::newInstance();
	int bd[WIDTH * HEIGHT];
	Conversions::convertBoard(board, bd);
	solver->parseGame(bd, NULL, 0);
	SolverResult result = solver->solveExact(
		currentPlayer == Chess::BLACK ? Solver::BLACK : Solver::WHITE, false
	);
	delete solver;
	if (endGameInfoShowing) endGameInfoWindow->Cursor = Cursors::Default;
	this->Cursor = Cursors::Default;
	
	bestResult = result.getResult();
	currentEndGame = thisGame;
	currentEndGameEmpties = empties;
	currentEndGamePlayer = currentPlayer;
	showEndGameInfo(thisGame, result.getResult(), empties, currentPlayer);

	return true;
}

void frmMain::showEndGameInfo(WThorGame^ game, int bestResult, int empties, Chess player) {
	if (endGameInfoShowing)
		endGameInfoWindow->showInfo(game, bestResult, empties, player);
	else {
		endGameInfoWindow = gcnew frmEndGameInfo(this);
		endGameInfoWindow->Show(this);
		endGameInfoWindow->showInfo(game, bestResult, empties, player);
	}
}

void frmMain::enterEndGameMode() {
	endGameMode = true;
	tsbtnBlack->Enabled = false;
	tsbtnWhite->Enabled = false;
	tsbtnAnalyze->Enabled = false;
	mnuAnalyze->Enabled = false;
	tsmnuShowEndGameInfo->Enabled = true;
	mnuShowEndGameInfo->Enabled = true;
	setBoard();
	setGameInfo();
}

void frmMain::leaveEndGameMode() {
	endGameMode = false;
	tsbtnBlack->Enabled = true;
	tsbtnWhite->Enabled = true;
	tsbtnAnalyze->Enabled = true;
	tsmnuShowEndGameInfo->Enabled = false;
	mnuShowEndGameInfo->Enabled = false;
	setBoard();
	setGameInfo();
	currentEndGame = nullptr;
	currentEndGameEmpties = 0;
	currentEndGamePlayer = Chess::AVAILABLE;
}

void frmMain::endGameInfoShown() {
	endGameInfoShowing = true;
	this->Enabled = false;
}

void frmMain::endGameInfoClosed() {
	this->Enabled = true;
	if (gcBlack->getCurrentPlayer() == currentEndGamePlayer)
		startGame();
	enterEndGameMode();
	endGameInfoShowing = false;
	endGameInfoWindow = nullptr;
	wthorReader->unloadGameFile();
}

void frmMain::acquireChange() {
	startNewEndGame(currentEndGameDifficulty);
}

void frmMain::startCustomEndGame() {
	if (analyzeMode) return;

	setPaused(true);
	frmSetCustomEmpties^ customEmptiesWindow = 
		gcnew frmSetCustomEmpties(userInfo->CustomEmpties);

	try {
		Windows::Forms::DialogResult res = customEmptiesWindow->ShowDialog(this);
		setPaused(false);
		if (res != Windows::Forms::DialogResult::OK) return;

		userInfo->CustomEmpties = customEmptiesWindow->CustomEmpties;
		startNewEndGame(EndGameDifficulty::CUSTOM);
	} finally {
		delete customEmptiesWindow;
	}
}

System::Void frmMain::tsmnuEndGameBeginner_Click(System::Object^  sender, System::EventArgs^  e) {
	startNewEndGame(EndGameDifficulty::BEGINNER);
}
System::Void frmMain::tsmnuEndGameAmateur_Click(System::Object^  sender, System::EventArgs^  e) {
	startNewEndGame(EndGameDifficulty::AMATEUR);
}
System::Void frmMain::tsmnuEndGameProfessional_Click(System::Object^  sender, System::EventArgs^  e) {
	startNewEndGame(EndGameDifficulty::PROFESSIONAL);
}
System::Void frmMain::tsmnuEndGameMaster_Click(System::Object^  sender, System::EventArgs^  e) {
	startNewEndGame(EndGameDifficulty::MASTER);
}
System::Void frmMain::tsmnuEndGameCustom_Click(System::Object^  sender, System::EventArgs^  e) {
	startCustomEndGame();
}
System::Void frmMain::mnuEndGameBeginner_Click(System::Object^  sender, System::EventArgs^  e) {
	startNewEndGame(EndGameDifficulty::BEGINNER);
}
System::Void frmMain::mnuEndGameAmateur_Click(System::Object^  sender, System::EventArgs^  e) {
	startNewEndGame(EndGameDifficulty::AMATEUR);
}
System::Void frmMain::mnuEndGameProfessional_Click(System::Object^  sender, System::EventArgs^  e) {
	startNewEndGame(EndGameDifficulty::PROFESSIONAL);
}
System::Void frmMain::mnuEndGameMaster_Click(System::Object^  sender, System::EventArgs^  e) {
	startNewEndGame(EndGameDifficulty::MASTER);
}
System::Void frmMain::mnuEndGameCustom_Click(System::Object^  sender, System::EventArgs^  e) {
	startCustomEndGame();
}
System::Void frmMain::tsbtnShowStatistics_Click(System::Object^  sender, System::EventArgs^  e) {
	showStatistics();
}

void frmMain::reshowEndGameInfo() {
	if (!endGameMode) return;
	game->pauseGame();
	showEndGameInfo(currentEndGame, bestResult, currentEndGameEmpties, currentEndGamePlayer);
}

System::Void frmMain::tsmnuShowEndGameInfo_Click(System::Object^  sender, System::EventArgs^  e) {
	reshowEndGameInfo();
}

System::Void frmMain::mnuShowEndGameInfo_Click(System::Object^  sender, System::EventArgs^  e) {
	reshowEndGameInfo();
}

System::Void frmMain::mnuForceEndSolve_Click(System::Object^  sender, System::EventArgs^  e) {
	forceEndSolve();
}

System::Void frmMain::mnuClearCache_Click(System::Object^  sender, System::EventArgs^  e) {
	clearCache();
}

void frmMain::clearCache() {
	if (searching) return;
	Solver::clearCache();
}

System::Void frmMain::mnuDefaultTheme_Click(System::Object^  sender, System::EventArgs^  e) {
	changeTheme(DEFAULT_THEME_NAME);
}

System::Void frmMain::mnuSomeTheme_Click(System::Object^  sender, System::EventArgs^  e) {
	changeTheme(safe_cast<ThemeMenuItem^>(sender)->getThemeName());
}

#ifdef CHRISTMAS
void frmMain::showChristmasWish() {
	playSound(startSound);
	frmWish^ wishWindow = gcnew frmWish(this);
	wishWindow->Opacity = 0.0;
	wishWindow->Text = applicationName + " " + applicationVersion;
	wishWindow->Show();
	while (wishShowing) {
		Threading::Thread::Sleep(50);
		if (wishWindow->Opacity < 1.0)
			wishWindow->Opacity += 0.05;
		Application::DoEvents();
	}
}

void frmMain::wishShown() {
	wishShowing = true;
}

void frmMain::wishClosed() {
	wishShowing = false;
}
#endif

System::Void frmMain::tsbtnForceEndSolve_Click(System::Object^  sender, System::EventArgs^  e) {
	forceEndSolve();
}

void frmMain::stopSearch() {
	if (!searching || analyzing || endGameMode) return;

	if (!tipping) {
		setPaused(true);
		bool ifbreak = breakFairness("停止搜索将记您本次对局为负。\n确实要停止搜索吗？");
		setPaused(false);
		if (!ifbreak) return;
	}

	game->goBack(0);
}

System::Void frmMain::tsbtnStopSearch_Click(System::Object^  sender, System::EventArgs^  e) {
	stopSearch();
}

System::Void frmMain::mnuStopSearch_Click(System::Object^  sender, System::EventArgs^  e) {
	stopSearch();
}

System::Void frmMain::mnuUseBook_Click(System::Object^  sender, System::EventArgs^  e) {
	if (userInfo->UseBook) {
		setPaused(true);
		bool ifbreak = breakFairness("取消“使用棋谱”将记您本次对局为负。\n确实要取消“使用棋谱”吗？");
		setPaused(false);
		if (!ifbreak) return;
	}
	userInfo->UseBook = !userInfo->UseBook;
	if (!userInfo->UseBook) userInfo->FreeMode = true;
	mnuUseBook->Checked = userInfo->UseBook;
	mnuFreeMode->Checked = userInfo->FreeMode;
	if (gameJustStarted) setFairness();
}

void frmMain::notifyUser() {
	if (Form::ActiveForm != this)
		FlashWindowEx(this->Handle, FLASHW_ALL | FLASHW_TIMERNOFG, 7, 0);
}

System::Void frmMain::mnuAutoClean_Click(System::Object^  sender, System::EventArgs^  e) {
	userInfo->AutoCleanTable = !userInfo->AutoCleanTable;
	mnuAutoClean->Checked = userInfo->AutoCleanTable;
}

void frmMain::autoClear() {
	if (userInfo->AutoCleanTable)
		Solver::clearCache();
}

System::Void frmMain::mnuShowPV_Click(System::Object^  sender, System::EventArgs^  e) {
	userInfo->ShowPrincipleVariation = !userInfo->ShowPrincipleVariation;
	mnuShowPV->Checked = userInfo->ShowPrincipleVariation;
	showPVChanged();
}

void frmMain::showPVChanged() {
	if (searchState && !needShowPV()) {
		ssPV->Text = "最优着法序列: (仅自由模式下进行搜索时显示)";
	}
	bool visible = userInfo->ShowPrincipleVariation;
	if (visible == statusBar2->Visible) return;
	statusBar2->Visible = visible;
	setLayout();
}

void frmMain::WndProc(System::Windows::Forms::Message% m) {
	if (SecondInstance::WM_SISTART && m.Msg == SecondInstance::WM_SISTART) {
		if (this->WindowState == FormWindowState::Minimized)
			SecondInstance::CLRShowWindow(this->Handle, SecondInstance::CLR_SW_RESTORE);
		notifyUser();
	}
	System::Windows::Forms::Form::WndProc(m);
}

void frmMain::copyBoard() {
	System::String^ str = BoardResolver::getBoardString(getCurrentBoard(), gcBlack->getCurrentPlayer());
	try {
		Clipboard::SetText(str);
		//setPaused(true);
		//MessageBox::Show(this, "已将当前局面复制到剪贴板。", "复制局面", 
		//	MessageBoxButtons::OK, MessageBoxIcon::Information);
		//setPaused(false);
		prompt("已将当前局面复制到剪贴板。");
	} catch (...) {
		prompt("复制局面失败！！", iconError);
	}
}

void frmMain::pasteBoard() {
	if (analyzeMode) return;
	System::String^ str;
	try {
		str = Clipboard::GetText();
	} catch (...) {
		prompt("读取剪贴板失败！！", iconError);
	}

	Board^ bd = gcnew Board(WIDTH, HEIGHT);
	Chess fp = BoardResolver::processBoard(str, bd);

	if (fp != Chess::AVAILABLE)
		setupBoard(bd, fp);
	else
		prompt("剪贴板不含有效局面！！", iconError);
}

void frmMain::prompt(System::String ^content) {
	int lTimeout = DEFAULT_PROMPT_TIMEOUT;
	prompt(content, lTimeout);
}

void frmMain::prompt(System::String ^content, int timeout) {
	System::Drawing::Image^ lIcon = iconInfo;
	prompt(content, timeout, lIcon);
}

void frmMain::prompt(System::String ^content, System::Drawing::Image ^icon) {
	int lTimeout = DEFAULT_PROMPT_TIMEOUT;
	prompt(content, lTimeout, icon);
}

void frmMain::prompt(System::String ^content, int timeout, System::Drawing::Image ^icon) {
	ssPrompt->TextImageRelation = TextImageRelation::ImageBeforeText;
	ssPrompt->Text = content;
	ssPrompt->Image = icon;
	ssPlayers->Visible = false;
	ssPrompt->Visible = true;
	tmrPrompt->Interval = timeout;
	tmrPrompt->Enabled = false;
	tmrPrompt->Enabled = true;
}

System::Void frmMain::tmrPrompt_Tick(System::Object ^sender, System::EventArgs ^e) {
	tmrPrompt->Enabled = false;
	ssPrompt->Visible = false;
	ssPlayers->Visible = true;
}

System::Void frmMain::mnuCopy_Click(System::Object ^sender, System::EventArgs ^e) {
	copyBoard();
}

System::Void frmMain::mnuPaste_Click(System::Object ^sender, System::EventArgs ^e) {
	pasteBoard();
}
