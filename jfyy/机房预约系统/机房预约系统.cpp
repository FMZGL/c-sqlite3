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
//�������Ա�Ӳ˵�
void managerMenu(Identity*& person) {
	//����Ա�˵�
	
	//ǿ��ת��ָ�������
	Manager* man = (Manager*)person;
	int select = 0;
	while (true) {

		person->operMenu();

		cin >> select;
		if (select == 1) {
			cout << "����˺�" << endl;
			man->addPerson();
		}
		else if (select == 2) {
			cout << "�鿴�˺�" << endl;
			man->showPerson();
		}
		else if (select == 3) {
			cout << "�鿴����" << endl;
			man->showComputer();
		}
		else if (select == 4) {
			cout << "���ԤԼ" << endl;
			man->cleanFile();
		}
		else {
			cout << "ע���ɹ�" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

//����ѧ���Ӳ˵�
void studentMenu(Identity*& person) {

	//ǿ��ת��ָ�������
	Student* man = (Student*)person;
	int select = 0;
	while (true) {

		man->operMenu();

		cin >> select;
		if (select == 1) {
			cout << "����ԤԼ" << endl;
			man->applyOrder();
		}
		else if (select == 2) {
			cout << "��ʾԤԼ" << endl;
			man->showOrder();
		}
		else if (select == 3) {
			cout << "��ʾ����ԤԼ" << endl;
			man->showAllOrder();
		}
		else if (select == 4) {
			cout << "ȡ��ԤԼ" << endl;
			man->cancelOrder();
		}
		else {
			cout << "ע���ɹ�" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

//�����ʦ���˵�
void teacherMenu(Identity*& person) {
	//ǿ��ת��ָ�������
	Teacher* man = (Teacher*)person;
	int select = 0;
	while (true) {

		man->operMenu();

		cin >> select;
		if (select == 1) {
			cout << "�鿴ԤԼ" << endl;
			man->showAllOrder();
		}
		else if (select == 2) {
			cout << "���ԤԼ" << endl;
			man->validOrder();
		}
		else {
			cout << "ע���ɹ�" << endl;
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

//��¼��֤
void LoginIn(string Filename, int type) {
	//����һ����ָ��,����ָ���������
	Identity* person = NULL;

	//���ļ�
	sqlite3* db;
	char *errmeg = 0;
	int rc = sqlite3_open("resource/resource.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	//׼�������û�����Ϣ
	string id;
	string name;
	string password;
	if (type == 1) {
		cout << "������ѧ��ѧ�ţ�" << endl;
		cin >> id;
	}
	else if (type == 2) {
		cout << "������ְ���ţ�" << endl;
		cin >> id;
	}
	cout << "�������û�����" << endl;
	cin >> name;

	cout << "���������룺" << endl;
	cin >> password;

	if (type == 1) {
		//ѧ�������֤
		string sql = "SELECT * FROM student WHERE ID = ? AND USERNAME = ? AND PASSWORD = ?";
		sqlite3_stmt* stmt;

		if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
			cerr << "SQL ���׼��ʧ��: " << sqlite3_errmsg(db) << endl;
			return;
		}

		// �󶨲���
		sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);
		
		// ִ�в�ѯ
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			cout << "ѧ����֤��¼�ɹ�" << endl;
			system("pause");
			system("cls");
			// ������Դ
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			person = new Student(id, name, password);
			studentMenu(person);
		}
		else {
			// ������Դ
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			cout << "�˺Ż��������" << endl;
		}

		
	}
	else if (type == 2) {
		//��ʦ�����֤
		string sql = "SELECT * FROM teacher WHERE ID = ? AND USERNAME = ? AND PASSWORD = ?";
		sqlite3_stmt* stmt;

		if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
			cerr << "SQL ���׼��ʧ��: " << sqlite3_errmsg(db) << endl;
			return;
		}

		// �󶨲���
		sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);

		// ִ�в�ѯ
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			cout << "��ʦ��֤��¼�ɹ�" << endl;
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
			cout << "�˺Ż��������" << endl;
		}
	}
	else if(type==3){
		//����Ա�����֤
		string sql = "SELECT * FROM admin WHERE USERNAME = ? AND PASSWORD = ?";
		sqlite3_stmt* stmt;

		if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
			cerr << "SQL ���׼��ʧ��: " << sqlite3_errmsg(db) << endl;
			return;
		}

		// �󶨲���
		sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

		// ִ�в�ѯ
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			cout << "����Ա��֤��¼�ɹ�" << endl;
			system("pause");
			system("cls");
			// ������Դ
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			person = new Manager(name, password);
			managerMenu(person);
		}
		else {
			// ������Դ
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			cout << "�˺Ż��������" << endl;
			cout << "��֤��¼ʧ��" << endl;
		}
	}
	system("pause");
	system("cls");
}


int main(){
	int select;
	while (true) {
		cout << "********************��ӭ����ɽ���ƾ���ѧ����ԤԼϵͳ************************" << endl;
		cout << endl;
		cout << "�������������" << endl;
		cout << "\t\t ---------------------------------------\n";
		cout << "\t\t|                                       |\n";
		cout << "\t\t|              1,ѧ����¼**             |\n";
		cout << "\t\t|                                       |\n";
		cout << "\t\t|              2,��ʦ��¼**             |\n";
		cout << "\t\t|                                       |\n";
		cout << "\t\t|              3,����Ա��¼             |\n";
		cout << "\t\t|                                       |\n";
		cout << "\t\t|              0,��      ��             |\n";
		cout << "\t\t|                                       |\n";
		cout << "\t\t ---------------------------------------\n";
		cout << "��������ѡ��";
		cin >> select;
		switch (select)
		{
		case 1://ѧ�����
			LoginIn(STUDENT_DB, 1);
			break;
		case 2://��ʦ���
			LoginIn(TEACHER_DB, 2);
			break;
		case 3://����Ա���
			LoginIn(ADMIN_DB, 3);
			break;
		case 0://�˳�
			cout << "��ӭ��һ��ʹ��" << endl;
			return 0;
			break;
		default:
			cout << "����������������������" << endl;
			system("pause");
			system("cls");
		}

	}

   return 0;
}