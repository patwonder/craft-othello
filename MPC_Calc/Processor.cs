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

using System;
using System.Collections.Generic;
using System.Text;
using CraftEngineNet;
using System.Windows.Forms;
using System.Threading;
using System.IO;

namespace MPC_Calc {
	class Processor : IProcessor {
		private Task task;
		private Engine engine;
		private ProcessFinishCallback finishCallback;
		private ProcessProgressCallback progressCallback;

		private Thread processThread;
		private Random rnd;
		private volatile bool terminate;

		const int MIN_END_EMPTIES = 16;
		const int MAX_END_EMPTIES = 25;
		const int MIN_MID_EMPTIES = 3;
		const int MAX_MID_EMPTIES = 17;
		const int MIN_PROCESS_EMPTIES = MIN_END_EMPTIES;
		const int MAX_PROCESS_EMPTIES = 52;

		static Processor() {
			try {
				Engine.initialize();
				Engine.setCacheSize(1 << 21); // 128MB
			} catch (InitializationException e) {
				Console.WriteLine(e.StackTrace);
				MessageBox.Show("Failed to initialize craft engine, quitting...", "Fatal Error", MessageBoxButtons.OK,
					MessageBoxIcon.Stop);
				Application.Exit();
			}
		}

		public Processor(Task task) {
			this.task = task;
			rnd = new Random();
		}

		public Task Task {
			get {
				return task;
			}
		}

		private void writeHeader(TextWriter tw) {
			tw.Write("Empties");

			for (int i = MIN_MID_EMPTIES; i <= MAX_MID_EMPTIES; i++) {
				tw.Write('\t');
				tw.Write(i);
			}
			tw.WriteLine("\tEND");
			tw.Flush();
		}

		private Chess[,] getBoard(ulong black, ulong white) {
			Chess[,] board = new Chess[8, 8];
			ulong mask = 1;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					board[i, j] = ((black & mask) != 0 ? Chess.BLACK : 
						((white & mask) != 0 ? Chess.WHITE : Chess.AVAILABLE));
					mask <<= 1;
				}
			}
			return board;
		}

		private int getEmpties(Chess[,] board) {
			int empties = 0;
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
					if (board[i, j] == Chess.AVAILABLE)
						empties++;
			return empties;
		}

		private bool isRegularEval(int eval) {
			return (eval < Global.INFINITY - 64 && eval > -Global.INFINITY + 64);
		}

		private void processRecord(TextWriter tw, ulong black, ulong white) {
			Chess[,] board = getBoard(black, white);
			int empties = getEmpties(board);
			if (empties >= MIN_PROCESS_EMPTIES && empties <= MAX_PROCESS_EMPTIES) {
				List<int> result = new List<int>();

				engine.setBoard(board);
				for (int mid = MIN_MID_EMPTIES; mid <= MAX_MID_EMPTIES; mid++) {
					if (empties <= mid) { // avoid searching a 16 empty pos to depth 17
						result.Add(Global.INFINITY);
						continue;
					}
					SearchResult res = engine.solve(Chess.BLACK, mid, false);
					if (terminate) return;
					result.Add(res.Evaluation);
				}

				if (empties >= MIN_END_EMPTIES && empties <= MAX_END_EMPTIES) {
					SearchResult res = engine.solveExact(Chess.BLACK, false);
					if (terminate) return;
					result.Add(res.Evaluation);
				}

				tw.Write(empties);

				for (int i = 0; i < result.Count; i++) {
					tw.Write('\t');
					tw.Write(isRegularEval(result[i]) ? result[i].ToString() : "-");
				}
				tw.WriteLine();
				tw.Flush();
			}
		}

		private int stepForward() {
			return rnd.Next(1, 20);
		}

        private void processThreadFunc() {
			try {
				using (FileStream infs = new FileStream(task.FileName, FileMode.Open, FileAccess.Read)) {

					String outFileName = Path.GetFileNameWithoutExtension(task.FileName) + ".txt";
					bool printHeader = false;

					FileStream outfs = null;
					try {
						if (File.Exists(outFileName)) {
							outfs = new FileStream(outFileName, FileMode.Append, FileAccess.Write);
						} else {
							outfs = new FileStream(outFileName, FileMode.Create, FileAccess.Write);
							printHeader = true;
						}

						BinaryReader inbr = new BinaryReader(infs);
						TextWriter outtw = new StreamWriter(outfs);

						if (printHeader) {
							writeHeader(outtw);
						}

						int total = inbr.ReadInt32();
						progressCallback(task.Process, total);

						int calculated = 0;
						for (int current = task.Process; current < total; current += stepForward()) {
							task.Process = current;
							progressCallback(current, total);
							infs.Seek(24 * current + 4, SeekOrigin.Begin);
							int nouse = inbr.ReadInt32();
							ulong black = inbr.ReadUInt64();
							ulong white = inbr.ReadUInt64();
							nouse = inbr.ReadInt32();
							processRecord(outtw, black, white);
							if (terminate) return;
							if (++calculated == 50) {
								Engine.clearCache();
								calculated = 0;
							}
						}
						task.Process = total;
						progressCallback(task.Process, total);
					} finally {
						outfs.Close();
					}
				}
			} catch (Exception e) {
				MessageBox.Show(e.Message, "Fatal Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			} finally {
				finishCallback();
			}
		}

		#region IProcessor 成员


		public void startProcess() {
			engine = new Engine();
			terminate = false;
			processThread = new Thread(processThreadFunc);
			processThread.Priority = ThreadPriority.Lowest;
			processThread.Start();
		}

		public void stopProcess() {
			terminate = true;
			engine.abortSearch(processThread);
		}

		public void setProcessFinishCallback(ProcessFinishCallback callback) {
			finishCallback = callback;
		}

		public void setProcessProgressCallback(ProcessProgressCallback callback) {
			progressCallback = callback;
		}

		#endregion
	}
}
