#include"orderFile.h"

//构造函数
orderFile::orderFile() {
	/*this->m_size = 0;

	ifstream ifs;
	ifs.open(ORDER_FILE, ios::in);

	map<string, string>mp;
	string date;
	string interval;
	string room;
	string id;
	string status;
	while (ifs >> date && ifs >> interval && ifs >> room && ifs >> id && ifs >> status) {
		this->m_size++;
		bool flag = 0;
		string key = "";
		string value = "";
		for (int i = 0; i < date.size(); i++) {
			if (date[i] == ':') {
				flag = 1;
				continue;
			}
			if (flag == 0) {
				key += date[i];
			}
			else {
				value += date[i];
			}
		}
		mp[key] = value;

		key = "";
		value = "";
		flag = 0;
		for (int i = 0; i < interval.size(); i++) {
			if (interval[i] == ':') {
				flag = 1;
				continue;
			}
			if (flag == 0) {
				key += interval[i];
			}
			else {
				value += interval[i];
			}
		}
		mp[key] = value;

		key = "";
		value = "";
		flag = 0;
		for (int i = 0; i < interval.size(); i++) {
			if (interval[i] == ':') {
				flag = 1;
				continue;
			}
			if (flag == 0) {
				key += interval[i];
			}
			else {
				value += interval[i];
			}
		}
		mp[key] = value;

		key = "";
		value = "";
		flag = 0;
		for (int i = 0; i < room.size(); i++) {
			if (room[i] == ':') {
				flag = 1;
				continue;
			}
			if (flag == 0) {
				key += room[i];
			}
			else {
				value += room[i];
			}
		}
		mp[key] = value;

		key = "";
		value = "";
		flag = 0;
		for (int i = 0; i < id.size(); i++) {
			if (id[i] == ':') {
				flag = 1;
				continue;
			}
			if (flag == 0) {
				key += id[i];
			}
			else {
				value += id[i];
			}
		}
		mp[key] = value;

		key = "";
		value = "";
		flag = 0;
		for (int i = 0; i < status.size(); i++) {
			if (status[i] == ':') {
				flag = 1;
				continue;
			}
			if (flag == 0) {
				key += status[i];
			}
			else {
				value += status[i];
			}
		}
		mp[key] = value;

		this->m[this->m_size] = mp;
	}

	ifs.close();*/
	
}

//更新状态
void orderFile::updateFile() {

}