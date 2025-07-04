#include"Student.h";


// Ĭ�Ϲ���
Student::Student() {

}

//���ι���
Student::Student(string id, string name, string password) {
	this->id = id;
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
void Student::InitRoom(sqlite3* db) {
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
//�˵�ҳ��
void Student::operMenu() {
	cout << "��ӭѧ���û���" << this->m_Name << "��¼" << endl;
	cout << "\t\t ---------------------------------------\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              1, ����ԤԼ              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              2, ��ʾ����ԤԼ          |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              3, ��ʾ����ԤԼ          |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              4, ȡ��ԤԼ              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t|              0, ע    ��              |\n";
	cout << "\t\t|                                       |\n";
	cout << "\t\t ---------------------------------------\n";
}

//����ԤԼ
void Student::applyOrder() {
	int date = 0;
	int interval = 0;
	string room;
	cout << "�����Ŀ���ʱ��Ϊ��һ������" << endl;
	cout << "��������ҪԤԼ�����ڣ�" << endl;
	cout << "1.��һ" << endl;
	cout << "2.�ܶ�" << endl;
	cout << "3.����" << endl;
	cout << "4.����" << endl;
	cout << "5.����" << endl;
	while (true) {
		cin >> date;
		if (date >= 1 && date <= 5) {
			break;
		}
		else {
			cout << "������������������" << endl;
		}
	}

	cout << "��������ҪԤԼ��ʱ��Σ�" << endl;
	cout << "1.����" << endl;
	cout << "2.����" << endl;
	while (true) {
		cin >> interval;
		if (interval>=1&&interval<=2) {
			break;
		}
		else {
			cout << "������������������" << endl;
		}
	}

	cout << "������ϢΪ��" << endl;
	for (vector<computerRoom>::iterator it = vcRoom.begin(); it != vcRoom.end(); it++) {
		cout << "������ţ�" << it->m_id << " " << "�������Ϊ��" << it->m_room << endl;
	}
	cout << "��������Ҫѡ��Ļ�����";
	cin >> room;
	cout << "�ύ�ɹ��������" << endl;

	//�����ݿ�
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

	// ׼�� SQL ���
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "׼���������ʧ��: " << sqlite3_errmsg(db) << endl;
		return;
	}

	// �󶨲�����ע�ⶼ���ַ�����
	sqlite3_bind_int(stmt, 1, date);
	sqlite3_bind_int(stmt, 2, interval);
	sqlite3_bind_text(stmt, 3, room.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, this->id.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 5, "Waiting", -1, SQLITE_STATIC);
	// ִ�в���
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		cerr << "����ʧ��: " << sqlite3_errmsg(db) << endl;
	}

	sqlite3_finalize(stmt); // �ͷ���Դ
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
//�鿴����ԤԼ
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
	// ׼�� SQL ���
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "׼���������ʧ��: " << sqlite3_errmsg(db) << endl;
		return;
	}
	// �󶨲�����ע�ⶼ���ַ�����
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
		if (sqlite3_column_int(stmt, 1) == 1)intv = "����";
		if (sqlite3_column_int(stmt, 1) == 2)intv = "����";
		RID = (const char*)sqlite3_column_text(stmt, 2);
		SID = (const char*)sqlite3_column_text(stmt, 3);
		Status= (const char*)sqlite3_column_text(stmt, 4);
		cout << "���ڣ�" << date << " ʱ��Σ�" << intv << " ������ţ�" << RID << " ѧ��ѧ�ţ�" << SID << " ���״̬��" << Status << endl;
	}
	sqlite3_finalize(stmt); // �ͷ���Դ
	sqlite3_close(db);
	system("pause");
	system("cls");
}

//�鿴����ԤԼ
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

//ȡ��ԤԼ
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
	// ׼�� SQL ���
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "׼���������ʧ��: " << sqlite3_errmsg(db) << endl;
		return;
	}
	// �󶨲�����ע�ⶼ���ַ�����
	sqlite3_bind_text(stmt, 1, this->id.c_str(), -1, SQLITE_STATIC);
	if (sqlite3_step(stmt) == SQLITE_DONE) {
		cout << "ԤԼ��¼ɾ���ɹ�" << endl;
	}
	sqlite3_finalize(stmt); // �ͷ���Դ
	sqlite3_close(db);
	system("pause");
	system("cls");
}