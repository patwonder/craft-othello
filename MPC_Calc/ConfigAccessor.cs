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
								ConfigAccessor.cs
		作者：Patrick
		概述：包含类 Catagory, ConfigReader 和 ConfigAccessor 的实现。
			ConfigAccessor 类封装了对配置文件的读写操作，提供易于使用的接口。

********************************************************************************
*/

using System;
using System.Collections.Generic;
using System.IO;

namespace Utility {

	/// <summary>
	/// 配置文件的一个分类
	/// </summary>
	public class Catagory {
		/// <summary>
		/// 创建一个Catagory实例
		/// </summary>
		/// <param name="name">该实例的名称</param>
		public Catagory(string name) {
			this.name = name;
			attributes = new Dictionary<string, string>();
		}
		/// <summary>
		/// 读取或设置该Catagory实例的名称
		/// </summary>
		public string Name {
			get {
				return name;
			}
			set {
				this.name = value;
			}
		}
		/// <summary>
		/// 添加一个键/值对(属性)，若键已存在，则用新值覆盖旧值
		/// </summary>
		/// <param name="key">要添加的键(属性名)</param>
		/// <param name="value">要添加的值(属性值)</param>
		public void addAttribute(string key, string value) {
			attributes[key] = value;
		}
		/// <summary>
		/// 删除一个键/值对(属性)
		/// </summary>
		/// <param name="key">要删除的键(属性)</param>
		/// <returns>若键(属性名)存在且成功删除，返回true，否则返回false</returns>
		public bool removeAttribute(string key) {
			return attributes.Remove(key);
		}
		/// <summary>
		/// 清除所有键(属性)
		/// </summary>
		public void clearAttributes() {
			attributes.Clear();
		}
		/// <summary>
		/// 获取一个键(属性名)对应的值(属性值)
		/// </summary>
		/// <param name="key">要获取的键(属性名)</param>
		/// <param name="defaultValue">指定不存在要获取的键(属性名)时，返回的默认值</param>
		/// <returns>若存在键(属性名)对应的值(属性值)，则返回该值，否则返回defaultValue</returns>
		public string getAttribute(string key, string defaultValue) {
			string value;
			if (attributes.TryGetValue(key, out value))
				return value;
			else return defaultValue;
		}
		/// <summary>
		/// 获取一个键(属性名)对应的32位有符号整数值(属性值)
		/// </summary>
		/// <param name="key">要获取的键(属性名)</param>
		/// <param name="defaultValue">指定不存在要获取的键(属性名)，或者无法将值(属性值)
		/// 解析为32位有符号整数时，返回的默认值</param>
		/// <returns>若存在键(属性名)对应的值(属性值)，且成功将其解析为32位有符号整数值，则返回该整数值，
		/// 否则返回defaultValue</returns>
		public int getIntAttribute(string key, int defaultValue) {
			string value;
			int result;
			if (attributes.TryGetValue(key, out value))
				if (Int32.TryParse(value, out result))
					return result;
			return defaultValue;
		}
		/// <summary>
		/// 获取一个键(属性名)对应的32位无符号整数值(属性值)
		/// </summary>
		/// <param name="key">要获取的键(属性名)</param>
		/// <param name="defaultValue">指定不存在要获取的键(属性名)，或者无法将值(属性值)
		/// 解析为32位无符号整数时，返回的默认值</param>
		/// <returns>若存在键(属性名)对应的值(属性值)，且成功将其解析为32位无符号整数值，则返回该整数值，
		/// 否则返回defaultValue</returns>
		public uint getUIntAttribute(string key, uint defaultValue) {
			string value;
			uint result;
			if (attributes.TryGetValue(key, out value))
				if (UInt32.TryParse(value, out result))
					return result;
			return defaultValue;
		}
		/// <summary>
		/// 获取键/值对(属性)的枚举器
		/// </summary>
		/// <returns>键/值对(属性)的枚举器</returns>
		public Dictionary<string, string>.Enumerator getEnumerator() {
			return attributes.GetEnumerator();
		}

		private string name;
		private Dictionary<string, string> attributes;

	}

	/// <summary>
	/// 配置文件读取工具，建议换用ConfigAccessor以获得写入功能
	/// </summary>
	public class ConfigReader {
		/// <summary>
		/// 构造一个新的ConfigReader实例
		/// </summary>
		public ConfigReader() {
			configFileName = "";
			catagories = new Dictionary<string, Catagory>();
		}
		/// <summary>
		/// 从指定文件中加载属性分类
		/// </summary>
		/// <param name="filePath">指定文件路径</param>
		/// <returns>若成功加载，返回true，否则返回false</returns>
		public bool readConfigFromFile(string filePath) {
			this.clearConfig();

			FileStream fs;
			try {
				fs = new FileStream(filePath, FileMode.Open, FileAccess.Read);
			} catch (Exception) {
				return false;
			}

			StreamReader reader = new StreamReader(fs);
			try {
				Catagory catagory = newCatagory("");
				while (!reader.EndOfStream) {
					string line = reader.ReadLine();
					switch (parseLine(line)) {
						case LineType.CATAGORY:
							catagory = newCatagory(parseCatagoryName(line));
							break;
						case LineType.ATTRIBUTE:
							parseAttribute(line, catagory);
							break;
						default:
							break;
					}
				}
			} catch (Exception) {
				catagories.Clear();
				return false;
			}

			reader.Close();
			configFileName = filePath;
			return true;
		}
		/// <summary>
		/// 获取键(分类名)对应的分类
		/// </summary>
		/// <param name="key">要获取的键(分类名)</param>
		/// <returns>若存在key对应的分类，返回该分类的实例，否则返回空引用(null)</returns>
		public Catagory getCatagory(string key) {
			Catagory value;
			if (catagories.TryGetValue(key, out value))
				return value;
			else return null;
		}
		/// <summary>
		/// 获取当前配置文件的路径
		/// </summary>
		/// <returns>当前配置文件的路径</returns>
		public string getConfigFileName() {
			return configFileName;
		}
		/// <summary>
		/// 清空加载的配置分类
		/// </summary>
		public void clearConfig() {
			configFileName = "";
			catagories.Clear();
		}

		private enum LineType { CATAGORY, ATTRIBUTE, RUBBISH };

		private Catagory newCatagory(string catagory) {
			Catagory newCatagory;
			if (catagories.TryGetValue(catagory, out newCatagory))
				return newCatagory;
			else {
				newCatagory = new Catagory(catagory);
				catagories.Add(catagory, newCatagory);
				return newCatagory;
			}
		}

		private LineType parseLine(string line) {
			line = line.Trim('\t', ' ');
			if (line.Length >= 1 && line[0] == '#') return LineType.RUBBISH;
			if (line.Length > 2 && line[0] == '[' && line[line.Length - 1] == ']'
				&& isIdentifier(line.Substring(1, line.Length - 2).Trim('\t', ' ')))
				return LineType.CATAGORY;
			int eqIdx = line.IndexOf('=');
			if (eqIdx > 0 && isIdentifier(line.Substring(0, eqIdx).Trim('\t', ' ')))
				return LineType.ATTRIBUTE;
			return LineType.RUBBISH;
		}

		private string parseCatagoryName(string line) {
			line = line.Trim('\t', ' ');
			return line.Substring(1, line.Length - 2).Trim('\t', ' ');
		}
		private void parseAttribute(string line, Catagory catagory) {
			int eqIdx = line.IndexOf('=');
			string key = line.Substring(0, eqIdx).Trim('\t', ' ');
			string value = line.Substring(eqIdx + 1).Trim('\t', ' ');
			if (value[0] == '\"' && value[value.Length - 1] == '\"')
				value = value.Substring(1, value.Length - 2);
			catagory.addAttribute(key, value);
		}
		private bool isIdentifier(string id) {
			if (id == null) return false;
			if (id.Length == 0) return false;
			if (!(id[0] == '_' || (id[0] >= 'a' && id[0] <= 'z')
				|| (id[0] >= 'A' && id[0] <= 'Z')))
				return false;
			for (int i = 1; i < id.Length; i++) {
				if (!(id[i] == '_' || (id[i] >= 'a' && id[i] <= 'z')
					|| (id[i] >= 'A' && id[i] <= 'Z') || (id[i] >= '0' && id[i] <= '9')))
					return false;
			}
			return true;
		}

		private string configFileName;
		private Dictionary<string, Catagory> catagories;
	}

	/// <summary>
	/// 配置文件读写工具
	/// </summary>
	public class ConfigAccessor {
		/// <summary>
		/// 构造一个新的ConfigAccessor实例
		/// </summary>
		public ConfigAccessor() {
			configFileName = "";
			catagories = new Dictionary<string, Catagory>();
		}
		/// <summary>
		/// 从指定文件中加载属性分类
		/// </summary>
		/// <param name="filePath">指定文件路径</param>
		/// <returns>若成功加载，返回true，否则返回false</returns>
		public bool readConfigFromFile(string filePath) {
			this.clearConfig();

			FileStream fs;
			try {
				fs = new FileStream(filePath, FileMode.Open, FileAccess.Read);
			} catch (Exception) {
				return false;
			}

			StreamReader reader = new StreamReader(fs);
			try {
				Catagory catagory = newCatagory("");
				while (!reader.EndOfStream) {
					string line = reader.ReadLine();
					switch (parseLine(line)) {
						case LineType.CATAGORY:
							catagory = newCatagory(parseCatagoryName(line));
							break;
						case LineType.ATTRIBUTE:
							parseAttribute(line, catagory);
							break;
						default:
							break;
					}
				}
			} catch (Exception) {
				catagories.Clear();
				return false;
			} finally {
				reader.Close();
			}

			configFileName = filePath;
			return true;
		}

		/// <summary>
		/// 将属性分类写入当前读取的文件中
		/// </summary>
		/// <returns>若成功写入，返回true，否则返回false</returns>
		public bool writeConfigToFile() {
			if (configFileName == "")
				return false;
			return writeConfigToFile(configFileName);
		}

		private static void writeCatagory(StreamWriter writer, Catagory current) {
			Dictionary<string, string>.Enumerator enumAttr =
				current.getEnumerator();
			while (enumAttr.MoveNext()) {
				writer.WriteLine("{0} = \"{1}\"",
					enumAttr.Current.Key, enumAttr.Current.Value);
			}
		}
		/// <summary>
		/// 将属性分类写入指定的文件中
		/// </summary>
		/// <param name="filename">指定的文件路径</param>
		/// <returns>若成功写入，返回true，否则返回false</returns>
		public bool writeConfigToFile(string filename) {
			FileStream fs;
			try {
				fs = new FileStream(filename, FileMode.Create, FileAccess.Write);
			} catch (Exception) {
				return false;
			}

			StreamWriter writer = new StreamWriter(fs);
			try {
				Catagory defaultCatagory;
				if (catagories.TryGetValue("", out defaultCatagory)) {
					writeCatagory(writer, defaultCatagory);
					writer.WriteLine();
				}
				Dictionary<string, Catagory>.Enumerator enumerator =
					catagories.GetEnumerator();
				while (enumerator.MoveNext()) {
					Catagory current = enumerator.Current.Value;
					if (current.Name != "") {
						writer.WriteLine("[{0}]", current.Name);
						writeCatagory(writer, current);
						writer.WriteLine();
					}
				}
			} catch (Exception) {
				return false;
			} finally {
				writer.Close();
			}

			return true;
		}
		/// <summary>
		/// 加入一个键(分类名)/分类对，若键已存在，则用新分类覆盖旧分类
		/// </summary>
		/// <param name="key">要添加的键(分类名)</param>
		/// <param name="value">要添加的分类</param>
		public void addCatagory(string key, Catagory value) {
			catagories[key] = value;
		}
		/// <summary>
		/// 从配置中移除一个分类
		/// </summary>
		/// <param name="key">要移除的键(分类名)</param>
		/// <returns>若存在该分类，且成功移除，返回true，否则返回false</returns>
		public bool removeCatatory(string key) {
			return catagories.Remove(key);
		}
		/// <summary>
		/// 获取键(分类名)对应的分类
		/// </summary>
		/// <param name="key">要获取的键(分类名)</param>
		/// <returns>若存在key对应的分类，返回该分类的实例，否则返回空引用(null)</returns>
		public Catagory getCatagory(string key) {
			Catagory value;
			if (catagories.TryGetValue(key, out value))
				return value;
			else return null;
		}
		/// <summary>
		/// 获取当前配置文件的路径
		/// </summary>
		/// <returns>当前配置文件的路径</returns>
		public string getConfigFileName() {
			return configFileName;
		}
		/// <summary>
		/// 清空加载的配置分类
		/// </summary>
		public void clearConfig() {
			configFileName = "";
			catagories.Clear();
		}

		private enum LineType { CATAGORY, ATTRIBUTE, RUBBISH };

		private Catagory newCatagory(string catagory) {
			Catagory newCatagory;
			if (catagories.TryGetValue(catagory, out newCatagory))
				return newCatagory;
			else {
				newCatagory = new Catagory(catagory);
				catagories.Add(catagory, newCatagory);
				return newCatagory;
			}
		}

		private LineType parseLine(string line) {
			line = line.Trim('\t', ' ');
			if (line.Length >= 1 && line[0] == '#') return LineType.RUBBISH;
			if (line.Length > 2 && line[0] == '[' && line[line.Length - 1] == ']'
				&& isIdentifier(line.Substring(1, line.Length - 2).Trim('\t', ' ')))
				return LineType.CATAGORY;
			int eqIdx = line.IndexOf('=');
			if (eqIdx > 0 && isIdentifier(line.Substring(0, eqIdx).Trim('\t', ' ')))
				return LineType.ATTRIBUTE;
			return LineType.RUBBISH;
		}

		private string parseCatagoryName(string line) {
			line = line.Trim('\t', ' ');
			return line.Substring(1, line.Length - 2).Trim('\t', ' ');
		}
		private void parseAttribute(string line, Catagory catagory) {
			int eqIdx = line.IndexOf('=');
			string key = line.Substring(0, eqIdx).Trim('\t', ' ');
			string value = line.Substring(eqIdx + 1).Trim('\t', ' ');
			if (value[0] == '\"' && value[value.Length - 1] == '\"')
				value = value.Substring(1, value.Length - 2);
			catagory.addAttribute(key, value);
		}
		private bool isIdentifier(string id) {
			if (id == null) return false;
			if (id.Length == 0) return false;
			if (!(id[0] == '_' || (id[0] >= 'a' && id[0] <= 'z')
				|| (id[0] >= 'A' && id[0] <= 'Z')))
				return false;
			for (int i = 1; i < id.Length; i++) {
				if (!(id[i] == '_' || (id[i] >= 'a' && id[i] <= 'z')
					|| (id[i] >= 'A' && id[i] <= 'Z') || (id[i] >= '0' && id[i] <= '9')))
					return false;
			}
			return true;
		}

		private string configFileName;
		private Dictionary<string, Catagory> catagories;
	}
}
