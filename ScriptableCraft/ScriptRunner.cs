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
using System.IO;
using CraftEngineNet;

namespace ScriptableCraft {
	class NodeCounter {
		public TimeSpan totalTime = new TimeSpan();
		public ulong totalNodes = 0;
	}

	class ScriptRunner {
		public static char[] COMMENT_LEADERS = { '#', '%' };
		public static char[] DELIMERS = { '\t', ' ' };
		public static char[] BLACK = { 'B', 'X' };
		public static char[] WHITE = { 'W', 'O' };
		public const string END = "% End of the endgame script"; // compatibility with zebra scripts

		private static Chess parseChess(char ch) {
			ch = Char.ToUpper(ch);
			if (Array.IndexOf(BLACK, ch) >= 0) {
				return Chess.BLACK;
			} else if (Array.IndexOf(WHITE, ch) >= 0) {
				return Chess.WHITE;
			} else return Chess.AVAILABLE;
		}

		private static Chess[,] parseBoard(string board) {
			if (board.Length != Engine.MAXSTEP) {
				return null;
			}
			Chess[,] result = new Chess[Global.WIDTH, Global.HEIGHT];
			for (int i = 0; i < Global.HEIGHT; i++) {
				for (int j = 0; j < Global.WIDTH; j++) {
					result[j, i] = parseChess(board[i * Global.WIDTH + j]);
				}
			}
			return result;
		}

		private static Chess parseMover(string mover) {
			if (mover.Length != 1) {
				return Chess.AVAILABLE;
			}
			return parseChess(mover[0]);
		}

		private static string resultString(SearchResult res, bool wld, int mid) {
			if (mid != 0) {
				return res.MidEvaluationString + " @ " + mid.ToString();
			} else if (wld) {
				if (res.Evaluation > 0) {
					return "Win";
				} else if (res.Evaluation < 0) {
					return "Loss";
				} else {
					return "Draw";
				}
			} else {
				if (res.Evaluation > 0) {
					return "+" + res.Evaluation;
				} else {
					return res.Evaluation.ToString();
				}
			}
		}

		private static string moveString(SearchResult res) {
			return Conversions.moveToString(res.Move);
		}

		private static string nodesString(ulong nodes) {
			if (nodes >= 1000000000) // Gn
				return ((double)(nodes / 1000000) / 1000).ToString("0.000") + " Gn";
			else if (nodes >= 100000000)
				return ((double)(nodes / 100000) / 10).ToString("000.0") + " Mn";
			else if (nodes >= 10000000)
				return ((double)(nodes / 10000) / 100).ToString("00.00") + " Mn";
			else if (nodes >= 1000000)
				return ((double)(nodes / 1000) / 1000).ToString("0.000") + " Mn";
			else if (nodes >= 100000)
				return ((double)(nodes / 100) / 10).ToString("000.0") + " Kn";
			else if (nodes >= 10000)
				return ((double)(nodes / 10) / 100).ToString("00.00") + " Kn";
			else if (nodes >= 1000)
				return ((double)(nodes) / 1000).ToString("0.000") + " Kn";
			else return nodes + " n";
		}

		private static string npsString(TimeSpan timeSpan, ulong nodes) {
			string nps = (timeSpan.TotalMilliseconds < 1.0) ? (nodes > 0 ? "+Inf Kn/s" : "0 Kn/s")
							: ((int)(nodes / timeSpan.TotalMilliseconds) + " Kn/s");
			return nps;
		}

		private static void processBoard(string[] boardParts, TextWriter output, NodeCounter counter) {
			// check params first
			if (boardParts.Length < 2) {
				output.Write("Error: not enough arguments supplied.\t");
				return;
			}
			Chess[,] board = parseBoard(boardParts[0]);
			if (board == null) {
				output.Write("Error: board not recognized.\t");
				return;
			}

			Chess mover = parseMover(boardParts[1]);
			if (mover == Chess.AVAILABLE) {
				output.Write("Error: mover not recognized.\t");
				return;
			}

			bool wld = false, warmup = false;
			int mid = 0;
			for (int i = 2; i < boardParts.Length; i++) {
				string option = boardParts[i].ToUpper();
				if (option == "WLD")
					wld = true;
				else if (option == "WARMUP")
					warmup = true;
				else if (option.StartsWith("MID")) {
					Int32.TryParse(option.Substring(3), out mid);
					if (mid < 0) mid = 0;
				}
			}

			Engine.clearCache();
			Engine engine = new Engine();
			engine.setBoard(board);

			DateTime timeStart = DateTime.Now;
			SearchResult res = (mid == 0) ? engine.solveExact(mover, wld) : engine.solve(mover, mid, false);
			TimeSpan timeSpan = DateTime.Now - timeStart;

			SearchStats stats = engine.getSearchStats();

			engine.Dispose();

			output.Write(resultString(res, wld, mid));
			output.Write("\t");
			output.Write(moveString(res));
			output.Write("\t");
			output.Write(nodesString(stats.EvaluationCount));
			output.Write("\t");
			output.Write(npsString(timeSpan, stats.EvaluationCount));
			output.Write("\t");
			output.Write(timeSpan.TotalSeconds.ToString("0.0") + " s");
			output.Write("\t");

			if (!warmup) {
				counter.totalNodes += stats.EvaluationCount;
				counter.totalTime += timeSpan;
			}
		}

		private static void processLine(string line, TextWriter output, NodeCounter counter) {
			string comment, board;
			int first = line.IndexOfAny(COMMENT_LEADERS);
			if (first < 0) {
				comment = "";
				board = line.Trim(DELIMERS);
			} else {
				comment = line.Substring(first);
				board = line.Substring(0, first).Trim(DELIMERS);
			}
			string[] boardParts = board.Split(DELIMERS, StringSplitOptions.RemoveEmptyEntries);
			if (boardParts.Length == 0) { // we are done here
				output.WriteLine(comment);
				output.Flush();
				return;
			}

			processBoard(boardParts, output, counter);
			output.WriteLine(comment);
			output.Flush();
		}

		private static void outputCounter(NodeCounter counter, TextWriter output) {
			output.Write("Total:");
			output.Write("\t");
			output.Write("");
			output.Write("\t");
			output.Write(nodesString(counter.totalNodes));
			output.Write("\t");
			output.Write(npsString(counter.totalTime, counter.totalNodes));
			output.Write("\t");
			output.Write(counter.totalTime.TotalSeconds.ToString("0.0") + " s");
			output.WriteLine("\t");
		}

        public static void runScript(TextReader input, TextWriter output) {
			NodeCounter counter = new NodeCounter();
			string line;
			while ((line = input.ReadLine()) != null) {
				processLine(line, output, counter);
				if (line == END) break;
			}
			outputCounter(counter, output);
		}
	}
}
