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

namespace ScriptableCraft {
	class Program {
		public const int HASH_BITS = 21; // 128MB

		private static void showUsage() {
			Console.WriteLine("Usage: scrCraft <script file> [-o <output file>] [-h <hash bits[16..23]>]");
			Console.WriteLine("   or: scrCraft <script file> [--output <output file>] [--hash <hash bits[16..23]>]");
			Console.WriteLine(" <script file>: specify the script file to run");
			Console.WriteLine(" -o, --output : specify the output file, the default is standard output");
			Console.WriteLine(" -h, --hash   : specify the hash table bits, default is 21");
			Console.WriteLine("                hash table size is 64 * (1 << hashbits), listed as follows:");
			Console.WriteLine("                  hashbits    table size");
			Console.WriteLine("                     16           4 MB");
			Console.WriteLine("                     17           8 MB");
			Console.WriteLine("                     18          16 MB");
			Console.WriteLine("                     19          32 MB");
			Console.WriteLine("                     20          64 MB");
			Console.WriteLine("                     21         128 MB");
			Console.WriteLine("                     22         256 MB");
			Console.WriteLine("                     23         512 MB");
		}
		private static void runScript(string source, string output) {
			FileStream infs;
			try {
				infs = new FileStream(source, FileMode.Open, FileAccess.Read);
			} catch (Exception) {
				Console.WriteLine("Error: cannot open script file \"" + source + "\"");
				return;
			}

			using (TextReader tr = new StreamReader(infs)) {
				TextWriter tw;
				if (output != null) {
					FileStream outfs;
					try {
						outfs = new FileStream(output, FileMode.Create, FileAccess.Write);
					} catch (Exception) {
						Console.WriteLine("Error: cannot open output file \"" + output + "\"");
						return;
					}
					tw = new StreamWriter(outfs);
				} else {
					tw = Console.Out;
				}
				try {
					ScriptRunner.runScript(tr, tw);
				} finally {
					if (output != null) {
						tw.Close();
					}
				}
			}
		}
		static void Main(string[] args) {
			if (args.Length == 0) {
				showUsage();
				return;
			}
			string source = null, output = null, reason = null;
			int hashbits = 0;
			bool successful = true;
			for (int i = 0; i < args.Length; i++) {
				if (args[i] == "-o" || args[i] == "--output") {
					if (output == null && i < args.Length - 1) {
						output = args[++i];
					} else {
						successful = false;
						reason = (output != null) ? "duplicate output files" : "no output file";
						break;
					}
				} else if (args[i] == "-h" || args[i] == "--hash") {
					if (hashbits == 0 && i < args.Length - 1) {
						try {
							hashbits = Int32.Parse(args[++i]);
							if (hashbits > 23 || hashbits < 16) {
								hashbits = 0;
								successful = false;
								reason = "hash bits out of range [16..23]";
							}
						} catch (Exception) {
							hashbits = 0;
							successful = false;
							reason = "hash bits error";
						}
					} else {
						successful = false;
						reason = (hashbits != 0) ? "duplicate hash bits" : "no hash bits";
						break;
					}
				} else {
					if (source == null) {
						source = args[i];
					} else {
						successful = false;
						reason = "duplicate script files";
						break;
					}
				}
			}
			if (source == null) {
				successful = false;
				reason = "no script file";
			}
			if (!successful) {
				Console.WriteLine("Error: " + reason);
				return;
			}
			CraftEngineNet.Engine.initialize();
			if (hashbits == 0) hashbits = HASH_BITS;
			CraftEngineNet.Engine.setCacheSize(1u << hashbits);
			runScript(source, output);
		}
	}
}
