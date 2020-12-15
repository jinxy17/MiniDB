#include "DataOperater.h"
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <cassert>
using namespace std;

DataOperater::DataOperater(SysManager *smm, IndexManager *ixm, FileManager *_fileManager, BufPageManager *_bufPageManager) {
	_smm = smm; 
	_ixm = ixm;
	fileManager = _fileManager; 
	bufPageManager = _bufPageManager;
}
DataOperater::~DataOperater() {}

//插入一条数据,values为插入数据值,cloumns为对应列号(插入null没有列号),nullcolumnnub为null数据列数
void DataOperater::Insert(const string tableName, vector<BufType> values, vector<int> columns,int nullcolumnnub) {
	assert(values.size() == columns.size());
	int tableID = _smm->_fromNameToID(tableName);
	vector<AttrInfo> attrsinfos = _smm->_tables[tableID].attrs;

	// 非空列数目+空列数 必须等于 列数目
	int attrNum = columns.size();
	if (attrNum + nullcolumnnub != _smm->_tables[tableID].attrNum) {
		fprintf(stderr, "Error: invalid values number!\n");
		return;
	}

	// 1.存在外键,不能直接插入
	for (int i = 0; i < _smm->_tableNum; i++) if (i != tableID) {
		if (_smm->_tables[i].foreignSet.find(tableName) != _smm->_tables[i].foreignSet.end()) {
			fprintf(stderr, "Error: foreign keys on the table!\n");
			return;
		}
	}
	vector<string> attrs;
	for (int i = 0; i < columns.size(); i++) {
		int attrID = columns[i];
		attrs.push_back(attrsinfos[attrID].attrName);
	}

	
	BufType data = new unsigned int[_smm->_tables[tableID].recordSize >> 2];
	unsigned long long *bitmap = (unsigned long long*)data;
	bitmap[0] = 0;
	// 设置默认值的代码,因为文法只有显示null,所以默认值已经无效,这段代码相应删除
	// for (int i = 0; i < _smm->_tables[tableID].attrNum; i++) if (attrsinfos[i].defaultValue != nullptr) {
	// 	int attrID = i;
	// 	BufType value = attrsinfos[i].defaultValue;
	// 	bitmap[0] |= 1ull << attrID;
	// 	if (attrsinfos[attrID].attrType == INT) {
	// 		assert(attrsinfos[attrID].attrLength == 4);
	// 		int d = *(int*)value;
	// 		memcpy(data + attrsinfos[attrID].offset, &d, 4);
	// 	} else if (attrsinfos[attrID].attrType == FLOAT) {
	// 		assert(attrsinfos[attrID].attrLength == 8);
	// 		double dd = *(double*)value;
	// 		memcpy(data + attrsinfos[attrID].offset, &dd, 8);
	// 	} else if (attrsinfos[attrID].attrType == STRING) {
	// 		char *ddd = (char*)value;
	// 		int size = strlen(ddd);
	// 		memset(data + attrsinfos[attrID].offset, 0, sizeof(char) * attrsinfos[attrID].attrLength);
	// 		//fill(data + attrsinfos[attrID].offset, data + attrsinfos[attrID].offset + (attrsinfos[attrID].attrLength >> 2), 0);
	// 		// 把字符串长度设定为表定义的长度，注意末尾\0
	// 		if (size >= attrsinfos[attrID].attrLength) {
	// 			size = attrsinfos[attrID].attrLength;
	// 			ddd[size - 1] = '\0';
	// 		}
	// 		memcpy(data + attrsinfos[attrID].offset, ddd, size);
	// 	}
	// }
	for (int i = 0; i < attrNum; i++) {
		int attrID = _smm->_fromNameToID(attrs[i], tableID);
		if (attrID == -1) {
			fprintf(stderr, "Error: invalid column name!\n");
			delete [] data;
			return;
		}
		bitmap[0] |= 1ull << attrID;
		if (attrsinfos[attrID].attrType == INT) {
			assert(attrsinfos[attrID].attrLength == 4);
			int d = *(int*)values[i];
			memcpy(data + attrsinfos[attrID].offset, &d, 4);
		} else if (attrsinfos[attrID].attrType == FLOAT) {
			assert(attrsinfos[attrID].attrLength == 8);
			double dd = *(double*)values[i];
			memcpy(data + attrsinfos[attrID].offset, &dd, 8);
		} else if (attrsinfos[attrID].attrType == STRING) {
			char *ddd = (char*)values[i];
			int size = strlen(ddd);
			memset(data + attrsinfos[attrID].offset, 0, sizeof(char) * attrsinfos[attrID].attrLength);
			// 把字符串长度设定为表定义的长度，注意末尾\0
			if (size >= attrsinfos[attrID].attrLength) {
				size = attrsinfos[attrID].attrLength;
				ddd[size - 1] = '\0';
			}
			memcpy(data + attrsinfos[attrID].offset, ddd, size);
		}
	}
	for (int i = 0; i < _smm->_tables[tableID].attrNum; i++) {
		if (attrsinfos[i].notNull && (bitmap[0] & (1ull << i) == 0)) {
			fprintf(stderr, "Error: some NOT-NULL columns are null!\n");
			delete [] data;
			return;
		}
	}
	// 若有主键,则需检查主键是否重复
	int primary_size = _smm->_tables[tableID].primarySize;
	BufType primaryData = nullptr;
	if (primary_size != 0) {
		primaryData = _smm->_getPrimaryKey(tableID, data);
		int indexFileID;
		_ixm->OpenIndex((tableName + ".primary").c_str(), indexFileID);
		SIndexManager *indexhandle = new SIndexManager(bufPageManager, indexFileID);
		bool check = indexhandle->Exists(primaryData);
		_ixm->CloseIndex(indexFileID);
		delete indexhandle;
		if (check) {
			fprintf(stderr, "Error: repetitive primary keys!\n");
			delete [] data;
			if (primaryData != nullptr) delete [] primaryData;
			return;
		}
	}
	// 遍历所有外键
	for (int i = 0; i < _smm->_tables[tableID].foreignNum; i++) {
		string refName = _smm->_tables[tableID].foreign[i];
		int refID = _smm->_fromNameToID(refName);
		int refSize = _smm->_tables[refID].primarySize;
		BufType refData = new unsigned int[refSize >> 2];
		// 遍历ref表的所有主键，pos记录当前主键在主键索引中的offset
		int pos = 0;
		for (int j = 0; j < _smm->_tables[refID].primary.size(); j++) {
			string primaryName = _smm->_tables[refID].attrs[_smm->_tables[refID].primary[j]].attrName;
			int attr = -1;
			// 查找tableID里的对应列
			for (int k = 0; k < _smm->_tables[tableID].attrNum; k++) {
				if (attrsinfos[k].reference == refName && attrsinfos[k].foreignKeyName == primaryName) {
					attr = k;
					break;
				}
			}
			if (attr == -1) {
				fprintf(stderr, "Error: foreign keys are not complete!\n");
				delete [] data;
				if (primaryData != nullptr) delete [] primaryData;
				delete [] refData;
				return;
			}
			memcpy(refData + pos, data + attrsinfos[attr].offset, attrsinfos[attr].attrLength);
			pos += (attrsinfos[attr].attrLength >> 2);
		}
		int indexFileID;
		_ixm->OpenIndex((refName + ".primary").c_str(), indexFileID);
		SIndexManager *indexhandle = new SIndexManager(bufPageManager, indexFileID);
		bool check = indexhandle->Exists(refData);
		_ixm->CloseIndex(indexFileID);
		delete indexhandle;
		delete [] refData;
		if (!check) {
			fprintf(stderr, "Error: invalid foreign keys!\n");
			delete [] data;
			if (primaryData != nullptr) delete [] primaryData;
			return;
		}
	}
	unsigned int recID;
	int fileID = _smm->_tableFileID[tableName];
	RecManager *filehandle = new RecManager(bufPageManager, fileID, 0, false);
	filehandle->insertRec(data, recID);
	int pageID = recID >> 16;
	int slotID = recID << 16 >> 16;
	// printf("pageID:%d,slotID:%d\n",pageID,slotID);
	if (primary_size != 0) {
		int indexFileID;
		_ixm->OpenIndex((tableName + ".primary").c_str(), indexFileID);
		SIndexManager *indexhandle = new SIndexManager(bufPageManager, indexFileID);
		int pageID = recID >> ID_SEGM;
		int slotID = recID - (pageID << ID_SEGM);
		indexhandle->insertIx(primaryData, pageID, slotID);
		_ixm->CloseIndex(indexFileID);
		delete indexhandle;
		delete [] primaryData;
	}
	for (int i = 0; i < _smm->_tables[tableID].attrNum; i++) {
		if (attrsinfos[i].haveIndex) {
			BufType attrData = data + attrsinfos[i].offset;
			int indexFileID;
			_ixm->OpenIndex((tableName + "." + attrsinfos[i].attrName).c_str(), indexFileID);
			SIndexManager *indexhandle = new SIndexManager(bufPageManager, indexFileID);
			int pageID = recID >> ID_SEGM;
			int slotID = recID - (pageID << ID_SEGM);
			indexhandle->insertIx(attrData, pageID, slotID);
			_ixm->CloseIndex(indexFileID);
			delete indexhandle;
		}
	}
	delete [] data;
	delete filehandle;
}

void DataOperater::Update(const Assigns assigns, vector<Relation> relations) {
	assert(assigns.attrs.size() == assigns.values.size());
	assert(assigns.attrs.size() == assigns.assignnull.size());
	string tableName = assigns.table;
	//	有关于表的检查
	int tableID = _smm->_fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: such table does not exist!\n");
		return;
	}
	for (int i = 0; i < _smm->_tableNum; i++) if (i != tableID) {
		if (_smm->_tables[i].foreignSet.find(tableName) != _smm->_tables[i].foreignSet.end()) {
			fprintf(stderr, "Error: foreign keys on the table!\n");
			return;
		}
	}

	vector<int> offsets;
	vector<int> attrIDs;
	vector<int> attrLengths;
	vector<int> attrTypes;

	vector<int> indexFileIDs;
	vector<SIndexManager*> sindexhandles;
	for(int i = 0; i < assigns.attrs.size(); i++) {
		// 有关于主键外键等的各种检查
		int attrID = _smm->_fromNameToID(assigns.attrs[i], tableID);
		if (attrID == -1) {
			fprintf(stderr, "Error: such column does not exist!\n");
			return;
		}
		if (_smm->_tables[tableID].attrs[attrID].primary) {
			fprintf(stderr, "Error: cannot update primary keys!\n");
			return;
		}
		if (_smm->_tables[tableID].attrs[attrID].reference != "") {
			fprintf(stderr, "Error: cannot update foreign keys!\n");
			return;
		}
		int attrLength = _smm->_tables[tableID].attrs[attrID].attrLength;
		int attrType = _smm->_tables[tableID].attrs[attrID].attrType;
		if (assigns.values[i] == nullptr && _smm->_tables[tableID].attrs[attrID].notNull) {
			fprintf(stderr, "Error: set NOT-NULL columns to be null!\n");
			return;
		}

		attrIDs.push_back(attrID);
		offsets.push_back(_smm->_tables[tableID].attrs[attrID].offset);
		attrLengths.push_back(attrLength);
		attrTypes.push_back(attrType);
		// 找一个索引
		if (_smm->_tables[tableID].attrs[attrID].haveIndex) {
			int indexFileID = -1;
			_ixm->OpenIndex((tableName + "." + assigns.attrs[i]).c_str(), indexFileID);
			SIndexManager* sindexhandle = new SIndexManager(bufPageManager, indexFileID);
			indexFileIDs.push_back(indexFileID);
			sindexhandles.push_back(sindexhandle);
		} else {
			indexFileIDs.push_back(-1);
			sindexhandles.push_back(nullptr);
		}
	}

	//准备开始遍历
	int recordSize = _smm->_tables[tableID].recordSize;
	RecManager *filehandle = new RecManager(bufPageManager, _smm->_tableFileID[tableName], 0, false);
	RecManager::Iterator * iter = new RecManager::Iterator(filehandle);
	
	vector<int> attrID1, attrID2;
	for (int i = 0; i < relations.size(); i++) {
		int attr = _smm->_fromNameToID(relations[i].attr1, tableID);
		if (attr == -1) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID1.push_back(attr);
		if (relations[i].data != nullptr) {
			attrID2.push_back(-1);
			continue;
		}
		attr = _smm->_fromNameToID(relations[i].attr2, tableID);
		if (attr == -1) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID2.push_back(attr);
		if (_smm->_tables[tableID].attrs[attrID1[i]].attrType != _smm->_tables[tableID].attrs[attrID2[i]].attrType) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid comparison!\n");
			return;
		}
	}
	// 是否有索引且支持索引
	bool found = false;
	int indexAttr = -1, indexRel = -1, indexFileID = -1;
	SIndexManager *indexscan = nullptr;
	for (int i = 0; i < relations.size(); i++) {
		if (attrID2[i] != -1) continue;
		if (relations[i].op != EQ_OP) continue;
		if (!_smm->_tables[tableID].attrs[attrID1[i]].haveIndex) continue;
		found = true;
		indexAttr = attrID1[i]; indexRel = i;
		_ixm->OpenIndex((tableName + "." + _smm->_tables[tableID].attrs[indexAttr].attrName).c_str(), indexFileID);
		indexscan = new SIndexManager(bufPageManager, indexFileID);
		if (!indexscan->OpenScan(relations[i].data, true)) {
			delete indexscan;
			_ixm->CloseIndex(indexFileID);
			delete iter;
			delete filehandle; 
			return;
		}
		break;
	}

	unsigned int recID;
	int pageID,slotID;
	BufType data = new unsigned int[recordSize >> 2];
	while (1) {
		bool hasNext;
		// 有索引则用索引查询,否则直接查询
		if (found) {
			hasNext = indexscan->GetNextEntry(pageID, slotID);
			recID = pageID << 16 + slotID;
			// printf("使用索引遍历");
		} else {
			hasNext = iter->next(data, recID);
			if (!hasNext) break;
			pageID = recID >> 16;
			slotID = (recID << 16 >> 16);
			// printf("page:%d,slot:%d\n",pageID,slotID);
		}
		bool ok = true;
		// 用于设置Null标记
		unsigned long long *bitmap = (unsigned long long*)data;
		for (int i = 0; i < relations.size(); i++) {
			BufType data1 = data + _smm->_tables[tableID].attrs[attrID1[i]].offset;
			BufType data2 = relations[i].data;
			if (data2 == nullptr) {
				data2 = data + _smm->_tables[tableID].attrs[attrID2[i]].offset;
			}
			if (relations[i].op == IS_NULL) {
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) == 0) continue;
				else {
					ok = false;
					break;
				}
			}
			if (relations[i].op == IS_NOT_NULL) {
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) != 0) continue;
				else {
					ok = false;
					break;
				}
			}
			if ((bitmap[0] & (1ull << attrID1[i])) == 0) {
				ok = false; break;
			}
			if ((attrID2[i] != -1) && ((bitmap[0] & (1ull << attrID2[i])) == 0)) {
				ok = false; break;
			}
			ok = _compare(data1, data2, relations[i].op, _smm->_tables[tableID].attrs[attrID1[i]].attrType);
			if (!ok) break;
		}
		if (ok) {
			for(int i = 0;i < attrIDs.size();i++){
				if (indexFileIDs[i] != -1) {
					sindexhandles[i]->deleteIx((void*)(data + offsets[i]), pageID, slotID);
				}
				fill(data + offsets[i], data + offsets[i] + (attrLengths[i] >> 2), 0);
				if (assigns.assignnull[i] == false && attrTypes[i] == STRING) {
					int strLen = strlen((char*)assigns.values[i]);
					if (strLen >= attrLengths[i]) {
						strLen = attrLengths[i];
						((char*)assigns.values[i])[strLen - 1] = '\0';
					}
					memcpy(data + offsets[i], assigns.values[i], strLen);
				} else if(assigns.assignnull[i] == false){
					memcpy(data + offsets[i], assigns.values[i], attrLengths[i]);
				}
				// 原来是null,更新之后不是null
				if ((bitmap[0] & (1ull << attrIDs[i])) == 0 && assigns.assignnull[i] == false){
					bitmap[0] |= 1ull << attrIDs[i];
				}
				// 原来不是null,更新之后是null
				else if((bitmap[0] & (1ull << attrIDs[i])) != 0 && assigns.assignnull[i]){
					bitmap[0] ^= 1ull << attrIDs[i];
				}
				if (indexFileIDs[i] != -1) {
					sindexhandles[i]->insertIx((void*)(data + offsets[i]), pageID, slotID);
				}
			}
			// printf("before update -- pageID:%d,slotID:%d\n",pageID,slotID);
			filehandle->updateRec(data,recID);
		}
	}
	// TODO: delete data会引发内存异常,暂时找不到问题所在
	// delete [] data;
	for(int i = 0;i < attrIDs.size();i++){
		if (indexFileIDs[i] != -1) {
			_ixm->CloseIndex(indexFileIDs[i]);
			delete sindexhandles[i];
		}
	}
	delete iter;
	delete filehandle;
}

//更新数据
// TODO：添加多个set
void DataOperater::Update(const Assign assign, vector<Relation> relations) {
	string tableName = assign.table;
	string attrName = assign.attr;

	//	有关于表的检查
	int tableID = _smm->_fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: such table does not exist!\n");
		return;
	}
	for (int i = 0; i < _smm->_tableNum; i++) if (i != tableID) {
		if (_smm->_tables[i].foreignSet.find(tableName) != _smm->_tables[i].foreignSet.end()) {
			fprintf(stderr, "Error: foreign keys on the table!\n");
			return;
		}
	}

	// 有关于主键外键等的各种检查
	int attrID = _smm->_fromNameToID(assign.attr, tableID);
	if (attrID == -1) {
		fprintf(stderr, "Error: such column does not exist!\n");
		return;
	}
	if (_smm->_tables[tableID].attrs[attrID].primary) {
		fprintf(stderr, "Error: cannot update primary keys!\n");
		return;
	}
	if (_smm->_tables[tableID].attrs[attrID].reference != "") {
		fprintf(stderr, "Error: cannot update foreign keys!\n");
		return;
	}
	int attrLength = _smm->_tables[tableID].attrs[attrID].attrLength;
	int attrType = _smm->_tables[tableID].attrs[attrID].attrType;
	if (assign.value == nullptr && _smm->_tables[tableID].attrs[attrID].notNull) {
		fprintf(stderr, "Error: set NOT-NULL columns to be null!\n");
		return;
	}

	int indexFileID = -1;
	SIndexManager *sindexhandle;
	int recordSize = _smm->_tables[tableID].recordSize;
	int offset = _smm->_tables[tableID].attrs[attrID].offset;
	if (_smm->_tables[tableID].attrs[attrID].haveIndex) {
		_ixm->OpenIndex((tableName + "." + attrName).c_str(), indexFileID);
		sindexhandle = new SIndexManager(bufPageManager, indexFileID);
	}
	RecManager *filehandle = new RecManager(bufPageManager, _smm->_tableFileID[tableName], 0, false);
	RecManager::Iterator * iter = new RecManager::Iterator(filehandle);
	
	vector<int> attrID1, attrID2;
	for (int i = 0; i < relations.size(); i++) {
		int attr = _smm->_fromNameToID(relations[i].attr1, tableID);
		if (attr == -1) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID1.push_back(attr);
		if (relations[i].data != nullptr) {
			attrID2.push_back(-1);
			continue;
		}
		attr = _smm->_fromNameToID(relations[i].attr2, tableID);
		if (attr == -1) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID2.push_back(attr);
		if (_smm->_tables[tableID].attrs[attrID1[i]].attrType != _smm->_tables[tableID].attrs[attrID2[i]].attrType) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid comparison!\n");
			return;
		}
	}

	unsigned int recID;
	BufType data = new unsigned int[recordSize >> 2];
	while (1) {
		bool hasNext = iter->next(data, recID);
		if (!hasNext) break;
		int pageID = recID >> 16;
		int slotID = (recID << 16 >> 16);

		bool ok = true;
		// 用于设置Null标记
		unsigned long long *bitmap = (unsigned long long*)data;
		for (int i = 0; i < relations.size(); i++) {
			BufType data1 = data + _smm->_tables[tableID].attrs[attrID1[i]].offset;
			BufType data2 = relations[i].data;
			if (data2 == nullptr) {
				data2 = data + _smm->_tables[tableID].attrs[attrID2[i]].offset;
			}
			if (relations[i].op == IS_NULL) {
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) == 0) continue;
				else {
					ok = false;
					break;
				}
			}
			if (relations[i].op == IS_NOT_NULL) {
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) != 0) continue;
				else {
					ok = false;
					break;
				}
			}
			if ((bitmap[0] & (1ull << attrID1[i])) == 0) {
				ok = false; break;
			}
			if ((attrID2[i] != -1) && ((bitmap[0] & (1ull << attrID2[i])) == 0)) {
				ok = false; break;
			}
			ok = _compare(data1, data2, relations[i].op, _smm->_tables[tableID].attrs[attrID1[i]].attrType);
			if (!ok) break;
		}
		if (ok) {
			if (indexFileID != -1) {
				sindexhandle->deleteIx((void*)(data + offset), pageID, slotID);
			}
			fill(data + offset, data + offset + (attrLength >> 2), 0);
			if (assign.assignnull == false && attrType == STRING) {
				int strLen = strlen((char*)assign.value);
				if (strLen >= attrLength) {
					strLen = attrLength;
					((char*)assign.value)[strLen - 1] = '\0';
				}
				memcpy(data + offset, assign.value, strLen);
			} else if(assign.assignnull == false){
				memcpy(data + offset, assign.value, attrLength);
			}
			// 原来是null,更新之后不是null
			if ((bitmap[0] & (1ull << attrID)) == 0 && assign.assignnull == false){
				bitmap[0] |= 1ull << attrID;
			}
			// 原来不是null,更新之后是null
			else if((bitmap[0] & (1ull << attrID)) != 0 && assign.assignnull){
				bitmap[0] ^= 1ull << attrID;
			}
			// printf("before update -- pageID:%d,slotID:%d\n",pageID,slotID);
			filehandle->updateRec(data,recID);
			if (indexFileID != -1) {
				sindexhandle->insertIx((void*)(data + offset), pageID, slotID);
			}
		}
	}
	// TODO: delete data会引发内存异常,暂时找不到问题所在
	// delete [] data;
	if (indexFileID != -1) {
		_ixm->CloseIndex(indexFileID);
		delete sindexhandle;
	}
	delete iter;
	delete filehandle;
}

void DataOperater::Delete(const string tableName, vector<Relation> relations) {
	int tableID = _smm->_fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: such table does not exist!\n");
		return;
	}
	for (int i = 0; i < _smm->_tableNum; i++) if (i != tableID) {
		if (_smm->_tables[i].foreignSet.find(tableName) != _smm->_tables[i].foreignSet.end()) {
			fprintf(stderr, "Error: foreign keys on the table!\n");
			return;
		}
	}
	RecManager *filehandle = new RecManager(bufPageManager, _smm->_tableFileID[tableName], 0, false);
	RecManager::Iterator * iter = new RecManager::Iterator(filehandle);
	
	vector<int> attrID1, attrID2;
	for (int i = 0; i < relations.size(); i++) {
		int attr = _smm->_fromNameToID(relations[i].attr1, tableID);
		if (attr == -1) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID1.push_back(attr);
		if (relations[i].data != nullptr) {
			attrID2.push_back(-1);
			continue;
		}
		attr = _smm->_fromNameToID(relations[i].attr2, tableID);
		if (attr == -1) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID2.push_back(attr);
		if (_smm->_tables[tableID].attrs[attrID1[i]].attrType != _smm->_tables[tableID].attrs[attrID2[i]].attrType) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid comparison!\n");
			return;
		}
	}
	int primaryIndex = -1;
	SIndexManager *primaryhandle;
	vector<int> attrs, indexes;
	vector<SIndexManager*> handles;
	if (_smm->_tables[tableID].primary.size() != 0) {
		_ixm->OpenIndex((tableName + ".primary").c_str(), primaryIndex);
		primaryhandle = new SIndexManager(bufPageManager, primaryIndex);
	}
	for (int i = 0; i < _smm->_tables[tableID].attrNum; i++) if (_smm->_tables[tableID].attrs[i].haveIndex) {
		attrs.push_back(i);
		int indexFileID;
		_ixm->OpenIndex((tableName + '.' + _smm->_tables[tableID].attrs[i].attrName).c_str(), indexFileID);
		indexes.push_back(indexFileID);
		handles.push_back(new SIndexManager(bufPageManager, indexFileID));
	}
	int recordSize = _smm->_tables[tableID].recordSize;

	BufType data = new unsigned int[recordSize >> 2];
	unsigned int recID;
	while (1) {
		bool hasNext = iter->next(data, recID);
		if (!hasNext) break;
		int pageID = recID >> 16;
		int slotID = (recID << 16 >> 16);

		bool ok = true;
		unsigned long long *bitmap = (unsigned long long*)data;
		for (int i = 0; i < relations.size(); i++) {
			BufType data1 = data + _smm->_tables[tableID].attrs[attrID1[i]].offset;
			BufType data2 = relations[i].data;
			if (data2 == nullptr) {
				data2 = data + _smm->_tables[tableID].attrs[attrID2[i]].offset;
			}
			if (relations[i].op == IS_NULL) {
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) == 0) continue;
				else {
					ok = false;
					break;
				}
			}
			if (relations[i].op == IS_NOT_NULL) {
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) != 0) continue;
				else {
					ok = false;
					break;
				}
			}
			if ((bitmap[0] & (1ull << attrID1[i])) == 0) {
				ok = false; break;
			}
			if ((attrID2[i] != -1) && ((bitmap[0] & (1ull << attrID2[i])) == 0)) {
				ok = false; break;
			}
			ok = _compare(data1, data2, relations[i].op, _smm->_tables[tableID].attrs[attrID1[i]].attrType);
			if (!ok) break;
		}
		if (ok) {
			if (primaryIndex != -1) {
				BufType primaryData = _smm->_getPrimaryKey(tableID, data);
				primaryhandle->deleteIx(primaryData, pageID, slotID);
				delete primaryData;
			}
			for (int i = 0; i < handles.size(); i++) {
				BufType attrData = data + _smm->_tables[tableID].attrs[attrs[i]].offset;
				handles[i]->deleteIx(attrData, pageID, slotID);
			}
			filehandle->deleteRec(recID);
			// if (!filehandle->DeleteRec(pageID, slotID)) cout << "failed" << endl;
		}
	}
	// TODO: delete data会引发内存异常,暂时找不到问题所在
	// delete data;
	if (primaryIndex != -1) {
		_ixm->CloseIndex(primaryIndex);
		delete primaryhandle;
	}
	for (int i = 0; i < handles.size(); i++) {
		_ixm->CloseIndex(indexes[i]);
		delete handles[i];
	}
	delete iter;
	delete filehandle;
}

void DataOperater::Select(const string tableName, vector<Relation> relations, vector<string> attrNames) {
	int tableID = _smm->_fromNameToID(tableName);
	if (tableID == -1) {
		fprintf(stderr, "Error: such table does not exist!\n");
		return;
	}
	vector<int> attrIDs;
	for (int i = 0; i < attrNames.size(); i++) {
		//printf("列名:%s\n",attrNames[i].c_str());
		if(attrNames[i] == "*") {
			//printf("所有列\n");
			attrIDs.clear();
			for (int attrID = 0; attrID < _smm->_tables[tableID].attrNum; attrID++)
				attrIDs.push_back(attrID);
			break;
		}
		int attrID = _smm->_fromNameToID(attrNames[i], tableID);
		if (attrID == -1) {
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrIDs.push_back(attrID);
	}
	RecManager *filehandle = new RecManager(bufPageManager, _smm->_tableFileID[tableName], 0, false);
	RecManager::Iterator * iter = new RecManager::Iterator(filehandle);

	vector<int> attrID1, attrID2;
	for (int i = 0; i < relations.size(); i++) {
		//列名:%s\n",relations[i].attr1.c_str());
		int attr = _smm->_fromNameToID(relations[i].attr1, tableID);
		if (attr == -1) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID1.push_back(attr);
		if (relations[i].data != nullptr) {
			attrID2.push_back(-1);
			continue;
		}
		attr = _smm->_fromNameToID(relations[i].attr2, tableID);
		if (attr == -1) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID2.push_back(attr);
		if (_smm->_tables[tableID].attrs[attrID1[i]].attrType != _smm->_tables[tableID].attrs[attrID2[i]].attrType) {
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid comparison!\n");
			return;
		}
	}
	// 是否有索引且支持索引
	bool found = false;
	int indexAttr = -1, indexRel = -1, indexFileID = -1;
	SIndexManager *indexscan = nullptr;
	for (int i = 0; i < relations.size(); i++) {
		if (attrID2[i] != -1) continue;
		if (relations[i].op != EQ_OP) continue;
		if (!_smm->_tables[tableID].attrs[attrID1[i]].haveIndex) continue;
		found = true;
		indexAttr = attrID1[i]; indexRel = i;
		_ixm->OpenIndex((tableName + "." + _smm->_tables[tableID].attrs[indexAttr].attrName).c_str(), indexFileID);
		indexscan = new SIndexManager(bufPageManager, indexFileID);
		if (!indexscan->OpenScan(relations[i].data, true)) {
			delete indexscan;
			_ixm->CloseIndex(indexFileID);
			delete iter;
			delete filehandle; 
			return;
		}
		break;
	}
	int recordSize = _smm->_tables[tableID].recordSize;
	int outcnt = 0;

	unsigned int recID;
	int pageID, slotID;

	BufType data = new unsigned int[recordSize >> 2];
	while (1) {
		bool hasNext;
		// 有索引则用索引查询,否则直接查询
		if (found) {
			hasNext = indexscan->GetNextEntry(pageID, slotID);
			// filehandle->GetRec(pageID, slotID, data);
		} else {
			hasNext = iter->next(data, recID);
			if (!hasNext) break;
			pageID = recID >> 16;
			slotID = (recID << 16 >> 16);
			// printf("page:%d,slot:%d\n",pageID,slotID);
		}
		bool ok = true;
		unsigned long long *bitmap = (unsigned long long*)data;
		for (int i = 0; i < relations.size(); i++) {
			BufType data1 = data + _smm->_tables[tableID].attrs[attrID1[i]].offset;
			BufType data2 = relations[i].data;
			if (data2 == nullptr) {
				data2 = data + _smm->_tables[tableID].attrs[attrID2[i]].offset;
			}
			if (relations[i].op == IS_NULL) {
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) == 0) continue;
				else {
					ok = false;
					break;
				}
			}
			if (relations[i].op == IS_NOT_NULL) {
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) != 0) continue;
				else {
					ok = false;
					break;
				}
			}
			if ((bitmap[0] & (1ull << attrID1[i])) == 0) {
				ok = false; break;
			}
			if ((attrID2[i] != -1) && ((bitmap[0] & (1ull << attrID2[i])) == 0)) {
				ok = false; break;
			}
			ok = _compare(data1, data2, relations[i].op, _smm->_tables[tableID].attrs[attrID1[i]].attrType);
			if (i == indexRel && !ok) hasNext = false;
			if (!ok) break;
		}
		if (ok) {
			outcnt++;
			if (outcnt <= 100) {
				putchar('|');
				//cout << " bitmap: " << bitmap[0] << " |";
				for (int i = 0; i < attrIDs.size(); i++) {
					BufType out = data + _smm->_tables[tableID].attrs[attrIDs[i]].offset;
					//cout << " " << (bitmap[0] & (1ull << attrIDs[i])) << " ";
					if ((bitmap[0] & (1ull << attrIDs[i])) == 0) {
						printf(" NULL |");
						continue;
					}
					if (_smm->_tables[tableID].attrs[attrIDs[i]].attrType == INT) {
						printf(" INT %d ", *(int*)out);
					} else if (_smm->_tables[tableID].attrs[attrIDs[i]].attrType == FLOAT) {
						printf(" FLOAT %.6lf ", *(double*)out);
					} else if (_smm->_tables[tableID].attrs[attrIDs[i]].attrType == STRING) {
						printf(" STRING %s ", (char*)out);
					}
					putchar('|');
				}
				putchar('\n');
			}
		}
	}
	// TODO: delete data会引发内存异常,暂时找不到问题所在
	// delete data;
	if (outcnt > 100) {
		puts("...");
		printf("Altogether %d records.\n", outcnt);
	}
	if (found) {
		delete indexscan;
		_ixm->CloseIndex(indexFileID);
	}
	delete iter;
	delete filehandle;
}

void DataOperater::Select(string tableName1, string tableName2, vector<Relation> relations, vector<string> attrNames) {
	int tableID1 = _smm->_fromNameToID(tableName1);
	if (tableID1 == -1) {
		fprintf(stderr, "Error: such table does not exist!\n");
		return;
	}
	int tableID2 = _smm->_fromNameToID(tableName2);
	if (tableID2 == -1) {
		fprintf(stderr, "Error: such table does not exist!\n");
		return;
	}
	if (tableID1 == tableID2) {
		fprintf(stderr, "Error: cannot join same tables!\n");
		return;
	}
	vector<pair<int, int> > attrIDs;
	for (int i = 0; i < attrNames.size(); i++) {
		int pos = attrNames[i].find('.');
		if (pos == string::npos) {
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		string table = attrNames[i].substr(0, pos);
		string attr = attrNames[i].substr(pos + 1);
		int attrID = -1;
		if (table == tableName1) {
			attrID = _smm->_fromNameToID(attr, tableID1);
			attrIDs.push_back(make_pair(tableID1, attrID));
		} else if (table == tableName2) {
			attrID = _smm->_fromNameToID(attr, tableID2);
			attrIDs.push_back(make_pair(tableID2, attrID));
		}
		if (attrID == -1) {
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
	}
	vector<pair<int, int> > attrID1, attrID2;
	for (int i = 0; i < relations.size(); i++) {
		int tableID = _smm->_fromNameToID(relations[i].table1);
		if (tableID == -1) {
			fprintf(stderr, "Error: invalid tables!\n");
			return;
		}
		int attr = _smm->_fromNameToID(relations[i].attr1, tableID);
		if (attr == -1) {
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID1.push_back(make_pair(tableID, attr));
		if (relations[i].data != nullptr) {
			attrID2.push_back(make_pair(-1, -1));
			continue;
		}
		tableID = _smm->_fromNameToID(relations[i].table2);
		if (tableID == -1) {
			fprintf(stderr, "Error: invalid tables!\n");
			return;
		}
		attr = _smm->_fromNameToID(relations[i].attr2, tableID);
		if (attr == -1) {
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID2.push_back(make_pair(tableID, attr));
		if (_smm->_tables[attrID1[i].first].attrs[attrID1[i].second].attrType != _smm->_tables[attrID2[i].first].attrs[attrID2[i].second].attrType) {
			fprintf(stderr, "Error: invalid comparison!\n");
			return;
		}
	}
	bool found = false;
	int indexAttr = -1, indexRel = -1, indexFileID = -1;
	SIndexManager *indexscan = nullptr;
	for (int i = 0; i < relations.size(); i++) {
		if (attrID2[i].first == -1) continue;
		if (attrID1[i].first == attrID2[i].first) continue;
		if (relations[i].op != EQ_OP) continue;
		if (_smm->_tables[attrID1[i].first].attrs[attrID1[i].second].haveIndex) {
			swap(attrID1[i], attrID2[i]);
		}
		if (!_smm->_tables[attrID2[i].first].attrs[attrID2[i].second].haveIndex) continue;
		if (attrID1[i].first != tableID1) {
			swap(tableID1, tableID2);
			tableName1.swap(tableName2);
		}
		found = true;
		indexAttr = attrID2[i].second; indexRel = i;
		_ixm->OpenIndex((_smm->_tables[attrID2[i].first].tableName + "." +_smm->_tables[attrID2[i].first].attrs[attrID2[i].second].attrName).c_str(), indexFileID);
		indexscan = new SIndexManager(bufPageManager, indexFileID);
		break;
	}
	RecManager *filehandle1 = new RecManager(bufPageManager, _smm->_tableFileID[tableName1], 0, false);
	RecManager::Iterator * iter1 = new RecManager::Iterator(filehandle1);

	RecManager *filehandle2 = new RecManager(bufPageManager, _smm->_tableFileID[tableName2], 0, false);
	RecManager::Iterator * iter2 = new RecManager::Iterator(filehandle2);

	int recordSize1 = _smm->_tables[tableID1].recordSize, recordSize2 = _smm->_tables[tableID2].recordSize;

	unsigned int recID;
	int pageID, slotID;
	int outcnt = 0;
	while (1) {
		BufType data1 = new unsigned int[recordSize1 >> 2], data2 = new unsigned int[recordSize2 >> 2];
		bool hasNext = iter1->next(data1,recID);
		pageID = recID >> 16;
		slotID = (recID << 16 >> 16);
		if (found) {
			BufType searchData = data1 + _smm->_tables[attrID1[indexRel].first].attrs[attrID1[indexRel].second].offset;
			if (!indexscan->OpenScan(searchData, true)) {
				delete [] data1;
				delete [] data2;
				if (!hasNext) break;
				continue;
			}
		}
		// if (!iter2->OpenScan(filehandle2)) {
		// 	delete [] data1;
		// 	delete [] data2;
		// 	if (!hasNext) break;
		// 	continue;
		// }
		while (1) {
			bool hasNext2;
			if (found) {
				hasNext2 = indexscan->GetNextEntry(pageID, slotID);
				// filehandle2->GetRec(pageID, slotID, data2);
			} else {
				hasNext2 = iter2->next(data2, recID);
				if (!hasNext2) break;
				pageID = recID >> 16;
				slotID = (recID << 16 >> 16);
			}
			bool ok = true;
			unsigned long long *bitmap1 = (unsigned long long*)data1;
			unsigned long long *bitmap2 = (unsigned long long*)data2;
			for (int i = 0; i < relations.size(); i++) {
				BufType attr1;
				//cout << recordSize1 << " " << recordSize2 << endl;
				//cout << _smm->_tables[attrID1[i].first].attrs[attrID1[i].second].offset << " " << _smm->_tables[attrID2[i].first].attrs[attrID2[i].second].offset << endl;
				if (attrID1[i].first == tableID1) {
					attr1 = data1 + _smm->_tables[tableID1].attrs[attrID1[i].second].offset;
					if (relations[i].op == IS_NULL) {
						if ((bitmap1[0] & (1ull << attrID1[i].second)) == 0) continue;
						else {
							ok = false; break;
						}
					}
					if (relations[i].op == IS_NOT_NULL) {
						if ((bitmap1[0] & (1ull << attrID1[i].second)) != 0) continue;
						else {
							ok = false; break;
						}
					}
					if ((bitmap1[0] & (1ull << attrID1[i].second)) == 0) {
						ok = false; break;
					}
				} else {
					attr1 = data2 + _smm->_tables[tableID2].attrs[attrID1[i].second].offset;
					if (relations[i].op == IS_NULL) {
						if ((bitmap2[0] & (1ull << attrID1[i].second)) == 0) continue;
						else {
							ok = false; break;
						}
					}
					if (relations[i].op == IS_NOT_NULL) {
						if ((bitmap2[0] & (1ull << attrID1[i].second)) != 0) continue;
						else {
							ok = false; break;
						}
					}
					if ((bitmap2[0] & (1ull << attrID1[i].second)) == 0) {
						ok = false; break;
					}
				}
				//cout << *(int*)attr1 << endl;
				BufType attr2 = relations[i].data;
				if (attr2 == nullptr) {
					if (attrID2[i].first == tableID1) {
						attr2 = data1 + _smm->_tables[tableID1].attrs[attrID2[i].second].offset;
						if ((bitmap1[0] & (1ull << attrID2[i].second)) == 0) {
							ok = false; break;
						}
					} else {
						attr2 = data2 + _smm->_tables[tableID2].attrs[attrID2[i].second].offset;
						if ((bitmap2[0] & (1ull << attrID2[i].second)) == 0) {
							ok = false; break;
						}
					}
				}
				//cout << *(int*)attr2 << endl;
				ok = _compare(attr1, attr2, relations[i].op, _smm->_tables[attrID1[i].first].attrs[attrID1[i].second].attrType);
				if (i == indexRel && !ok) hasNext2 = false;
				if (!ok) break;
			}
			if (ok) {
				outcnt++;
				if (outcnt <= 100) {
					putchar('|');
					//cout << " bitmap: " << bitmap[0] << " |";
					for (int i = 0; i < attrIDs.size(); i++) {
						BufType out;
						if (attrIDs[i].first == tableID1) {
							out = data1 + _smm->_tables[tableID1].attrs[attrIDs[i].second].offset;
							if ((bitmap1[0] & (1ull << attrIDs[i].second)) == 0) {
								printf(" NULL |");
								continue;
							}
						} else {
							out = data2 + _smm->_tables[tableID2].attrs[attrIDs[i].second].offset;
							if ((bitmap2[0] & (1ull << attrIDs[i].second)) == 0) {
								printf(" NULL |");
								continue;
							}
						}
						//cout << " " << (bitmap[0] & (1ull << attrIDs[i])) << " ";
						if (_smm->_tables[attrIDs[i].first].attrs[attrIDs[i].second].attrType == INT) {
							printf(" INT %d ", *(int*)out);
						} else if (_smm->_tables[attrIDs[i].first].attrs[attrIDs[i].second].attrType == FLOAT) {
							printf(" FLOAT %.6lf ", *(double*)out);
						} else if (_smm->_tables[attrIDs[i].first].attrs[attrIDs[i].second].attrType == STRING) {
							printf(" STRING %s ", (char*)out);
						}
						putchar('|');
					}
					putchar('\n');
				}
			}
			if (!hasNext2) break;
		}
		// TODO: delete data会引发内存异常,暂时找不到问题所在
		// delete [] data1;
		// delete [] data2;
		if (!hasNext) break;
	}
	if (outcnt > 100) {
		puts("...");
		printf("Altogether %d records.\n", outcnt);
	}
	if (found) {
		delete indexscan;
		_ixm->CloseIndex(indexFileID);
	}
	delete iter1;
	delete filehandle1;
	delete iter2;
	delete filehandle2;
}

bool DataOperater::_compare(BufType data1, BufType data2, CompOp op, int type) {
	if (type == STRING) {
		int result = strcmp((char*)data1, (char*)data2);
		if (op == EQ_OP) return result == 0;
		if (op == NE_OP) return result != 0;
		if (op == LT_OP) return result < 0;
		if (op == GT_OP) return result > 0;
		if (op == LE_OP) return result <= 0;
		if (op == GE_OP) return result >= 0;
	} else if (type == INT) {
		if (op == EQ_OP) return *(int*)data1 == *(int*)data2;
		if (op == NE_OP) return *(int*)data1 != *(int*)data2;
		if (op == LT_OP) return *(int*)data1 < *(int*)data2;
		if (op == GT_OP) return *(int*)data1 > *(int*)data2;
		if (op == LE_OP) return *(int*)data1 <= *(int*)data2;
		if (op == GE_OP) return *(int*)data1 >= *(int*)data2;
	} else if (type == FLOAT) {
		if (op == EQ_OP) return *(double*)data1 == *(double*)data2;
		if (op == NE_OP) return *(double*)data1 != *(double*)data2;
		if (op == LT_OP) return *(double*)data1 < *(double*)data2;
		if (op == GT_OP) return *(double*)data1 > *(double*)data2;
		if (op == LE_OP) return *(double*)data1 <= *(double*)data2;
		if (op == GE_OP) return *(double*)data1 >= *(double*)data2;
	}
}

// void DataOperater::Load(const string tableName, const string fileName) {
// 	int tableID = _smm->_fromNameToID(tableName);
// 	if (tableID == -1) {
// 		fprintf(stderr, "Error: such table does not exist!\n");
// 		return;
// 	}
// 	ifstream load(fileName);
// 	string str;
// 	vector<string> attrs;
// 	for (int i = 0; i < _smm->_tables[tableID].attrNum; i++) {
// 		attrs.push_back(_smm->_tables[tableID].attrs[i].attrName);
// 	}
// 	//cout << tableName << endl;
// 	cout << "loading ..." << endl;
// 	//bool flag = false;
// 	while (getline(load, str)) {
// 		vector<BufType> values;
// 		string buf = "";
// 		int attrCnt = 0;
// 		for (int i = 0; i < str.length(); i++) {
// 			if (str[i] == '|') {
// 				//cout << buf << endl;
// 				if (_smm->_tables[tableID].attrs[attrCnt].attrType == INT) {
// 					int *d = new int; *d = atoi(buf.c_str());
// 					values.push_back((BufType)d);
// 				} else if (_smm->_tables[tableID].attrs[attrCnt].attrType == FLOAT) {
// 					double *d = new double; *d = atof(buf.c_str());
// 					values.push_back((BufType)d);
// 				} else if (_smm->_tables[tableID].attrs[attrCnt].attrType == STRING) {
// 					char *d = new char[_smm->_tables[tableID].attrs[attrCnt].attrLength];
// 					memset(d, 0, sizeof(char) * _smm->_tables[tableID].attrs[attrCnt].attrLength);
// 					//cout << "char: " << _smm->_tables[tableID].attrs[attrCnt].attrLength << ' ' << buf.length() << endl;
// 					memcpy(d, buf.c_str(), min(_smm->_tables[tableID].attrs[attrCnt].attrLength, (int)buf.length()));
// 					values.push_back((BufType)d);
// 				}
// 				buf = "";
// 				attrCnt++;
// 			} else {
// 				buf.push_back(str[i]);
// 			}
// 		}
// 		Insert(tableName, attrs, values);
// 	}
// 	load.close();
// 	cout << "done." << endl;
// }