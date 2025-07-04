#include"Manager.h"
#include"grobalFile.h"
#include<fstream>
#include"sqlite/sqlite3.h"
Manager::Manager() {

}

Manager::Manager(string name, string password) {
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
	this->InitVec(db);
	sqlite3_close(db);
}
void Manager::InitRoom(sqlite3* db) {
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
void Manager::InitVec(sqlite3* db) {
	VStu.clear();
	VTea.clear();

	const char* sql = "SELECT ID,USERNAME,PASSWORD FROM student";
	sqlite3_stmt* stmt;

	// 预处理 SQL 语句
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "准备 SQL 语句失败: " << sqlite3_errmsg(db) << endl;
		return;
	}

	// 执行查询并获取每一行
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		Student stu;
		stu.id = (const char*)sqlite3_column_text(stmt, 0);
		stu.m_Name= (const char*)sqlite3_column_text(stmt, 1);
		stu.m_Password = (const char*)sqlite3_column_text(stmt, 2);
		VStu.push_back(stu);
	}

	sqlite3_finalize(stmt); // 释放资源

	sql = "SELECT ID,USERNAME,PASSWORD FROM teacher";
	stmt = nullptr;
	// 预处理 SQL 语句
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "准备 SQL 语句失败: " << sqlite3_errmsg(db) << endl;
		return;
	}

	// 执行查询并获取每一行
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		Teacher tea;
		tea.m_empId = (const char*)sqlite3_column_text(stmt, 0);
		tea.m_Name = (const char*)sqlite3_column_text(stmt, 1);
		tea.m_Password = (const char*)sqlite3_column_text(stmt, 2);
		VTea.push_back(tea);
	}

	sqlite3_finalize(stmt); // 释放资源

}

void Manager::operMenu() {
	cout << "欢迎管理员："<< this->m_Name << "登录" << endl;
	cout << "\t\t ---------------------------------------\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              1, 添加账号              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              2, 查看账号              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              3, 查看机房              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              4, 清空预约              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              0, 注    销              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t ---------------------------------------\n";
}

bool Manager::checkRepeat(string id, int type) {
	if (type == 1) {
		for (vector<Student>::iterator it = VStu.begin(); it != VStu.end(); it++) {
			if (it->id == id)return true;
		}
	}
	else if (type == 2) {
		for (vector<Teacher>::iterator it = VTea.begin(); it != VTea.end(); it++) {
			if (it->m_empId == id)return true;
		}
	}
	return false;
	return 0;
}

void Manager::addPerson() {
	cout << "请输入需要添加账号的类型" << endl;
	cout << "1. 添加学生账号" << endl;
	cout << "2. 添加教师账号" << endl;
	string fileName;
	string tip;
	ofstream ofs;
	string errtip;
	int select;
	cin >> select;
	if (select == 1) {
		tip = "请输入学生的学号";
		errtip = "****!学生学号已存在，请重新输入***";
	}
	else {
		tip = "请输入教师的职工号";
		errtip = "***!教师职工号已存在，请重新输入***";
	}

	string id;
	string name;
	string password;

	while (true) {
		cout << tip << endl;
		cin >> id;
		cout << "请输入用户名" << endl;
		cin >> name;
		cout << "请输入密码" << endl;
		cin >> password;
		if (checkRepeat(id, select)) {
			cout << errtip << endl;
			cout << endl;
		}
		else {
			break;
		}

	}
	sqlite3* db;
	char* errmeg = 0;
	int rc;
	rc = sqlite3_open("resource/resource.db", &db);
	
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	
	const char* sql;
	if(select==1)sql= "INSERT INTO student (ID, USERNAME, PASSWORD) VALUES (?, ?, ?)";
	else sql= "INSERT INTO teacher (ID, USERNAME, PASSWORD) VALUES (?, ?, ?)";
	sqlite3_stmt* stmt = nullptr;

	// 准备 SQL 语句
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "准备插入语句失败: " << sqlite3_errmsg(db) << endl;
		return;
	}

	// 绑定参数（注意都是字符串）
	sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);

	// 执行插入
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		cerr << "插入失败: " << sqlite3_errmsg(db) << endl;
	}
	else {
		cout << "插入成功！" << endl;
	}

	sqlite3_finalize(stmt); // 释放资源
	//更新容器
	this->InitVec(db);
	sqlite3_close(db);
	system("pause");
	system("cls");

}

void Manager::showPerson() {
	cout << "学生账号的数量为：" << VStu.size() << endl;
	for (vector<Student>::iterator it = VStu.begin(); it != VStu.end(); it++) {
		cout << it->id << " " << it->m_Name << " " << it->m_Password << endl;
	}
	cout << "教师账号的数量为：" << VTea.size() << endl;
	for (vector<Teacher>::iterator it = VTea.begin(); it != VTea.end();it++) {
		cout << it->m_empId << " " << it->m_Name << " " << it->m_Password << endl;
	}

	system("pause");
	system("cls");
}

void Manager::showComputer() {
	cout << "机房数量为：" << vcRoom.size() << endl;
	for (vector<computerRoom>::iterator it = vcRoom.begin(); it != vcRoom.end(); it++) {
		cout << "机房编号：" << it->m_id << "   " << "容量为：" << it->m_room << endl;
	}

	system("pause");
	system("cls");
}

void Manager::cleanFile() {
	sqlite3* db;
	char* errmeg = 0;
	int rc;
	rc = sqlite3_open("resource/resource.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	const char* sql = "DELETE FROM order_table;";
	sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
	sqlite3_close(db);
}