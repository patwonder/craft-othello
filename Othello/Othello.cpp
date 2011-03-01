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
								Othello.cpp
		作者：Patrick
		概述：定义入口点 main 函数。

********************************************************************************
*/

// Othello.cpp : main project file.

#include "stdafx.h"
//#include <time.h>
//#define LEARN
//#define LEARNBOOK
//#define DEBUG

#ifndef LEARN
#ifndef LEARNBOOK
#include "frmMain.h"
#include "frmStartUp.h"
#include "CSingleInstance.h"
#include "SecondInstance.h"
#endif
#endif
#ifdef LEARN
#include "PatternLearning.h"
#endif
#ifdef LEARNBOOK
#include "PatternPreparer.h"
#endif

#include "Executor.h"

#ifdef DEBUG
#define SHOW(A) MessageBox::Show(#A + " = " + (A))
#else
#define SHOW(A) void(0)
#endif //DEBUG

using namespace Othello;

#define UNINSTALLER "msiexec.exe"
#define UPGRADE_CODE "{874DC322-366F-4F04-98A2-238FC1C23BCE}" // Won't change

using namespace CraftEngine;

namespace Othello {

	void doUninstall(String^ productCode) {
		Windows::Forms::DialogResult res = MessageBox::Show(
			"确实要卸载 " + __APP_NAME__ + " " + __APP_VERSION__ + " 吗？"
			/*+ "\nProductCode: " + productCode*/,
			"卸载 " + __APP_NAME__, MessageBoxButtons::YesNo, MessageBoxIcon::Warning,
			MessageBoxDefaultButton::Button2);
		if (res == Windows::Forms::DialogResult::No) return;
		execute(UNINSTALLER + " /x " + productCode + " /passive");
	}

	void doReset() {
		try {
			File::Delete(Application::LocalUserAppDataPath + "\\book.craft");
			File::Delete(Application::LocalUserAppDataPath + "\\config.craft");
		} catch (...) {
		}
	}
	
	ref class GlobalObjects {
	public:
		static frmMain^ mainForm;
	};

	void Application_Idle(System::Object^ sender, System::EventArgs^ e) {
		while (GlobalObjects::mainForm->OnIdle()) {
			Application::DoEvents();
		}
	}

} // namespace Othello

[STAThreadAttribute]
int main(array<System::String ^> ^args) {

	try {
		// Enabling Windows XP visual effects before any controls are created
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false); 

		//Detect uninstall option
		if (args->Length == 2 && 
			(args[0]->ToUpper()->Equals("/UNINSTALL") || args[0]->ToUpper()->Equals("-UNINSTALL"))) {
				doUninstall(args[1]);
				return EXIT_SUCCESS;
		}

		//Reset user profile
		if (args->Length == 1 && 
			(args[0]->ToUpper()->Equals("/RESET") || args[0]->ToUpper()->Equals("-RESET"))) {
				doReset();
				return EXIT_SUCCESS;
		}

		// Detect multiple instances, using the upgrade code for the mutex
#ifndef LEARN
#ifndef LEARNBOOK
		/*
		String^ executablePath = Application::ExecutablePath;
		String^ appPath = executablePath->Substring(0, executablePath->LastIndexOf('\\'));
		appPath = appPath->Replace("_", "__");
		appPath = appPath->Replace("\\", "_p");
		*/
		CSingleInstance^ si = gcnew CSingleInstance(/*appPath*/UPGRADE_CODE);
		try { /* mutex release */
			if (si->isRunning()) {
				if (SecondInstance::WM_SISTART) {
					SecondInstance::CLRPostMessage((System::IntPtr)SecondInstance::CLR_HWND_BROADCAST,
						SecondInstance::WM_SISTART, System::IntPtr::Zero, System::IntPtr::Zero);
				} else {
					MessageBox::Show(__APP_NAME__ + " 已经在运行了！", "提示", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				}
				return EXIT_SUCCESS;
			}
			SHOW(Application::CompanyName);
			SHOW(Application::ProductName);
			SHOW(Application::ProductVersion);
#endif
#endif
			/*
			String^ bookPath = Application::ExecutablePath->Substring(0, executablePath->LastIndexOf('\\') + 1) + "book.craft";
			String^ patternPath = Application::ExecutablePath->Substring(0, executablePath->LastIndexOf('\\') + 1) + "data.craft";

			char* bp = (char*)(int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(bookPath);
			char* pp = (char*)(int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(patternPath);
			bool init = Solver::initialize(pp, bp);
			System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)bp);
			System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)pp);

			if (!init) {
				MessageBox::Show(__APP_NAME__ + " 未能正常初始化。请确保程序文件是完整的。\n尝试重新安装应用程序以解决此问题。",
					__APP_NAME__ + " 初始化失败", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return EXIT_FAILURE;
			}
			*/
			frmStartUp^ startupForm = gcnew frmStartUp();
			try { /* dialog disposal */
				DialogResult res = startupForm->ShowDialog();
				if (res != Windows::Forms::DialogResult::Yes) return EXIT_FAILURE;

				// Create the main window and run it
				GlobalObjects::mainForm = 
#ifdef LEARN 
					gcnew PatternLearning();
#else
#ifdef LEARNBOOK
					gcnew PatternPreparer();
#else
					//gcnew frmMain();
					startupForm->LoadedMainForm;
#endif
#endif //LEARN
			} finally { /* dialog disposal */
				delete startupForm;
				startupForm = nullptr;
			}

			Application::Idle += gcnew EventHandler(Application_Idle);
			Application::Run(GlobalObjects::mainForm);
			Application::Idle -= gcnew EventHandler(Application_Idle);

			delete GlobalObjects::mainForm;
			GlobalObjects::mainForm = nullptr;
			Solver::cleanup();
#ifndef LEARN
#ifndef LEARNBOOK
		} finally { /* mutex release */
			si->closeMutex();
		}
#endif
#endif
		return EXIT_SUCCESS;
	} catch (Exception^ e) {
		MessageBox::Show(e->ToString(), "Sorry, an error has occured", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return EXIT_FAILURE;
	} finally {

	}
}
