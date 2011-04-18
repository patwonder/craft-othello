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
								frmAnalyzer.cpp
		作者：Patrick
		概述：包含类 frmAnalyzer 的定义。
			frmAnalyzer 类实现“棋局分析”窗体。

********************************************************************************
*/

#include "StdAfx.h"
#include "frmAnalyzer.h"
#include "Conversions.h"
#include "Solver.h"

using namespace Othello;
using namespace CraftEngine;

frmAnalyzer::frmAnalyzer(AnalyzeController^ controller, GameContext^ gc,
						 PlayerType defaultPlayerType) {
	InitializeComponent();

	this->controller = controller;
	this->gc = gc;
	controller->analyzerOpened();
	previousStep = gc->getCurrentStep();
	isGameEnded = gc->isGameEnded();
	if (isGameEnded)
		endDescription = getEndDescription();
	currentPlayerType = defaultPlayerType;
	addAnalyzers();
	solver = Solver::newInstance();
	analyzeResult = solver->getAnalyzeResult();
	solverStart = gcnew System::Threading::ThreadStart(this, &frmAnalyzer::solverStarter);
	dAnalyzeResult = gcnew DetailedAnalyzeResult();
	btnSave->Enabled = false;
	tmrSetLayout->Enabled = false;
	tmrSetLayout->Interval = 200;
	btnContinue->Enabled = false;
	setLayout();
}

frmAnalyzer::~frmAnalyzer() {
	if (components)
		delete components;
	components = nullptr;
	this->!frmAnalyzer();
}

frmAnalyzer::!frmAnalyzer() {
	delete solver;
	solver = NULL;
}

String^ frmAnalyzer::getTwoCharRep(int value) {
	return (value < 10) ? " " + value : value.ToString();
}

String^ frmAnalyzer::getAnalyzerString(PlayerType type) {
	SearchOptions opt = Players::getAISearchOptions(type);
	String^ str = Players::getAIPlayerName(type) 
		+ ":" + getTwoCharRep(opt.midGameDepth) + "步/" + getTwoCharRep(opt.exactGameStep) + "终局";
	if (opt.winLossStep > opt.exactGameStep) {
		str += "/" + getTwoCharRep(opt.winLossStep) + "胜负";
	}
	return str;
}

void frmAnalyzer::addAnalyzers() {
	ComboBox::ObjectCollection^ items = cbAnalyzer->Items;
	items->Clear();
	items->Add(getAnalyzerString(PlayerType::EARTH));
	items->Add(getAnalyzerString(PlayerType::CLOUDS));
	items->Add(getAnalyzerString(PlayerType::ORBIT));
	items->Add(getAnalyzerString(PlayerType::MOON));
	items->Add(getAnalyzerString(PlayerType::STAR));
	items->Add(getAnalyzerString(PlayerType::GALAXY));
	items->Add(getAnalyzerString(PlayerType::UNIVERSE));
	switch (currentPlayerType) {
	case PlayerType::EARTH:
		cbAnalyzer->SelectedIndex = 0;
		break;
	case PlayerType::CLOUDS:
		cbAnalyzer->SelectedIndex = 1;
		break;
	case PlayerType::ORBIT:
		cbAnalyzer->SelectedIndex = 2;
		break;
	case PlayerType::MOON:
		cbAnalyzer->SelectedIndex = 3;
		break;
	case PlayerType::STAR:
		cbAnalyzer->SelectedIndex = 4;
		break;
	case PlayerType::GALAXY:
		cbAnalyzer->SelectedIndex = 5;
		break;
	case PlayerType::UNIVERSE:
		cbAnalyzer->SelectedIndex = 6;
		break;
	default:
		cbAnalyzer->SelectedIndex = 6;
	}
}

void frmAnalyzer::solverStarter() {
	solver->analyzeGame(options.midGameDepth, options.winLossStep, options.exactGameStep);
	isDone = true;
}

void frmAnalyzer::parseGame() {
	Board^ board = gc->getInitialBoard();
	int bd[WIDTH * HEIGHT];
	Conversions::convertBoard(board, bd);
	int steps[WIDTH * HEIGHT];
	int pptr = 0;
	for (int i = 1; i < previousStep; i++) {
		Othello::Move move = gc->getMove(i);
		if (move.getColor() != Chess::AVAILABLE) {
			steps[pptr++] = move.getX() * HEIGHT + move.getY();
		}
	}
	int fp = Solver::AV;
	for (int i = 1; i < previousStep; i++) {
		Othello::Move move = gc->getMove(i);
		if (move.getColor() != Chess::AVAILABLE) {
			fp = (move.getColor() == Chess::BLACK) ? Solver::BLACK : Solver::WHITE;
			break;
		}
	}
	if (fp != Solver::AV) solver->parseGame(bd, steps, pptr, fp);
	else solver->clearGame();
}

void frmAnalyzer::stopAnalyze() {
	aborted = true;
}

void frmAnalyzer::startAnalyze() {
	//if (previousStep == 1) return;
	analyzing = true;
	controller->analyzeStarted();
	aborted = false;
	parseGame();
	isDone = false;
	analyzeResult->clear();
	currentStep = 1;
	currentMaxStep = 0;
	lstResult->Items->Clear();
	dAnalyzeResult->clear();
	lblState->Text = "正在分析...";
	btnAnalyze->Text = "停止分析(&A)";
	btnSave->Enabled = false;
	cbAnalyzer->Enabled = false;
	this->Cursor = ::Cursors::WaitCursor;
	options = Players::getAISearchOptions(currentPlayerType);
	solverThread = gcnew System::Threading::Thread(solverStart);
	solverThread->Start();
	pbAnalyze->Value = 0;
	pbAnalyze->Style = ProgressBarStyle::Marquee;
	btnContinue->Enabled = false;
	bool updated = true;
	while (currentStep < previousStep 
		&& gc->getMove(currentStep).getColor() == Chess::AVAILABLE) {
		dAnalyzeResult->addAnalyzedResult(getAnalyzedResult(
			currentStep, analyzeResult->getEmpties(currentMaxStep),
			Chess::AVAILABLE, WIDTH * HEIGHT, 0, WIDTH * HEIGHT, 0, options
		));
		lstResult->Items->Add(dAnalyzeResult->getAnalyzedResult(currentStep).getGeneralDescription());
		currentStep++;
	}
	while (currentStep < previousStep) {
		while (currentMaxStep < analyzeResult->getMaxMoves()) {
			int playedMove = analyzeResult->getPlayedMove(currentMaxStep);
			int playedEval = analyzeResult->getPlayedEval(currentMaxStep);
			int bestMove = analyzeResult->getBestMove(currentMaxStep);
			int bestEval = analyzeResult->getBestEval(currentMaxStep);
			dAnalyzeResult->addAnalyzedResult(getAnalyzedResult(
				currentStep, analyzeResult->getEmpties(currentMaxStep),
				Conversions::intToChess(analyzeResult->getPlayer(currentMaxStep)),
				playedMove, playedEval, bestMove, bestEval,
				options
			));
			lstResult->Items->Add(dAnalyzeResult->getAnalyzedResult(currentStep).getGeneralDescription());
			currentMaxStep++;
			currentStep++;
			while (currentStep < previousStep 
				&& gc->getMove(currentStep).getColor() == Chess::AVAILABLE) {
				dAnalyzeResult->addAnalyzedResult(getAnalyzedResult(
					currentStep, analyzeResult->getEmpties(currentMaxStep),
					Chess::AVAILABLE, WIDTH * HEIGHT, 0, WIDTH * HEIGHT, 0, options
				));
				lstResult->Items->Add(dAnalyzeResult->getAnalyzedResult(currentStep).getGeneralDescription());
				currentStep++;
			}
			updated = false;
		}
		if (lstResult->Items->Count && !updated) {
			lstResult->SetSelected(lstResult->Items->Count - 1, true);
			int progress;
			pbAnalyze->Value = progress = solver->getPercent();
			if (progress == 0) {
				if (pbAnalyze->Style != ProgressBarStyle::Marquee)
					pbAnalyze->Style = ProgressBarStyle::Marquee;
			} else {
				if (pbAnalyze->Style != ProgressBarStyle::Continuous)
					pbAnalyze->Style = ProgressBarStyle::Continuous;
			}
			updated = true;
		}
		System::Threading::Thread::Sleep(40);
		System::Windows::Forms::Application::DoEvents();
		if (aborted) {
			solver->abortSearch();
			solverThread->Join();
			solver->abortSearchComplete();
			this->Cursor = ::Cursors::Default;
			cbAnalyzer->Enabled = true;
			btnClose->Enabled = true;
			btnAnalyze->Text = "开始分析(&A)";
			lblState->Text = "分析中断";
			btnContinue->Enabled = (lstResult->Items->Count > 0);
			pbAnalyze->Style = ProgressBarStyle::Continuous;
			controller->analyzeEnded();
			analyzing = false;
			return;
		}
	}
	if (isGameEnded) {
		dAnalyzeResult->addAnalyzedResult(AnalyzedMove(endDescription, Chess::AVAILABLE, "", "", 0, 0, "", "",
			System::Drawing::Point(-1, -1), System::Drawing::Point(-1, -1), QuestionLevel::NONE));
		lstResult->Items->Add(endDescription);
		lstResult->SetSelected(previousStep - 1, true);
	}
	pbAnalyze->Value = 100;
	pbAnalyze->Style = ProgressBarStyle::Continuous;
	this->Cursor = ::Cursors::Default;
	cbAnalyzer->Enabled = true;
	btnSave->Enabled = true;
	btnAnalyze->Text = "开始分析(&A)";
	lblState->Text = "分析完毕";
	btnContinue->Enabled = (lstResult->Items->Count > 0);
	controller->analyzeEnded();
	analyzing = false;
}

AnalyzedMove frmAnalyzer::getAnalyzedResult(int step, int empties, Chess color,
	int playedMove, int playedEval, int bestMove, int bestEval,
	SearchOptions options) {
		const int MID_EVAL_DIFFERENCE = 5;
		QuestionLevel questionLevel = QuestionLevel::NONE;
		System::String^ sPlayedEval = getResultDescription(playedEval, playedMove, empties);
		System::String^ sBestEval = getResultDescription(bestEval, bestMove, empties);
		System::String^ sPlayedMove = Conversions::intToString(playedMove);
		System::String^ sBestMove = Conversions::intToString(bestMove);
		String^ result = ((step < 10) ? ("0" + step.ToString()) : step.ToString()) + ".";
		result += " " + sPlayedMove;
		if (playedMove >= WIDTH * HEIGHT || playedMove < -WIDTH * HEIGHT) {
			result += "   (欠行)";
		} else {
			if (empties > options.winLossStep) {
				bool questionable;
				if (bestEval >= Solver::INFINITE - Solver::MAXSTEP) {
					questionable = playedEval < bestEval;
				} else if (bestEval <= -Solver::INFINITE + Solver::MAXSTEP) {
					questionable = playedEval < bestEval;
				} else {
					questionable = bestEval - playedEval >= MID_EVAL_DIFFERENCE * Solver::RULER;
				}
				if (questionable) result += "? "; else result += "  ";
				result += " 分析值: " + sPlayedEval;
				if (questionable) {
					result += " 建议下" + sBestMove + ": " + sBestEval;
					questionLevel = QuestionLevel::NORMAL;
				}
			} else if (empties > options.exactGameStep) {
				if (bestEval > 0 && playedEval == 0) {
					result += "?  " + ((color == Chess::BLACK) ? "黑方" : "白方") + "下了和着"
						+ " 应该下" + sBestMove + ": " + sBestEval;
					questionLevel = QuestionLevel::NORMAL;
				} else if (bestEval >= 0 && playedEval < 0) {
					result += "?? " + ((color == Chess::BLACK) ? "黑方" : "白方") + "下了败着"
						+ " 应该下" + sBestMove + ": " + sBestEval;
					questionLevel = QuestionLevel::BAD;
				} else {
					result += "   " + sPlayedEval + "  胜负正确";
				}
			} else {
				if (bestEval > 0 && playedEval == 0) {
					result += "?  " + ((color == Chess::BLACK) ? "黑方" : "白方") + "下了和着"
						+ " 应该下" + sBestMove + ": " + sBestEval;
					questionLevel = QuestionLevel::NORMAL;
				} else if (bestEval >= 0 && playedEval < 0) {
					result += "?? " + ((color == Chess::BLACK) ? "黑方" : "白方") + "下了败着("
						+ sPlayedEval + ")"
						+ " 应该下" + sBestMove + ": " + sBestEval;
					questionLevel = QuestionLevel::BAD;
				} else if (bestEval > playedEval) {
					result += "?  " + ((color == Chess::BLACK) ? "黑方" : "白方") + "损失"
						+ (bestEval - playedEval) / 2 + "子(" + sPlayedEval + ")"
						+ " 应该下" + sBestMove + ": " + sBestEval;
					questionLevel = QuestionLevel::NORMAL;
				} else {
					result += "   " + sPlayedEval + "  正确";
				}
			}
		}
		return AnalyzedMove(result, color, sPlayedEval, sBestEval, playedEval, bestEval, 
			sPlayedMove, sBestMove, Conversions::intToPoint(playedMove), Conversions::intToPoint(bestMove),
			questionLevel);
}

System::String^ frmAnalyzer::getResultDescription(int res, int bestMove, int empties) {
	System::String^ result;
	if (empties > options.winLossStep) {
		if (res > Solver::INFINITE - Solver::MAXSTEP) { // early win
			result = ((bestMove < 0) ? "" : (res == Solver::INFINITE ? "" : "≥")) 
				+ "+" + (res - Solver::INFINITE + Solver::MAXSTEP).ToString();
			if (bestMove < 0) result += "(棋谱)"; else result += " @ " + options.midGameDepth;
		} else if (res < -Solver::INFINITE + Solver::MAXSTEP) { // early loss
			result = ((bestMove < 0) ? "" : (res == -Solver::INFINITE ? "" : "≤"))
				+ (res + Solver::INFINITE - Solver::MAXSTEP).ToString();
			if (bestMove < 0) result += "(棋谱)"; else result += " @ " + options.midGameDepth;
		} else {
			if (bestMove >= 0) {
				result = ((res > 0) ? "+" : ((res == 0) ? "" : "-"))
					+ (System::Math::Abs((double)(res)) / Solver::RULER).ToString("0.00")
					+ " @ " + options.midGameDepth;;
			} else {
				result = ((res > 0) ? "+" : ((res == 0) ? "" : "-"))
					+ (System::Math::Abs((double)(res)) / Solver::RULER).ToString("0.00")
					+ "(棋谱)";
			}
		}
	} else if (empties > options.exactGameStep) {
		if (res > 0)
			result = "胜";
		else if (res == 0)
			result = "平";
		else result = "负";
	} else {
		result = ((res > 0) ? "+" : "") 
			+ res.ToString();
	}
	return result;
}

System::Void frmAnalyzer::frmAnalyzer_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
	if (analyzing) {
		stopAnalyze();
	}
}

System::Void frmAnalyzer::cbAnalyzer_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	switch (cbAnalyzer->SelectedIndex) {
	case 0:
		currentPlayerType = PlayerType::EARTH;
		break;
	case 1:
		currentPlayerType = PlayerType::CLOUDS;
		break;
	case 2:
		currentPlayerType = PlayerType::ORBIT;
		break;
	case 3:
		currentPlayerType = PlayerType::MOON;
		break;
	case 4:
		currentPlayerType = PlayerType::STAR;
		break;
	case 5:
		currentPlayerType = PlayerType::GALAXY;
		break;
	case 6:
		currentPlayerType = PlayerType::UNIVERSE;
		break;
	default:
		currentPlayerType = PlayerType::EARTH;
	}
}

String^ frmAnalyzer::getEndDescription() {
	int nBlack = 0, nWhite = 0, empties = 0;
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
			if (gc->get(i, j) == Chess::BLACK)
				nBlack++;
			else if (gc->get(i, j) == Chess::WHITE)
				nWhite++;
			else empties++;
	if (nBlack > nWhite) nBlack += empties;
	else if (nWhite > nBlack) nWhite += empties;
	String^ res = "终局比分: " + nBlack + ":" + nWhite;
	if (nBlack > nWhite) res += " 黑方胜";
	else if (nBlack == nWhite) res += " 平局";
	else res += " 白方胜";
	return res;
}

System::Void frmAnalyzer::frmAnalyzer_Load(System::Object ^sender, System::EventArgs ^e) {
	CenterToParent();
}
System::Void frmAnalyzer::btnClose_Click(System::Object ^sender, System::EventArgs ^e) {
	this->Close();
}
System::Void frmAnalyzer::btnAnalyze_Click(System::Object ^sender, System::EventArgs ^e) {
	if (!analyzing)
		startAnalyze();
	else
		stopAnalyze();
}
System::Void frmAnalyzer::frmAnalyzer_FormClosed(System::Object ^sender, System::Windows::Forms::FormClosedEventArgs ^e) {
	controller->analyzerClosed();
}
System::Void frmAnalyzer::lstResult_SelectedIndexChanged(System::Object ^sender, System::EventArgs ^e) {
	int selectedIndex = lstResult->SelectedIndex;
/*	if (analyzing && selectedIndex < previousStep - 1)
		controller->showMove(selectedIndex + 2);
	else */
	controller->showMove(selectedIndex + 1);
}
AnalyzedMove frmAnalyzer::getAnalyzedResult(int step) {
	if (step < getSteps() && step >= 1)
		return dAnalyzeResult->getAnalyzedResult(step);
	return dAnalyzeResult->getAnalyzedResult(0);
}
int frmAnalyzer::getSteps() {
	return dAnalyzeResult->getSteps();
}

System::Void frmAnalyzer::frmAnalyzer_SizeChanged(System::Object ^sender, System::EventArgs ^e) {
	tmrSetLayout->Enabled = false;
	tmrSetLayout->Enabled = true;
}
System::Void frmAnalyzer::tmrSetLayout_Tick(System::Object ^sender, System::EventArgs ^e) {
	tmrSetLayout->Enabled = false;
	if (this->WindowState != FormWindowState::Minimized)
		setLayout();
}

void frmAnalyzer::setLayout() {
	int rightLimit = this->ClientSize.Width - 12;
	pbAnalyze->Width = rightLimit - pbAnalyze->Left;
	pnlButtons->Top = this->ClientSize.Height - 12 - pnlButtons->Height;
	pnlButtons->Left = rightLimit - pnlButtons->Width;
	btnContinue->Top = pnlButtons->Top;
	lstResult->Size = System::Drawing::Size(rightLimit - lstResult->Left, 
		pnlButtons->Top - 6 - lstResult->Top);
}

System::Void frmAnalyzer::btnSave_Click(System::Object^  sender, System::EventArgs^  e) {
	saveResult();
}

void frmAnalyzer::saveResult(String^ fileName) {
	IO::FileStream^ fs;
	try {
		fs = gcnew IO::FileStream(fileName, IO::FileMode::Create, IO::FileAccess::Write);
	} catch (Exception^) {
		System::Windows::Forms::MessageBox::Show(this, "无法写入 " + fileName + " 文件。",
			"无法保存分析结果", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Stop);
		return;
	}
	IO::StreamWriter^ writer = gcnew IO::StreamWriter(fs, System::Text::Encoding::UTF8);
	try {
		String^ versionInfo = __APP_NAME__ + " " + __APP_VERSION__ + " 分析结果文件";
		String^ now = System::DateTime::Now.ToString("yyyy年M月d日 H:mm:ss",
			Globalization::CultureInfo::CreateSpecificCulture("zh-CN"));
		String^ symbolDescription = "黑棋: " + black + "  白棋: " + white + "  空格: " + av
			+ "\r\n已下棋步: " + played + "  建议棋步: " + best;

		writer->WriteLine(versionInfo);
		writer->WriteLine(now);
		writer->WriteLine(symbolDescription);
		writer->WriteLine();

		ChessBoard^ bd = gcnew ChessBoard(gc->getInitialBoard());
		Chess fp = gc->getFirstPlayer();
		String^ gameInfo = "先手方: " + (fp == Chess::BLACK ? "黑方" : "白方");
		String^ initBoard = chessBoardToString(bd);

		writer->WriteLine(gameInfo);
		writer->WriteLine("初始棋盘:\r\n" + initBoard);

		for (int i = 1; i < dAnalyzeResult->getSteps(); i++) {
			AnalyzedMove am = dAnalyzeResult->getAnalyzedResult(i);
			String^ description = am.getGeneralDescription();
			String^ board = chessBoardToString(bd, am);

			writer->WriteLine(description);
			writer->Write(board);
			writer->WriteLine();

			if (i < previousStep) {
				Othello::Move move = gc->getMove(i);
				if (move.getColor() != Chess::AVAILABLE)
					bd->putChess(move.getX(), move.getY(), move.getColor());
				else bd->recordInvalidMove(Chess::AVAILABLE);
			}
		}

	} catch (Exception^) {
		System::Windows::Forms::MessageBox::Show(this, "写入 " + fileName + " 文件出错。",
			"保存分析结果出错", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Stop);
		writer->Close();
		return;
	}
	writer->Close();
	System::Windows::Forms::MessageBox::Show(this, "分析结果已保存在：\n" + fileName,
		"分析结果已保存", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
}

void frmAnalyzer::saveResult() {
	SaveFileDialog^ dialog = gcnew SaveFileDialog();
	dialog->Title = "保存分析结果";
	dialog->Filter = "文本文档(*.txt)|*.txt";
	dialog->CheckFileExists = false;
	dialog->FileName = "";

	try {
		Windows::Forms::DialogResult res = dialog->ShowDialog(this);
		if (res == Windows::Forms::DialogResult::Cancel) {
			return;
		}
		saveResult(dialog->FileName);
	} finally {
		delete dialog;
	}
}

String^ frmAnalyzer::chessBoardToString(ChessBoard ^cb) {
	String^ const COLUMN = "  A B C D E F G H ";
	array<Char, 2>^ temp = gcnew array<Char, 2>(HEIGHT, WIDTH + 1);
	for (int r = 0; r < HEIGHT; r++) {
		temp[r, 0] = Char(r + '1');
		for (int c = 0; c < WIDTH; c++) {
			switch (cb->get(c, r)) {
			case Chess::BLACK:
				temp[r, c + 1] = black;
				break;
			case Chess::WHITE:
				temp[r, c + 1] = white;
				break;
			default:
				temp[r, c + 1] = av;
			}
		}
	}
	String^ result = COLUMN + "\r\n";
	for (int r = 0; r < HEIGHT; r++) {
		for (int c = 0; c <= WIDTH; c++) {
			result += temp[r, c] + " ";
		}
		result += "\r\n";
	}
	return result;
}

String^ frmAnalyzer::chessBoardToString(ChessBoard ^cb, AnalyzedMove move) {
	String^ const COLUMN = "  A B C D E F G H ";
	array<Char, 2>^ temp = gcnew array<Char, 2>(HEIGHT, WIDTH + 1);
	for (int r = 0; r < HEIGHT; r++) {
		temp[r, 0] = r + '1';
		for (int c = 0; c < WIDTH; c++) {
			switch (cb->get(c, r)) {
			case Chess::BLACK:
				temp[r, c + 1] = black;
				break;
			case Chess::WHITE:
				temp[r, c + 1] = white;
				break;
			default:
				temp[r, c + 1] = av;
			}
		}
	}
	if (move.getPlayer() != Chess::AVAILABLE) {
		Point p = move.getPPlayedMove();
		temp[p.Y, p.X + 1] = played;
		if (move.getQuestionLevel() != QuestionLevel::NONE) {
			p = move.getPBestMove();
			temp[p.Y, p.X + 1] = best;
		}
	}
	String^ result = COLUMN + "\r\n";
	for (int r = 0; r < HEIGHT; r++) {
		for (int c = 0; c <= WIDTH; c++) {
			result += temp[r, c] + " ";
		}
		result += "\r\n";
	}
	return result;
}

PlayerType frmAnalyzer::getAnalyzerType() {
	return currentPlayerType;
}

System::Void frmAnalyzer::btnContinue_Click(System::Object ^sender, System::EventArgs ^e) {
	int selectedIndex = lstResult->SelectedIndex;
/*	if (analyzing && selectedIndex < previousStep - 1)
		controller->showMove(selectedIndex + 2);
	else */
	if (selectedIndex >= 0)
		controller->showMove(selectedIndex + 1);
	else controller->showMove(previousStep);
	controller->startFromCurrentStep();
}
