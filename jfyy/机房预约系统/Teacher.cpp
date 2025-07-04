#include"Teacher.h"

//默认构造
Teacher::Teacher() {

}

//有参构造
Teacher::Teacher(string empId, string name, string password) {
	this-> m_empId = empId;
	this->m_Name = name;
	this->m_Password = password;
	//获取机房的信息
	sqlite3* db;
	char* errmeg = 0;
	int rc;
	rc = sqlite3_open("resource/resource.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(1);
	}
	this->InitRoom(db);
	sqlite3_close(db);
}
void Teacher::InitRoom(sqlite3* db) {
	vcRoom.clear();
	const char* sql = "SELECT RID, Rsize FROM computerRoom;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "机房SQL准备失败: " << sqlite3_errmsg(db) << endl;
		return;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		computerRoom com;
		com.m_id = sqlite3_column_int(stmt, 0);
		com.m_room = sqlite3_column_int(stmt, 1);
		vcRoom.push_back(com);
	}

	sqlite3_finalize(stmt);

}
//菜单界面
void Teacher::operMenu() {
	cout << "欢迎教师用户：" << this->m_Name << "登录" << endl;
	cout << "\t\t ---------------------------------------\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              1, 查看预约              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              2, 审核预约              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              0, 注    销              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t ---------------------------------------\n";
}

//查看所有预约
void Teacher::showAllOrder() {
	sqlite3* db;
	char* errmeg = 0;
	int rc;
	rc = sqlite3_open("resource/resource.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	const char* sql = "SELECT * FROM order_table;";
	sqlite3_stmt* stmt = nullptr;
	// 准备 SQL 语句
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "准备插入语句失败: " << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string date, intv, SID, RID, Status;
		if (sqlite3_column_int(stmt, 0) == 1)date = "Mon";
		if (sqlite3_column_int(stmt, 0) == 2)date = "Tues";
		if (sqlite3_column_int(stmt, 0) == 3)date = "Wed";
		if (sqlite3_column_int(stmt, 0) == 4)date = "Thur";
		if (sqlite3_column_int(stmt, 0) == 5)date = "Fri";
		if (sqlite3_column_int(stmt, 0) == 6)date = "Sat";
		if (sqlite3_column_int(stmt, 0) == 7)date = "Sun";
		if (sqlite3_column_int(stmt, 1) == 1)intv = "上午";
		if (sqlite3_column_int(stmt, 1) == 2)intv = "下午";
		RID = (const char*)sqlite3_column_text(stmt, 2);
		SID = (const char*)sqlite3_column_text(stmt, 3);
		Status = (const char*)sqlite3_column_text(stmt, 4);
		cout << "日期：" << date << " 时间段：" << intv << " 机房编号：" << RID << " 学生学号：" << SID << " 审核状态：" << Status << endl;
	}
	sqlite3_finalize(stmt); // 释放资源
	sqlite3_close(db);
	system("pause");
	system("cls");
}

//审核预约
void Teacher::validOrder() {
	sqlite3* db;
	char* errmeg = 0;
	int rc;
	rc = sqlite3_open("resource/resource.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	const char* sql = "UPDATE order_table SET Status='Accept' WHERE Status='Waiting';";
	sqlite3_stmt* stmt = nullptr;
	// 准备 SQL 语句
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "准备插入语句失败: " << sqlite3_errmsg(db) << endl;
		return;
	}
	if (sqlite3_step(stmt) == SQLITE_DONE) {
		cout << "一键审核完成" << endl;
	}
	sqlite3_finalize(stmt); // 释放资源
	sqlite3_close(db);
	system("pause");
	system("cls");
}