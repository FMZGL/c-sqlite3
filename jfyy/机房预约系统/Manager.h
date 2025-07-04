#pragma once
#include<iostream>
using namespace std;
#include"Identity.h"
#include<string>
#include<vector>
#include"Student.h"
#include"Teacher.h"
#include"computerRoom.h"
#include"sqlite/sqlite3.h"
class Manager : public Identity {
public:

	Manager();

	Manager(string name, string password);

	virtual void operMenu();

	void addPerson();

	void showPerson();

	void showComputer();

	void cleanFile();

	void InitVec(sqlite3* db);//��ʼ������

	void InitRoom(sqlite3* db);//��ʼ������

	bool checkRepeat(string id, int type);

	vector<Student>VStu;

	vector<Teacher>VTea;

	vector<computerRoom>vcRoom;
};