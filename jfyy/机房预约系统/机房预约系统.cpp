#include<iostream>
using namespace std;
#include<string>
#include<fstream>
#include"grobalFile.h"
#include"Identity.h"
#include"Student.h"
#include"Manager.h"
#include"Teacher.h"
#include"sqlite/sqlite3.h"
//进入管理员子菜单
void managerMenu(Identity*& person) {
	//管理员菜单
	
	//强制转变指针的类型
	Manager* man = (Manager*)person;
	int select = 0;
	while (true) {

		person->operMenu();

		cin >> select;
		if (select == 1) {
			cout << "添加账号" << endl;
			man->addPerson();
		}
		else if (select == 2) {
			cout << "查看账号" << endl;
			man->showPerson();
		}
		else if (select == 3) {
			cout << "查看机房" << endl;
			man->showComputer();
		}
		else if (select == 4) {
			cout << "清空预约" << endl;
			man->cleanFile();
		}
		else {
			cout << "注销成功" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

//进入学生子菜单
void studentMenu(Identity*& person) {

	//强制转变指针的类型
	Student* man = (Student*)person;
	int select = 0;
	while (true) {

		man->operMenu();

		cin >> select;
		if (select == 1) {
			cout << "申请预约" << endl;
			man->applyOrder();
		}
		else if (select == 2) {
			cout << "显示预约" << endl;
			man->showOrder();
		}
		else if (select == 3) {
			cout << "显示所有预约" << endl;
			man->showAllOrder();
		}
		else if (select == 4) {
			cout << "取消预约" << endl;
			man->cancelOrder();
		}
		else {
			cout << "注销成功" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

//进入教师主菜单
void teacherMenu(Identity*& person) {
	//强制转变指针的类型
	Teacher* man = (Teacher*)person;
	int select = 0;
	while (true) {

		man->operMenu();

		cin >> select;
		if (select == 1) {
			cout << "查看预约" << endl;
			man->showAllOrder();
		}
		else if (select == 2) {
			cout << "审核预约" << endl;
			man->validOrder();
		}
		else {
			cout << "注销成功" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
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

//登录验证
void LoginIn(string Filename, int type) {
	//创建一个父指针,用于指向子类对象
	Identity* person = NULL;

	//读文件
	sqlite3* db;
	char *errmeg = 0;
	int rc = sqlite3_open("resource/resource.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	//准备接受用户的信息
	string id;
	string name;
	string password;
	if (type == 1) {
		cout << "请输入学生学号：" << endl;
		cin >> id;
	}
	else if (type == 2) {
		cout << "请输入职工号：" << endl;
		cin >> id;
	}
	cout << "请输入用户名：" << endl;
	cin >> name;

	cout << "请输入密码：" << endl;
	cin >> password;

	if (type == 1) {
		//学生身份验证
		string sql = "SELECT * FROM student WHERE ID = ? AND USERNAME = ? AND PASSWORD = ?";
		sqlite3_stmt* stmt;

		if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
			cerr << "SQL 语句准备失败: " << sqlite3_errmsg(db) << endl;
			return;
		}

		// 绑定参数
		sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);
		
		// 执行查询
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			cout << "学生验证登录成功" << endl;
			system("pause");
			system("cls");
			// 清理资源
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			person = new Student(id, name, password);
			studentMenu(person);
		}
		else {
			// 清理资源
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			cout << "账号或密码错误！" << endl;
		}

		
	}
	else if (type == 2) {
		//教师身份验证
		string sql = "SELECT * FROM teacher WHERE ID = ? AND USERNAME = ? AND PASSWORD = ?";
		sqlite3_stmt* stmt;

		if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
			cerr << "SQL 语句准备失败: " << sqlite3_errmsg(db) << endl;
			return;
		}

		// 绑定参数
		sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);

		// 执行查询
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			cout << "教师验证登录成功" << endl;
			system("pause");
			system("cls");
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			person = new Teacher(id, name, password);
			teacherMenu(person);
		}
		else {
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			cout << "账号或密码错误！" << endl;
		}
	}
	else if(type==3){
		//管理员身份验证
		string sql = "SELECT * FROM admin WHERE USERNAME = ? AND PASSWORD = ?";
		sqlite3_stmt* stmt;

		if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
			cerr << "SQL 语句准备失败: " << sqlite3_errmsg(db) << endl;
			return;
		}

		// 绑定参数
		sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

		// 执行查询
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			cout << "管理员验证登录成功" << endl;
			system("pause");
			system("cls");
			// 清理资源
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			person = new Manager(name, password);
			managerMenu(person);
		}
		else {
			// 清理资源
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			cout << "账号或密码错误！" << endl;
			cout << "验证登录失败" << endl;
		}
	}
	system("pause");
	system("cls");
}


int main(){
	int select;
	while (true) {
		cout << "********************欢迎来到山东财经大学机房预约系统************************" << endl;
		cout << endl;
		cout << "请输入您的身份" << endl;
		cout << "\t\t ---------------------------------------\n";
		cout << "\t\t|                                       |\n";
		cout << "\t\t|              1,学生登录**             |\n";
		cout << "\t\t|                                       |\n";
		cout << "\t\t|              2,教师登录**             |\n";
		cout << "\t\t|                                       |\n";
		cout << "\t\t|              3,管理员登录             |\n";
		cout << "\t\t|                                       |\n";
		cout << "\t\t|              0,退      出             |\n";
		cout << "\t\t|                                       |\n";
		cout << "\t\t ---------------------------------------\n";
		cout << "输入您的选择：";
		cin >> select;
		switch (select)
		{
		case 1://学生身份
			LoginIn(STUDENT_DB, 1);
			break;
		case 2://教师身份
			LoginIn(TEACHER_DB, 2);
			break;
		case 3://管理员身份
			LoginIn(ADMIN_DB, 3);
			break;
		case 0://退出
			cout << "欢迎下一次使用" << endl;
			return 0;
			break;
		default:
			cout << "您的输入有误，请重新输入" << endl;
			system("pause");
			system("cls");
		}

	}

   return 0;
}