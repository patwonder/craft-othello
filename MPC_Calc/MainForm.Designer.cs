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

namespace MPC_Calc
{
	partial class MainForm {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.label1 = new System.Windows.Forms.Label();
			this.lblFile = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.lblProgress = new System.Windows.Forms.Label();
			this.pb = new System.Windows.Forms.ProgressBar();
			this.btnStopWork = new System.Windows.Forms.Button();
			this.btnExit = new System.Windows.Forms.Button();
			this.notifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
			this.mnu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.mnuShow = new System.Windows.Forms.ToolStripMenuItem();
			this.mnuStopWork = new System.Windows.Forms.ToolStripMenuItem();
			this.mnuExit = new System.Windows.Forms.ToolStripMenuItem();
			this.mnu.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(15, 17);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(65, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "当前文件：";
			// 
			// lblFile
			// 
			this.lblFile.Location = new System.Drawing.Point(86, 17);
			this.lblFile.Name = "lblFile";
			this.lblFile.Size = new System.Drawing.Size(296, 11);
			this.lblFile.TabIndex = 1;
			this.lblFile.Text = "lblFile";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(15, 42);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(41, 12);
			this.label2.TabIndex = 2;
			this.label2.Text = "进度：";
			// 
			// lblProgress
			// 
			this.lblProgress.AutoSize = true;
			this.lblProgress.Location = new System.Drawing.Point(86, 42);
			this.lblProgress.Name = "lblProgress";
			this.lblProgress.Size = new System.Drawing.Size(71, 12);
			this.lblProgress.TabIndex = 3;
			this.lblProgress.Text = "lblProgress";
			// 
			// pb
			// 
			this.pb.Location = new System.Drawing.Point(17, 68);
			this.pb.Name = "pb";
			this.pb.Size = new System.Drawing.Size(362, 22);
			this.pb.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
			this.pb.TabIndex = 4;
			// 
			// btnStopWork
			// 
			this.btnStopWork.Location = new System.Drawing.Point(227, 108);
			this.btnStopWork.Name = "btnStopWork";
			this.btnStopWork.Size = new System.Drawing.Size(73, 26);
			this.btnStopWork.TabIndex = 1;
			this.btnStopWork.Text = "停止处理";
			this.btnStopWork.UseVisualStyleBackColor = true;
			this.btnStopWork.Click += new System.EventHandler(this.btnStopWork_Click);
			// 
			// btnExit
			// 
			this.btnExit.Location = new System.Drawing.Point(306, 108);
			this.btnExit.Name = "btnExit";
			this.btnExit.Size = new System.Drawing.Size(73, 26);
			this.btnExit.TabIndex = 2;
			this.btnExit.Text = "退出程序";
			this.btnExit.UseVisualStyleBackColor = true;
			this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
			// 
			// notifyIcon
			// 
			this.notifyIcon.BalloonTipIcon = System.Windows.Forms.ToolTipIcon.Info;
			this.notifyIcon.BalloonTipTitle = "MPC Pre-search Tool";
			this.notifyIcon.ContextMenuStrip = this.mnu;
			this.notifyIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon.Icon")));
			this.notifyIcon.Text = "MPC Pre-search Tool";
			this.notifyIcon.MouseClick += new System.Windows.Forms.MouseEventHandler(this.notifyIcon_Click);
			// 
			// mnu
			// 
			this.mnu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuShow,
            this.mnuStopWork,
            this.mnuExit});
			this.mnu.Name = "mnu";
			this.mnu.Size = new System.Drawing.Size(142, 70);
			// 
			// mnuShow
			// 
			this.mnuShow.Name = "mnuShow";
			this.mnuShow.Size = new System.Drawing.Size(141, 22);
			this.mnuShow.Text = "显示界面(&G)";
			this.mnuShow.Click += new System.EventHandler(this.mnuShow_Click);
			// 
			// mnuStopWork
			// 
			this.mnuStopWork.Name = "mnuStopWork";
			this.mnuStopWork.Size = new System.Drawing.Size(141, 22);
			this.mnuStopWork.Text = "停止处理(&S)";
			this.mnuStopWork.Click += new System.EventHandler(this.mnuStopWork_Click);
			// 
			// mnuExit
			// 
			this.mnuExit.Name = "mnuExit";
			this.mnuExit.Size = new System.Drawing.Size(141, 22);
			this.mnuExit.Text = "退出程序(&X)";
			this.mnuExit.Click += new System.EventHandler(this.mnuExit_Click);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(393, 146);
			this.Controls.Add(this.btnExit);
			this.Controls.Add(this.btnStopWork);
			this.Controls.Add(this.pb);
			this.Controls.Add(this.lblProgress);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.lblFile);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "MainForm";
			this.Text = "Multi-Prob Cut Pre-search Tool";
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
			this.Resize += new System.EventHandler(this.MainForm_Resize);
			this.mnu.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label lblFile;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label lblProgress;
		private System.Windows.Forms.ProgressBar pb;
		private System.Windows.Forms.Button btnStopWork;
		private System.Windows.Forms.Button btnExit;
		private System.Windows.Forms.NotifyIcon notifyIcon;
		private System.Windows.Forms.ContextMenuStrip mnu;
		private System.Windows.Forms.ToolStripMenuItem mnuShow;
		private System.Windows.Forms.ToolStripMenuItem mnuStopWork;
		private System.Windows.Forms.ToolStripMenuItem mnuExit;
	}
}