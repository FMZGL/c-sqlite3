#pragma once
#include<iostream>
#include<map>
#include<fstream>
#include"grobalFile.h"
using namespace std;

class orderFile {
public:
	//���캯��
	orderFile();

	//����״̬
	void updateFile();

	//�洢ԤԼ��Ϣ
	map<int, map<string, string>>m;

	//ԤԼ����
	int m_size;
};