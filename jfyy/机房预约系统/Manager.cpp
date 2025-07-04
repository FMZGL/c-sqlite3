#include"Manager.h"
#include"grobalFile.h"
#include<fstream>
#include"sqlite/sqlite3.h"
Manager::Manager() {

}

Manager::Manager(string name, string password) {
	this->m_Name = name;
	this->m_Password = password;
	//��ȡ��������Ϣ
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
		cerr << "����SQL׼��ʧ��: " << sqlite3_errmsg(db) << endl;
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

	// Ԥ���� SQL ���
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "׼�� SQL ���ʧ��: " << sqlite3_errmsg(db) << endl;
		return;
	}

	// ִ�в�ѯ����ȡÿһ��
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		Student stu;
		stu.id = (const char*)sqlite3_column_text(stmt, 0);
		stu.m_Name= (const char*)sqlite3_column_text(stmt, 1);
		stu.m_Password = (const char*)sqlite3_column_text(stmt, 2);
		VStu.push_back(stu);
	}

	sqlite3_finalize(stmt); // �ͷ���Դ

	sql = "SELECT ID,USERNAME,PASSWORD FROM teacher";
	stmt = nullptr;
	// Ԥ���� SQL ���
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "׼�� SQL ���ʧ��: " << sqlite3_errmsg(db) << endl;
		return;
	}

	// ִ�в�ѯ����ȡÿһ��
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		Teacher tea;
		tea.m_empId = (const char*)sqlite3_column_text(stmt, 0);
		tea.m_Name = (const char*)sqlite3_column_text(stmt, 1);
		tea.m_Password = (const char*)sqlite3_column_text(stmt, 2);
		VTea.push_back(tea);
	}

	sqlite3_finalize(stmt); // �ͷ���Դ

}

void Manager::operMenu() {
	cout << "��ӭ����Ա��"<< this->m_Name << "��¼" << endl;
	cout << "\t\t ---------------------------------------\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              1, ����˺�              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              2, �鿴�˺�              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              3, �鿴����              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              4, ���ԤԼ              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              0, ע    ��              |\n";
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
	cout << "��������Ҫ����˺ŵ�����" << endl;
	cout << "1. ���ѧ���˺�" << endl;
	cout << "2. ��ӽ�ʦ�˺�" << endl;
	string fileName;
	string tip;
	ofstream ofs;
	string errtip;
	int select;
	cin >> select;
	if (select == 1) {
		tip = "������ѧ����ѧ��";
		errtip = "****!ѧ��ѧ���Ѵ��ڣ�����������***";
	}
	else {
		tip = "�������ʦ��ְ����";
		errtip = "***!��ʦְ�����Ѵ��ڣ�����������***";
	}

	string id;
	string name;
	string password;

	while (true) {
		cout << tip << endl;
		cin >> id;
		cout << "�������û���" << endl;
		cin >> name;
		cout << "����������" << endl;
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

	// ׼�� SQL ���
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "׼���������ʧ��: " << sqlite3_errmsg(db) << endl;
		return;
	}

	// �󶨲�����ע�ⶼ���ַ�����
	sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);

	// ִ�в���
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		cerr << "����ʧ��: " << sqlite3_errmsg(db) << endl;
	}
	else {
		cout << "����ɹ���" << endl;
	}

	sqlite3_finalize(stmt); // �ͷ���Դ
	//��������
	this->InitVec(db);
	sqlite3_close(db);
	system("pause");
	system("cls");

}

void Manager::showPerson() {
	cout << "ѧ���˺ŵ�����Ϊ��" << VStu.size() << endl;
	for (vector<Student>::iterator it = VStu.begin(); it != VStu.end(); it++) {
		cout << it->id << " " << it->m_Name << " " << it->m_Password << endl;
	}
	cout << "��ʦ�˺ŵ�����Ϊ��" << VTea.size() << endl;
	for (vector<Teacher>::iterator it = VTea.begin(); it != VTea.end();it++) {
		cout << it->m_empId << " " << it->m_Name << " " << it->m_Password << endl;
	}

	system("pause");
	system("cls");
}

void Manager::showComputer() {
	cout << "��������Ϊ��" << vcRoom.size() << endl;
	for (vector<computerRoom>::iterator it = vcRoom.begin(); it != vcRoom.end(); it++) {
		cout << "������ţ�" << it->m_id << "   " << "����Ϊ��" << it->m_room << endl;
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