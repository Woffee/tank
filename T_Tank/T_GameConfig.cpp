#include "T_GameConfig.h"
#include "T_Config.h"
#include <fstream>
#include <iostream>
#include "stdio.h"
#include <string>
using namespace std;

#define COMMENT_CHAR '#'



bool IsSpace(char c)
{
	if (' ' == c || '\t' == c)
		return true;
	return false;
}

bool IsCommentChar(char c)
{
	switch (c) {
	case COMMENT_CHAR:
		return true;
	default:
		return false;
	}
}

// 去掉头尾的空白符
string Trim(string & str)
{
	if (str.empty()) {
		return "";
	}
	int i, start_pos, end_pos;
	for (i = 0; i < str.size(); ++i) {
		if (!IsSpace(str[i])) {
			break;
		}
	}
	if (i == str.size()) { // 全部是空白字符串
		str = "";
		return "";
	}

	start_pos = i;

	for (i = str.size() - 1; i >= 0; --i) {
		if (!IsSpace(str[i])) {
			break;
		}
	}
	end_pos = i;

	return str.substr(start_pos, end_pos - start_pos + 1);
}


int AnalyseLine(const string & line, string & key, string & value)
{
	if (line.empty())
		return -1;
	int start_pos = 0, end_pos = line.size() - 1, pos;
	if ((pos = line.find(COMMENT_CHAR)) != -1) {
		if (0 == pos) {  // 行的第一个字符就是注释字符
			return -1;
		}
		end_pos = pos - 1;
	}
	string new_line = line.substr(start_pos, start_pos + 1 - end_pos);  // 预处理，删除注释部分
	if ((pos = new_line.find('=')) != -1)
	{
		key = new_line.substr(0, pos);
		value = new_line.substr(pos + 1, end_pos + 1 - (pos + 1));

		key = Trim(key);
		if (key.empty()) {
			return -1;
		}
		value = Trim(value);
		return CONFIG_KEY_VALUE;
	}

	if ((pos = new_line.find('[')) != -1 && (end_pos = new_line.find(']')) != -1)
	{

		key = new_line.substr(pos + 1, end_pos-pos-1);
		key = Trim(key);
		if (key.empty()) return -1;
		return CONFIG_TYPE;
	}
	return CONFIG_LONG_VALUE;
}

bool T_GameConfig::readConfig(const string & filePath)
{

	ifstream infile;
	infile.open(filePath);   //将文件流对象与文件连接起来 
	assert(infile.is_open());                            //若失败,则输出错误消息,并终止程序运行 

	string line="", key="", value="";
	int index = -1;
	while (getline(infile, line)) {
		if (index+1 >= CONFIG_SIZE)break;
		int ret = AnalyseLine(line, key, value);
		if (ret == CONFIG_TYPE) {
			index++;
			configs[index].type = key;
		}
		else if (index >= 0 && ret == CONFIG_KEY_VALUE) {
			configs[index].data[key] = value;
		}
		else if (index >= 0 && ret == CONFIG_LONG_VALUE) {
			configs[index].data[key] += line;
		}
	}

	infile.close();
	return true;
}


void processMapData(vector< vector<int> > & layer_data,string str_map, const int ROWS, const int COLS)
{
	int map_data[50 * 50];
	int num = -1, index = 0, len = str_map.length();
	for (int i = 0; i < len; ++i)
	{
		if (str_map[i] >= '0' && str_map[i] <= '9')
		{
			if (num == -1)num = 0;
			num = num * 10 + str_map[i] - '0';
		}
		if (str_map[i] == ',' || i == len - 1)
		{
			map_data[index++] = num;
			num = 0;
		}
	}

	for (int i = 0; i < index; ++i)
	{
		int col = i%COLS;
		int row = i / COLS;
		layer_data[row][col] = map_data[i];
	}
}

void T_GameConfig:: getLayerData(const int type, vector< vector<int> > & layer_data, const int ROWS, const int COLS)
{
	switch (type)
	{
	case LAYER_MAP_BACK:
		for (int i = 0; i < CONFIG_SIZE; ++i)
		{
			if ("layer" == configs[i].type && "background" == configs[i].data["type"])
			{
				processMapData(layer_data, configs[i].data["data"], ROWS, COLS);
			}
		}
		break;
	case LAYER_MAP_BARR:
		for (int i = 0; i < CONFIG_SIZE; ++i)
		{
			if ("layer" == configs[i].type && "wall"== configs[i].data["type"] )
			{
				processMapData(layer_data,configs[i].data["data"],  ROWS, COLS);
			}
		}
		break;
	default:
		break;
	}
}