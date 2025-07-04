#include"Teacher.h"

//Ĭ�Ϲ���
Teacher::Teacher() {

}

//�вι���
Teacher::Teacher(string empId, string name, string password) {
	this-> m_empId = empId;
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
	sqlite3_close(db);
}
void Teacher::InitRoom(sqlite3* db) {
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
//�˵�����
void Teacher::operMenu() {
	cout << "��ӭ��ʦ�û���" << this->m_Name << "��¼" << endl;
	cout << "\t\t ---------------------------------------\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              1, �鿴ԤԼ              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              2, ���ԤԼ              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              0, ע    ��              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t ---------------------------------------\n";
}

//�鿴����ԤԼ
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
	// ׼�� SQL ���
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "׼���������ʧ��: " << sqlite3_errmsg(db) << endl;
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
		if (sqlite3_column_int(stmt, 1) == 1)intv = "����";
		if (sqlite3_column_int(stmt, 1) == 2)intv = "����";
		RID = (const char*)sqlite3_column_text(stmt, 2);
		SID = (const char*)sqlite3_column_text(stmt, 3);
		Status = (const char*)sqlite3_column_text(stmt, 4);
		cout << "���ڣ�" << date << " ʱ��Σ�" << intv << " ������ţ�" << RID << " ѧ��ѧ�ţ�" << SID << " ���״̬��" << Status << endl;
	}
	sqlite3_finalize(stmt); // �ͷ���Դ
	sqlite3_close(db);
	system("pause");
	system("cls");
}

//���ԤԼ
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
	// ׼�� SQL ���
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "׼���������ʧ��: " << sqlite3_errmsg(db) << endl;
		return;
	}
	if (sqlite3_step(stmt) == SQLITE_DONE) {
		cout << "һ��������" << endl;
	}
	sqlite3_finalize(stmt); // �ͷ���Դ
	sqlite3_close(db);
	system("pause");
	system("cls");
}