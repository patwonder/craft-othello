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

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Utility;
using System.IO;
using Microsoft.Win32;

namespace MPC_Calc {
	public partial class MainForm : Form {
		private IProcessor processor;
		private bool processing = false;
		private Task task = null;
		private const String TASK_FILE = "task.ini";
		//private bool close_lock = true;

		public MainForm() {
			InitializeComponent();
		}

		private Task loadTaskFromFile() {
			ConfigAccessor accessor = new ConfigAccessor();
			if (!accessor.readConfigFromFile(TASK_FILE)) return null;
			Catagory cat = accessor.getCatagory("Task");
			if (cat == null) return null;
			String file = cat.getAttribute("File", null);
			int process = cat.getIntAttribute("Process", -1);
			if (file == null || process < 0) return null;
			Task task = new Task(file);
			task.Process = process;
			return task;
		}

		private Task askForTask() {
			using (OpenFileDialog dlgChooseFile = new OpenFileDialog()) {
				dlgChooseFile.Multiselect = false;
				dlgChooseFile.Filter = "Pattern Configuration Database(*.pcd)|*.PCD";
				dlgChooseFile.CheckFileExists = true;
				dlgChooseFile.Title = "选择数据文件";
				dlgChooseFile.FileName = "";
				DialogResult res = dlgChooseFile.ShowDialog(this);
				if (res == DialogResult.Cancel) {
					return null;
				}
				return new Task(dlgChooseFile.FileName);
			}
		}

		private Task getTask() {
			Task task = loadTaskFromFile();
			if (task == null) {
				return askForTask();
			} else return task;
		}

		private void saveTask(Task task) {
			ConfigAccessor accessor = new ConfigAccessor();
			Catagory cat = new Catagory("Task");
			cat.addAttribute("File", task.FileName);
			cat.addAttribute("Process", task.Process.ToString());
			accessor.addCatagory(cat.Name, cat);
			accessor.writeConfigToFile(TASK_FILE);
		}

		private void finished() {
			if (!processing) return;
			try {
				File.Delete(TASK_FILE);
			} catch (Exception) {
			}
			processing = false;
			MessageBox.Show(this, "处理完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
			//close_lock = false;
			this.Close();
		}

		private void progress(int current, int total) {
			lblProgress.Text = (current * 100 / total) + "% (" + current + "/" + total + ")";
			pb.Minimum = 0;
			pb.Maximum = total;
			pb.Value = current;
			if (current == 0)
				pb.Style = ProgressBarStyle.Marquee;
			else pb.Style = ProgressBarStyle.Continuous;
		}

        private void saveAndExit() {
			processor.stopProcess();
			processing = false;
			saveTask(task);
			//close_lock = false;
			this.Close();
		}

		private void showWindow() {
			System.Diagnostics.Process.GetCurrentProcess().PriorityClass
				= System.Diagnostics.ProcessPriorityClass.Normal;
			this.Show();
			this.WindowState = FormWindowState.Normal;
			this.Activate();
		}

		private void hideWindow() {
			this.Hide();
			System.Diagnostics.Process.GetCurrentProcess().PriorityClass 
				= System.Diagnostics.ProcessPriorityClass.Idle;
			showTip();
		}

		private void setTip() {
			notifyIcon.BalloonTipText = "文件：" + lblFile.Text + "\n" + "进度：" + lblProgress.Text;
		}

		private void showTip() {
			setTip();
			notifyIcon.ShowBalloonTip(3000);
		}

		private void stopWorkAndExit() {
			DialogResult res =
				MessageBox.Show(this, "这将无法恢复此文件的处理进度！！\n确实要停止处理么？", "警告", MessageBoxButtons.YesNo,
				MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2);
			if (res != DialogResult.Yes) return;

			processor.stopProcess();

			try {
				File.Delete(TASK_FILE);
			} catch (Exception) {

			}

			processing = false;

			//close_lock = false;
			this.Close();
		}

		private void MainForm_Load(object sender, EventArgs e) {
			task = getTask();
			if (task == null) {
				//close_lock = false;
				this.Close();
				return;
			}

			lblFile.Text = Path.GetFileName(task.FileName);

			processor = new Processor(task);
			processor.setProcessFinishCallback(delegate() {
				if (InvokeRequired) {
					BeginInvoke(new SimpleDelegate(delegate() {
						this.finished();
					}));
				} else this.finished();
			});
			processor.setProcessProgressCallback(delegate(int current, int total) {
				if (InvokeRequired) {
					BeginInvoke(new SimpleDelegate(delegate() {
						this.progress(current, total);
					}));
				} else this.progress(current, total);
			});
			SystemEvents.SessionEnding += new SessionEndingEventHandler(SystemEvents_SessionEnding);
			processing = true;
			processor.startProcess();
			notifyIcon.Visible = true;
		}

		private void SystemEvents_SessionEnding(object sender, SessionEndingEventArgs e) {
			saveAndExit();
		}

		private void btnExit_Click(object sender, EventArgs e) {
			saveAndExit();
		}

		private void btnStopWork_Click(object sender, EventArgs e) {
			stopWorkAndExit();
		}

		private void mnuShow_Click(object sender, EventArgs e) {
			showWindow();
		}

		private void MainForm_FormClosing(object sender, FormClosingEventArgs e) {
			//e.Cancel = close_lock;
			//if (close_lock) hideWindow();
		}

		private void MainForm_Resize(object sender, EventArgs e) {
			if (WindowState == FormWindowState.Minimized) {
				hideWindow();
			}
		}

		private void mnuStopWork_Click(object sender, EventArgs e) {
			stopWorkAndExit();
		}

		private void mnuExit_Click(object sender, EventArgs e) {
			saveAndExit();
		}

		private void notifyIcon_Click(object sender, MouseEventArgs e) {
			if (e.Button == MouseButtons.Left) {
				if (this.Visible)
					hideWindow();
				else showWindow();
			}
		}

		private void MainForm_FormClosed(object sender, FormClosedEventArgs e) {
			if (processing) {
				processor.stopProcess();
				processing = false;
				saveTask(task);
			}
			SystemEvents.SessionEnding -= new SessionEndingEventHandler(SystemEvents_SessionEnding);
		}

	}
}
