#pragma once
#include<iostream>
using namespace std;
#include"Identity.h";
#include<vector>
#include<fstream>
#include"orderFile.h"
#include<string>
#include"sqlite/sqlite3.h"
//学生类
class Student : public Identity {

public:
	// 默认构造
	Student();
	
	//含参构造
	Student(string id, string name, string password);

	//菜单页面
	virtual void operMenu();

	//申请预约
	void applyOrder();

	//查看自身预约
	void showOrder();

	//查看所有预约
	void showAllOrder();

	//取消预约
	void cancelOrder();

	void InitRoom(sqlite3* db);
	//id
	string id;

	//机房信息
	vector<computerRoom>vcRoom;
};