#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include"Identity.h";
#include<string>
#include"sqlite/sqlite3.h"
class Teacher :public Identity {
public:

	//默认构造
	Teacher();

	//有参构造
	Teacher(string empId, string name, string password);

	//菜单界面
	virtual void operMenu();

	//查看所有预约
	void showAllOrder();

	//审核预约
	void validOrder();
	void InitRoom(sqlite3* db);
	//
	string m_empId;
	//机房信息
	vector<computerRoom>vcRoom;

};