#include "SysManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <unistd.h>
using namespace std;

SysManager::SysManager(IndexManager *ixm, FileManager *_fileManager, BufPageManager *_bufPageManager) {
	_ixm = ixm;
	fileManager = _fileManager; bufPageManager = _bufPageManager;
}

SysManager::~SysManager() {}

void SysManager::Show() {
	cout << _tableNum << " table(s) in " << _DBName << endl;
	for (int tableID = 0; tableID < _tableNum; tableID++) {
		cout << "Table " << _tables[tableID].tableName << ":" << endl;
		for (int attrID = 0; attrID < _tables[tableID].attrNum; attrID++) {
			cout << _tables[tableID].attrs[attrID].attrName << " ";
			if (_tables[tableID].attrs[attrID].attrType == INT) cout << "INT";
			else if (_tables[tableID].attrs[attrID].attrType == DATE) cout << "DATE";
			else if (_tables[tableID].attrs[attrID].attrType == FLOAT) cout << "FLOAT";
			else if (_tables[tableID].attrs[attrID].attrType == STRING) cout << "CHAR(" << _tables[tableID].attrs[attrID].attrLength << ")";
			if (_tables[tableID].attrs[attrID].notNull) cout << " NOT NULL";
			if (_tables[tableID].attrs[attrID].primary) cout << " PRIMARY KEY";
			if (_tables[tableID].attrs[attrID].defaultValue != nullptr) {
				if (_tables[tableID].attrs[attrID].attrType == INT) cout << " DEFAULT " << *(int*)_tables[tableID].attrs[attrID].defaultValue;
				else if (_tables[tableID].attrs[attrID].attrType == DATE) cout << " DEFAULT " << *(int*)_tables[tableID].attrs[attrID].defaultValue;
				else if (_tables[tableID].attrs[attrID].attrType == FLOAT) cout << " DEFAULT " << *(double*)_tables[tableID].attrs[attrID].defaultValue;
				else if (_tables[tableID].attrs[attrID].attrType == STRING) cout << " DEFAULT '" << (char*)_tables[tableID].attrs[attrID].defaultValue << "'";
			}
			if (_tables[tableID].attrs[attrID].reference != "") {
				cout << " REFERENCES " << _tables[tableID].attrs[attrID].reference << "(" << _tables[tableID].attrs[attrID].foreignKeyName << ")";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void SysManager::OpenDB(const string DBName) {
	_DBName.assign(DBName);
	chdir(DBName.c_str());
	ifstream metain("meta.db");
	if (metain.fail()) {
		fprintf(stderr, "Error: cannot open meta-data file!\n");
		return;
	}
	metain >> _tableNum;
	_tables.clear();
	_tableFileID.clear();
	for (int i = 0; i < _tableNum; i++) {
		_tables.push_back(TableInfo());
		metain >> _tables[i].tableName;
		// metain >> _tables[i].pkName;
		getline(metain, _tables[i].pkName);
		getline(metain, _tables[i].pkName);
		int fkNum;
		metain >> fkNum;
		for (int i = 0; i < fkNum; i++) {
			string fkName, refName;
			metain >> fkName;
			metain >> refName;
			_tables[i].fkNames[fkName] = refName;
		}
		metain >> _tables[i].attrNum;
		metain >> _tables[i].foreignNum;
		_tables[i].attrs.clear();
		_tables[i].foreign.clear();
		_tables[i].foreignSet.clear();
		_tables[i].primary.clear();
		metain >> _tables[i].recordSize;
		metain >> _tables[i].primarySize;
		for (int j = 0; j < _tables[i].attrNum; j++) {
			_tables[i].attrs.push_back(AttrInfo());
			_tables[i].attrs[j].notNull = _tables[i].attrs[j].primary = _tables[i].attrs[j].haveIndex = false;
			_tables[i].attrs[j].defaultValue = nullptr;
			_tables[i].attrs[j].reference = _tables[i].attrs[j].foreignKeyName = "";
			metain >> _tables[i].attrs[j].attrName;
			metain >> _tables[i].attrs[j].offset;
			string str;
			metain >> str;
			if (str == "INT") {
				_tables[i].attrs[j].attrType = INT;
				_tables[i].attrs[j].attrLength = 4;
			} else if (str == "DATE") {
				_tables[i].attrs[j].attrType = DATE;
				_tables[i].attrs[j].attrLength = 4;
			} else if (str == "STRING") {
				_tables[i].attrs[j].attrType = STRING;
				metain >> _tables[i].attrs[j].attrLength;
			} else if (str == "FLOAT") {
				_tables[i].attrs[j].attrType = FLOAT;
				_tables[i].attrs[j].attrLength = 8;
			}
			while (1) {
				metain >> str;
				if (str == "NOTNULL") {
					_tables[i].attrs[j].notNull = true;
				} else if (str == "PRIMARY") {
					_tables[i].attrs[j].primary = true;
					_tables[i].primary.push_back(j);
				} else if (str == "DEFAULT") {
					if (_tables[i].attrs[j].attrType == INT) {
						int *d = new int; metain >> *d;
						_tables[i].attrs[j].defaultValue = (BufType)d;
					} else if (_tables[i].attrs[j].attrType == DATE) {
						int *d = new int; metain >> *d;
						_tables[i].attrs[j].defaultValue = (BufType)d;
					} else if (_tables[i].attrs[j].attrType == STRING) {
						string d; 
						getline(metain, d); // "\n"
						getline(metain, d);
						_tables[i].attrs[j].defaultValue = (BufType)(new char[_tables[i].attrs[j].attrLength]);
						memset(_tables[i].attrs[j].defaultValue, 0, _tables[i].attrs[j].attrLength * sizeof(char));
						strcpy((char*)_tables[i].attrs[j].defaultValue, d.c_str());
					} else if (_tables[i].attrs[j].attrType == FLOAT) {
						double *d = new double; metain >> *d;
						_tables[i].attrs[j].defaultValue = (BufType)d;
					}
				} else if (str == "FOREIGN") {
					metain >> _tables[i].attrs[j].reference;
					metain >> _tables[i].attrs[j].foreignKeyName;
					if (_tables[i].foreignSet.find(_tables[i].attrs[j].reference) == _tables[i].foreignSet.end()) {
						_tables[i].foreign.push_back(_tables[i].attrs[j].reference);
						_tables[i].foreignSet.insert(_tables[i].attrs[j].reference);
					}
				} else if (str == "INDEX") {
					_tables[i].attrs[j].haveIndex = true;
				} else if (str == "END") {
					break;
				}
			}
		}
	}
	// read _indexes
	int indexesNum;
	metain >> indexesNum;
	for (int i = 0; i < indexesNum; i++) {
		string indexName;
		string tableName;
		metain >> indexName;
		metain >> tableName;
		int attrNum;
		vector<string> attrs;
		metain >> attrNum;
		for (int j = 0; j < attrNum; j++) {
			string attr;
			metain >> attr;
			attrs.push_back(attr);
		}
		_indexes.insert(pair<string, pair<string, vector<string> > >(indexName, pair<string, vector<string> >(tableName, attrs)));
	}
	metain.close();
	for (int i = 0; i < _tableNum; i++) {
		int fileID;
		fileManager->openFile(_tables[i].tableName.c_str(), fileID);
		_tableFileID[_tables[i].tableName] = fileID;
	}
}

void SysManager::CloseDB() {
	ofstream metaout("meta.db");
	if (metaout.fail()) {
		fprintf(stderr, "Error: cannot open meta-data file!\n");
		return;
	}
	metaout << _tableNum << "\n";
	for (int i = 0; i < _tableNum; i++) {
		metaout << _tables[i].tableName << "\n";
		metaout << _tables[i].pkName << "\n";
		metaout << _tables[i].fkNames.size() << "\n";
		for (auto [i, j] : _tables[i].fkNames) {
			metaout << i << "\n";
			metaout << j << "\n";
		}
		metaout << _tables[i].attrNum << "\n";
		metaout << _tables[i].foreignNum << "\n";
		metaout << _tables[i].recordSize << "\n";
		metaout << _tables[i].primarySize << "\n";
		for (int j = 0; j < _tables[i].attrNum; j++) {
			metaout << _tables[i].attrs[j].attrName << "\n";
			metaout << _tables[i].attrs[j].offset << "\n";
			if (_tables[i].attrs[j].attrType == INT) {
				metaout << "INT\n";
			} else if (_tables[i].attrs[j].attrType == DATE) {
				metaout << "DATE\n";
			} else if (_tables[i].attrs[j].attrType == STRING) {
				metaout << "STRING\n";
				metaout << _tables[i].attrs[j].attrLength << "\n";
			} else if (_tables[i].attrs[j].attrType == FLOAT) {
				metaout << "FLOAT\n";
			}
			if (_tables[i].attrs[j].notNull) {
				metaout << "NOTNULL\n";
			}
			if (_tables[i].attrs[j].primary) {
				metaout << "PRIMARY\n";
			}
			if (_tables[i].attrs[j].defaultValue != nullptr) {
				metaout << "DEFAULT\n";
				if (_tables[i].attrs[j].attrType == INT) {
					metaout << *((int*)_tables[i].attrs[j].defaultValue) << "\n";
					delete (int*)_tables[i].attrs[j].defaultValue;
				} else if (_tables[i].attrs[j].attrType == DATE) {
					metaout << *((int*)_tables[i].attrs[j].defaultValue) << "\n";
					delete (int*)_tables[i].attrs[j].defaultValue;
				} else if (_tables[i].attrs[j].attrType == STRING) {
					metaout << (char*)_tables[i].attrs[j].defaultValue << "\n";
					delete [] (char*)_tables[i].attrs[j].defaultValue;
				} else if (_tables[i].attrs[j].attrType == FLOAT) {
					metaout << *((double*)_tables[i].attrs[j].defaultValue) << "\n";
					delete (double*)_tables[i].attrs[j].defaultValue;
				}
			}
			if (_tables[i].attrs[j].reference != "") {
				metaout << "FOREIGN\n" << _tables[i].attrs[j].reference << "\n" << _tables[i].attrs[j].foreignKeyName << "\n";
			}
			if (_tables[i].attrs[j].haveIndex) {
				metaout << "INDEX\n";
			}
			metaout << "END\n";
		}
	}
	// write _indexes
	metaout << _indexes.size() << "\n";
	for (auto [i, j] : _indexes) {
		metaout << i << "\n";
		metaout << j.first << "\n";
		auto k = j.second;
		metaout << k.size() << "\n";
		for (auto l : k) {
			metaout << l << "\n";
		}
	}
	metaout.close();
	for (int i = 0; i < _tableNum; i++) {
		if (fileManager->closeFile(_tableFileID[_tables[i].tableName]))
			fprintf(stderr, "Error: failed to close table file!\n");
	}
	_tables.clear();
	_tableFileID.clear();
	chdir("..");
}

void SysManager::CreateTable(TableInfo* table) {
	int recordSize = 8; // bitmap for checking whether each column is NULL
	for (int i = 0; i < _tableNum; i++) {
		//cout << _tables[i].tableName << " " << table->tableName << endl;
		if (_tables[i].tableName == table->tableName) {
			fprintf(stderr, "Error: table already exists!\n");
			return;
		}
	}
	for (int i = 0; i < table->attrNum; i++) {
		table->attrs[i].offset = recordSize >> 2;
		if (table->attrs[i].attrType == INT) {
			table->attrs[i].attrLength = 4;
		} else if (table->attrs[i].attrType == DATE) {
			table->attrs[i].attrLength = 4;
		} else if (table->attrs[i].attrType == FLOAT) {
			table->attrs[i].attrLength = 8;
		}
		while (table->attrs[i].attrLength % 4 != 0) table->attrs[i].attrLength++;
		int size = table->attrs[i].attrLength;
		recordSize += size;
		if (table->attrs[i].primary) {
			table->attrs[i].notNull = true;
		}
		if (table->attrs[i].defaultValue != nullptr) {
			if (table->attrs[i].attrType == INT) {
				table->attrs[i].defaultValue = (BufType)new int(*(int *)table->attrs[i].defaultValue);
			} else if (table->attrs[i].attrType == DATE) {
				table->attrs[i].defaultValue = (BufType)new int(*(int *)table->attrs[i].defaultValue);
			} else if (table->attrs[i].attrType == STRING) {
				BufType temp = (BufType)(new char[table->attrs[i].attrLength]);
				strcpy((char*)temp, (char*)table->attrs[i].defaultValue);
				table->attrs[i].defaultValue = temp;
			} else if (table->attrs[i].attrType == FLOAT) {
				table->attrs[i].defaultValue = (BufType)new double(*(double *)table->attrs[i].defaultValue);
			}
		}
		if (table->attrs[i].reference != "") {
			if (table->attrs[i].reference == table->tableName) {
				fprintf(stderr, "Error: reference cannot be the same table!\n");
				return;
			}
			bool ok = false;
			for (int j = 0; j < _tables.size(); j++)
				if (_tables[j].tableName == table->attrs[i].reference) {
					for (int k = 0; k < _tables[j].attrNum; k++)
						if (_tables[j].attrs[k].primary && _tables[j].attrs[k].attrName == table->attrs[i].foreignKeyName) {
							table->attrs[i].attrType = _tables[j].attrs[k].attrType;
							table->attrs[i].attrLength = _tables[j].attrs[k].attrLength;
							ok = true;
							break;
						}
					if (ok) break;
				}
			if (!ok) {
				fprintf(stderr, "Error: invalid foreign key!\n");
				return;
			}
		}
	}
	if (recordSize >= PAGE_SIZE - 8) {
		fprintf(stderr, "Error: record size too large!\n");
		return;
	}
	_tables.push_back(*table);
	_tables[_tableNum].recordSize = recordSize;
	_tables[_tableNum].primarySize = 0;
	_tables[_tableNum].primary.clear();
	_tables[_tableNum].foreign.clear();
	_tables[_tableNum].foreignSet.clear();
	_tables[_tableNum].foreignNum = 0;
	for (int i = 0; i < _tables[_tableNum].attrNum; i++) {
		if (_tables[_tableNum].attrs[i].primary) {
			_tables[_tableNum].primary.push_back(i);
		}
		if (_tables[_tableNum].attrs[i].reference != "") {
			if (_tables[_tableNum].foreignSet.find(_tables[_tableNum].attrs[i].reference) != _tables[_tableNum].foreignSet.end()) continue;
			_tables[_tableNum].foreignSet.insert(_tables[_tableNum].attrs[i].reference);
			_tables[_tableNum].foreign.push_back(_tables[_tableNum].attrs[i].reference);
			_tables[_tableNum].foreignNum++;
		}
	}
	// _rmm->CreateFile(table->tableName.c_str(), recordSize);
	fileManager->createFile(table->tableName.c_str());
	int fileID;
	fileManager->openFile(table->tableName.c_str(), fileID);
	RecManager * rm = new RecManager(bufPageManager, fileID, recordSize, true);
	delete rm;
	// cout << table->tableName << ": " << fileID << endl;
	_tableFileID[table->tableName] = fileID;
	if (_tables[_tableNum].primary.size() != 0) {
		int primary_size = 0;
		for (int i = 0; i < _tables[_tableNum].primary.size(); i++) {
			primary_size += _tables[_tableNum].attrs[_tables[_tableNum].primary[i]].attrLength;
		}
		_tables[_tableNum].primarySize = primary_size;
		_ixm->CreateIndex((table->tableName + ".primary").c_str(), STRING, primary_size);
		// _ixm->CreateIndex(table->tableName.c_str(), "primary", STRING, primary_size);
	}
	_tableNum++;
}

void SysManager::DropTable(const string tableName) {
	int tableID = _fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: such table does not exist!\n");
		return;
	}
	if (_checkForeignKeyOnTable(tableID)) {
		fprintf(stderr, "Error: foreign key on the table!\n");
		return;
	}
	if (fileManager->closeFile(_tableFileID[_tables[tableID].tableName])) {
		fprintf(stderr, "Error: failed to close table file!\n");
		return;
	}
	// fileManager->DestroyFile(_tables[tableID].tableName.c_str());
	system(("rm " + _tables[tableID].tableName).c_str());
	system(("rm " + _tables[tableID].tableName + ".*").c_str());
	_tables.erase(_tables.begin() + tableID);
	_tableNum--;
}

void SysManager::_CreateIndex(const string tableName, const string attr) {
	int tableID, attrID;
	tableID = _fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: table does not exist!\n");
		return;
	}
	bool found = false;
	for (int i = 0; i < _tables[tableID].attrNum; i++) if (_tables[tableID].attrs[i].attrName == attr) {
		found = true;
		attrID = i;
		if (!_tables[tableID].attrs[i].notNull) {
			fprintf(stderr, "Error: the column must be not null!\n");
			return;
		}
		if (_tables[tableID].attrs[i].haveIndex) {
			fprintf(stderr, "Error: index already exists!\n");
			return;
		}
		break;
	}
	if (!found) {
		fprintf(stderr, "Error: column does not exist!\n");
		return;
	}
	int fileID = _tableFileID[tableName];
	_tables[tableID].attrs[attrID].haveIndex = true;
	_ixm->CreateIndex((tableName + "." + attr).c_str(), _tables[tableID].attrs[attrID].attrType, _tables[tableID].attrs[attrID].attrLength);
	int indexID;
	_ixm->OpenIndex((tableName + "." + attr).c_str(), indexID);
	SIndexManager * sixm = new SIndexManager(bufPageManager, indexID);
	// cout << " - recSize:" << _tables[tableID].recordSize << endl;
	RecManager * rm = new RecManager(bufPageManager, fileID, _tables[tableID].recordSize, false);
	RecManager::Iterator * iter = new RecManager::Iterator(rm);
	BufType e;
	unsigned int id;
	while (iter->next(e, id)) {
		int pageID = id >> 16;
		int slotID = (id << 16 >> 16);
		BufType insertData = e + _tables[tableID].attrs[attrID].offset;
		// cout << " - insertData:" << (const char *)insertData << endl;
		sixm->insertIx((void*)insertData, pageID, slotID);
	}
	delete iter;
	delete rm;
	delete sixm;
	_ixm->CloseIndex(indexID);
}

void SysManager::_DropIndex(const string tableName, const string attr) {
	int tableID = _fromNameToID(tableName), attrID;
	if (tableID == -1) {
		fprintf(stderr, "Error: table does not exist!\n");
		return;
	}
	bool found = false;
	for (int i = 0; i < _tables[tableID].attrNum; i++) if (_tables[tableID].attrs[i].attrName == attr) {
		found = true;
		attrID = i;
		if (!_tables[tableID].attrs[i].haveIndex) {
			fprintf(stderr, "Error: index does not exist!\n");
			return;
		}
		break;
	}
	if (!found) {
		fprintf(stderr, "Error: column does not exist!\n");
		return;
	}
	_tables[tableID].attrs[attrID].haveIndex = false;
	_ixm->DeleteIndex((tableName + "." + attr).c_str());
}

void SysManager::CreateIndex(const string idxName, const string tableName, const vector<string> attrs) {
	bool ret = _indexes.insert(pair<string, pair<string, vector<string> > >(idxName, pair<string, vector<string> >(tableName, attrs))).second;
	if (!ret) {
		fprintf(stderr, "Error: index name exists!\n");
		return;
	}
	for (auto e: attrs) {
		_CreateIndex(tableName, e);
	}
}

void SysManager::AddIndex(const string idxName, const vector<string> attrs) {
	map<string, pair<string, vector<string> > >::iterator it = _indexes.find(idxName);
	if (it == _indexes.end()) {
		fprintf(stderr, "Error: index name doesn't exist!\n");
		return;
	}
	vector<string> & v = it->second.second;
	v.insert(v.end(), attrs.begin(), attrs.end());
	for (auto e: attrs) {
		_CreateIndex(it->second.first, e);
	}
}

void SysManager::DropIndex(const string idxName) {
	map<string, pair<string, vector<string> > >::iterator it = _indexes.find(idxName);
	if (it == _indexes.end()) {
		fprintf(stderr, "Error: index name doesn't exist!\n");
		return;
	}
	for (auto e: it->second.second) {
		_DropIndex(it->second.first, e);
	}
	_indexes.erase(it);
}

void SysManager::AddPrimaryKey(const string tableName, const vector<string> attrs, const string pkName) {
	int tableID = _fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: invalid table!\n");
		return;
	}
	if (_tables[tableID].primary.size() != 0) {
		fprintf(stderr, "Error: primary key already exists!\n");
		return;
	}
	int primarySize = 0;
	for (int i = 0; i < attrs.size(); i++) {
		int attrID = _fromNameToID(attrs[i], tableID);
		//cout << attrID << endl;
		if (attrID == -1) {
			fprintf(stderr, "Error: invalid columns!\n");
			_tables[tableID].primary.clear();
			return;
		}
		if (_tables[tableID].attrs[attrID].reference != "") {
			fprintf(stderr, "Error: primary keys can not be foreign keys!\n");
			_tables[tableID].primary.clear();
			return;
		}
		if (!_tables[tableID].attrs[attrID].notNull) {
			fprintf(stderr, "Error: primary keys must be not null!\n");
			_tables[tableID].primary.clear();
			return;
		}
		_tables[tableID].primary.push_back(attrID);
		primarySize += _tables[tableID].attrs[attrID].attrLength;
	}
	sort(_tables[tableID].primary.begin(), _tables[tableID].primary.end());
	for (int i = 0; i < _tables[tableID].primary.size(); i++) {
		_tables[tableID].attrs[_tables[tableID].primary[i]].primary = true;
	}
	_tables[tableID].primarySize = primarySize;
	int fileID = _tableFileID[tableName];
	
	_ixm->CreateIndex((tableName + ".primary").c_str(), STRING, primarySize);
	int indexID;
	_ixm->OpenIndex((tableName + ".primary").c_str(), indexID);
	SIndexManager * sixm = new SIndexManager(bufPageManager, indexID);
	RecManager * rm = new RecManager(bufPageManager, fileID, _tables[tableID].recordSize, false);
	RecManager::Iterator * iter = new RecManager::Iterator(rm);
	BufType data;
	unsigned int id;
	while (iter->next(data, id)) {
		int pageID = id >> 16;
		int slotID = (id << 16 >> 16);
		BufType insertData = _getPrimaryKey(tableID, data);
		if (sixm->Exists((void*)insertData)) {
			fprintf(stderr, "Error: repetitive primary keys!\n");
			for (int i = 0; i < _tables[tableID].primary.size(); i++) {
				_tables[tableID].attrs[_tables[tableID].primary[i]].primary = false;
			}
			_tables[tableID].primary.clear();
			_tables[tableID].primarySize = 0;
			delete [] insertData;
			delete iter;
			delete rm;
			delete sixm;
			_ixm->CloseIndex(indexID);
			system((string("rm ") + tableName + string(".primary")).c_str());
			return;
		}
		sixm->insertIx((void*)insertData, pageID, slotID);
		delete [] insertData;
	}
	delete iter;
	delete rm;
	delete sixm;
	_ixm->CloseIndex(indexID);
	_tables[tableID].pkName = pkName;


}

void SysManager::DropPrimaryKey(const string tableName, const string pkName) {
	int tableID = _fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: invalid table!\n");
		return;
	}
	if (pkName != "" && _tables[tableID].pkName != pkName) {
		fprintf(stderr, "Error: wrong primary key name!\n");
		return;
	}
	if (_tables[tableID].primary.size() == 0) {
		fprintf(stderr, "Error: primary key does not exist!\n");
		return;
	}
	for (int i = 0; i < _tableNum; i++) if (_tables[i].foreignSet.find(tableName) != _tables[i].foreignSet.end()) {
		fprintf(stderr, "Error: foreign keys on the table!\n");
		return;
	}
	for (int i = 0; i < _tables[tableID].attrNum; i++) {
		_tables[tableID].attrs[i].primary = false;
	}
	_tables[tableID].primary.clear();
	_tables[tableID].primarySize = 0;
	system((string("rm ") + tableName + string(".primary")).c_str());
}

void SysManager::AddForeignKey(const string tableName, const string fkName, const vector<string> attrs, const string refName, const vector<string> foreigns) {
	int tableID = _fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: invalid table!\n");
		return;
	}
	int refID = _fromNameToID(refName);
	if (refID == -1) {
		fprintf(stderr, "Error: invalid reference table!\n");
		return;
	}
	if (attrs.size() != foreigns.size() || foreigns.size() != _tables[refID].primary.size()) {
		fprintf(stderr, "Error: should cover all primary keys!\n");
		return;
	}
	if (_tables[tableID].foreignSet.find(refName) != _tables[tableID].foreignSet.end()) {
		fprintf(stderr, "Error: foreign keys already exists!\n");
		return;
	}
	vector<int> attrIDs, foreignIDs;
	int keyNum = _tables[refID].primary.size();
	/*for (int i = 0; i < keyNum; i++) {
		cout << _tables[refID].primary[i] << endl;
	}*/
	for (int i = 0; i < keyNum; i++) {
		attrIDs.push_back(_fromNameToID(attrs[i], tableID));
		foreignIDs.push_back(_fromNameToID(foreigns[i], refID));
		// cout << attrIDs[i] << " " << foreignIDs[i] << endl;
		if (attrIDs[i] == -1 || foreignIDs[i] == -1) {
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		if (!_tables[tableID].attrs[attrIDs[i]].notNull) {
			fprintf(stderr, "Error: foreign keys must be NOT-NULL!\n");
			return;
		}
	}
	int fileID = _tableFileID[tableName];
	int recordSize = _tables[tableID].recordSize;
	int refSize = _tables[refID].primarySize;
	RecManager * rm = new RecManager(bufPageManager, fileID, recordSize, false);
	RecManager::Iterator * iter = new RecManager::Iterator(rm);
	int indexID;
	_ixm->OpenIndex((refName + ".primary").c_str(), indexID);
	SIndexManager * sixm = new SIndexManager(bufPageManager, indexID);
	BufType data;
	BufType refData = new unsigned int[refSize >> 2];
	unsigned int id;
	while (iter->next(data, id)) {
		int pageID = id >> 16;
		int slotID = (id << 16 >> 16);
		int pos = 0;
		for (int i = 0; i < _tables[refID].primary.size(); i++) {
			string primaryName = _tables[refID].attrs[_tables[refID].primary[i]].attrName;
			int primaryID = _tables[refID].primary[i];
			//cout << primaryName << " " << primaryID << endl;
			int attr = -1;
			for (int j = 0; j < keyNum; j++) if (foreignIDs[j] == primaryID) {
				attr = attrIDs[j]; break;
			}
			//cout << _tables[tableID].attrs[attr].attrName << endl;
			memcpy(refData + pos, data + _tables[tableID].attrs[attr].offset, _tables[tableID].attrs[attr].attrLength);
			pos += (_tables[tableID].attrs[attr].attrLength >> 2);
		}
		// debug
		// for (int k = 0; k < 32; k++) {
		// 	char cc = *((char *)refData + k);
		// 	if (cc >= 32 && cc <= 126) cout << cc;
		// 	else cout << "*";
		// }
		// cout << endl;
		bool check = sixm->Exists(refData);
		if (!check) {
			//cout << (char*)refData << endl;
			fprintf(stderr, "Error: some foreign keys are not in the reference table!\n");
			delete [] refData;
			delete sixm;
			_ixm->CloseIndex(indexID);
			delete iter;
			delete rm;
			return;
		}

	}
	delete [] refData;
	delete sixm;
	_ixm->CloseIndex(indexID);
	delete iter;
	delete rm;

	for (int i = 0; i < attrIDs.size(); i++) {
		_tables[tableID].attrs[attrIDs[i]].reference = refName;
		_tables[tableID].attrs[attrIDs[i]].foreignKeyName = _tables[refID].attrs[foreignIDs[i]].attrName;
	}
	_tables[tableID].foreignSet.insert(refName);
	_tables[tableID].foreign.push_back(refName);
	_tables[tableID].fkNames[fkName] = refName;

}

void SysManager::DropForeignKey(const string tableName, const string fkName) { 
	int tableID = _fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: invalid table!\n");
		return;
	}
	if (_tables[tableID].fkNames.find(fkName) == _tables[tableID].fkNames.end()) {
		fprintf(stderr, "Error: wrong foreign key name!\n");
		return;
	}
	string refName = _tables[tableID].fkNames[fkName];
	int refID = _fromNameToID(refName);
	if (refID == -1) {
		fprintf(stderr, "Error: invalid reference table!\n");
		return;
	}
	if (_tables[tableID].foreignSet.find(refName) == _tables[tableID].foreignSet.end()) {
		fprintf(stderr, "Error: table does not have the foreign keys!\n");
		return;
	}
	_tables[tableID].foreignSet.erase(refName);
	for (int i = 0; i < _tables[tableID].foreign.size(); i++) if (_tables[tableID].foreign[i] == refName) {
		_tables[tableID].foreign.erase(_tables[tableID].foreign.begin() + i);
		break;
	}
	for (int i = 0; i < _tables[tableID].attrNum; i++) if (_tables[tableID].attrs[i].reference == refName) {
		_tables[tableID].attrs[i].reference = "";
		_tables[tableID].attrs[i].foreignKeyName = "";
	}
	_tables[tableID].fkNames.erase(fkName);
}

void SysManager::AddColumn(const string tableName, AttrInfo attr) {
	int tableID = _fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: invalid table!\n");
		return;
	}
	attr.haveIndex = false;
	attr.notNull = true;
	attr.primary = false;
	attr.reference = attr.foreignKeyName = "";
	if (attr.attrType == INT) {
		attr.attrLength = 4;
	} else if (attr.attrType == DATE) {
		attr.attrLength = 4;
	} else if (attr.attrType == FLOAT) {
		attr.attrLength = 8;
	}
	while (attr.attrLength % 4 != 0) attr.attrLength++;
	if (attr.defaultValue != nullptr) {
		if (attr.attrType == INT) {
			attr.defaultValue = (BufType)new int(*(int *)attr.defaultValue);
		} else if (attr.attrType == DATE) {
			attr.defaultValue = (BufType)new int(*(int *)attr.defaultValue);
		} else if (attr.attrType == STRING) {
			BufType temp = (BufType)(new char[attr.attrLength]);
			strcpy((char*)temp, (char*)attr.defaultValue);
			attr.defaultValue = temp;
		} else if (attr.attrType == FLOAT) {
			attr.defaultValue = (BufType)new double(*(double *)attr.defaultValue);
		}
	}
	int recordSize = _tables[tableID].recordSize;
	attr.offset = recordSize >> 2;
	int newRecordSize = recordSize + attr.attrLength;
	system(("rm " + tableName + ".*").c_str()); // delete index (including primary)
	fileManager->createFile((tableName + ".backup.temp").c_str());
	int fileID = _tableFileID[tableName];
	int newFileID;
	fileManager->openFile((tableName + ".backup.temp").c_str(), newFileID);
	
	RecManager * rm = new RecManager(bufPageManager, fileID, recordSize, false);
	RecManager * newrm = new RecManager(bufPageManager, newFileID, newRecordSize, true);
	RecManager::Iterator * iter = new RecManager::Iterator(rm);
	BufType data;
	unsigned int id;
	int attrNum = _tables[tableID].attrNum;
	while (iter->next(data, id)) {
		int pageID = id >> 16;
		int slotID = (id << 16 >> 16);
		unsigned long long *bitmap = (unsigned long long*)data;
		bitmap[0] |= (1ull << attrNum);
		BufType newData = new unsigned int[newRecordSize >> 2];
		memcpy((char*)newData, data, recordSize);
		memcpy((char*)newData + recordSize, attr.defaultValue, attr.attrLength);
		newrm->insertRec(newData, id);
		delete [] newData;
	}
	delete iter;
	delete newrm;
	delete rm;
	fileManager->closeFile(newFileID);
	fileManager->closeFile(fileID);
	system(("rm " + tableName).c_str());
	system(("mv " + tableName + ".backup.temp " + tableName).c_str());
	fileManager->openFile(tableName.c_str(), newFileID);
	_tableFileID[tableName] = newFileID;
	_tables[tableID].attrNum++;
	_tables[tableID].recordSize = newRecordSize;
	_tables[tableID].attrs.push_back(attr);

	for (int i = 0; i < _tables[tableID].attrNum; i++) if (_tables[tableID].attrs[i].haveIndex) {
		string attr = _tables[tableID].attrs[i].attrName;
		_tables[tableID].attrs[i].haveIndex = false;
		_CreateIndex(tableName, attr);
	}
	vector<string> attrs;
	if (_tables[tableID].primary.size() != 0) {
		attrs.clear();
		for (int i = 0; i < _tables[tableID].primary.size(); i++) {
			attrs.push_back(_tables[tableID].attrs[_tables[tableID].primary[i]].attrName);
		}
		_tables[tableID].primary.clear();
		_tables[tableID].primarySize = 0;
		AddPrimaryKey(tableName, attrs);
	}
}

void SysManager::DropColumn(const string tableName, string attrName) {
	int tableID = _fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: invalid table!\n");
		return;
	}
	int attrID = _fromNameToID(attrName, tableID);
	if (attrID == -1) {
		fprintf(stderr, "Error: invalid column!\n");
		return;
	}
	if (_tables[tableID].attrs[attrID].primary || _tables[tableID].attrs[attrID].reference != "") {
		fprintf(stderr, "Error: cannot drop primary keys or foreign keys!\n");
		return;
	}
	int recordSize = _tables[tableID].recordSize;
	int newRecordSize = recordSize - _tables[tableID].attrs[attrID].attrLength;
	system(("rm " + tableName + ".*").c_str()); // delete index (including primary)
	fileManager->createFile((tableName + ".backup.temp").c_str());
	int fileID = _tableFileID[tableName];
	int newFileID;
	fileManager->openFile((tableName + ".backup.temp").c_str(), newFileID);
	
	RecManager * rm = new RecManager(bufPageManager, fileID, recordSize, false);
	RecManager * newrm = new RecManager(bufPageManager, newFileID, newRecordSize, true);
	RecManager::Iterator * iter = new RecManager::Iterator(rm);

	int attrNum = _tables[tableID].attrNum;
	int pos =_tables[tableID].attrs[attrID].offset;
	int len = _tables[tableID].attrs[attrID].attrLength >> 2;
	//cout << pos << " " << len << endl;
	for (int i = attrID + 1; i < attrNum; i++) {
		_tables[tableID].attrs[i].offset -= len;
	}
	BufType data;
	unsigned int id;
	while (iter->next(data, id)) {
		int pageID = id >> 16;
		int slotID = (id << 16 >> 16);
		unsigned long long *bitmap = (unsigned long long*)data;
		for (int i = pos; i < (newRecordSize >> 2); i++) {
			data[i] = data[i + len];
		}
		for (int i = attrID; i < attrNum - 1; i++) {
			if (bitmap[0] & (1ull << (i + 1))) {
				bitmap[0] |= (1ull << i);
			} else {
				bitmap[0] &= ~(1ull << i);
			}
		}
		bitmap[0] &= ~(1ull << (attrNum - 1));
		newrm->insertRec(data, id);
	}
	delete iter;
	delete newrm;
	delete rm;
	fileManager->closeFile(newFileID);
	fileManager->closeFile(fileID);

	system(("rm " + tableName).c_str());
	system(("mv " + tableName + ".backup.temp " + tableName).c_str());
	fileManager->openFile(tableName.c_str(), newFileID);
	_tableFileID[tableName] = newFileID;
	_tables[tableID].attrNum--;
	_tables[tableID].recordSize = newRecordSize;
	_tables[tableID].attrs.erase(_tables[tableID].attrs.begin() + attrID);
	for (int i = 0; i < _tables[tableID].attrNum; i++) if (_tables[tableID].attrs[i].haveIndex) {
		string attr = _tables[tableID].attrs[i].attrName;
		_tables[tableID].attrs[i].haveIndex = false;
		_CreateIndex(tableName, attr);
	}
	vector<string> attrs;
	if (_tables[tableID].primary.size() != 0) {
		attrs.clear();
		for (int i = 0; i < _tables[tableID].attrNum; i++) if (_tables[tableID].attrs[i].primary) {
			attrs.push_back(_tables[tableID].attrs[i].attrName);
		}
		_tables[tableID].primary.clear();
		_tables[tableID].primarySize = 0;
		AddPrimaryKey(tableName, attrs);
	}
}

void SysManager::RenameColumn(const string tableName, string oldAttrName, string newAttrName) {
	int tableID = _fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: invalid table!\n");
		return;
	}
	int attrID = _fromNameToID(oldAttrName, tableID);
	if (attrID == -1) {
		fprintf(stderr, "Error: invalid column!\n");
		return;
	}
	if (_tables[tableID].attrs[attrID].primary) {
		fprintf(stderr, "Error: cannot rename primary keys!\n");
		return;
	}
	_tables[tableID].attrs[attrID].attrName = newAttrName;
	if (_tables[tableID].attrs[attrID].haveIndex) {
		_tables[tableID].attrs[attrID].haveIndex = false;
		system(("rm " + tableName + "." + oldAttrName).c_str()); // delete old index
		_CreateIndex(tableName, newAttrName);
	}
	for (auto [i, j] : _indexes) {
		if (j.first == tableName) {
			for (int k = 0; k < j.second.size(); k++) {
				if (j.second[k] == oldAttrName) {
					_indexes[i].second[k] = newAttrName;
				}
			}
		}
	}
}

void SysManager::ChangeColumn(const string tableName, string oldAttrName, AttrInfo newAttr) {
	int tableID = _fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: invalid table!\n");
		return;
	}
	int attrID = _fromNameToID(oldAttrName, tableID);
	if (attrID == -1) {
		fprintf(stderr, "Error: invalid column!\n");
		return;
	}
	if (_tables[tableID].attrs[attrID].primary || _tables[tableID].attrs[attrID].reference != "") {
		fprintf(stderr, "Error: cannot change primary keys or foreign keys!\n");
		return;
	}
	// pre-process newAttr
	newAttr.haveIndex = false;
	newAttr.notNull = true;
	newAttr.primary = false;
	newAttr.reference = newAttr.foreignKeyName = "";
	if (newAttr.attrType == INT) {
		newAttr.attrLength = 4;
	} else if (newAttr.attrType == DATE) {
		newAttr.attrLength = 4;
	} else if (newAttr.attrType == FLOAT) {
		newAttr.attrLength = 8;
	}
	while (newAttr.attrLength % 4 != 0) newAttr.attrLength++;
	if (newAttr.defaultValue != nullptr) {
		if (newAttr.attrType == INT) {
			newAttr.defaultValue = (BufType)new int(*(int *)newAttr.defaultValue);
		} else if (newAttr.attrType == DATE) {
			newAttr.defaultValue = (BufType)new int(*(int *)newAttr.defaultValue);
		} else if (newAttr.attrType == STRING) {
			BufType temp = (BufType)(new char[newAttr.attrLength]);
			strcpy((char*)temp, (char*)newAttr.defaultValue);
			newAttr.defaultValue = temp;
		} else if (newAttr.attrType == FLOAT) {
			newAttr.defaultValue = (BufType)new double(*(double *)newAttr.defaultValue);
		}
	}

	AttrInfo & oldAttr = _tables[tableID].attrs[attrID];
	int recordSize = _tables[tableID].recordSize;
	int newRecordSize = recordSize - oldAttr.attrLength + newAttr.attrLength;
	newAttr.offset = oldAttr.offset;
	system(("rm " + tableName + ".*").c_str()); // delete index (including primary)
	fileManager->createFile((tableName + ".backup.temp").c_str());
	int fileID = _tableFileID[tableName];
	int newFileID;
	fileManager->openFile((tableName + ".backup.temp").c_str(), newFileID);
	
	RecManager * rm = new RecManager(bufPageManager, fileID, recordSize, false);
	RecManager * newrm = new RecManager(bufPageManager, newFileID, newRecordSize, true);
	RecManager::Iterator * iter = new RecManager::Iterator(rm);
	int attrNum = _tables[tableID].attrNum;
	int off = (newAttr.attrLength - oldAttr.attrLength) >> 2;
	for (int i = attrID + 1; i < attrNum; i++) {
		_tables[tableID].attrs[i].offset += off;
	}
	BufType data;
	unsigned int id;
	while (iter->next(data, id)) {
		int pageID = id >> 16;
		int slotID = (id << 16 >> 16);
		BufType newData = new unsigned int[newRecordSize >> 2];
		memcpy((char*)newData, (char*)data, (oldAttr.offset << 2));
		if (newAttr.attrType == STRING) {
			if (oldAttr.attrType == STRING) {
				sprintf((char*)newData + (oldAttr.offset << 2), "%s", (char*)&data[oldAttr.offset]); 
			} else if (oldAttr.attrType == INT || oldAttr.attrType == DATE) {
				sprintf((char*)newData + (oldAttr.offset << 2), "%d", *(int*)&data[oldAttr.offset]); 
			} else { // oldAttr.attrType == FLOAT
				sprintf((char*)newData + (oldAttr.offset << 2), "%f", *(double*)&data[oldAttr.offset]); 
			}
		} else if (newAttr.attrType == INT || newAttr.attrType == DATE) {
			if (oldAttr.attrType == STRING) {
				*(int*)&newData[oldAttr.offset] = stoi((char*)&data[oldAttr.offset]);
				cout << *(int*)&newData[oldAttr.offset] << endl;
			} else if (oldAttr.attrType == INT || oldAttr.attrType == DATE) {
				*(int*)&newData[oldAttr.offset] = *(int*)&data[oldAttr.offset];
			} else { // oldAttr.attrType == FLOAT
				*(int*)&newData[oldAttr.offset] = *(double*)&data[oldAttr.offset];
			}
		} else { // newAttr.attrType == FLOAT
			if (oldAttr.attrType == STRING) {
				*(double*)&newData[oldAttr.offset] = stof((char*)&data[oldAttr.offset]);
			} else if (oldAttr.attrType == INT || oldAttr.attrType == DATE) {
				*(double*)&newData[oldAttr.offset] = *(int*)&data[oldAttr.offset];
			} else { // oldAttr.attrType == FLOAT
				*(double*)&newData[oldAttr.offset] = *(double*)&data[oldAttr.offset];
			}
		}
		memcpy((char*)newData + (oldAttr.offset << 2) + newAttr.attrLength, 
			   (char*)data + (oldAttr.offset << 2) + oldAttr.attrLength,
			   recordSize - (oldAttr.offset << 2) - oldAttr.attrLength);
		newrm->insertRec(newData, id);
		delete [] newData;
	}
	delete iter;
	delete newrm;
	delete rm;
	fileManager->closeFile(newFileID);
	fileManager->closeFile(fileID);

	system(("rm " + tableName).c_str());
	system(("mv " + tableName + ".backup.temp " + tableName).c_str());
	fileManager->openFile(tableName.c_str(), newFileID);
	_tableFileID[tableName] = newFileID;
	_tables[tableID].recordSize = newRecordSize;
	_tables[tableID].attrs[attrID] = newAttr;
	for (int i = 0; i < _tables[tableID].attrNum; i++) if (_tables[tableID].attrs[i].haveIndex) {
		string attr = _tables[tableID].attrs[i].attrName;
		_tables[tableID].attrs[i].haveIndex = false;
		_CreateIndex(tableName, attr);
	}
	vector<string> attrs;
	if (_tables[tableID].primary.size() != 0) {
		attrs.clear();
		for (int i = 0; i < _tables[tableID].attrNum; i++) if (_tables[tableID].attrs[i].primary) {
			attrs.push_back(_tables[tableID].attrs[i].attrName);
		}
		_tables[tableID].primary.clear();
		_tables[tableID].primarySize = 0;
		AddPrimaryKey(tableName, attrs);
	}
}

bool SysManager::_checkForeignKeyOnTable(int tableID) {
	string tableName = _tables[tableID].tableName;
	for (int i = 0; i < _tableNum; i++) if (i != tableID) {
		if (_tables[i].foreignSet.find(tableName) != _tables[i].foreignSet.end()) {
			return true;
		}
	}
	return false;
}

int SysManager::_fromNameToID(const string tableName) {
	for (int i = 0; i < _tableNum; i++)
		if (_tables[i].tableName == tableName)
			return i;
	return -1;
}

int SysManager::_fromNameToID(const string attrName, const int tableID) {
	for (int i = 0; i < _tables[tableID].attrNum; i++)
		if (_tables[tableID].attrs[i].attrName == attrName)
			return i;
	return -1;
}

BufType SysManager::_getPrimaryKey(int tableID, BufType data) {
	BufType primaryKey = new unsigned int[_tables[tableID].primarySize >> 2];
	int pos = 0;
	for (int i = 0; i < _tables[tableID].attrNum; i++) if (_tables[tableID].attrs[i].primary) {
		memset(primaryKey + pos, 0, sizeof(char) * _tables[tableID].attrs[i].attrLength);
		memcpy(primaryKey + pos, data + _tables[tableID].attrs[i].offset, _tables[tableID].attrs[i].attrLength);
		pos += (_tables[tableID].attrs[i].attrLength >> 2);
	}
	return primaryKey;
}
