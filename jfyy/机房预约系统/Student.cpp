#include"Student.h";


// 默认构造
Student::Student() {

}

//含参构造
Student::Student(string id, string name, string password) {
	this->id = id;
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
void Student::InitRoom(sqlite3* db) {
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
//菜单页面
void Student::operMenu() {
	cout << "欢迎学生用户：" << this->m_Name << "登录" << endl;
	cout << "\t\t ---------------------------------------\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              1, 申请预约              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              2, 显示自身预约          |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              3, 显示所有预约          |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              4, 取消预约              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              0, 注    销              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t ---------------------------------------\n";
}

//申请预约
void Student::applyOrder() {
	int date = 0;
	int interval = 0;
	string room;
	cout << "机房的开放时间为周一至周五" << endl;
	cout << "请输入您要预约的日期：" << endl;
	cout << "1.周一" << endl;
	cout << "2.周二" << endl;
	cout << "3.周三" << endl;
	cout << "4.周四" << endl;
	cout << "5.周五" << endl;
	while (true) {
		cin >> date;
		if (date >= 1 && date <= 5) {
			break;
		}
		else {
			cout << "输入有误请重新输入" << endl;
		}
	}

	cout << "请输入您要预约的时间段：" << endl;
	cout << "1.上午" << endl;
	cout << "2.下午" << endl;
	while (true) {
		cin >> interval;
		if (interval>=1&&interval<=2) {
			break;
		}
		else {
			cout << "输入有误，请重新输入" << endl;
		}
	}

	cout << "机房信息为：" << endl;
	for (vector<computerRoom>::iterator it = vcRoom.begin(); it != vcRoom.end(); it++) {
		cout << "机房编号：" << it->m_id << " " << "最大容量为：" << it->m_room << endl;
	}
	cout << "请输入你要选择的机房：";
	cin >> room;
	cout << "提交成功，审核中" << endl;

	//打开数据库
	sqlite3* db;
	char* errmeg = 0;
	int rc;
	rc = sqlite3_open("resource/resource.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	const char* sql;
	sql = "INSERT INTO order_table(Date,Interval,RID,StuID,Status)VALUES(?,?,?,?,?);";
	sqlite3_stmt* stmt = nullptr;

	// 准备 SQL 语句
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "准备插入语句失败: " << sqlite3_errmsg(db) << endl;
		return;
	}

	// 绑定参数（注意都是字符串）
	sqlite3_bind_int(stmt, 1, date);
	sqlite3_bind_int(stmt, 2, interval);
	sqlite3_bind_text(stmt, 3, room.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, this->id.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 5, "Waiting", -1, SQLITE_STATIC);
	// 执行插入
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		cerr << "插入失败: " << sqlite3_errmsg(db) << endl;
	}

	sqlite3_finalize(stmt); // 释放资源
	sqlite3_close(db);
	system("pause");
	system("cls");
}
static int callback(void* data, int argc, char** argv, char** azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//查看自身预约
void Student::showOrder() {
	sqlite3* db;
	char* errmeg = 0;
	int rc;
	rc = sqlite3_open("resource/resource.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	const char* sql = "SELECT * FROM order_table WHERE StuID=?;";
	sqlite3_stmt* stmt = nullptr;
	// 准备 SQL 语句
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "准备插入语句失败: " << sqlite3_errmsg(db) << endl;
		return;
	}
	// 绑定参数（注意都是字符串）
	sqlite3_bind_text(stmt, 1,this->id.c_str(), -1, SQLITE_STATIC);
	if (sqlite3_step(stmt) == SQLITE_ROW) {
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
		Status= (const char*)sqlite3_column_text(stmt, 4);
		cout << "日期：" << date << " 时间段：" << intv << " 机房编号：" << RID << " 学生学号：" << SID << " 审核状态：" << Status << endl;
	}
	sqlite3_finalize(stmt); // 释放资源
	sqlite3_close(db);
	system("pause");
	system("cls");
}

//查看所有预约
void Student::showAllOrder() {
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

//取消预约
void Student::cancelOrder() {
	sqlite3* db;
	char* errmeg = 0;
	int rc;
	rc = sqlite3_open("resource/resource.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	const char* sql = "DELETE FROM order_table WHERE StuID=?;";
	sqlite3_stmt* stmt = nullptr;
	// 准备 SQL 语句
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "准备插入语句失败: " << sqlite3_errmsg(db) << endl;
		return;
	}
	// 绑定参数（注意都是字符串）
	sqlite3_bind_text(stmt, 1, this->id.c_str(), -1, SQLITE_STATIC);
	if (sqlite3_step(stmt) == SQLITE_DONE) {
		cout << "预约记录删除成功" << endl;
	}
	sqlite3_finalize(stmt); // 释放资源
	sqlite3_close(db);
	system("pause");
	system("cls");
}