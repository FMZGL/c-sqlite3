#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include"Identity.h";
#include<string>
#include"sqlite/sqlite3.h"
class Teacher :public Identity {
public:

	//Ĭ�Ϲ���
	Teacher();

	//�вι���
	Teacher(string empId, string name, string password);

	//�˵�����
	virtual void operMenu();

	//�鿴����ԤԼ
	void showAllOrder();

	//���ԤԼ
	void validOrder();
	void InitRoom(sqlite3* db);
	//
	string m_empId;
	//������Ϣ
	vector<computerRoom>vcRoom;

};