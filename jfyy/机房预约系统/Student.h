#pragma once
#include<iostream>
using namespace std;
#include"Identity.h";
#include<vector>
#include<fstream>
#include"orderFile.h"
#include<string>
#include"sqlite/sqlite3.h"
//ѧ����
class Student : public Identity {

public:
	// Ĭ�Ϲ���
	Student();
	
	//���ι���
	Student(string id, string name, string password);

	//�˵�ҳ��
	virtual void operMenu();

	//����ԤԼ
	void applyOrder();

	//�鿴����ԤԼ
	void showOrder();

	//�鿴����ԤԼ
	void showAllOrder();

	//ȡ��ԤԼ
	void cancelOrder();

	void InitRoom(sqlite3* db);
	//id
	string id;

	//������Ϣ
	vector<computerRoom>vcRoom;
};