#pragma once
#include <iostream>
#include "fstream"
#include "stdio.h"
#include <vector>
#include <map>
#include "iostream"
#include <cassert>
using namespace std;

typedef map<string, string> Layer;

const int CONFIG_SIZE = 5;

enum {
	CONFIG_TYPE = 0,
	CONFIG_KEY_VALUE,
	CONFIG_LONG_VALUE
};

struct Config {
	string type = "";
	map<string, string> data;
};

class T_GameConfig
{
	
	Config configs[CONFIG_SIZE];

public:
	bool readConfig(const string & filePath);
	void getLayerData(const int type, vector< vector<int> >& layer_data, const int ROWS, const int COLS);
};