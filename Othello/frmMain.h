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
								frmMain.h
		作者：Patrick
		概述：包含类 frmMain 的声明。
			frmMain 类实现 Craft 程序主窗体。

********************************************************************************
*/

#include "Common.h"
#include "ChessPicBox.h"
#include "Game.h"
#include "RandomPlayer.h"
#include "dlgChooseMachineType.h"
#include "Solver.h"
#include "Craft.h"
#include "BigEater.h"
#include "Executor.h"
#include "BookLearning.h"
#include "Setting.h"
#include "MemorySize.h"
#include "SearchRelated.h"
#include "frmAnalyzer.h"
#include "frmStatistics.h"
#include "WThorReader.h"
#include "frmEndGameInfo.h"
#include "frmSetCustomEmpties.h"
#include "ConfigReader.h"
#include "FlashWindow.h"

#define CONFIG_FILE_EXTENSION ".CraftCfg"

#ifdef CHRISTMAS
#include "frmWish.h"
#define CHRISTMAS_THEME "xmas"
#endif

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Media;
using namespace System::Runtime::InteropServices;

namespace Othello {

	/// <summary>
	/// Summary for frmMain
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmMain : public System::Windows::Forms::Form, public Observer, 
		public SearchDisplayer, public AnalyzeController, public StatisticsController,
		public EndGameInfoController
#ifdef CHRISTMAS
		, public WishController
#endif
	{
	private:
		static const int END_SOLVE_WARNING_DEPTH = 26;
		static const int DEFAULT_PROMPT_TIMEOUT = 3000;

		ref class GUIPlayer : public AbstractPlayer {
		private:
			frmMain^ guiForm;
			bool terminated;
			String^ name;
		public:
			GUIPlayer(frmMain^ form, String^ name) {
				guiForm = form;
				this->name = name;
			}
			virtual void init(GameContext^ gc, Chess color) {
				/*
				using System::Diagnostics::Debug;
				Debug::WriteLine("Initialized " + name + ". I am the " 
					+ (color == Chess::BLACK ? "Black" : "White") + " side.");
				Debug::WriteLine("  The opponent's name is " + gc->getOpponentName());
				if (gc->getTotalTimeLimit() == 0)
					Debug::WriteLine("  No time limit for this game.");
				else
					Debug::WriteLine("  The time limit for this game is " + gc->getTotalTimeLimit() + " ms.");
				*/
			}
			virtual int myTurn(GameContext^ gc, Othello::Move lastMove) {
				terminated = false;
				/*
				using System::Diagnostics::Debug;
				Debug::WriteLine(name + ": " + "My turn, current step : " + gc->getCurrentStep());
				Debug::WriteLine("  Available positions: " + gc->getAvailableCount());

				if (gc->getAvailableCount() != 0) {
					for (int i = 0; i < gc->getAvailableCount(); i++)
						Debug::Write(gc->getAvailableMove(i) + " ");
					Debug::WriteLine("");
				}
				*/
				guiForm->notifyUser();

				if (gc->getAvailableCount() == 0) {
					MessageBox::Show(guiForm, "这回合您欠行。", "提示", MessageBoxButtons::OK,
						MessageBoxIcon::Information);
					return 0;
				}
				guiForm->setGUIPlay(true);
				int result;
				while ((result = guiForm->getGUIPlay()) == -1) {
					Application::DoEvents();
					if (terminated) { result = 0; break; }
					System::Threading::Thread::Sleep(40);
				}
				return result;
			}
			virtual void reset() {
				terminated = true;
				/*
				using System::Diagnostics::Debug;
				Debug::WriteLine(name + ": Game reseted.");
				*/
			}
			virtual System::String^ getName() {
				return name;
			}
			virtual int getStepDelay() {
				return 0;
			}
			virtual void setStepDelay(int delay) {
			}
		};

		value class GameOption {
		public:
			PlayerType black, white;
			Chess firstPlayer;
			Board^ board;
			int totalTimeLimit;
		};

		value struct ChessOption {
			Chess color;
			bool selected;
			bool focused;
			bool available;
			bool bordered;
			bool tip;
			bool next;
		};

		value struct WThorFileInfo {
			String^ fileLocation;
			int numberOfGames;
			int yearOfGames;
		};
		
		enum class EndGameDifficulty {
			BEGINNER, AMATEUR, PROFESSIONAL, MASTER, CUSTOM
		};

		ref class ThemeMenuItem : public System::Windows::Forms::ToolStripMenuItem {
		public:
			ThemeMenuItem(System::String^ name) : themeName(name) {}
			System::String^ getThemeName() { return themeName; }
			void setThemeName(System::String^ name) { themeName = name; }
		private:
			System::String^ themeName;
		};

		GameOption defaultOption;
		bool GUIPlay;
		int GUIResult;
		Game^ game;
		Point ulCorner;
		Point lrCorner;
		AbstractPlayer^ blackPlayer;
		AbstractPlayer^ whitePlayer;
		PlayerType blackType, whiteType;
		Image^ bChessOrigin;
		Image^ wChessOrigin;
		Image^ abChessOrigin;
		Image^ awChessOrigin;
		Image^ sabChessOrigin;
		Image^ sawChessOrigin;
		Image^ bBChessOrigin;
		Image^ bWChessOrigin;
		Image^ fBChessOrigin;
		Image^ fWChessOrigin;
		Image^ nBChessOrigin;
		Image^ nWChessOrigin;
		Image^ fsbChessOrigin;
		Image^ fswChessOrigin;
		Image^ nsbChessOrigin;
		Image^ nswChessOrigin;
		Image^ iconBusy, ^iconRest;
		Image^ iconInfo, ^iconWarning, ^iconError;
		Image^ bgImage, ^cbImage;
		Image^ bdSetupBlack, ^bdSetupWhite, ^bdSetupAv;
		SoundPlayer^ startSound, ^goSound, ^winSound, ^drawSound, ^loseSound;
		UserInfo^ userInfo;
		bool canPlaySound;
		//static Point DEFAULT_ULCORNER = Point(75, 112);
		//static Point DEFAULT_LRCORNER = Point(1150, 1186);
		static Point DEFAULT_ULCORNER = Point(74, 112);
		static Point DEFAULT_LRCORNER = Point(1154, 1192);
#ifdef CHRISTMAS
		static DateTime XmasEve = DateTime(2008, 12, 24);
		static DateTime XmasEnd = DateTime(2008, 12, 31);
#endif
		static String^ executablePath = System::Windows::Forms::Application::ExecutablePath;
		static String^ appPath = executablePath->Substring(0, executablePath->LastIndexOf('\\') + 1);
		static String^ themePath = appPath + "theme\\";
		static String^ themeConfigFile = themePath + "list" + CONFIG_FILE_EXTENSION;
		static String^ startSoundLocation = themePath + "default\\sound\\start.wav";
		static String^ goSoundLocation = themePath + "default\\sound\\go.wav";
		static String^ winSoundLocation = themePath + "default\\sound\\win.wav";
		static String^ drawSoundLocation = themePath + "default\\sound\\draw.wav";
		static String^ loseSoundLocation = themePath + "default\\sound\\lose.wav";
		static String^ jouFileLocation = appPath + "thor\\WTHOR.JOU";
		static String^ trnFileLocation = appPath + "thor\\WTHOR.TRN";
		static String^ wtbFilePath = appPath + "thor\\";
		static String^ userPath = Application::LocalUserAppDataPath + "\\";
		static String^ configPath = userPath + "config.craft";
		static String^ shortKeys = 
			"操作: 键盘快捷键\n" 
			"打开 新游戏 菜单: Ctrl+N\n"
			"打开 新残局 菜单: Ctrl+M\n"
			"开始游戏: Enter 或 Space\n"
			"移动光标: ↑↓→←\n"
			"落子: Enter 或 Space\n"
			"前瞻一步: E\n"
			"悔棋: Backspace, Ctrl+Z 或 Ctrl+←\n"
			"继续: Ctrl+→\n"
			"重新开始: Ctrl+R\n"
			"打开 选择黑方 菜单: Ctrl+B\n"
			"打开 选择白方 菜单: Ctrl+W\n"
			"预设棋局: Ctrl+P\n"
			"学习: Ctrl+L\n"
			"统计信息: Ctrl+I\n"
			"提示: T\n"
			"终局求解: F\n"
			"停止搜索: P\n"
			"分析棋局: Ctrl+A\n"
			"显示残局信息: Ctrl+K\n"
			"打开/保存游戏: Ctrl+O/Ctrl+S\n"
			"复制/粘贴局面: Ctrl+C/Ctrl+V\n"
			"显示帮助: F1\n"
			"退出游戏: Ctrl+X";
		array<ChessPicBox^, 2>^ board;
		//Image::GetThumbnailImageAbort^ commonImageAbort;
		GameContext^ gcBlack;
		GameContext^ gcWhite;
		bool gameRunning, gamePaused;
		static array<String^>^ column = {"A", "B", "C", "D", "E", "F", "G", "H"};
		static array<String^>^ row = {"1", "2", "3", "4", "5", "6", "7", "8"};
		array<ChessOption, 2>^ imageBoard;
		int ssPlayersOffset;
		int ssPVOffset;
		Point selection;
		static String^ applicationName = __APP_NAME__;
		static String^ applicationVersion = __APP_VERSION__;
		static String^ helpFile = appPath + "Help.chm";
		String^ backgroundName;
		unsigned long long memorySize;
		bool fairGame, gameJustStarted;
		int goBackChance, tipChance;
		AbstractPlayer^ guiTipper;
		bool peekMode;
		bool searchState;
		System::Drawing::Point focusedMove;
		System::Drawing::Point selectedMove;
		System::Drawing::Point tipPos;
		frmAnalyzer^ analyzer;
		frmStatistics^ statisticsWindow;
		frmEndGameInfo^ endGameInfoWindow;
		bool analyzeMode, analyzing;
		bool statisticsShowing;
		bool endGameInfoShowing;
		int previousStep;
		bool previousLearnEnabled;
		bool endGameMode;
		WThorReader^ wthorReader;
		Generic::List<WThorFileInfo>^ wthorFileList;
		int totalWThorGames;
		int bestResult;
		System::Random^ rnd;
		EndGameDifficulty currentEndGameDifficulty;
		WThorGame^ currentEndGame;
		Chess currentEndGamePlayer;
		int currentEndGameEmpties;
		bool searching;
		bool tipping;
		bool continueGame;
		System::Collections::Generic::SortedList<String^, String^>^ themeList;
		System::ComponentModel::ComponentResourceManager^ resources;
		System::Collections::Generic::List<ThemeMenuItem^>^ themeMenuList;
#ifdef CHRISTMAS
		bool wishShowing;
#endif
	/// <summary>
	/// Required designer variable.
	/// </summary>
	private: System::Windows::Forms::ToolStripDropDownButton^  tsbtnBlack;
	private: System::Windows::Forms::ToolStripDropDownButton^  tsbtnNew;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripDropDownButton^  tsbtnWhite;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuManToMan;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuMachines;
	private: System::Windows::Forms::ToolStripButton^  tsbtnRestart;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuMachines;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuManToMan;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuRestart;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuBlackMan;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuBlackRandom;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuWhiteMan;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuWhiteRandom;
	private: System::Windows::Forms::ToolStripStatusLabel^  ssResult;
	private: System::Windows::Forms::ToolStripButton^  tsbtnBack;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator7;
	private: System::Windows::Forms::ToolStripButton^  tsbtnContinue;
	private: System::Windows::Forms::ToolStripProgressBar^  tspb1;
	private: System::Windows::Forms::ToolStripStatusLabel^  ssSpeed;
	private: System::Windows::Forms::ToolStripStatusLabel^  ssNodes;
	private: System::Windows::Forms::ToolStripStatusLabel^  ssSpace;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuSetting;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuTableSize;
	private: System::Windows::Forms::ToolStripMenuItem^  mnu4MB;
	private: System::Windows::Forms::ToolStripMenuItem^  mnu8MB;
	private: System::Windows::Forms::ToolStripMenuItem^  mnu16MB;
	private: System::Windows::Forms::ToolStripMenuItem^  mnu32MB;
	private: System::Windows::Forms::ToolStripMenuItem^  mnu64MB;
	private: System::Windows::Forms::ToolStripMenuItem^  mnu128MB;
	private: System::Windows::Forms::ToolStripMenuItem^  mnu256MB;
	private: System::Windows::Forms::ToolStripMenuItem^  mnu512MB;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuDelay;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuDelayClose;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuDelay05s;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuDelay1s;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuDelay2s;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuPlaySound;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuHelp;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuHelpContents;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuAbout;
	private: System::Windows::Forms::ToolStripButton^  tsbtnLearn;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator9;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuLearnImmediately;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuView;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuShowEvaluation;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuShowSpeed;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuShowProgress;
	private: System::Windows::Forms::ToolStripButton^  tsbtnSetupBoard;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::PictureBox^  picBoard;
	private: System::Windows::Forms::StatusStrip^  statusBar;
	private: System::Windows::Forms::ToolStripStatusLabel^  ssPlayers;
	private: System::Windows::Forms::ToolStrip^  toolBar;
	private: System::Windows::Forms::MenuStrip^  menuBar;
	private: System::Windows::Forms::Timer^  tmrLayout;
	private: System::Windows::Forms::Panel^  infoPanel;
	private: System::Windows::Forms::PictureBox^  picBlack;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  lblWNum;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  lblBNum;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::PictureBox^  picWhite;
	private: System::Windows::Forms::Label^  lblWCount;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  lblBCount;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  lblCStep;
	private: System::Windows::Forms::Label^  lblCurrentStep;
	private: System::Windows::Forms::PictureBox^  picWPointer;
	private: System::Windows::Forms::PictureBox^  picBPointer;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuGame;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuNew;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuExit;
	private: System::Windows::Forms::ToolStripButton^  tsbtnExit;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuControl;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuBack;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuForward;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuSetupBoard;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator8;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuLearn;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator10;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuOpenGame;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuSaveGame;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem3;
	private: System::Windows::Forms::ToolStripButton^  tsbtnOpenGame;
	private: System::Windows::Forms::ToolStripButton^  tsbtnSaveGame;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator11;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuFreeMode;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator12;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuShowStatistics;
	private: System::Windows::Forms::ToolStripButton^  tsbtnTip;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuTip;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem4;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuShortkeys;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuManBlackToMachine;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuManWhiteToMachine;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuManBlackToMachine;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuManWhiteToMachine;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem5;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuBlackBigEat;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuBlackEarth;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuBlackClouds;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuBlackOrbit;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuBlackMoon;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuBlackStar;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuBlackGalaxy;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuBlackUniverse;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem6;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuWhiteBigEat;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuWhiteEarth;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuWhiteClouds;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuWhiteOrbit;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuWhiteMoon;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuWhiteStar;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuWhiteGalaxy;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuWhiteUniverse;
	private: System::Windows::Forms::ListBox^  lstSteps;
	private: System::Windows::Forms::ToolStripButton^  tsbtnAnalyze;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuAnalyze;
	private: System::Windows::Forms::ToolStripDropDownButton^  tsmnuNewEndGame;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuEndGameBeginner;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuEndGameAmateur;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuEndGameProfessional;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuEndGameMaster;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem7;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuEndGameCustom;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuNewEndGame;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuEndGameBeginner;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuEndGameAmateur;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuEndGameProfessional;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuEndGameMaster;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator13;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuEndGameCustom;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator14;
	private: System::Windows::Forms::ToolStripButton^  tsbtnShowStatistics;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem8;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmnuShowEndGameInfo;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuShowEndGameInfo;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuForceEndSolve;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuClearCache;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem9;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuTheme;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuDefaultTheme;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem10;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator15;
	private: System::Windows::Forms::ToolStripButton^  tsbtnForceEndSolve;
	private: System::Windows::Forms::ToolStripButton^  tsbtnStopSearch;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuStopSearch;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuUseBook;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuAutoClean;
	private: System::Windows::Forms::ToolStripMenuItem^  mnu1GB;
	private: System::Windows::Forms::ToolStripMenuItem^  mnu2GB;
	private: System::Windows::Forms::Label^  lblEmpty;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::StatusStrip^  statusBar2;
	private: System::Windows::Forms::ToolStripStatusLabel^  ssPV;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuShowPV;
	private: System::Windows::Forms::ToolStripStatusLabel^  ssPrompt;
	private: System::Windows::Forms::Timer^  tmrPrompt;
private: System::Windows::Forms::ToolStripMenuItem^  mnuCopy;
private: System::Windows::Forms::ToolStripMenuItem^  mnuPaste;
	private: System::Windows::Forms::Button^  btnStart;

#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmMain::typeid));
				 this->picBoard = (gcnew System::Windows::Forms::PictureBox());
				 this->statusBar = (gcnew System::Windows::Forms::StatusStrip());
				 this->ssPlayers = (gcnew System::Windows::Forms::ToolStripStatusLabel());
				 this->ssPrompt = (gcnew System::Windows::Forms::ToolStripStatusLabel());
				 this->ssResult = (gcnew System::Windows::Forms::ToolStripStatusLabel());
				 this->ssSpeed = (gcnew System::Windows::Forms::ToolStripStatusLabel());
				 this->ssNodes = (gcnew System::Windows::Forms::ToolStripStatusLabel());
				 this->ssSpace = (gcnew System::Windows::Forms::ToolStripStatusLabel());
				 this->tspb1 = (gcnew System::Windows::Forms::ToolStripProgressBar());
				 this->toolBar = (gcnew System::Windows::Forms::ToolStrip());
				 this->tsbtnNew = (gcnew System::Windows::Forms::ToolStripDropDownButton());
				 this->tsmnuManBlackToMachine = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuManWhiteToMachine = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuManToMan = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuMachines = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuNewEndGame = (gcnew System::Windows::Forms::ToolStripDropDownButton());
				 this->tsmnuEndGameBeginner = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuEndGameAmateur = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuEndGameProfessional = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuEndGameMaster = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripMenuItem7 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsmnuEndGameCustom = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripMenuItem8 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsmnuShowEndGameInfo = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsbtnRestart = (gcnew System::Windows::Forms::ToolStripButton());
				 this->tsbtnSetupBoard = (gcnew System::Windows::Forms::ToolStripButton());
				 this->toolStripSeparator14 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsbtnOpenGame = (gcnew System::Windows::Forms::ToolStripButton());
				 this->tsbtnSaveGame = (gcnew System::Windows::Forms::ToolStripButton());
				 this->toolStripSeparator11 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsbtnBack = (gcnew System::Windows::Forms::ToolStripButton());
				 this->tsbtnContinue = (gcnew System::Windows::Forms::ToolStripButton());
				 this->tsbtnTip = (gcnew System::Windows::Forms::ToolStripButton());
				 this->tsbtnForceEndSolve = (gcnew System::Windows::Forms::ToolStripButton());
				 this->tsbtnStopSearch = (gcnew System::Windows::Forms::ToolStripButton());
				 this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsbtnBlack = (gcnew System::Windows::Forms::ToolStripDropDownButton());
				 this->tsmnuBlackMan = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripMenuItem5 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsmnuBlackRandom = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuBlackBigEat = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuBlackEarth = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsmnuBlackClouds = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuBlackOrbit = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuBlackMoon = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsmnuBlackStar = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuBlackGalaxy = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuBlackUniverse = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsbtnWhite = (gcnew System::Windows::Forms::ToolStripDropDownButton());
				 this->tsmnuWhiteMan = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripMenuItem6 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsmnuWhiteRandom = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuWhiteBigEat = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuWhiteEarth = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsmnuWhiteClouds = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuWhiteOrbit = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuWhiteMoon = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsmnuWhiteStar = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuWhiteGalaxy = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tsmnuWhiteUniverse = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator9 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsbtnShowStatistics = (gcnew System::Windows::Forms::ToolStripButton());
				 this->tsbtnAnalyze = (gcnew System::Windows::Forms::ToolStripButton());
				 this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->tsbtnLearn = (gcnew System::Windows::Forms::ToolStripButton());
				 this->tsbtnExit = (gcnew System::Windows::Forms::ToolStripButton());
				 this->menuBar = (gcnew System::Windows::Forms::MenuStrip());
				 this->mnuGame = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuNew = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuManBlackToMachine = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuManWhiteToMachine = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuManToMan = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuMachines = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuNewEndGame = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuEndGameBeginner = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuEndGameAmateur = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuEndGameProfessional = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuEndGameMaster = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator13 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->mnuEndGameCustom = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->mnuShowEndGameInfo = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuRestart = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator10 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->mnuOpenGame = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuSaveGame = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator12 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->mnuShowStatistics = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->mnuExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuControl = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuSetupBoard = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuCopy = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuPaste = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator8 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->mnuBack = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuForward = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuTip = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuForceEndSolve = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuStopSearch = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->mnuAnalyze = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuLearn = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuSetting = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuTheme = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuDefaultTheme = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripMenuItem10 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->toolStripSeparator15 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->mnuClearCache = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuTableSize = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnu4MB = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnu8MB = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnu16MB = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnu32MB = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnu64MB = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnu128MB = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnu256MB = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnu512MB = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnu1GB = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnu2GB = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuAutoClean = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripMenuItem9 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->mnuDelay = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuDelayClose = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuDelay05s = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuDelay1s = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuDelay2s = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuPlaySound = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuUseBook = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuLearnImmediately = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuFreeMode = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuView = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuShowEvaluation = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuShowSpeed = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuShowProgress = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuShowPV = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuHelpContents = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->mnuShortkeys = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->mnuAbout = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->tmrLayout = (gcnew System::Windows::Forms::Timer(this->components));
				 this->infoPanel = (gcnew System::Windows::Forms::Panel());
				 this->btnStart = (gcnew System::Windows::Forms::Button());
				 this->picWhite = (gcnew System::Windows::Forms::PictureBox());
				 this->picBPointer = (gcnew System::Windows::Forms::PictureBox());
				 this->picWPointer = (gcnew System::Windows::Forms::PictureBox());
				 this->lblEmpty = (gcnew System::Windows::Forms::Label());
				 this->lblWCount = (gcnew System::Windows::Forms::Label());
				 this->label6 = (gcnew System::Windows::Forms::Label());
				 this->lblBCount = (gcnew System::Windows::Forms::Label());
				 this->label5 = (gcnew System::Windows::Forms::Label());
				 this->lblCStep = (gcnew System::Windows::Forms::Label());
				 this->lblWNum = (gcnew System::Windows::Forms::Label());
				 this->label4 = (gcnew System::Windows::Forms::Label());
				 this->lblBNum = (gcnew System::Windows::Forms::Label());
				 this->label3 = (gcnew System::Windows::Forms::Label());
				 this->picBlack = (gcnew System::Windows::Forms::PictureBox());
				 this->lblCurrentStep = (gcnew System::Windows::Forms::Label());
				 this->label7 = (gcnew System::Windows::Forms::Label());
				 this->label2 = (gcnew System::Windows::Forms::Label());
				 this->label1 = (gcnew System::Windows::Forms::Label());
				 this->lstSteps = (gcnew System::Windows::Forms::ListBox());
				 this->statusBar2 = (gcnew System::Windows::Forms::StatusStrip());
				 this->ssPV = (gcnew System::Windows::Forms::ToolStripStatusLabel());
				 this->tmrPrompt = (gcnew System::Windows::Forms::Timer(this->components));
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picBoard))->BeginInit();
				 this->statusBar->SuspendLayout();
				 this->toolBar->SuspendLayout();
				 this->menuBar->SuspendLayout();
				 this->infoPanel->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picWhite))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picBPointer))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picWPointer))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picBlack))->BeginInit();
				 this->statusBar2->SuspendLayout();
				 this->SuspendLayout();
				 // 
				 // picBoard
				 // 
				 this->picBoard->BackColor = System::Drawing::SystemColors::Control;
				 this->picBoard->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
				 this->picBoard->Location = System::Drawing::Point(0, 64);
				 this->picBoard->Margin = System::Windows::Forms::Padding(0);
				 this->picBoard->Name = L"picBoard";
				 this->picBoard->Size = System::Drawing::Size(492, 434);
				 this->picBoard->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
				 this->picBoard->TabIndex = 3;
				 this->picBoard->TabStop = false;
				 this->picBoard->Visible = false;
				 this->picBoard->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &frmMain::backBoard_MouseMove);
				 this->picBoard->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &frmMain::backBoard_MouseClick);
				 this->picBoard->MouseHover += gcnew System::EventHandler(this, &frmMain::backBoard_MouseHover);
				 // 
				 // statusBar
				 // 
				 this->statusBar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->ssPlayers, this->ssPrompt, 
					 this->ssResult, this->ssSpeed, this->ssNodes, this->ssSpace, this->tspb1});
				 this->statusBar->Location = System::Drawing::Point(0, 519);
				 this->statusBar->Name = L"statusBar";
				 this->statusBar->RenderMode = System::Windows::Forms::ToolStripRenderMode::ManagerRenderMode;
				 this->statusBar->Size = System::Drawing::Size(659, 23);
				 this->statusBar->TabIndex = 4;
				 this->statusBar->Text = L"状态栏";
				 // 
				 // ssPlayers
				 // 
				 this->ssPlayers->AutoSize = false;
				 this->ssPlayers->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
				 this->ssPlayers->Name = L"ssPlayers";
				 this->ssPlayers->Size = System::Drawing::Size(230, 18);
				 this->ssPlayers->Text = L"LongUsername VS LongUsername";
				 this->ssPlayers->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
				 // 
				 // ssPrompt
				 // 
				 this->ssPrompt->AutoSize = false;
				 this->ssPrompt->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
				 this->ssPrompt->Name = L"ssPrompt";
				 this->ssPrompt->Size = System::Drawing::Size(0, 18);
				 this->ssPrompt->Text = L"Prompt";
				 this->ssPrompt->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
				 this->ssPrompt->Visible = false;
				 // 
				 // ssResult
				 // 
				 this->ssResult->AutoSize = false;
				 this->ssResult->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
				 this->ssResult->Name = L"ssResult";
				 this->ssResult->Size = System::Drawing::Size(108, 18);
				 this->ssResult->Text = L"+128.00 @ 18";
				 this->ssResult->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
				 // 
				 // ssSpeed
				 // 
				 this->ssSpeed->AutoSize = false;
				 this->ssSpeed->Name = L"ssSpeed";
				 this->ssSpeed->Size = System::Drawing::Size(80, 18);
				 this->ssSpeed->Text = L"99999 Kn/s";
				 this->ssSpeed->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
				 // 
				 // ssNodes
				 // 
				 this->ssNodes->AutoSize = false;
				 this->ssNodes->Name = L"ssNodes";
				 this->ssNodes->Size = System::Drawing::Size(90, 18);
				 this->ssNodes->Text = L"9999.999 Gn";
				 this->ssNodes->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
				 // 
				 // ssSpace
				 // 
				 this->ssSpace->AutoSize = false;
				 this->ssSpace->Name = L"ssSpace";
				 this->ssSpace->Size = System::Drawing::Size(15, 18);
				 // 
				 // tspb1
				 // 
				 this->tspb1->MarqueeAnimationSpeed = 50;
				 this->tspb1->Name = L"tspb1";
				 this->tspb1->Size = System::Drawing::Size(100, 17);
				 this->tspb1->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
				 // 
				 // toolBar
				 // 
				 this->toolBar->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
				 this->toolBar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(22) {this->tsbtnNew, this->tsmnuNewEndGame, 
					 this->tsbtnRestart, this->tsbtnSetupBoard, this->toolStripSeparator14, this->tsbtnOpenGame, this->tsbtnSaveGame, this->toolStripSeparator11, 
					 this->tsbtnBack, this->tsbtnContinue, this->tsbtnTip, this->tsbtnForceEndSolve, this->tsbtnStopSearch, this->toolStripSeparator7, 
					 this->tsbtnBlack, this->tsbtnWhite, this->toolStripSeparator9, this->tsbtnShowStatistics, this->tsbtnAnalyze, this->toolStripSeparator2, 
					 this->tsbtnLearn, this->tsbtnExit});
				 this->toolBar->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::HorizontalStackWithOverflow;
				 this->toolBar->Location = System::Drawing::Point(0, 25);
				 this->toolBar->Name = L"toolBar";
				 this->toolBar->RightToLeft = System::Windows::Forms::RightToLeft::No;
				 this->toolBar->Size = System::Drawing::Size(659, 39);
				 this->toolBar->TabIndex = 6;
				 this->toolBar->Text = L"工具条";
				 // 
				 // tsbtnNew
				 // 
				 this->tsbtnNew->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnNew->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->tsmnuManBlackToMachine, 
					 this->tsmnuManWhiteToMachine, this->tsmnuManToMan, this->tsmnuMachines});
				 this->tsbtnNew->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnNew.Image")));
				 this->tsbtnNew->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnNew->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnNew->Name = L"tsbtnNew";
				 this->tsbtnNew->Size = System::Drawing::Size(45, 36);
				 this->tsbtnNew->Text = L"新游戏";
				 // 
				 // tsmnuManBlackToMachine
				 // 
				 this->tsmnuManBlackToMachine->Name = L"tsmnuManBlackToMachine";
				 this->tsmnuManBlackToMachine->Size = System::Drawing::Size(144, 22);
				 this->tsmnuManBlackToMachine->Text = L"我做黑方(&B)";
				 this->tsmnuManBlackToMachine->Click += gcnew System::EventHandler(this, &frmMain::tsmnuManBlackToMachine_Click);
				 // 
				 // tsmnuManWhiteToMachine
				 // 
				 this->tsmnuManWhiteToMachine->Name = L"tsmnuManWhiteToMachine";
				 this->tsmnuManWhiteToMachine->Size = System::Drawing::Size(144, 22);
				 this->tsmnuManWhiteToMachine->Text = L"我做白方(&W)";
				 this->tsmnuManWhiteToMachine->Click += gcnew System::EventHandler(this, &frmMain::tsmnuManWhiteToMachine_Click);
				 // 
				 // tsmnuManToMan
				 // 
				 this->tsmnuManToMan->Name = L"tsmnuManToMan";
				 this->tsmnuManToMan->Size = System::Drawing::Size(144, 22);
				 this->tsmnuManToMan->Text = L"人对人(&M)";
				 this->tsmnuManToMan->Click += gcnew System::EventHandler(this, &frmMain::tsmnuManToMan_Click);
				 // 
				 // tsmnuMachines
				 // 
				 this->tsmnuMachines->Name = L"tsmnuMachines";
				 this->tsmnuMachines->Size = System::Drawing::Size(144, 22);
				 this->tsmnuMachines->Text = L"机器对战(&A)";
				 this->tsmnuMachines->Click += gcnew System::EventHandler(this, &frmMain::tsmnuMachines_Click);
				 // 
				 // tsmnuNewEndGame
				 // 
				 this->tsmnuNewEndGame->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsmnuNewEndGame->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->tsmnuEndGameBeginner, 
					 this->tsmnuEndGameAmateur, this->tsmnuEndGameProfessional, this->tsmnuEndGameMaster, this->toolStripMenuItem7, this->tsmnuEndGameCustom, 
					 this->toolStripMenuItem8, this->tsmnuShowEndGameInfo});
				 this->tsmnuNewEndGame->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsmnuNewEndGame.Image")));
				 this->tsmnuNewEndGame->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsmnuNewEndGame->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsmnuNewEndGame->Name = L"tsmnuNewEndGame";
				 this->tsmnuNewEndGame->Size = System::Drawing::Size(45, 36);
				 this->tsmnuNewEndGame->Text = L"新残局";
				 // 
				 // tsmnuEndGameBeginner
				 // 
				 this->tsmnuEndGameBeginner->Name = L"tsmnuEndGameBeginner";
				 this->tsmnuEndGameBeginner->Size = System::Drawing::Size(144, 22);
				 this->tsmnuEndGameBeginner->Text = L"初学者(&B)";
				 this->tsmnuEndGameBeginner->Click += gcnew System::EventHandler(this, &frmMain::tsmnuEndGameBeginner_Click);
				 // 
				 // tsmnuEndGameAmateur
				 // 
				 this->tsmnuEndGameAmateur->Name = L"tsmnuEndGameAmateur";
				 this->tsmnuEndGameAmateur->Size = System::Drawing::Size(144, 22);
				 this->tsmnuEndGameAmateur->Text = L"业余玩家(&A)";
				 this->tsmnuEndGameAmateur->Click += gcnew System::EventHandler(this, &frmMain::tsmnuEndGameAmateur_Click);
				 // 
				 // tsmnuEndGameProfessional
				 // 
				 this->tsmnuEndGameProfessional->Name = L"tsmnuEndGameProfessional";
				 this->tsmnuEndGameProfessional->Size = System::Drawing::Size(144, 22);
				 this->tsmnuEndGameProfessional->Text = L"职业玩家(&P)";
				 this->tsmnuEndGameProfessional->Click += gcnew System::EventHandler(this, &frmMain::tsmnuEndGameProfessional_Click);
				 // 
				 // tsmnuEndGameMaster
				 // 
				 this->tsmnuEndGameMaster->Name = L"tsmnuEndGameMaster";
				 this->tsmnuEndGameMaster->Size = System::Drawing::Size(144, 22);
				 this->tsmnuEndGameMaster->Text = L"特级大师(&M)";
				 this->tsmnuEndGameMaster->Click += gcnew System::EventHandler(this, &frmMain::tsmnuEndGameMaster_Click);
				 // 
				 // toolStripMenuItem7
				 // 
				 this->toolStripMenuItem7->Name = L"toolStripMenuItem7";
				 this->toolStripMenuItem7->Size = System::Drawing::Size(141, 6);
				 // 
				 // tsmnuEndGameCustom
				 // 
				 this->tsmnuEndGameCustom->Name = L"tsmnuEndGameCustom";
				 this->tsmnuEndGameCustom->Size = System::Drawing::Size(144, 22);
				 this->tsmnuEndGameCustom->Text = L"自定义(&C)";
				 this->tsmnuEndGameCustom->Click += gcnew System::EventHandler(this, &frmMain::tsmnuEndGameCustom_Click);
				 // 
				 // toolStripMenuItem8
				 // 
				 this->toolStripMenuItem8->Name = L"toolStripMenuItem8";
				 this->toolStripMenuItem8->Size = System::Drawing::Size(141, 6);
				 // 
				 // tsmnuShowEndGameInfo
				 // 
				 this->tsmnuShowEndGameInfo->Name = L"tsmnuShowEndGameInfo";
				 this->tsmnuShowEndGameInfo->Size = System::Drawing::Size(144, 22);
				 this->tsmnuShowEndGameInfo->Text = L"残局信息(&K)";
				 this->tsmnuShowEndGameInfo->Click += gcnew System::EventHandler(this, &frmMain::tsmnuShowEndGameInfo_Click);
				 // 
				 // tsbtnRestart
				 // 
				 this->tsbtnRestart->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnRestart->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnRestart.Image")));
				 this->tsbtnRestart->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnRestart->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnRestart->Name = L"tsbtnRestart";
				 this->tsbtnRestart->Size = System::Drawing::Size(36, 36);
				 this->tsbtnRestart->Text = L"重新开始";
				 this->tsbtnRestart->Click += gcnew System::EventHandler(this, &frmMain::tsbtnRestart_Click);
				 // 
				 // tsbtnSetupBoard
				 // 
				 this->tsbtnSetupBoard->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnSetupBoard->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnSetupBoard.Image")));
				 this->tsbtnSetupBoard->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnSetupBoard->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnSetupBoard->Name = L"tsbtnSetupBoard";
				 this->tsbtnSetupBoard->Size = System::Drawing::Size(36, 36);
				 this->tsbtnSetupBoard->Text = L"预设棋局";
				 this->tsbtnSetupBoard->ToolTipText = L"预设棋局";
				 this->tsbtnSetupBoard->Click += gcnew System::EventHandler(this, &frmMain::tsbtnSetupBoard_Click);
				 // 
				 // toolStripSeparator14
				 // 
				 this->toolStripSeparator14->Name = L"toolStripSeparator14";
				 this->toolStripSeparator14->Size = System::Drawing::Size(6, 39);
				 // 
				 // tsbtnOpenGame
				 // 
				 this->tsbtnOpenGame->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnOpenGame->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnOpenGame.Image")));
				 this->tsbtnOpenGame->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnOpenGame->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnOpenGame->Name = L"tsbtnOpenGame";
				 this->tsbtnOpenGame->Size = System::Drawing::Size(36, 36);
				 this->tsbtnOpenGame->Text = L"打开游戏";
				 this->tsbtnOpenGame->Click += gcnew System::EventHandler(this, &frmMain::tsbtnOpenGame_Click);
				 // 
				 // tsbtnSaveGame
				 // 
				 this->tsbtnSaveGame->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnSaveGame->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnSaveGame.Image")));
				 this->tsbtnSaveGame->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnSaveGame->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnSaveGame->Name = L"tsbtnSaveGame";
				 this->tsbtnSaveGame->Size = System::Drawing::Size(36, 36);
				 this->tsbtnSaveGame->Text = L"保存游戏";
				 this->tsbtnSaveGame->Click += gcnew System::EventHandler(this, &frmMain::tsbtnSaveGame_Click);
				 // 
				 // toolStripSeparator11
				 // 
				 this->toolStripSeparator11->Name = L"toolStripSeparator11";
				 this->toolStripSeparator11->Size = System::Drawing::Size(6, 39);
				 // 
				 // tsbtnBack
				 // 
				 this->tsbtnBack->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnBack->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnBack.Image")));
				 this->tsbtnBack->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnBack->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnBack->Name = L"tsbtnBack";
				 this->tsbtnBack->Size = System::Drawing::Size(36, 36);
				 this->tsbtnBack->Text = L"悔棋";
				 this->tsbtnBack->Click += gcnew System::EventHandler(this, &frmMain::tsbtnBack_Click);
				 // 
				 // tsbtnContinue
				 // 
				 this->tsbtnContinue->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnContinue->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnContinue.Image")));
				 this->tsbtnContinue->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnContinue->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnContinue->Name = L"tsbtnContinue";
				 this->tsbtnContinue->Size = System::Drawing::Size(36, 36);
				 this->tsbtnContinue->Text = L"继续";
				 this->tsbtnContinue->Click += gcnew System::EventHandler(this, &frmMain::tsbtnContinue_Click);
				 // 
				 // tsbtnTip
				 // 
				 this->tsbtnTip->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnTip->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnTip.Image")));
				 this->tsbtnTip->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnTip->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnTip->Name = L"tsbtnTip";
				 this->tsbtnTip->Size = System::Drawing::Size(36, 36);
				 this->tsbtnTip->Text = L"提示";
				 this->tsbtnTip->Click += gcnew System::EventHandler(this, &frmMain::tsbtnTip_Click);
				 // 
				 // tsbtnForceEndSolve
				 // 
				 this->tsbtnForceEndSolve->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnForceEndSolve->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnForceEndSolve.Image")));
				 this->tsbtnForceEndSolve->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnForceEndSolve->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnForceEndSolve->Name = L"tsbtnForceEndSolve";
				 this->tsbtnForceEndSolve->Size = System::Drawing::Size(36, 36);
				 this->tsbtnForceEndSolve->Text = L"终局求解";
				 this->tsbtnForceEndSolve->Click += gcnew System::EventHandler(this, &frmMain::tsbtnForceEndSolve_Click);
				 // 
				 // tsbtnStopSearch
				 // 
				 this->tsbtnStopSearch->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnStopSearch->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnStopSearch.Image")));
				 this->tsbtnStopSearch->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnStopSearch->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnStopSearch->Name = L"tsbtnStopSearch";
				 this->tsbtnStopSearch->Size = System::Drawing::Size(36, 36);
				 this->tsbtnStopSearch->Text = L"停止搜索";
				 this->tsbtnStopSearch->Click += gcnew System::EventHandler(this, &frmMain::tsbtnStopSearch_Click);
				 // 
				 // toolStripSeparator7
				 // 
				 this->toolStripSeparator7->Name = L"toolStripSeparator7";
				 this->toolStripSeparator7->Size = System::Drawing::Size(6, 39);
				 // 
				 // tsbtnBlack
				 // 
				 this->tsbtnBlack->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnBlack->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(13) {this->tsmnuBlackMan, 
					 this->toolStripMenuItem5, this->tsmnuBlackRandom, this->tsmnuBlackBigEat, this->tsmnuBlackEarth, this->toolStripSeparator3, this->tsmnuBlackClouds, 
					 this->tsmnuBlackOrbit, this->tsmnuBlackMoon, this->toolStripSeparator4, this->tsmnuBlackStar, this->tsmnuBlackGalaxy, this->tsmnuBlackUniverse});
				 this->tsbtnBlack->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnBlack.Image")));
				 this->tsbtnBlack->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnBlack->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnBlack->Name = L"tsbtnBlack";
				 this->tsbtnBlack->Size = System::Drawing::Size(45, 36);
				 this->tsbtnBlack->Text = L"选择黑方";
				 // 
				 // tsmnuBlackMan
				 // 
				 this->tsmnuBlackMan->Name = L"tsmnuBlackMan";
				 this->tsmnuBlackMan->Size = System::Drawing::Size(173, 22);
				 this->tsmnuBlackMan->Text = L"人类玩家(&Human)";
				 this->tsmnuBlackMan->Click += gcnew System::EventHandler(this, &frmMain::tsmnuBlackMan_Click);
				 // 
				 // toolStripMenuItem5
				 // 
				 this->toolStripMenuItem5->Name = L"toolStripMenuItem5";
				 this->toolStripMenuItem5->Size = System::Drawing::Size(170, 6);
				 // 
				 // tsmnuBlackRandom
				 // 
				 this->tsmnuBlackRandom->Name = L"tsmnuBlackRandom";
				 this->tsmnuBlackRandom->Size = System::Drawing::Size(173, 22);
				 this->tsmnuBlackRandom->Text = L"随机(&Random)";
				 this->tsmnuBlackRandom->Click += gcnew System::EventHandler(this, &frmMain::tsmnuBlackRandom_Click);
				 // 
				 // tsmnuBlackBigEat
				 // 
				 this->tsmnuBlackBigEat->Name = L"tsmnuBlackBigEat";
				 this->tsmnuBlackBigEat->Size = System::Drawing::Size(173, 22);
				 this->tsmnuBlackBigEat->Text = L"大食(&Big Eat)";
				 this->tsmnuBlackBigEat->Click += gcnew System::EventHandler(this, &frmMain::tsmnuBlackBigEat_Click);
				 // 
				 // tsmnuBlackEarth
				 // 
				 this->tsmnuBlackEarth->Name = L"tsmnuBlackEarth";
				 this->tsmnuBlackEarth->Size = System::Drawing::Size(173, 22);
				 this->tsmnuBlackEarth->Text = L"地球(&Earth)";
				 this->tsmnuBlackEarth->Click += gcnew System::EventHandler(this, &frmMain::tsmnuBlackEarth_Click);
				 // 
				 // toolStripSeparator3
				 // 
				 this->toolStripSeparator3->Name = L"toolStripSeparator3";
				 this->toolStripSeparator3->Size = System::Drawing::Size(170, 6);
				 // 
				 // tsmnuBlackClouds
				 // 
				 this->tsmnuBlackClouds->Name = L"tsmnuBlackClouds";
				 this->tsmnuBlackClouds->Size = System::Drawing::Size(173, 22);
				 this->tsmnuBlackClouds->Text = L"云朵(&Clouds)";
				 this->tsmnuBlackClouds->Click += gcnew System::EventHandler(this, &frmMain::tsmnuBlackClouds_Click);
				 // 
				 // tsmnuBlackOrbit
				 // 
				 this->tsmnuBlackOrbit->Name = L"tsmnuBlackOrbit";
				 this->tsmnuBlackOrbit->Size = System::Drawing::Size(173, 22);
				 this->tsmnuBlackOrbit->Text = L"轨道(&Orbit)";
				 this->tsmnuBlackOrbit->Click += gcnew System::EventHandler(this, &frmMain::tsmnuBlackOrbit_Click);
				 // 
				 // tsmnuBlackMoon
				 // 
				 this->tsmnuBlackMoon->Name = L"tsmnuBlackMoon";
				 this->tsmnuBlackMoon->Size = System::Drawing::Size(173, 22);
				 this->tsmnuBlackMoon->Text = L"月亮(&Moon)";
				 this->tsmnuBlackMoon->Click += gcnew System::EventHandler(this, &frmMain::tsmnuBlackMoon_Click);
				 // 
				 // toolStripSeparator4
				 // 
				 this->toolStripSeparator4->Name = L"toolStripSeparator4";
				 this->toolStripSeparator4->Size = System::Drawing::Size(170, 6);
				 // 
				 // tsmnuBlackStar
				 // 
				 this->tsmnuBlackStar->Name = L"tsmnuBlackStar";
				 this->tsmnuBlackStar->Size = System::Drawing::Size(173, 22);
				 this->tsmnuBlackStar->Text = L"星星(&Star)";
				 this->tsmnuBlackStar->Click += gcnew System::EventHandler(this, &frmMain::tsmnuBlackStar_Click);
				 // 
				 // tsmnuBlackGalaxy
				 // 
				 this->tsmnuBlackGalaxy->Name = L"tsmnuBlackGalaxy";
				 this->tsmnuBlackGalaxy->Size = System::Drawing::Size(173, 22);
				 this->tsmnuBlackGalaxy->Text = L"星系(&Galaxy)";
				 this->tsmnuBlackGalaxy->Click += gcnew System::EventHandler(this, &frmMain::tsmnuBlackGalaxy_Click);
				 // 
				 // tsmnuBlackUniverse
				 // 
				 this->tsmnuBlackUniverse->Name = L"tsmnuBlackUniverse";
				 this->tsmnuBlackUniverse->Size = System::Drawing::Size(173, 22);
				 this->tsmnuBlackUniverse->Text = L"宇宙(&Universe)";
				 this->tsmnuBlackUniverse->Click += gcnew System::EventHandler(this, &frmMain::tsmnuBlackUniverse_Click);
				 // 
				 // tsbtnWhite
				 // 
				 this->tsbtnWhite->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnWhite->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(13) {this->tsmnuWhiteMan, 
					 this->toolStripMenuItem6, this->tsmnuWhiteRandom, this->tsmnuWhiteBigEat, this->tsmnuWhiteEarth, this->toolStripSeparator5, this->tsmnuWhiteClouds, 
					 this->tsmnuWhiteOrbit, this->tsmnuWhiteMoon, this->toolStripSeparator6, this->tsmnuWhiteStar, this->tsmnuWhiteGalaxy, this->tsmnuWhiteUniverse});
				 this->tsbtnWhite->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnWhite.Image")));
				 this->tsbtnWhite->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnWhite->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnWhite->Name = L"tsbtnWhite";
				 this->tsbtnWhite->Size = System::Drawing::Size(45, 36);
				 this->tsbtnWhite->Text = L"选择白方";
				 // 
				 // tsmnuWhiteMan
				 // 
				 this->tsmnuWhiteMan->Name = L"tsmnuWhiteMan";
				 this->tsmnuWhiteMan->Size = System::Drawing::Size(173, 22);
				 this->tsmnuWhiteMan->Text = L"人类玩家(&Human)";
				 this->tsmnuWhiteMan->Click += gcnew System::EventHandler(this, &frmMain::tsmnuWhiteMan_Click);
				 // 
				 // toolStripMenuItem6
				 // 
				 this->toolStripMenuItem6->Name = L"toolStripMenuItem6";
				 this->toolStripMenuItem6->Size = System::Drawing::Size(170, 6);
				 // 
				 // tsmnuWhiteRandom
				 // 
				 this->tsmnuWhiteRandom->Name = L"tsmnuWhiteRandom";
				 this->tsmnuWhiteRandom->Size = System::Drawing::Size(173, 22);
				 this->tsmnuWhiteRandom->Text = L"随机(&Random)";
				 this->tsmnuWhiteRandom->Click += gcnew System::EventHandler(this, &frmMain::tsmnuWhiteRandom_Click);
				 // 
				 // tsmnuWhiteBigEat
				 // 
				 this->tsmnuWhiteBigEat->Name = L"tsmnuWhiteBigEat";
				 this->tsmnuWhiteBigEat->Size = System::Drawing::Size(173, 22);
				 this->tsmnuWhiteBigEat->Text = L"大食(&Big Eat)";
				 this->tsmnuWhiteBigEat->Click += gcnew System::EventHandler(this, &frmMain::tsmnuWhiteBigEat_Click);
				 // 
				 // tsmnuWhiteEarth
				 // 
				 this->tsmnuWhiteEarth->Name = L"tsmnuWhiteEarth";
				 this->tsmnuWhiteEarth->Size = System::Drawing::Size(173, 22);
				 this->tsmnuWhiteEarth->Text = L"地球(&Earth)";
				 this->tsmnuWhiteEarth->Click += gcnew System::EventHandler(this, &frmMain::tsmnuWhiteEarth_Click);
				 // 
				 // toolStripSeparator5
				 // 
				 this->toolStripSeparator5->Name = L"toolStripSeparator5";
				 this->toolStripSeparator5->Size = System::Drawing::Size(170, 6);
				 // 
				 // tsmnuWhiteClouds
				 // 
				 this->tsmnuWhiteClouds->Name = L"tsmnuWhiteClouds";
				 this->tsmnuWhiteClouds->Size = System::Drawing::Size(173, 22);
				 this->tsmnuWhiteClouds->Text = L"云朵(&Clouds)";
				 this->tsmnuWhiteClouds->Click += gcnew System::EventHandler(this, &frmMain::tsmnuWhiteClouds_Click);
				 // 
				 // tsmnuWhiteOrbit
				 // 
				 this->tsmnuWhiteOrbit->Name = L"tsmnuWhiteOrbit";
				 this->tsmnuWhiteOrbit->Size = System::Drawing::Size(173, 22);
				 this->tsmnuWhiteOrbit->Text = L"轨道(&Orbit)";
				 this->tsmnuWhiteOrbit->Click += gcnew System::EventHandler(this, &frmMain::tsmnuWhiteOrbit_Click);
				 // 
				 // tsmnuWhiteMoon
				 // 
				 this->tsmnuWhiteMoon->Name = L"tsmnuWhiteMoon";
				 this->tsmnuWhiteMoon->Size = System::Drawing::Size(173, 22);
				 this->tsmnuWhiteMoon->Text = L"月亮(&Moon)";
				 this->tsmnuWhiteMoon->Click += gcnew System::EventHandler(this, &frmMain::tsmnuWhiteMoon_Click);
				 // 
				 // toolStripSeparator6
				 // 
				 this->toolStripSeparator6->Name = L"toolStripSeparator6";
				 this->toolStripSeparator6->Size = System::Drawing::Size(170, 6);
				 // 
				 // tsmnuWhiteStar
				 // 
				 this->tsmnuWhiteStar->Name = L"tsmnuWhiteStar";
				 this->tsmnuWhiteStar->Size = System::Drawing::Size(173, 22);
				 this->tsmnuWhiteStar->Text = L"星星(&Star)";
				 this->tsmnuWhiteStar->Click += gcnew System::EventHandler(this, &frmMain::tsmnuWhiteStar_Click);
				 // 
				 // tsmnuWhiteGalaxy
				 // 
				 this->tsmnuWhiteGalaxy->Name = L"tsmnuWhiteGalaxy";
				 this->tsmnuWhiteGalaxy->Size = System::Drawing::Size(173, 22);
				 this->tsmnuWhiteGalaxy->Text = L"星系(&Galaxy)";
				 this->tsmnuWhiteGalaxy->Click += gcnew System::EventHandler(this, &frmMain::tsmnuWhiteGalaxy_Click);
				 // 
				 // tsmnuWhiteUniverse
				 // 
				 this->tsmnuWhiteUniverse->Name = L"tsmnuWhiteUniverse";
				 this->tsmnuWhiteUniverse->Size = System::Drawing::Size(173, 22);
				 this->tsmnuWhiteUniverse->Text = L"宇宙(&Universe)";
				 this->tsmnuWhiteUniverse->Click += gcnew System::EventHandler(this, &frmMain::tsmnuWhiteUniverse_Click);
				 // 
				 // toolStripSeparator9
				 // 
				 this->toolStripSeparator9->Name = L"toolStripSeparator9";
				 this->toolStripSeparator9->Size = System::Drawing::Size(6, 39);
				 // 
				 // tsbtnShowStatistics
				 // 
				 this->tsbtnShowStatistics->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnShowStatistics->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnShowStatistics.Image")));
				 this->tsbtnShowStatistics->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnShowStatistics->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnShowStatistics->Name = L"tsbtnShowStatistics";
				 this->tsbtnShowStatistics->Size = System::Drawing::Size(36, 36);
				 this->tsbtnShowStatistics->Text = L"统计信息";
				 this->tsbtnShowStatistics->Click += gcnew System::EventHandler(this, &frmMain::tsbtnShowStatistics_Click);
				 // 
				 // tsbtnAnalyze
				 // 
				 this->tsbtnAnalyze->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnAnalyze->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnAnalyze.Image")));
				 this->tsbtnAnalyze->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnAnalyze->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnAnalyze->Name = L"tsbtnAnalyze";
				 this->tsbtnAnalyze->Size = System::Drawing::Size(36, 36);
				 this->tsbtnAnalyze->Text = L"分析";
				 this->tsbtnAnalyze->Click += gcnew System::EventHandler(this, &frmMain::tsbtnAnalyze_Click);
				 // 
				 // toolStripSeparator2
				 // 
				 this->toolStripSeparator2->Name = L"toolStripSeparator2";
				 this->toolStripSeparator2->Size = System::Drawing::Size(6, 39);
				 // 
				 // tsbtnLearn
				 // 
				 this->tsbtnLearn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnLearn->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnLearn.Image")));
				 this->tsbtnLearn->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnLearn->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnLearn->Name = L"tsbtnLearn";
				 this->tsbtnLearn->Size = System::Drawing::Size(36, 36);
				 this->tsbtnLearn->Text = L"学习";
				 this->tsbtnLearn->Visible = false;
				 this->tsbtnLearn->Click += gcnew System::EventHandler(this, &frmMain::tsbtnLearn_Click);
				 // 
				 // tsbtnExit
				 // 
				 this->tsbtnExit->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->tsbtnExit->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbtnExit.Image")));
				 this->tsbtnExit->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
				 this->tsbtnExit->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->tsbtnExit->Name = L"tsbtnExit";
				 this->tsbtnExit->Size = System::Drawing::Size(36, 36);
				 this->tsbtnExit->Text = L"退出";
				 this->tsbtnExit->Click += gcnew System::EventHandler(this, &frmMain::tsbtnExit_Click);
				 // 
				 // menuBar
				 // 
				 this->menuBar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->mnuGame, this->mnuControl, 
					 this->mnuSetting, this->mnuView, this->mnuHelp});
				 this->menuBar->Location = System::Drawing::Point(0, 0);
				 this->menuBar->Name = L"menuBar";
				 this->menuBar->Size = System::Drawing::Size(659, 25);
				 this->menuBar->TabIndex = 7;
				 this->menuBar->Text = L"主菜单";
				 // 
				 // mnuGame
				 // 
				 this->mnuGame->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->mnuNew, this->mnuNewEndGame, 
					 this->mnuRestart, this->toolStripSeparator10, this->mnuOpenGame, this->mnuSaveGame, this->toolStripSeparator12, this->mnuShowStatistics, 
					 this->toolStripMenuItem2, this->mnuExit});
				 this->mnuGame->Name = L"mnuGame";
				 this->mnuGame->ShortcutKeyDisplayString = L"";
				 this->mnuGame->Size = System::Drawing::Size(61, 21);
				 this->mnuGame->Text = L"游戏(&G)";
				 // 
				 // mnuNew
				 // 
				 this->mnuNew->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->mnuManBlackToMachine, 
					 this->mnuManWhiteToMachine, this->mnuManToMan, this->mnuMachines});
				 this->mnuNew->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuNew.Image")));
				 this->mnuNew->Name = L"mnuNew";
				 this->mnuNew->Size = System::Drawing::Size(142, 22);
				 this->mnuNew->Text = L"新游戏(&N)";
				 // 
				 // mnuManBlackToMachine
				 // 
				 this->mnuManBlackToMachine->Name = L"mnuManBlackToMachine";
				 this->mnuManBlackToMachine->Size = System::Drawing::Size(144, 22);
				 this->mnuManBlackToMachine->Text = L"我做黑方(&B)";
				 this->mnuManBlackToMachine->Click += gcnew System::EventHandler(this, &frmMain::tsmnuManBlackToMachine_Click);
				 // 
				 // mnuManWhiteToMachine
				 // 
				 this->mnuManWhiteToMachine->Name = L"mnuManWhiteToMachine";
				 this->mnuManWhiteToMachine->Size = System::Drawing::Size(144, 22);
				 this->mnuManWhiteToMachine->Text = L"我做白方(&W)";
				 this->mnuManWhiteToMachine->Click += gcnew System::EventHandler(this, &frmMain::tsmnuManWhiteToMachine_Click);
				 // 
				 // mnuManToMan
				 // 
				 this->mnuManToMan->Name = L"mnuManToMan";
				 this->mnuManToMan->Size = System::Drawing::Size(144, 22);
				 this->mnuManToMan->Text = L"人对人(&M)";
				 this->mnuManToMan->Click += gcnew System::EventHandler(this, &frmMain::mnuManToMan_Click);
				 // 
				 // mnuMachines
				 // 
				 this->mnuMachines->Name = L"mnuMachines";
				 this->mnuMachines->Size = System::Drawing::Size(144, 22);
				 this->mnuMachines->Text = L"机器对战(&A)";
				 this->mnuMachines->Click += gcnew System::EventHandler(this, &frmMain::mnuMachines_Click);
				 // 
				 // mnuNewEndGame
				 // 
				 this->mnuNewEndGame->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->mnuEndGameBeginner, 
					 this->mnuEndGameAmateur, this->mnuEndGameProfessional, this->mnuEndGameMaster, this->toolStripSeparator13, this->mnuEndGameCustom, 
					 this->toolStripSeparator1, this->mnuShowEndGameInfo});
				 this->mnuNewEndGame->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuNewEndGame.Image")));
				 this->mnuNewEndGame->Name = L"mnuNewEndGame";
				 this->mnuNewEndGame->Size = System::Drawing::Size(142, 22);
				 this->mnuNewEndGame->Text = L"新残局(&M)";
				 // 
				 // mnuEndGameBeginner
				 // 
				 this->mnuEndGameBeginner->Name = L"mnuEndGameBeginner";
				 this->mnuEndGameBeginner->Size = System::Drawing::Size(144, 22);
				 this->mnuEndGameBeginner->Text = L"初学者(&B)";
				 this->mnuEndGameBeginner->Click += gcnew System::EventHandler(this, &frmMain::mnuEndGameBeginner_Click);
				 // 
				 // mnuEndGameAmateur
				 // 
				 this->mnuEndGameAmateur->Name = L"mnuEndGameAmateur";
				 this->mnuEndGameAmateur->Size = System::Drawing::Size(144, 22);
				 this->mnuEndGameAmateur->Text = L"业余玩家(&A)";
				 this->mnuEndGameAmateur->Click += gcnew System::EventHandler(this, &frmMain::mnuEndGameAmateur_Click);
				 // 
				 // mnuEndGameProfessional
				 // 
				 this->mnuEndGameProfessional->Name = L"mnuEndGameProfessional";
				 this->mnuEndGameProfessional->Size = System::Drawing::Size(144, 22);
				 this->mnuEndGameProfessional->Text = L"职业玩家(&P)";
				 this->mnuEndGameProfessional->Click += gcnew System::EventHandler(this, &frmMain::mnuEndGameProfessional_Click);
				 // 
				 // mnuEndGameMaster
				 // 
				 this->mnuEndGameMaster->Name = L"mnuEndGameMaster";
				 this->mnuEndGameMaster->Size = System::Drawing::Size(144, 22);
				 this->mnuEndGameMaster->Text = L"特级大师(&M)";
				 this->mnuEndGameMaster->Click += gcnew System::EventHandler(this, &frmMain::mnuEndGameMaster_Click);
				 // 
				 // toolStripSeparator13
				 // 
				 this->toolStripSeparator13->Name = L"toolStripSeparator13";
				 this->toolStripSeparator13->Size = System::Drawing::Size(141, 6);
				 // 
				 // mnuEndGameCustom
				 // 
				 this->mnuEndGameCustom->Name = L"mnuEndGameCustom";
				 this->mnuEndGameCustom->Size = System::Drawing::Size(144, 22);
				 this->mnuEndGameCustom->Text = L"自定义(&C)";
				 this->mnuEndGameCustom->Click += gcnew System::EventHandler(this, &frmMain::mnuEndGameCustom_Click);
				 // 
				 // toolStripSeparator1
				 // 
				 this->toolStripSeparator1->Name = L"toolStripSeparator1";
				 this->toolStripSeparator1->Size = System::Drawing::Size(141, 6);
				 // 
				 // mnuShowEndGameInfo
				 // 
				 this->mnuShowEndGameInfo->Name = L"mnuShowEndGameInfo";
				 this->mnuShowEndGameInfo->Size = System::Drawing::Size(144, 22);
				 this->mnuShowEndGameInfo->Text = L"残局信息(&K)";
				 this->mnuShowEndGameInfo->Click += gcnew System::EventHandler(this, &frmMain::mnuShowEndGameInfo_Click);
				 // 
				 // mnuRestart
				 // 
				 this->mnuRestart->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuRestart.Image")));
				 this->mnuRestart->Name = L"mnuRestart";
				 this->mnuRestart->Size = System::Drawing::Size(142, 22);
				 this->mnuRestart->Text = L"重新开始(&R)";
				 this->mnuRestart->Click += gcnew System::EventHandler(this, &frmMain::mnuRestart_Click);
				 // 
				 // toolStripSeparator10
				 // 
				 this->toolStripSeparator10->Name = L"toolStripSeparator10";
				 this->toolStripSeparator10->Size = System::Drawing::Size(139, 6);
				 // 
				 // mnuOpenGame
				 // 
				 this->mnuOpenGame->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuOpenGame.Image")));
				 this->mnuOpenGame->Name = L"mnuOpenGame";
				 this->mnuOpenGame->Size = System::Drawing::Size(142, 22);
				 this->mnuOpenGame->Text = L"打开游戏(&O)";
				 this->mnuOpenGame->Click += gcnew System::EventHandler(this, &frmMain::mnuOpenGame_Click);
				 // 
				 // mnuSaveGame
				 // 
				 this->mnuSaveGame->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuSaveGame.Image")));
				 this->mnuSaveGame->Name = L"mnuSaveGame";
				 this->mnuSaveGame->Size = System::Drawing::Size(142, 22);
				 this->mnuSaveGame->Text = L"保存游戏(&S)";
				 this->mnuSaveGame->Click += gcnew System::EventHandler(this, &frmMain::mnuSaveGame_Click);
				 // 
				 // toolStripSeparator12
				 // 
				 this->toolStripSeparator12->Name = L"toolStripSeparator12";
				 this->toolStripSeparator12->Size = System::Drawing::Size(139, 6);
				 // 
				 // mnuShowStatistics
				 // 
				 this->mnuShowStatistics->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuShowStatistics.Image")));
				 this->mnuShowStatistics->Name = L"mnuShowStatistics";
				 this->mnuShowStatistics->Size = System::Drawing::Size(142, 22);
				 this->mnuShowStatistics->Text = L"统计信息(&I)";
				 this->mnuShowStatistics->Click += gcnew System::EventHandler(this, &frmMain::mnuShowStatistics_Click);
				 // 
				 // toolStripMenuItem2
				 // 
				 this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
				 this->toolStripMenuItem2->Size = System::Drawing::Size(139, 6);
				 // 
				 // mnuExit
				 // 
				 this->mnuExit->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuExit.Image")));
				 this->mnuExit->Name = L"mnuExit";
				 this->mnuExit->Size = System::Drawing::Size(142, 22);
				 this->mnuExit->Text = L"退出(&X)";
				 this->mnuExit->Click += gcnew System::EventHandler(this, &frmMain::mnuExit_Click);
				 // 
				 // mnuControl
				 // 
				 this->mnuControl->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {this->mnuSetupBoard, 
					 this->mnuCopy, this->mnuPaste, this->toolStripSeparator8, this->mnuBack, this->mnuForward, this->mnuTip, this->mnuForceEndSolve, 
					 this->mnuStopSearch, this->toolStripMenuItem1, this->mnuAnalyze, this->mnuLearn});
				 this->mnuControl->Name = L"mnuControl";
				 this->mnuControl->Size = System::Drawing::Size(60, 21);
				 this->mnuControl->Text = L"控制(&C)";
				 // 
				 // mnuSetupBoard
				 // 
				 this->mnuSetupBoard->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuSetupBoard.Image")));
				 this->mnuSetupBoard->Name = L"mnuSetupBoard";
				 this->mnuSetupBoard->Size = System::Drawing::Size(140, 22);
				 this->mnuSetupBoard->Text = L"预设棋局(&S)";
				 this->mnuSetupBoard->Click += gcnew System::EventHandler(this, &frmMain::mnuSetupBoard_Click);
				 // 
				 // mnuCopy
				 // 
				 this->mnuCopy->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuCopy.Image")));
				 this->mnuCopy->Name = L"mnuCopy";
				 this->mnuCopy->Size = System::Drawing::Size(140, 22);
				 this->mnuCopy->Text = L"复制局面(&C)";
				 this->mnuCopy->Click += gcnew System::EventHandler(this, &frmMain::mnuCopy_Click);
				 // 
				 // mnuPaste
				 // 
				 this->mnuPaste->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuPaste.Image")));
				 this->mnuPaste->Name = L"mnuPaste";
				 this->mnuPaste->Size = System::Drawing::Size(140, 22);
				 this->mnuPaste->Text = L"粘贴局面(&V)";
				 this->mnuPaste->Click += gcnew System::EventHandler(this, &frmMain::mnuPaste_Click);
				 // 
				 // toolStripSeparator8
				 // 
				 this->toolStripSeparator8->Name = L"toolStripSeparator8";
				 this->toolStripSeparator8->Size = System::Drawing::Size(137, 6);
				 // 
				 // mnuBack
				 // 
				 this->mnuBack->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuBack.Image")));
				 this->mnuBack->Name = L"mnuBack";
				 this->mnuBack->Size = System::Drawing::Size(140, 22);
				 this->mnuBack->Text = L"悔棋(&B)";
				 this->mnuBack->Click += gcnew System::EventHandler(this, &frmMain::mnuBack_Click);
				 // 
				 // mnuForward
				 // 
				 this->mnuForward->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuForward.Image")));
				 this->mnuForward->Name = L"mnuForward";
				 this->mnuForward->Size = System::Drawing::Size(140, 22);
				 this->mnuForward->Text = L"继续(&F)";
				 this->mnuForward->Click += gcnew System::EventHandler(this, &frmMain::mnuForward_Click);
				 // 
				 // mnuTip
				 // 
				 this->mnuTip->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuTip.Image")));
				 this->mnuTip->Name = L"mnuTip";
				 this->mnuTip->Size = System::Drawing::Size(140, 22);
				 this->mnuTip->Text = L"提示(&T)";
				 this->mnuTip->Click += gcnew System::EventHandler(this, &frmMain::mnuTip_Click);
				 // 
				 // mnuForceEndSolve
				 // 
				 this->mnuForceEndSolve->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuForceEndSolve.Image")));
				 this->mnuForceEndSolve->Name = L"mnuForceEndSolve";
				 this->mnuForceEndSolve->Size = System::Drawing::Size(140, 22);
				 this->mnuForceEndSolve->Text = L"终局求解(&E)";
				 this->mnuForceEndSolve->Click += gcnew System::EventHandler(this, &frmMain::mnuForceEndSolve_Click);
				 // 
				 // mnuStopSearch
				 // 
				 this->mnuStopSearch->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuStopSearch.Image")));
				 this->mnuStopSearch->Name = L"mnuStopSearch";
				 this->mnuStopSearch->Size = System::Drawing::Size(140, 22);
				 this->mnuStopSearch->Text = L"停止搜索(&P)";
				 this->mnuStopSearch->Click += gcnew System::EventHandler(this, &frmMain::mnuStopSearch_Click);
				 // 
				 // toolStripMenuItem1
				 // 
				 this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
				 this->toolStripMenuItem1->Size = System::Drawing::Size(137, 6);
				 // 
				 // mnuAnalyze
				 // 
				 this->mnuAnalyze->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuAnalyze.Image")));
				 this->mnuAnalyze->Name = L"mnuAnalyze";
				 this->mnuAnalyze->Size = System::Drawing::Size(140, 22);
				 this->mnuAnalyze->Text = L"分析(&A)";
				 this->mnuAnalyze->Click += gcnew System::EventHandler(this, &frmMain::mnuAnalyze_Click);
				 // 
				 // mnuLearn
				 // 
				 this->mnuLearn->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"mnuLearn.Image")));
				 this->mnuLearn->Name = L"mnuLearn";
				 this->mnuLearn->Size = System::Drawing::Size(140, 22);
				 this->mnuLearn->Text = L"学习(&L)";
				 this->mnuLearn->Click += gcnew System::EventHandler(this, &frmMain::mnuLearn_Click);
				 // 
				 // mnuSetting
				 // 
				 this->mnuSetting->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(11) {this->mnuTheme, 
					 this->toolStripSeparator15, this->mnuClearCache, this->mnuTableSize, this->mnuAutoClean, this->toolStripMenuItem9, this->mnuDelay, 
					 this->mnuPlaySound, this->mnuUseBook, this->mnuLearnImmediately, this->mnuFreeMode});
				 this->mnuSetting->Name = L"mnuSetting";
				 this->mnuSetting->Size = System::Drawing::Size(59, 21);
				 this->mnuSetting->Text = L"设置(&S)";
				 // 
				 // mnuTheme
				 // 
				 this->mnuTheme->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->mnuDefaultTheme, 
					 this->toolStripMenuItem10});
				 this->mnuTheme->Name = L"mnuTheme";
				 this->mnuTheme->Size = System::Drawing::Size(152, 22);
				 this->mnuTheme->Text = L"主题(&T)";
				 // 
				 // mnuDefaultTheme
				 // 
				 this->mnuDefaultTheme->Name = L"mnuDefaultTheme";
				 this->mnuDefaultTheme->Size = System::Drawing::Size(141, 22);
				 this->mnuDefaultTheme->Text = L"经典主题(&D)";
				 this->mnuDefaultTheme->Click += gcnew System::EventHandler(this, &frmMain::mnuDefaultTheme_Click);
				 // 
				 // toolStripMenuItem10
				 // 
				 this->toolStripMenuItem10->Name = L"toolStripMenuItem10";
				 this->toolStripMenuItem10->Size = System::Drawing::Size(138, 6);
				 // 
				 // toolStripSeparator15
				 // 
				 this->toolStripSeparator15->Name = L"toolStripSeparator15";
				 this->toolStripSeparator15->Size = System::Drawing::Size(149, 6);
				 // 
				 // mnuClearCache
				 // 
				 this->mnuClearCache->Name = L"mnuClearCache";
				 this->mnuClearCache->Size = System::Drawing::Size(152, 22);
				 this->mnuClearCache->Text = L"清空置换表(&C)";
				 this->mnuClearCache->Click += gcnew System::EventHandler(this, &frmMain::mnuClearCache_Click);
				 // 
				 // mnuTableSize
				 // 
				 this->mnuTableSize->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->mnu4MB, 
					 this->mnu8MB, this->mnu16MB, this->mnu32MB, this->mnu64MB, this->mnu128MB, this->mnu256MB, this->mnu512MB, this->mnu1GB, this->mnu2GB});
				 this->mnuTableSize->Name = L"mnuTableSize";
				 this->mnuTableSize->Size = System::Drawing::Size(152, 22);
				 this->mnuTableSize->Text = L"置换表大小(&Z)";
				 // 
				 // mnu4MB
				 // 
				 this->mnu4MB->Name = L"mnu4MB";
				 this->mnu4MB->Size = System::Drawing::Size(132, 22);
				 this->mnu4MB->Text = L"4MB(&1)";
				 this->mnu4MB->Click += gcnew System::EventHandler(this, &frmMain::mnu4MB_Click);
				 // 
				 // mnu8MB
				 // 
				 this->mnu8MB->Name = L"mnu8MB";
				 this->mnu8MB->Size = System::Drawing::Size(132, 22);
				 this->mnu8MB->Text = L"8MB(&2)";
				 this->mnu8MB->Click += gcnew System::EventHandler(this, &frmMain::mnu8MB_Click);
				 // 
				 // mnu16MB
				 // 
				 this->mnu16MB->Name = L"mnu16MB";
				 this->mnu16MB->Size = System::Drawing::Size(132, 22);
				 this->mnu16MB->Text = L"16MB(&3)";
				 this->mnu16MB->Click += gcnew System::EventHandler(this, &frmMain::mnu16MB_Click);
				 // 
				 // mnu32MB
				 // 
				 this->mnu32MB->Name = L"mnu32MB";
				 this->mnu32MB->Size = System::Drawing::Size(132, 22);
				 this->mnu32MB->Text = L"32MB(&4)";
				 this->mnu32MB->Click += gcnew System::EventHandler(this, &frmMain::mnu32MB_Click);
				 // 
				 // mnu64MB
				 // 
				 this->mnu64MB->Name = L"mnu64MB";
				 this->mnu64MB->Size = System::Drawing::Size(132, 22);
				 this->mnu64MB->Text = L"64MB(&5)";
				 this->mnu64MB->Click += gcnew System::EventHandler(this, &frmMain::mnu64MB_Click);
				 // 
				 // mnu128MB
				 // 
				 this->mnu128MB->Name = L"mnu128MB";
				 this->mnu128MB->Size = System::Drawing::Size(132, 22);
				 this->mnu128MB->Text = L"128MB(&6)";
				 this->mnu128MB->Click += gcnew System::EventHandler(this, &frmMain::mnu128MB_Click);
				 // 
				 // mnu256MB
				 // 
				 this->mnu256MB->Name = L"mnu256MB";
				 this->mnu256MB->Size = System::Drawing::Size(132, 22);
				 this->mnu256MB->Text = L"256MB(&7)";
				 this->mnu256MB->Click += gcnew System::EventHandler(this, &frmMain::mnu256MB_Click);
				 // 
				 // mnu512MB
				 // 
				 this->mnu512MB->Name = L"mnu512MB";
				 this->mnu512MB->Size = System::Drawing::Size(132, 22);
				 this->mnu512MB->Text = L"512MB(&8)";
				 this->mnu512MB->Click += gcnew System::EventHandler(this, &frmMain::mnu512MB_Click);
				 // 
				 // mnu1GB
				 // 
				 this->mnu1GB->Name = L"mnu1GB";
				 this->mnu1GB->Size = System::Drawing::Size(132, 22);
				 this->mnu1GB->Text = L"1GB(&9)";
				 this->mnu1GB->Click += gcnew System::EventHandler(this, &frmMain::mnu1GB_Click);
				 // 
				 // mnu2GB
				 // 
				 this->mnu2GB->Name = L"mnu2GB";
				 this->mnu2GB->Size = System::Drawing::Size(132, 22);
				 this->mnu2GB->Text = L"2GB(&0)";
				 this->mnu2GB->Click += gcnew System::EventHandler(this, &frmMain::mnu2GB_Click);
				 // 
				 // mnuAutoClean
				 // 
				 this->mnuAutoClean->Name = L"mnuAutoClean";
				 this->mnuAutoClean->Size = System::Drawing::Size(152, 22);
				 this->mnuAutoClean->Text = L"自动清空(&A)";
				 this->mnuAutoClean->Click += gcnew System::EventHandler(this, &frmMain::mnuAutoClean_Click);
				 // 
				 // toolStripMenuItem9
				 // 
				 this->toolStripMenuItem9->Name = L"toolStripMenuItem9";
				 this->toolStripMenuItem9->Size = System::Drawing::Size(149, 6);
				 // 
				 // mnuDelay
				 // 
				 this->mnuDelay->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->mnuDelayClose, 
					 this->mnuDelay05s, this->mnuDelay1s, this->mnuDelay2s});
				 this->mnuDelay->Name = L"mnuDelay";
				 this->mnuDelay->Size = System::Drawing::Size(152, 22);
				 this->mnuDelay->Text = L"延迟响应(&D)";
				 // 
				 // mnuDelayClose
				 // 
				 this->mnuDelayClose->Name = L"mnuDelayClose";
				 this->mnuDelayClose->Size = System::Drawing::Size(120, 22);
				 this->mnuDelayClose->Text = L"关闭(&C)";
				 this->mnuDelayClose->Click += gcnew System::EventHandler(this, &frmMain::mnuDelayClose_Click);
				 // 
				 // mnuDelay05s
				 // 
				 this->mnuDelay05s->Name = L"mnuDelay05s";
				 this->mnuDelay05s->Size = System::Drawing::Size(120, 22);
				 this->mnuDelay05s->Text = L"0.5秒(&5)";
				 this->mnuDelay05s->Click += gcnew System::EventHandler(this, &frmMain::mnuDelay05s_Click);
				 // 
				 // mnuDelay1s
				 // 
				 this->mnuDelay1s->Name = L"mnuDelay1s";
				 this->mnuDelay1s->Size = System::Drawing::Size(120, 22);
				 this->mnuDelay1s->Text = L"1秒(&1)";
				 this->mnuDelay1s->Click += gcnew System::EventHandler(this, &frmMain::mnuDelay1s_Click);
				 // 
				 // mnuDelay2s
				 // 
				 this->mnuDelay2s->Name = L"mnuDelay2s";
				 this->mnuDelay2s->Size = System::Drawing::Size(120, 22);
				 this->mnuDelay2s->Text = L"2秒(&2)";
				 this->mnuDelay2s->Click += gcnew System::EventHandler(this, &frmMain::mnuDelay2s_Click);
				 // 
				 // mnuPlaySound
				 // 
				 this->mnuPlaySound->Name = L"mnuPlaySound";
				 this->mnuPlaySound->Size = System::Drawing::Size(152, 22);
				 this->mnuPlaySound->Text = L"音效(&S)";
				 this->mnuPlaySound->Click += gcnew System::EventHandler(this, &frmMain::mnuPlaySound_Click);
				 // 
				 // mnuUseBook
				 // 
				 this->mnuUseBook->Name = L"mnuUseBook";
				 this->mnuUseBook->Size = System::Drawing::Size(152, 22);
				 this->mnuUseBook->Text = L"使用棋谱(&B)";
				 this->mnuUseBook->Click += gcnew System::EventHandler(this, &frmMain::mnuUseBook_Click);
				 // 
				 // mnuLearnImmediately
				 // 
				 this->mnuLearnImmediately->Name = L"mnuLearnImmediately";
				 this->mnuLearnImmediately->Size = System::Drawing::Size(152, 22);
				 this->mnuLearnImmediately->Text = L"自动学习(&L)";
				 this->mnuLearnImmediately->Click += gcnew System::EventHandler(this, &frmMain::mnuLearnImmediately_Click);
				 // 
				 // mnuFreeMode
				 // 
				 this->mnuFreeMode->Name = L"mnuFreeMode";
				 this->mnuFreeMode->Size = System::Drawing::Size(152, 22);
				 this->mnuFreeMode->Text = L"自由模式(&F)";
				 this->mnuFreeMode->Click += gcnew System::EventHandler(this, &frmMain::mnuFreeMode_Click);
				 // 
				 // mnuView
				 // 
				 this->mnuView->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->mnuShowEvaluation, 
					 this->mnuShowSpeed, this->mnuShowProgress, this->mnuShowPV});
				 this->mnuView->Name = L"mnuView";
				 this->mnuView->Size = System::Drawing::Size(60, 21);
				 this->mnuView->Text = L"查看(&V)";
				 // 
				 // mnuShowEvaluation
				 // 
				 this->mnuShowEvaluation->Name = L"mnuShowEvaluation";
				 this->mnuShowEvaluation->Size = System::Drawing::Size(141, 22);
				 this->mnuShowEvaluation->Text = L"估值(&E)";
				 this->mnuShowEvaluation->Click += gcnew System::EventHandler(this, &frmMain::mnuShowEvaluation_Click);
				 // 
				 // mnuShowSpeed
				 // 
				 this->mnuShowSpeed->Name = L"mnuShowSpeed";
				 this->mnuShowSpeed->Size = System::Drawing::Size(141, 22);
				 this->mnuShowSpeed->Text = L"搜索速度(&D)";
				 this->mnuShowSpeed->Click += gcnew System::EventHandler(this, &frmMain::mnuShowSpeed_Click);
				 // 
				 // mnuShowProgress
				 // 
				 this->mnuShowProgress->Name = L"mnuShowProgress";
				 this->mnuShowProgress->Size = System::Drawing::Size(141, 22);
				 this->mnuShowProgress->Text = L"搜索进度(&P)";
				 this->mnuShowProgress->Click += gcnew System::EventHandler(this, &frmMain::mnuShowProgress_Click);
				 // 
				 // mnuShowPV
				 // 
				 this->mnuShowPV->Name = L"mnuShowPV";
				 this->mnuShowPV->Size = System::Drawing::Size(141, 22);
				 this->mnuShowPV->Text = L"最优序列(&V)";
				 this->mnuShowPV->Click += gcnew System::EventHandler(this, &frmMain::mnuShowPV_Click);
				 // 
				 // mnuHelp
				 // 
				 this->mnuHelp->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->mnuHelpContents, 
					 this->toolStripMenuItem4, this->mnuShortkeys, this->mnuAbout});
				 this->mnuHelp->Name = L"mnuHelp";
				 this->mnuHelp->Size = System::Drawing::Size(61, 21);
				 this->mnuHelp->Text = L"帮助(&H)";
				 // 
				 // mnuHelpContents
				 // 
				 this->mnuHelpContents->Name = L"mnuHelpContents";
				 this->mnuHelpContents->Size = System::Drawing::Size(148, 22);
				 this->mnuHelpContents->Text = L"帮助主题(&C)";
				 this->mnuHelpContents->Click += gcnew System::EventHandler(this, &frmMain::mnuHelpContents_Click);
				 // 
				 // toolStripMenuItem4
				 // 
				 this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
				 this->toolStripMenuItem4->Size = System::Drawing::Size(145, 6);
				 // 
				 // mnuShortkeys
				 // 
				 this->mnuShortkeys->Name = L"mnuShortkeys";
				 this->mnuShortkeys->Size = System::Drawing::Size(148, 22);
				 this->mnuShortkeys->Text = L"快捷键(&K)";
				 this->mnuShortkeys->Click += gcnew System::EventHandler(this, &frmMain::mnuShortkeys_Click);
				 // 
				 // mnuAbout
				 // 
				 this->mnuAbout->Name = L"mnuAbout";
				 this->mnuAbout->Size = System::Drawing::Size(148, 22);
				 this->mnuAbout->Text = L"关于 Craft(&A)";
				 this->mnuAbout->Click += gcnew System::EventHandler(this, &frmMain::mnuAbout_Click);
				 // 
				 // tmrLayout
				 // 
				 this->tmrLayout->Interval = 200;
				 this->tmrLayout->Tick += gcnew System::EventHandler(this, &frmMain::tmrLayout_Tick);
				 // 
				 // infoPanel
				 // 
				 this->infoPanel->Controls->Add(this->btnStart);
				 this->infoPanel->Controls->Add(this->picWhite);
				 this->infoPanel->Controls->Add(this->picBPointer);
				 this->infoPanel->Controls->Add(this->picWPointer);
				 this->infoPanel->Controls->Add(this->lblEmpty);
				 this->infoPanel->Controls->Add(this->lblWCount);
				 this->infoPanel->Controls->Add(this->label6);
				 this->infoPanel->Controls->Add(this->lblBCount);
				 this->infoPanel->Controls->Add(this->label5);
				 this->infoPanel->Controls->Add(this->lblCStep);
				 this->infoPanel->Controls->Add(this->lblWNum);
				 this->infoPanel->Controls->Add(this->label4);
				 this->infoPanel->Controls->Add(this->lblBNum);
				 this->infoPanel->Controls->Add(this->label3);
				 this->infoPanel->Controls->Add(this->picBlack);
				 this->infoPanel->Controls->Add(this->lblCurrentStep);
				 this->infoPanel->Controls->Add(this->label7);
				 this->infoPanel->Controls->Add(this->label2);
				 this->infoPanel->Controls->Add(this->label1);
				 this->infoPanel->Controls->Add(this->lstSteps);
				 this->infoPanel->Dock = System::Windows::Forms::DockStyle::Right;
				 this->infoPanel->Location = System::Drawing::Point(495, 64);
				 this->infoPanel->Name = L"infoPanel";
				 this->infoPanel->Size = System::Drawing::Size(164, 455);
				 this->infoPanel->TabIndex = 10;
				 // 
				 // btnStart
				 // 
				 this->btnStart->FlatStyle = System::Windows::Forms::FlatStyle::System;
				 this->btnStart->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->btnStart->Location = System::Drawing::Point(12, 153);
				 this->btnStart->Name = L"btnStart";
				 this->btnStart->Size = System::Drawing::Size(134, 35);
				 this->btnStart->TabIndex = 18;
				 this->btnStart->Text = L"开始！";
				 this->btnStart->UseVisualStyleBackColor = true;
				 this->btnStart->Visible = false;
				 this->btnStart->Click += gcnew System::EventHandler(this, &frmMain::btnStart_Click);
				 // 
				 // picWhite
				 // 
				 this->picWhite->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"picWhite.Image")));
				 this->picWhite->Location = System::Drawing::Point(41, 64);
				 this->picWhite->Name = L"picWhite";
				 this->picWhite->Size = System::Drawing::Size(40, 40);
				 this->picWhite->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
				 this->picWhite->TabIndex = 14;
				 this->picWhite->TabStop = false;
				 // 
				 // picBPointer
				 // 
				 this->picBPointer->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"picBPointer.Image")));
				 this->picBPointer->Location = System::Drawing::Point(8, 6);
				 this->picBPointer->Name = L"picBPointer";
				 this->picBPointer->Size = System::Drawing::Size(36, 33);
				 this->picBPointer->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
				 this->picBPointer->TabIndex = 17;
				 this->picBPointer->TabStop = false;
				 // 
				 // picWPointer
				 // 
				 this->picWPointer->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"picWPointer.Image")));
				 this->picWPointer->Location = System::Drawing::Point(8, 67);
				 this->picWPointer->Name = L"picWPointer";
				 this->picWPointer->Size = System::Drawing::Size(36, 33);
				 this->picWPointer->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
				 this->picWPointer->TabIndex = 17;
				 this->picWPointer->TabStop = false;
				 this->picWPointer->Visible = false;
				 // 
				 // lblEmpty
				 // 
				 this->lblEmpty->BackColor = System::Drawing::Color::Transparent;
				 this->lblEmpty->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->lblEmpty->Location = System::Drawing::Point(105, 132);
				 this->lblEmpty->Name = L"lblEmpty";
				 this->lblEmpty->Size = System::Drawing::Size(28, 14);
				 this->lblEmpty->TabIndex = 16;
				 this->lblEmpty->Text = L"2";
				 this->lblEmpty->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
				 // 
				 // lblWCount
				 // 
				 this->lblWCount->BackColor = System::Drawing::Color::Transparent;
				 this->lblWCount->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->lblWCount->Location = System::Drawing::Point(95, 107);
				 this->lblWCount->Name = L"lblWCount";
				 this->lblWCount->Size = System::Drawing::Size(28, 14);
				 this->lblWCount->TabIndex = 16;
				 this->lblWCount->Text = L"2";
				 this->lblWCount->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
				 // 
				 // label6
				 // 
				 this->label6->AutoSize = true;
				 this->label6->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->label6->Location = System::Drawing::Point(120, 107);
				 this->label6->Name = L"label6";
				 this->label6->Size = System::Drawing::Size(21, 14);
				 this->label6->TabIndex = 16;
				 this->label6->Text = L"步";
				 // 
				 // lblBCount
				 // 
				 this->lblBCount->BackColor = System::Drawing::Color::Transparent;
				 this->lblBCount->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->lblBCount->Location = System::Drawing::Point(95, 47);
				 this->lblBCount->Name = L"lblBCount";
				 this->lblBCount->Size = System::Drawing::Size(28, 14);
				 this->lblBCount->TabIndex = 16;
				 this->lblBCount->Text = L"2";
				 this->lblBCount->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
				 // 
				 // label5
				 // 
				 this->label5->AutoSize = true;
				 this->label5->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->label5->Location = System::Drawing::Point(120, 47);
				 this->label5->Name = L"label5";
				 this->label5->Size = System::Drawing::Size(21, 14);
				 this->label5->TabIndex = 16;
				 this->label5->Text = L"步";
				 // 
				 // lblCStep
				 // 
				 this->lblCStep->BackColor = System::Drawing::Color::Transparent;
				 this->lblCStep->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(0)));
				 this->lblCStep->Location = System::Drawing::Point(104, 160);
				 this->lblCStep->Name = L"lblCStep";
				 this->lblCStep->Size = System::Drawing::Size(29, 14);
				 this->lblCStep->TabIndex = 15;
				 this->lblCStep->Text = L"1";
				 this->lblCStep->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
				 // 
				 // lblWNum
				 // 
				 this->lblWNum->BackColor = System::Drawing::Color::Transparent;
				 this->lblWNum->Font = (gcnew System::Drawing::Font(L"Comic Sans MS", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(0)));
				 this->lblWNum->Location = System::Drawing::Point(87, 74);
				 this->lblWNum->Name = L"lblWNum";
				 this->lblWNum->Size = System::Drawing::Size(38, 26);
				 this->lblWNum->TabIndex = 15;
				 this->lblWNum->Text = L"2";
				 this->lblWNum->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
				 // 
				 // label4
				 // 
				 this->label4->AutoSize = true;
				 this->label4->Font = (gcnew System::Drawing::Font(L"黑体", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->label4->Location = System::Drawing::Point(119, 78);
				 this->label4->Name = L"label4";
				 this->label4->Size = System::Drawing::Size(29, 19);
				 this->label4->TabIndex = 15;
				 this->label4->Text = L"子";
				 // 
				 // lblBNum
				 // 
				 this->lblBNum->BackColor = System::Drawing::Color::Transparent;
				 this->lblBNum->Font = (gcnew System::Drawing::Font(L"Comic Sans MS", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(0)));
				 this->lblBNum->Location = System::Drawing::Point(87, 13);
				 this->lblBNum->Name = L"lblBNum";
				 this->lblBNum->Size = System::Drawing::Size(38, 26);
				 this->lblBNum->TabIndex = 15;
				 this->lblBNum->Text = L"2";
				 this->lblBNum->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
				 // 
				 // label3
				 // 
				 this->label3->AutoSize = true;
				 this->label3->Font = (gcnew System::Drawing::Font(L"黑体", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->label3->Location = System::Drawing::Point(119, 17);
				 this->label3->Name = L"label3";
				 this->label3->Size = System::Drawing::Size(29, 19);
				 this->label3->TabIndex = 15;
				 this->label3->Text = L"子";
				 // 
				 // picBlack
				 // 
				 this->picBlack->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"picBlack.Image")));
				 this->picBlack->Location = System::Drawing::Point(41, 4);
				 this->picBlack->Name = L"picBlack";
				 this->picBlack->Size = System::Drawing::Size(40, 40);
				 this->picBlack->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
				 this->picBlack->TabIndex = 14;
				 this->picBlack->TabStop = false;
				 // 
				 // lblCurrentStep
				 // 
				 this->lblCurrentStep->AutoSize = true;
				 this->lblCurrentStep->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->lblCurrentStep->Location = System::Drawing::Point(14, 160);
				 this->lblCurrentStep->Name = L"lblCurrentStep";
				 this->lblCurrentStep->Size = System::Drawing::Size(77, 14);
				 this->lblCurrentStep->TabIndex = 13;
				 this->lblCurrentStep->Text = L"当前步数：";
				 // 
				 // label7
				 // 
				 this->label7->AutoSize = true;
				 this->label7->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->label7->Location = System::Drawing::Point(14, 132);
				 this->label7->Name = L"label7";
				 this->label7->Size = System::Drawing::Size(63, 14);
				 this->label7->TabIndex = 13;
				 this->label7->Text = L"空格数：";
				 // 
				 // label2
				 // 
				 this->label2->AutoSize = true;
				 this->label2->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->label2->Location = System::Drawing::Point(38, 107);
				 this->label2->Name = L"label2";
				 this->label2->Size = System::Drawing::Size(63, 14);
				 this->label2->TabIndex = 13;
				 this->label2->Text = L"行动力：";
				 // 
				 // label1
				 // 
				 this->label1->AutoSize = true;
				 this->label1->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->label1->Location = System::Drawing::Point(38, 47);
				 this->label1->Name = L"label1";
				 this->label1->Size = System::Drawing::Size(63, 14);
				 this->label1->TabIndex = 13;
				 this->label1->Text = L"行动力：";
				 // 
				 // lstSteps
				 // 
				 this->lstSteps->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
				 this->lstSteps->Dock = System::Windows::Forms::DockStyle::Bottom;
				 this->lstSteps->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(134)));
				 this->lstSteps->FormattingEnabled = true;
				 this->lstSteps->ItemHeight = 14;
				 this->lstSteps->Location = System::Drawing::Point(0, 215);
				 this->lstSteps->Name = L"lstSteps";
				 this->lstSteps->ScrollAlwaysVisible = true;
				 this->lstSteps->Size = System::Drawing::Size(164, 240);
				 this->lstSteps->TabIndex = 12;
				 this->lstSteps->TabStop = false;
				 // 
				 // statusBar2
				 // 
				 this->statusBar2->BackColor = System::Drawing::SystemColors::Control;
				 this->statusBar2->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->ssPV});
				 this->statusBar2->Location = System::Drawing::Point(0, 497);
				 this->statusBar2->Name = L"statusBar2";
				 this->statusBar2->RenderMode = System::Windows::Forms::ToolStripRenderMode::ManagerRenderMode;
				 this->statusBar2->Size = System::Drawing::Size(495, 22);
				 this->statusBar2->SizingGrip = false;
				 this->statusBar2->TabIndex = 11;
				 this->statusBar2->Text = L"状态栏2";
				 // 
				 // ssPV
				 // 
				 this->ssPV->AutoSize = false;
				 this->ssPV->Name = L"ssPV";
				 this->ssPV->Size = System::Drawing::Size(470, 17);
				 this->ssPV->Text = L"Principle Variation";
				 this->ssPV->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
				 // 
				 // tmrPrompt
				 // 
				 this->tmrPrompt->Interval = 3000;
				 this->tmrPrompt->Tick += gcnew System::EventHandler(this, &frmMain::tmrPrompt_Tick);
				 // 
				 // frmMain
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(659, 542);
				 this->Controls->Add(this->statusBar2);
				 this->Controls->Add(this->picBoard);
				 this->Controls->Add(this->infoPanel);
				 this->Controls->Add(this->toolBar);
				 this->Controls->Add(this->statusBar);
				 this->Controls->Add(this->menuBar);
				 this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
				 this->KeyPreview = true;
				 this->MainMenuStrip = this->menuBar;
				 this->MinimumSize = System::Drawing::Size(400, 350);
				 this->Name = L"frmMain";
				 this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
				 this->Text = L"Craft";
				 this->Load += gcnew System::EventHandler(this, &frmMain::frmMain_Load);
				 this->ResizeBegin += gcnew System::EventHandler(this, &frmMain::frmMain_ResizeBegin);
				 this->SizeChanged += gcnew System::EventHandler(this, &frmMain::frmMain_SizeChanged);
				 this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &frmMain::frmMain_FormClosed);
				 this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &frmMain::frmMain_KeyUp);
				 this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &frmMain::frmMain_FormClosing);
				 this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &frmMain::frmMain_KeyDown);
				 this->ResizeEnd += gcnew System::EventHandler(this, &frmMain::frmMain_ResizeEnd);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picBoard))->EndInit();
				 this->statusBar->ResumeLayout(false);
				 this->statusBar->PerformLayout();
				 this->toolBar->ResumeLayout(false);
				 this->toolBar->PerformLayout();
				 this->menuBar->ResumeLayout(false);
				 this->menuBar->PerformLayout();
				 this->infoPanel->ResumeLayout(false);
				 this->infoPanel->PerformLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picWhite))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picBPointer))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picWPointer))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picBlack))->EndInit();
				 this->statusBar2->ResumeLayout(false);
				 this->statusBar2->PerformLayout();
				 this->ResumeLayout(false);
				 this->PerformLayout();

			 }
#pragma endregion

	private:System::Void frmMain_Load(System::Object ^sender, System::EventArgs ^e);
	private:System::Void picBoard_MouseClick(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e);
	private:System::Void picBoard_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private:System::Void picBoard_MouseLeave(System::Object^ sender, System::EventArgs^ e);
	private:System::Void picBoard_MouseUp(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e);
	private:System::Void picBoard_MouseDown(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e);
	private:System::Void frmMain_SizeChanged(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tmrLayout_Tick(System::Object ^sender, System::EventArgs ^e);
	private:System::Void backBoard_MouseClick(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e);
	private:System::Void mnuExit_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsbtnExit_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void frmMain_FormClosing(System::Object ^sender, System::Windows::Forms::FormClosingEventArgs ^e);
	private:System::Void btnStart_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuManBlackToMachine_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuManWhiteToMachine_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuManToMan_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuMachines_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsbtnRestart_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuManBlackToMachine_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuManWhiteToMachine_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuManToMan_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuMachines_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuRestart_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuBlackMan_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuBlackRandom_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuWhiteMan_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuWhiteRandom_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuBlackBigEat_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuBlackEarth_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuBlackClouds_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuBlackOrbit_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuBlackMoon_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuBlackStar_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuBlackGalaxy_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuBlackUniverse_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuWhiteBigEat_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuWhiteEarth_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuWhiteClouds_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuWhiteOrbit_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuWhiteMoon_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuWhiteStar_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuWhiteGalaxy_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsmnuWhiteUniverse_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsbtnBack_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void tsbtnContinue_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnu4MB_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnu8MB_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnu16MB_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnu32MB_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnu64MB_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnu128MB_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnu256MB_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnu512MB_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnu1GB_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnu2GB_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuDelayClose_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuDelay05s_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuDelay1s_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuDelay2s_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuPlaySound_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuAbout_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuHelpContents_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void frmMain_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);
	private:System::Void mnuLearnImmediately_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsbtnLearn_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuShowEvaluation_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuShowSpeed_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuShowProgress_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuLearn_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuSetupBoard_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsbtnSetupBoard_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuBack_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuForward_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuOpenGame_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuSaveGame_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsbtnOpenGame_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsbtnSaveGame_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuFreeMode_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuShowStatistics_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void frmMain_ResizeBegin(System::Object^  sender, System::EventArgs^  e);
	private:System::Void frmMain_ResizeEnd(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuTip_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsbtnTip_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuShortkeys_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void frmMain_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private:System::Void backBoard_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:System::Void backBoard_MouseHover(System::Object^  sender, System::EventArgs^  e);
	private:System::Void picBoard_MouseHover(System::Object^  sender, System::EventArgs^  e);
	private:System::Void frmMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e);
	private:System::Void mnuAnalyze_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsbtnAnalyze_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsmnuEndGameBeginner_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsmnuEndGameAmateur_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsmnuEndGameProfessional_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsmnuEndGameMaster_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsmnuEndGameCustom_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuEndGameBeginner_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuEndGameAmateur_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuEndGameProfessional_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuEndGameMaster_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuEndGameCustom_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsbtnShowStatistics_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsmnuShowEndGameInfo_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuShowEndGameInfo_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuForceEndSolve_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuClearCache_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuDefaultTheme_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuSomeTheme_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsbtnForceEndSolve_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tsbtnStopSearch_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuStopSearch_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuUseBook_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuAutoClean_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void mnuShowPV_Click(System::Object^  sender, System::EventArgs^  e);
	private:System::Void tmrPrompt_Tick(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuCopy_Click(System::Object ^sender, System::EventArgs ^e);
	private:System::Void mnuPaste_Click(System::Object ^sender, System::EventArgs ^e);
	private:
		void setSelection(int x, int y);
		AbstractPlayer^ createPlayer(PlayerType type, Chess colorReserved);
		void createGame(GameOption option);
		void setGUIPlay(bool state);
		int getGUIPlay();
		void loadDefaultTheme();
		void playSound(SoundPlayer^ player);
		bool imageAbort();
		void setMnuTableSize();
		void setLayout();
		void setBoard();
		void startNewGame();
		void startGame();
		void startNewGame(GameOption option);
		void setGameJustStarted(bool state);
		void exitProgram();
		void endCurrentGame();
		String^ getMoveDescription(int step, Othello::Move move);
		void addMove(int step, Othello::Move move);
		void setTsmnuBlack(PlayerType type, bool state);
		void setTsmnuWhite(PlayerType type, bool state);
		void setPaused(bool paused);
		PlayerType chooseMachineType(Chess color);
		void manBlackToMachine();
		void manWhiteToMachine();
		void manToMan();
		void machines();
		void restartGame();
		void changePlayer(PlayerType type, Chess color);
		void setWhiteType(PlayerType type);
		void setBlackType(PlayerType type);
		void restoreState();
		void goBack();
		void goForward();
		void stopSearch();
		void addMoves();
		void setStepDelay(int delay);
		void learnGame();
		void setMnuDelay();
		void loadConfig();
		void saveConfig();
		Board^ getCurrentBoard();
		void setupBoard();
		void setupBoard(Board^ board, Chess firstPlayer);
		bool checkMem(unsigned long long tableSize);
		void setTableSize(int size);
		void openGame();
		void openGameFile(String^ fileName);
		void saveGame();
		void saveGameFile(String^ fileName);
		void setFairness();
		void setGameInfo();
		void changeMode();
		int getGoBackChance(PlayerType type);
		int getTipChance(PlayerType type);
		bool breakFairness(String^ prompt);
		PlayerType readPlayerType(IO::BinaryReader^ br);
		Statistics^ readStatistics(IO::BinaryReader^ br);
		void writePlayerType(IO::BinaryWriter^ bw, PlayerType type);
		void writeStatistics(IO::BinaryWriter^ bw, Statistics^ stat);
		void showStatistics();
		void getGUITip();
		void forceEndSolve();
		void setTipPos(int x, int y);
		System::String^ getComment(PlayerType type, int result);
		void showShortKeys();
		void openHelp();
		void showAbout();
		void enterPeekMode();
		void leavePeekMode();
		void picBoard_MouseMoved(System::Object ^sender);
		void picBoard_MouseLeft();
		void showAnalyzer();
		void enterAnalyzeMode();
		void leaveAnalyzeMode();
		void startGameForGUIPlayer();
		void setQuestionLevel(QuestionLevel ql);
		void initEndGameMode();
		void startNewEndGame(EndGameDifficulty difficulty);
		bool startNewEndGame(int gameIndex, int empties);
		void enterEndGameMode();
		void leaveEndGameMode();
		void showEndGameInfo(WThorGame^ game, int bestResult, int empties, Chess player);
		void startCustomEndGame();
		void reshowEndGameInfo();
		System::String^ getEndGameResultDescription(int result);
		void setTranspositionTableAllowed(bool allowed);
		void clearCache();
		void initThemeList();
		void loadTheme(System::String^ theme);
		void applyTheme();
		System::Drawing::Image^ loadImage(System::String^ path, System::String^ defaultResourceName);
		void changeTheme(System::String^ newTheme);
		void setMnuTheme();
		void notifyUser();
		void saveBook();
		void autoClear();
		bool needShowPV();
		void showPVChanged();
		void copyBoard();
		void pasteBoard();
		void prompt(System::String ^content);
		void prompt(System::String^ content, int timeout);
		void prompt(System::String^ content, int timeout, System::Drawing::Image^ icon);
		void prompt(System::String ^content, System::Drawing::Image ^icon);

#ifdef CHRISTMAS
		void showChristmasWish();
#endif
	public:
		frmMain(void);
		bool OnIdle();

		virtual void playerMoved(Othello::Move move, 
			GameContext^ gcBlack, GameContext^ gcWhite);
		virtual void gameStarted(Chess firstPlayer,
			GameContext^ gcBlack, GameContext^ gcWhite);
		virtual void gameReseted(Chess firstPlayer,
			GameContext^ gcBlack, GameContext^ gcWhite);
		virtual void gameEnded(Chess winner, int nBlack, int nWhite, 
			GameContext^ gcBlack, GameContext^ gcWhite);
		virtual void boardChanged(Chess firstPlayer,
			GameContext^ gcBlack, GameContext^ gcWhite);
		virtual void playerChanged(Chess currentPlayer,
			GameContext^ gcBlack, GameContext^ gcWhite);
		virtual void timeLimitChanged(int timeLimit,
			GameContext^ gcBlack, GameContext^ gcWhite);
		virtual void playerOverTimed(Chess overTimer,
			GameContext^ gcBlack, GameContext^ gcWhite);
		virtual void gameGoneBack(int step,
			GameContext^ gcBlack, GameContext^ gcWhite);
		virtual void gameGoneForward(int step,
			GameContext^ gcBlack, GameContext^ gcWhite);
		virtual void gameHasBeenPaused(Chess currentPlayer,
			GameContext^ gcBlack, GameContext^ gcWhite);

		virtual void setSearchState(bool searching);
		virtual void setResult(System::String^ result);
		virtual void setSpeed(System::String^ speed);
		virtual void setTotalNum(System::String^ totalNum);
		virtual void setProgress(int percent);
		virtual void setFocusedMove(int x, int y);
		virtual void setSelectedMove(int x, int y);
		virtual void showPrincipleVariation(System::String^ pv);
		virtual void resetComponents();
		virtual void searchStarted();
		virtual void searchEnded();

		virtual void analyzeStarted();
		virtual void analyzeEnded();
		virtual void analyzerOpened();
		virtual void analyzerClosed();
		virtual void showMove(int step);
		virtual void startFromCurrentStep();

		virtual void statisticsWindowShown();
		virtual void statisticsWindowClosed();
		virtual void acquirePause(bool paused);

		virtual void endGameInfoShown();
		virtual void endGameInfoClosed();
		virtual void acquireChange();

#ifdef CHRISTMAS
		virtual void wishShown();
		virtual void wishClosed();
#endif
	protected:
		virtual void WndProc(System::Windows::Forms::Message% m) override;
		~frmMain();
	};
}
