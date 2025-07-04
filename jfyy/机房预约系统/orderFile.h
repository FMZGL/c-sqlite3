#pragma once
#include<iostream>
#include<map>
#include<fstream>
#include"grobalFile.h"
using namespace std;

class orderFile {
public:
	//构造函数
	orderFile();

	//更新状态
	void updateFile();

	//存储预约信息
	map<int, map<string, string>>m;

	//预约条数
	int m_size;
};