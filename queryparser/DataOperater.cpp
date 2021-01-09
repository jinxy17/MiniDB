#include "DataOperater.h"
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <cassert>
using namespace std;

DataOperater::DataOperater(SysManager *smm, IndexManager *ixm, FileManager *_fileManager, BufPageManager *_bufPageManager)
{
	_smm = smm;
	_ixm = ixm;
	fileManager = _fileManager;
	bufPageManager = _bufPageManager;
}
DataOperater::~DataOperater() {}

//插入一条数据,values为插入数据值,cloumns为对应列号(插入null没有列号),nullcolumnnub为null数据列数
bool DataOperater::Insert(const string tableName, vector<Value *> insertvalues, vector<int> columns, int nullcolumnnub)
{
	// printf("Insert into %s ,nullcolnub:%d\n",tableName.c_str(),nullcolumnnub);
	assert(insertvalues.size() == columns.size());
	int tableID = _smm->_fromNameToID(tableName);
	if (tableID == -1)
	{
		fprintf(stderr, "Error: such table does not exist!\n");
		return 0;
	}
	// printf("tableID:%d\n",tableID);
	vector<AttrInfo> attrsinfos = _smm->_tables[tableID].attrs;

	// 非空列数目+空列数 必须等于 列数目
	int attrNum = columns.size();
	if (attrNum + nullcolumnnub != _smm->_tables[tableID].attrNum)
	{
		fprintf(stderr, "Error: invalid values number!\n");
		return 0;
	}

	// 1.存在外键,不能直接插入
	for (int i = 0; i < _smm->_tableNum; i++)
		if (i != tableID)
		{
			if (_smm->_tables[i].foreignSet.find(tableName) != _smm->_tables[i].foreignSet.end())
			{
				fprintf(stderr, "Error: foreign keys on the table!\n");
				return 0;
			}
		}
	vector<string> attrs;
	vector<BufType> values;
	for (int i = 0; i < columns.size(); i++)
	{
		int attrID = columns[i];
		attrs.push_back(attrsinfos[attrID].attrName);
		//类型检查与转换
		if (attrsinfos[attrID].attrType == INT || attrsinfos[attrID].attrType == DATE)
		{
			if (insertvalues[i]->datatype == INT || insertvalues[i]->datatype == DATE)
			{
				values.push_back(insertvalues[i]->data);
			}
			else if (insertvalues[i]->datatype == FLOAT)
			{
				double fdata = *(double *)insertvalues[i]->data;
				int *idata = new int(fdata);
				values.push_back((BufType)idata);
			}
			else
			{
				printf("Error: data type does not fit!\n");
				return 0;
			}
		}
		else if (attrsinfos[attrID].attrType == FLOAT)
		{
			if (insertvalues[i]->datatype == INT || insertvalues[i]->datatype == DATE)
			{
				int idata = *(int *)insertvalues[i]->data;
				double *fdata = new double(idata);
				values.push_back((BufType)fdata);
			}
			else if (insertvalues[i]->datatype == FLOAT)
			{
				values.push_back(insertvalues[i]->data);
			}
			else
			{
				printf("Error: data type does not fit!\n");
				return 0;
			}
		}
		else if (attrsinfos[attrID].attrType == STRING)
		{
			if (insertvalues[i]->datatype == STRING)
			{
				values.push_back(insertvalues[i]->data);
			}
			else
			{
				printf("Error: data type does not fit!\n");
				return 0;
			}
		}
		else
		{
			//经过检查不允许插入NULL值域(实际上目前代码支持插入NULL),所以理论上不可能进入这里
			assert(0);
		}
	}
	//printf("\n");

	BufType data = new unsigned int[_smm->_tables[tableID].recordSize >> 2];
	unsigned long long *bitmap = (unsigned long long *)data;
	bitmap[0] = 0;
	// 设置默认值的代码,因为文法只有显式null,所以默认值已经无效,这段代码相应删除
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
	for (int i = 0; i < attrNum; i++)
	{
		int attrID = _smm->_fromNameToID(attrs[i], tableID);
		if (attrID == -1)
		{
			fprintf(stderr, "Error: invalid column name!\n");
			delete[] data;
			return 0;
		}
		bitmap[0] |= 1ull << attrID;
		if (attrsinfos[attrID].attrType == INT)
		{
			assert(attrsinfos[attrID].attrLength == 4);
			int d = *(int *)values[i];
			memcpy(data + attrsinfos[attrID].offset, &d, 4);
		}
		else if (attrsinfos[attrID].attrType == FLOAT)
		{
			assert(attrsinfos[attrID].attrLength == 8);
			double dd = *(double *)values[i];
			memcpy(data + attrsinfos[attrID].offset, &dd, 8);
		}
		else if (attrsinfos[attrID].attrType == STRING)
		{
			char *ddd = (char *)values[i];
			int size = strlen(ddd);
			memset(data + attrsinfos[attrID].offset, 0, sizeof(char) * attrsinfos[attrID].attrLength);
			// 把字符串长度设定为表定义的长度，注意末尾\0
			if (size >= attrsinfos[attrID].attrLength)
			{
				size = attrsinfos[attrID].attrLength;
				ddd[size - 1] = '\0';
			}
			memcpy(data + attrsinfos[attrID].offset, ddd, size);
		}
	}
	for (int i = 0; i < _smm->_tables[tableID].attrNum; i++)
	{
		if (attrsinfos[i].notNull && ((bitmap[0] & (1ull << i)) == 0))
		{
			fprintf(stderr, "Error: some NOT-NULL columns are null!\n");
			delete[] data;
			return 0;
		}
	}
	// 若有主键,则需检查主键是否重复
	int primary_size = _smm->_tables[tableID].primarySize;
	BufType primaryData = nullptr;
	if (primary_size != 0)
	{
		primaryData = _smm->_getPrimaryKey(tableID, data);
		int indexFileID;
		_ixm->OpenIndex((tableName + ".primary").c_str(), indexFileID);
		SIndexManager *sindexhandle = new SIndexManager(bufPageManager, indexFileID);
		bool check = sindexhandle->Exists(primaryData);
		delete sindexhandle;
		_ixm->CloseIndex(indexFileID);
		if (check)
		{
			fprintf(stderr, "Error: repetitive primary keys!\n");
			delete[] data;
			if (primaryData != nullptr)
				delete[] primaryData;
			return 0;
		}
	}
	// 遍历所有外键
	for (int i = 0; i < _smm->_tables[tableID].foreignNum; i++)
	{
		string refName = _smm->_tables[tableID].foreign[i];
		int refID = _smm->_fromNameToID(refName);
		int refSize = _smm->_tables[refID].primarySize;
		BufType refData = new unsigned int[refSize >> 2];
		// 遍历ref表的所有主键，pos记录当前主键在主键索引中的offset
		int pos = 0;
		for (int j = 0; j < _smm->_tables[refID].primary.size(); j++)
		{
			string primaryName = _smm->_tables[refID].attrs[_smm->_tables[refID].primary[j]].attrName;
			int attr = -1;
			// 查找tableID里的对应列
			for (int k = 0; k < _smm->_tables[tableID].attrNum; k++)
			{
				if (attrsinfos[k].reference == refName && attrsinfos[k].foreignKeyName == primaryName)
				{
					attr = k;
					break;
				}
			}
			if (attr == -1)
			{
				fprintf(stderr, "Error: foreign keys are not complete!\n");
				delete[] data;
				if (primaryData != nullptr)
					delete[] primaryData;
				delete[] refData;
				return 0;
			}
			memcpy(refData + pos, data + attrsinfos[attr].offset, attrsinfos[attr].attrLength);
			pos += (attrsinfos[attr].attrLength >> 2);
		}
		int indexFileID;
		_ixm->OpenIndex((refName + ".primary").c_str(), indexFileID);
		SIndexManager *sindexhandle = new SIndexManager(bufPageManager, indexFileID);
		bool check = sindexhandle->Exists(refData);
		delete sindexhandle;
		_ixm->CloseIndex(indexFileID);
		delete[] refData;
		if (!check)
		{
			fprintf(stderr, "Error: invalid foreign keys!\n");
			delete[] data;
			if (primaryData != nullptr)
				delete[] primaryData;
			return 0;
		}
	}
	unsigned int recID;
	int fileID = _smm->_tableFileID[tableName];
	RecManager *filehandle = new RecManager(bufPageManager, fileID, 0, false);
	filehandle->insertRec(data, recID);
	int pageID = recID >> 16;
	int slotID = recID << 16 >> 16;
	// printf("pageID:%d,slotID:%d\n",pageID,slotID);
	if (primary_size != 0)
	{
		int indexFileID;
		_ixm->OpenIndex((tableName + ".primary").c_str(), indexFileID);
		SIndexManager *sindexhandle = new SIndexManager(bufPageManager, indexFileID);
		int pageID = recID >> ID_SEGM;
		int slotID = recID - (pageID << ID_SEGM);
		sindexhandle->insertIx(primaryData, pageID, slotID);
		delete sindexhandle;
		_ixm->CloseIndex(indexFileID);
		delete[] primaryData;
	}
	for (int i = 0; i < _smm->_tables[tableID].attrNum; i++)
	{
		if (attrsinfos[i].haveIndex)
		{
			BufType attrData = data + attrsinfos[i].offset;
			int indexFileID;
			_ixm->OpenIndex((tableName + "." + attrsinfos[i].attrName).c_str(), indexFileID);
			SIndexManager *sindexhandle = new SIndexManager(bufPageManager, indexFileID);
			int pageID = recID >> ID_SEGM;
			int slotID = recID - (pageID << ID_SEGM);
			sindexhandle->insertIx(attrData, pageID, slotID);
			delete sindexhandle;
			_ixm->CloseIndex(indexFileID);
		}
	}
	delete[] data;
	delete filehandle;
	return 1;
}

void DataOperater::Update(const Assigns assigns, vector<Relation *> relations)
{
	assert(assigns.attrs.size() == assigns.values.size());
	assert(assigns.attrs.size() == assigns.assignnull.size());
	string tableName = assigns.table;
	//	有关于表的检查
	int tableID = _smm->_fromNameToID(tableName);
	if (tableID == -1)
	{
		fprintf(stderr, "Error: such table does not exist!\n");
		return;
	}
	for (int i = 0; i < _smm->_tableNum; i++)
		if (i != tableID)
		{
			if (_smm->_tables[i].foreignSet.find(tableName) != _smm->_tables[i].foreignSet.end())
			{
				fprintf(stderr, "Error: foreign keys on the table!\n");
				return;
			}
		}

	vector<int> offsets;
	vector<int> attrIDs;
	vector<int> attrLengths;
	vector<int> attrTypes;

	vector<int> indexFileIDs;
	vector<SIndexManager *> sindexhandles;
	for (int i = 0; i < assigns.attrs.size(); i++)
	{
		// 有关于主键外键等的各种检查
		int attrID = _smm->_fromNameToID(assigns.attrs[i], tableID);
		if (attrID == -1)
		{
			fprintf(stderr, "Error: such column does not exist!\n");
			return;
		}
		if (_smm->_tables[tableID].attrs[attrID].primary)
		{
			fprintf(stderr, "Error: cannot update primary keys!\n");
			return;
		}
		if (_smm->_tables[tableID].attrs[attrID].reference != "")
		{
			fprintf(stderr, "Error: cannot update foreign keys!\n");
			return;
		}
		int attrLength = _smm->_tables[tableID].attrs[attrID].attrLength;
		int attrType = _smm->_tables[tableID].attrs[attrID].attrType;
		if (assigns.values[i]->data == nullptr && _smm->_tables[tableID].attrs[attrID].notNull)
		{
			fprintf(stderr, "Error: set NOT-NULL columns to be null!\n");
			return;
		}
		if (assigns.values[i]->data != nullptr)
		{
			//类型检查与转换
			if (attrType == INT || attrType == DATE)
			{
				if (assigns.values[i]->datatype == INT || assigns.values[i]->datatype == DATE)
				{
					/*do nothing*/
				}
				else if (assigns.values[i]->datatype == FLOAT)
				{
					double fdata = *(double *)assigns.values[i]->data;
					int *idata = new int(fdata);
					assigns.values[i]->data = (BufType)idata;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else if (attrType == FLOAT)
			{
				if (assigns.values[i]->datatype == INT || assigns.values[i]->datatype == DATE)
				{
					int idata = *(int *)assigns.values[i]->data;
					double *fdata = new double(idata);
					assigns.values[i]->data = (BufType)fdata;
				}
				else if (assigns.values[i]->datatype == FLOAT)
				{
					/*do nothing*/
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else if (attrType == STRING)
			{
				if (assigns.values[i]->datatype == STRING)
				{
					/*do nothing*/
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else
			{
				//经过检查不允许插入NULL值域(实际上目前代码支持插入NULL),所以理论上不可能进入这里
				assert(0);
			}
		}

		attrIDs.push_back(attrID);
		offsets.push_back(_smm->_tables[tableID].attrs[attrID].offset);
		attrLengths.push_back(attrLength);
		attrTypes.push_back(attrType);
		// 找一个索引
		if (_smm->_tables[tableID].attrs[attrID].haveIndex)
		{
			int indexFileID = -1;
			_ixm->OpenIndex((tableName + "." + assigns.attrs[i]).c_str(), indexFileID);
			SIndexManager *sindexhandle = new SIndexManager(bufPageManager, indexFileID);
			indexFileIDs.push_back(indexFileID);
			sindexhandles.push_back(sindexhandle);
		}
		else
		{
			indexFileIDs.push_back(-1);
			sindexhandles.push_back(nullptr);
		}
	}

	//准备开始遍历
	int recordSize = _smm->_tables[tableID].recordSize;
	RecManager *filehandle = new RecManager(bufPageManager, _smm->_tableFileID[tableName], 0, false);
	RecManager::Iterator *iter = new RecManager::Iterator(filehandle);

	vector<int> attrID1, attrID2;
	for (int i = 0; i < relations.size(); i++)
	{
		int attr = _smm->_fromNameToID(relations[i]->attr1, tableID);
		if (attr == -1)
		{
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID1.push_back(attr);
		if (relations[i]->op == CompOp::IS_NULL || relations[i]->op == CompOp::IS_NOT_NULL)
		{
			attrID2.push_back(-1);
			continue;
		}
		if (relations[i]->value->data != nullptr)
		{
			attrID2.push_back(-1);
			//类型检查与转换
			if (_smm->_tables[tableID].attrs[attr].attrType == INT || _smm->_tables[tableID].attrs[attr].attrType == DATE)
			{
				if (relations[i]->value->datatype == INT || relations[i]->value->datatype == DATE)
				{
					continue;
				}
				else if (relations[i]->value->datatype == FLOAT)
				{
					double fdata = *(double *)relations[i]->value->data;
					int *idata = new int(fdata);
					relations[i]->value->data = (BufType)idata;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else if (_smm->_tables[tableID].attrs[attr].attrType == FLOAT)
			{
				if (relations[i]->value->datatype == INT || relations[i]->value->datatype == DATE)
				{
					int idata = *(int *)relations[i]->value->data;
					double *fdata = new double(idata);
					relations[i]->value->data = (BufType)fdata;
				}
				else if (relations[i]->value->datatype == FLOAT)
				{
					continue;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else if (_smm->_tables[tableID].attrs[attr].attrType == STRING)
			{
				if (relations[i]->value->datatype == STRING)
				{
					continue;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else
			{
				//经过检查不允许插入NULL值域(实际上目前代码支持插入NULL),所以理论上不可能进入这里
				assert(0);
			}
			continue;
		}
		attr = _smm->_fromNameToID(relations[i]->attr2, tableID);
		if (attr == -1)
		{
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID2.push_back(attr);
		if (_smm->_tables[tableID].attrs[attrID1[i]].attrType != _smm->_tables[tableID].attrs[attrID2[i]].attrType)
		{
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid comparison!\n");
			return;
		}
	}
	// 是否有索引且支持索引
	bool found = false;
	bool get_next = false;
	int indexAttr = -1, indexRel = -1, indexFileID = -1;
	SIndexManager *indexscan = nullptr;
	for (int i = 0; i < relations.size(); i++)
	{
		// printf("where列名:%s,是否有索引:%d\n",_smm->_tables[tableID].attrs[attrID1[i]].attrName.c_str(),_smm->_tables[tableID].attrs[attrID1[i]].haveIndex);
		// 列与列相比较,不能用索引遍历
		if (attrID2[i] != -1)
			continue;
		if (relations[i]->op == CompOp::IS_NULL || relations[i]->op == CompOp::IS_NOT_NULL || relations[i]->op == CompOp::NE_OP)
			continue;
		// if (relations[i]->op != EQ_OP) continue;
		if (!_smm->_tables[tableID].attrs[attrID1[i]].haveIndex)
			continue;
		found = true;
		bool lower = false;
		bool scanself = false;
		if (relations[i]->op == CompOp::LE_OP || relations[i]->op == CompOp::LT_OP)
		{
			//小于或者小于等于,采用GetPrev
			get_next = false;
			//LT->lower
			lower = relations[i]->op == CompOp::LT_OP;
			scanself = relations[i]->op == CompOp::LE_OP;
		}
		else if (relations[i]->op == CompOp::GE_OP || relations[i]->op == CompOp::GT_OP)
		{
			//大于或者大于等于,采用GetNext,且lower为true;
			get_next = true;
			//GE->lower
			lower = relations[i]->op == CompOp::GE_OP;
			scanself = relations[i]->op == CompOp::GE_OP;
		}
		else
		{
			//等于采用GetNext,且lower为true;
			get_next = true;
			lower = true;
			scanself = true;
		}

		indexAttr = attrID1[i];
		indexRel = i;
		_ixm->OpenIndex((tableName + "." + _smm->_tables[tableID].attrs[indexAttr].attrName).c_str(), indexFileID);
		indexscan = new SIndexManager(bufPageManager, indexFileID);
		if (!indexscan->OpenScan(relations[i]->value->data, lower, scanself))
		{
			delete indexscan;
			_ixm->CloseIndex(indexFileID);
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error! Failed to open index1");
			return;
		}
		break;
	}

	unsigned int recID;
	int pageID, slotID;
	BufType data = new unsigned int[recordSize >> 2];
	while (1)
	{
		bool hasNext;
		// 有索引则用索引查询,否则直接查询
		// printf("found:%d\n",found);
		if (found)
		{

			if (get_next)
			{
				hasNext = indexscan->GetNextEntry(pageID, slotID);
			}
			else
			{
				hasNext = indexscan->GetPrevEntry(pageID, slotID);
			}
			recID = (pageID << 16) + slotID;
			//printf("Scan,page:%d,slot:%d\n",pageID,slotID);
			filehandle->GetRec(data, recID);
		}
		else
		{
			hasNext = iter->next(data, recID);
			if (!hasNext)
				break;
			pageID = recID >> 16;
			slotID = (recID << 16 >> 16);
			// printf("page:%d,slot:%d\n",pageID,slotID);
		}
		bool ok = true;
		// 用于设置Null标记
		unsigned long long *bitmap = (unsigned long long *)data;
		for (int i = 0; i < relations.size(); i++)
		{
			BufType data1 = data + _smm->_tables[tableID].attrs[attrID1[i]].offset;
			BufType data2 = relations[i]->value->data;
			if (data2 == nullptr)
			{
				data2 = data + _smm->_tables[tableID].attrs[attrID2[i]].offset;
			}
			if (relations[i]->op == IS_NULL)
			{
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) == 0)
					continue;
				else
				{
					ok = false;
					break;
				}
			}
			if (relations[i]->op == IS_NOT_NULL)
			{
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) != 0)
					continue;
				else
				{
					ok = false;
					break;
				}
			}
			if ((bitmap[0] & (1ull << attrID1[i])) == 0)
			{
				ok = false;
				break;
			}
			if ((attrID2[i] != -1) && ((bitmap[0] & (1ull << attrID2[i])) == 0))
			{
				ok = false;
				break;
			}
			ok = _compare(data1, data2, relations[i]->op, _smm->_tables[tableID].attrs[attrID1[i]].attrType);
			if (!ok)
				break;
		}
		if (ok)
		{
			for (int i = 0; i < attrIDs.size(); i++)
			{
				if (indexFileIDs[i] != -1)
				{
					sindexhandles[i]->deleteIx((void *)(data + offsets[i]), pageID, slotID);
				}
				fill(data + offsets[i], data + offsets[i] + (attrLengths[i] >> 2), 0);
				if (assigns.assignnull[i] == false && attrTypes[i] == STRING)
				{
					int strLen = strlen((char *)assigns.values[i]->data);
					if (strLen >= attrLengths[i])
					{
						strLen = attrLengths[i];
						((char *)assigns.values[i]->data)[strLen - 1] = '\0';
					}
					memcpy(data + offsets[i], assigns.values[i]->data, strLen);
				}
				else if (assigns.assignnull[i] == false)
				{
					memcpy(data + offsets[i], assigns.values[i]->data, attrLengths[i]);
				}
				// 原来是null,更新之后不是null
				if ((bitmap[0] & (1ull << attrIDs[i])) == 0 && assigns.assignnull[i] == false)
				{
					bitmap[0] |= 1ull << attrIDs[i];
				}
				// 原来不是null,更新之后是null
				else if ((bitmap[0] & (1ull << attrIDs[i])) != 0 && assigns.assignnull[i])
				{
					bitmap[0] ^= 1ull << attrIDs[i];
				}
				if (indexFileIDs[i] != -1)
				{
					sindexhandles[i]->insertIx((void *)(data + offsets[i]), pageID, slotID);
				}
			}
			// printf("before update -- pageID:%d,slotID:%d\n",pageID,slotID);
			filehandle->updateRec(data, recID);
		}
		if (!hasNext)
			break;
	}
	// TODO: delete data会引发内存异常,暂时找不到问题所在
	// delete [] data;
	for (int i = 0; i < attrIDs.size(); i++)
	{
		if (indexFileIDs[i] != -1)
		{
			delete sindexhandles[i];
			_ixm->CloseIndex(indexFileIDs[i]);
		}
	}
	if (found)
	{
		delete indexscan;
		_ixm->CloseIndex(indexFileID);
	}
	delete iter;
	delete filehandle;
}

void DataOperater::Delete(const string tableName, vector<Relation *> relations)
{
	int tableID = _smm->_fromNameToID(tableName);
	if (tableID == -1)
	{
		fprintf(stderr, "Error: such table does not exist!\n");
		return;
	}
	for (int i = 0; i < _smm->_tableNum; i++)
		if (i != tableID)
		{
			if (_smm->_tables[i].foreignSet.find(tableName) != _smm->_tables[i].foreignSet.end())
			{
				fprintf(stderr, "Error: foreign keys on the table!\n");
				return;
			}
		}
	RecManager *filehandle = new RecManager(bufPageManager, _smm->_tableFileID[tableName], 0, false);
	RecManager::Iterator *iter = new RecManager::Iterator(filehandle);

	vector<int> attrID1, attrID2;
	for (int i = 0; i < relations.size(); i++)
	{
		int attr = _smm->_fromNameToID(relations[i]->attr1, tableID);
		if (attr == -1)
		{
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID1.push_back(attr);
		if (relations[i]->op == CompOp::IS_NULL || relations[i]->op == CompOp::IS_NOT_NULL)
		{
			attrID2.push_back(-1);
			continue;
		}
		if (relations[i]->value->data != nullptr)
		{
			attrID2.push_back(-1);
			//类型检查与转换
			if (_smm->_tables[tableID].attrs[attr].attrType == INT || _smm->_tables[tableID].attrs[attr].attrType == DATE)
			{
				if (relations[i]->value->datatype == INT || relations[i]->value->datatype == DATE)
				{
					continue;
				}
				else if (relations[i]->value->datatype == FLOAT)
				{
					double fdata = *(double *)relations[i]->value->data;
					int *idata = new int(fdata);
					relations[i]->value->data = (BufType)idata;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else if (_smm->_tables[tableID].attrs[attr].attrType == FLOAT)
			{
				if (relations[i]->value->datatype == INT || relations[i]->value->datatype == DATE)
				{
					int idata = *(int *)relations[i]->value->data;
					double *fdata = new double(idata);
					relations[i]->value->data = (BufType)fdata;
				}
				else if (relations[i]->value->datatype == FLOAT)
				{
					continue;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else if (_smm->_tables[tableID].attrs[attr].attrType == STRING)
			{
				if (relations[i]->value->datatype == STRING)
				{
					continue;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else
			{
				//经过检查不允许插入NULL值域(实际上目前代码支持插入NULL),所以理论上不可能进入这里
				assert(0);
			}
			continue;
		}
		attr = _smm->_fromNameToID(relations[i]->attr2, tableID);
		if (attr == -1)
		{
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID2.push_back(attr);
		if (_smm->_tables[tableID].attrs[attrID1[i]].attrType != _smm->_tables[tableID].attrs[attrID2[i]].attrType)
		{
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid comparison!\n");
			return;
		}
	}
	int primaryIndex = -1;
	SIndexManager *primaryhandle;
	vector<int> attrs, indexes;
	vector<SIndexManager *> handles;
	if (_smm->_tables[tableID].primary.size() != 0)
	{
		_ixm->OpenIndex((tableName + ".primary").c_str(), primaryIndex);
		primaryhandle = new SIndexManager(bufPageManager, primaryIndex);
	}
	for (int i = 0; i < _smm->_tables[tableID].attrNum; i++)
		if (_smm->_tables[tableID].attrs[i].haveIndex)
		{
			attrs.push_back(i);
			int indexFileID;
			_ixm->OpenIndex((tableName + '.' + _smm->_tables[tableID].attrs[i].attrName).c_str(), indexFileID);
			indexes.push_back(indexFileID);
			handles.push_back(new SIndexManager(bufPageManager, indexFileID));
		}
	int recordSize = _smm->_tables[tableID].recordSize;

	BufType data = new unsigned int[recordSize >> 2];
	unsigned int recID;
	while (1)
	{
		bool hasNext = iter->next(data, recID);
		if (!hasNext)
			break;
		int pageID = recID >> 16;
		int slotID = (recID << 16 >> 16);

		bool ok = true;
		unsigned long long *bitmap = (unsigned long long *)data;
		for (int i = 0; i < relations.size(); i++)
		{
			BufType data1 = data + _smm->_tables[tableID].attrs[attrID1[i]].offset;
			BufType data2 = relations[i]->value->data;
			if (data2 == nullptr)
			{
				data2 = data + _smm->_tables[tableID].attrs[attrID2[i]].offset;
			}
			if (relations[i]->op == IS_NULL)
			{
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) == 0)
					continue;
				else
				{
					ok = false;
					break;
				}
			}
			if (relations[i]->op == IS_NOT_NULL)
			{
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) != 0)
					continue;
				else
				{
					ok = false;
					break;
				}
			}
			if ((bitmap[0] & (1ull << attrID1[i])) == 0)
			{
				ok = false;
				break;
			}
			if ((attrID2[i] != -1) && ((bitmap[0] & (1ull << attrID2[i])) == 0))
			{
				ok = false;
				break;
			}
			ok = _compare(data1, data2, relations[i]->op, _smm->_tables[tableID].attrs[attrID1[i]].attrType);
			if (!ok)
				break;
		}
		if (ok)
		{
			if (primaryIndex != -1)
			{
				BufType primaryData = _smm->_getPrimaryKey(tableID, data);
				primaryhandle->deleteIx(primaryData, pageID, slotID);
				delete primaryData;
			}
			for (int i = 0; i < handles.size(); i++)
			{
				BufType attrData = data + _smm->_tables[tableID].attrs[attrs[i]].offset;
				handles[i]->deleteIx(attrData, pageID, slotID);
			}
			filehandle->deleteRec(recID);
			// if (!filehandle->DeleteRec(pageID, slotID)) cout << "failed" << endl;
		}
	}
	// TODO: delete data会引发内存异常,暂时找不到问题所在
	// delete data;
	if (primaryIndex != -1)
	{
		_ixm->CloseIndex(primaryIndex);
		delete primaryhandle;
	}
	for (int i = 0; i < handles.size(); i++)
	{
		_ixm->CloseIndex(indexes[i]);
		delete handles[i];
	}
	delete iter;
	delete filehandle;
}

void DataOperater::Select(const string tableName, vector<Relation *> relations, vector<string> attrNames)
{
	int tableID = _smm->_fromNameToID(tableName);
	if (tableID == -1)
	{
		fprintf(stderr, "Error: such table does not exist!\n");
		return;
	}
	// printf("select from table:%s\n",tableName.c_str());
	vector<int> attrIDs;
	for (int i = 0; i < attrNames.size(); i++)
	{
		//printf("列名:%s\n",attrNames[i].c_str());
		if (attrNames[i] == "*")
		{
			//printf("所有列\n");
			attrIDs.clear();
			for (int attrID = 0; attrID < _smm->_tables[tableID].attrNum; attrID++)
				attrIDs.push_back(attrID);
			attrNames.clear();
			for (int attrID = 0; attrID < _smm->_tables[tableID].attrNum; attrID++)
			{
				string attrname = _smm->_tables[tableID].attrs[attrID].attrName;
				attrNames.push_back(attrname);
			}
			break;
		}
		int attrID = _smm->_fromNameToID(attrNames[i], tableID);
		if (attrID == -1)
		{
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrIDs.push_back(attrID);
	}
	RecManager *filehandle = new RecManager(bufPageManager, _smm->_tableFileID[tableName], 0, false);
	RecManager::Iterator *iter = new RecManager::Iterator(filehandle);

	vector<int> attrID1, attrID2;
	for (int i = 0; i < relations.size(); i++)
	{
		//列名:%s\n",relations[i]->attr1.c_str());
		int attr = _smm->_fromNameToID(relations[i]->attr1, tableID);
		if (attr == -1)
		{
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID1.push_back(attr);
		if (relations[i]->op == CompOp::IS_NULL || relations[i]->op == CompOp::IS_NOT_NULL)
		{
			attrID2.push_back(-1);
			continue;
		}
		if (relations[i]->value->data != nullptr)
		{
			attrID2.push_back(-1);
			//类型检查与转换
			if (_smm->_tables[tableID].attrs[attr].attrType == INT || _smm->_tables[tableID].attrs[attr].attrType == DATE)
			{
				if (relations[i]->value->datatype == INT || relations[i]->value->datatype == DATE)
				{
					continue;
				}
				else if (relations[i]->value->datatype == FLOAT)
				{
					double fdata = *(double *)relations[i]->value->data;
					int *idata = new int(fdata);
					relations[i]->value->data = (BufType)idata;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else if (_smm->_tables[tableID].attrs[attr].attrType == FLOAT)
			{
				if (relations[i]->value->datatype == INT || relations[i]->value->datatype == DATE)
				{
					int idata = *(int *)relations[i]->value->data;
					double *fdata = new double(idata);
					relations[i]->value->data = (BufType)fdata;
				}
				else if (relations[i]->value->datatype == FLOAT)
				{
					continue;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else if (_smm->_tables[tableID].attrs[attr].attrType == STRING)
			{
				if (relations[i]->value->datatype == STRING)
				{
					continue;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else
			{
				//经过检查不允许插入NULL值域(实际上目前代码支持插入NULL),所以理论上不可能进入这里
				assert(0);
			}
			continue;
		}
		attr = _smm->_fromNameToID(relations[i]->attr2, tableID);
		if (attr == -1)
		{
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
		attrID2.push_back(attr);
		if (_smm->_tables[tableID].attrs[attrID1[i]].attrType != _smm->_tables[tableID].attrs[attrID2[i]].attrType)
		{
			delete iter;
			delete filehandle;
			fprintf(stderr, "Error: invalid comparison!\n");
			return;
		}
	}
	// 是否有索引且支持索引
	bool found = false;
	bool get_next = false;
	int indexAttr = -1, indexRel = -1, indexFileID = -1;
	SIndexManager *indexscan = nullptr;
	for (int i = 0; i < relations.size(); i++)
	{
		if (attrID2[i] != -1)
			continue;
		//if (relations[i]->op != EQ_OP) continue;
		if (relations[i]->op == CompOp::IS_NULL || relations[i]->op == CompOp::IS_NOT_NULL || relations[i]->op == CompOp::NE_OP)
			continue;
		if (!_smm->_tables[tableID].attrs[attrID1[i]].haveIndex)
			continue;
		found = true;
		printf("Scan with index: %s\n", _smm->_tables[tableID].attrs[attrID1[i]].attrName.c_str());
		bool lower = false;
		bool scanself = false;
		if (relations[i]->op == CompOp::LE_OP || relations[i]->op == CompOp::LT_OP)
		{
			//小于或者小于等于,采用GetPrev
			get_next = false;
			//LT->lower
			lower = relations[i]->op == CompOp::LT_OP;
			scanself = relations[i]->op == CompOp::LE_OP;
		}
		else if (relations[i]->op == CompOp::GE_OP || relations[i]->op == CompOp::GT_OP)
		{
			//大于或者大于等于,采用GetNext
			get_next = true;
			//GE->lower
			lower = relations[i]->op == CompOp::GE_OP;
			scanself = relations[i]->op == CompOp::GE_OP;
		}
		else
		{
			//等于采用GetNext,且lower为true;
			get_next = true;
			lower = true;
			scanself = true;
		}
		indexAttr = attrID1[i];
		indexRel = i;
		_ixm->OpenIndex((tableName + "." + _smm->_tables[tableID].attrs[indexAttr].attrName).c_str(), indexFileID);
		indexscan = new SIndexManager(bufPageManager, indexFileID);
		if (!indexscan->OpenScan(relations[i]->value->data, lower, scanself))
		{
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
	while (1)
	{
		bool hasNext;
		// 有索引则用索引查询,否则直接查询
		if (found)
		{
			if (get_next)
			{
				hasNext = indexscan->GetNextEntry(pageID, slotID);
			}
			else
			{
				hasNext = indexscan->GetPrevEntry(pageID, slotID);
			}
			recID = (pageID << 16) + slotID;
			filehandle->GetRec(data, recID);
		}
		else
		{
			hasNext = iter->next(data, recID);
			if (!hasNext)
				break;
			pageID = recID >> 16;
			slotID = (recID << 16 >> 16);
			// printf("page:%d,slot:%d\n",pageID,slotID);
		}
		bool ok = true;
		unsigned long long *bitmap = (unsigned long long *)data;
		for (int i = 0; i < relations.size(); i++)
		{
			BufType data1 = data + _smm->_tables[tableID].attrs[attrID1[i]].offset;
			BufType data2 = relations[i]->value->data;
			if (data2 == nullptr)
			{
				data2 = data + _smm->_tables[tableID].attrs[attrID2[i]].offset;
			}
			if (relations[i]->op == IS_NULL)
			{
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) == 0)
					continue;
				else
				{
					ok = false;
					break;
				}
			}
			if (relations[i]->op == IS_NOT_NULL)
			{
				//cout << (bitmap[0] & (1ull << attrID1[i])) << endl;
				if ((bitmap[0] & (1ull << attrID1[i])) != 0)
					continue;
				else
				{
					ok = false;
					break;
				}
			}
			if ((bitmap[0] & (1ull << attrID1[i])) == 0)
			{
				ok = false;
				break;
			}
			if ((attrID2[i] != -1) && ((bitmap[0] & (1ull << attrID2[i])) == 0))
			{
				ok = false;
				break;
			}
			ok = _compare(data1, data2, relations[i]->op, _smm->_tables[tableID].attrs[attrID1[i]].attrType);
			if (i == indexRel && !ok)
				hasNext = false;
			if (!ok)
				break;
		}
		if (ok)
		{
			outcnt++;
			if (outcnt <= 100)
			{
				putchar('|');
				for (int i = 0; i < attrIDs.size(); i++)
				{
					printf(" %s ", attrNames[i].c_str());
					BufType out = data + _smm->_tables[tableID].attrs[attrIDs[i]].offset;
					if ((bitmap[0] & (1ull << attrIDs[i])) == 0)
					{
						printf(" NULL |");
						continue;
					}
					if (_smm->_tables[tableID].attrs[attrIDs[i]].attrType == INT)
					{
						printf(" %d ", *(int *)out);
					}
					else if (_smm->_tables[tableID].attrs[attrIDs[i]].attrType == FLOAT)
					{
						printf(" %.6lf ", *(double *)out);
					}
					else if (_smm->_tables[tableID].attrs[attrIDs[i]].attrType == STRING)
					{
						printf(" %s ", (char *)out);
					}
					putchar('|');
				}
				putchar('\n');
			}
		}
		if (!hasNext)
			break;
	}
	// TODO: delete data会引发内存异常,暂时找不到问题所在
	// delete data;
	if (outcnt > 100)
	{
		puts("...");
		printf("Altogether %d records.\n", outcnt);
	}
	if (outcnt == 0){
		printf("No results.\n");
	}
	if (found)
	{
		delete indexscan;
		_ixm->CloseIndex(indexFileID);
	}
	delete iter;
	delete filehandle;
}

//判断一个元素是否在vector中
template <class T>
bool invector(vector<T> vec, T st)
{
	for (T t : vec)
	{
		if (st == t)
			return true;
	}
	return false;
}

void DataOperater::Select(vector<string> tableNames, vector<Tcol *> cols, vector<Relation *> relations)
{
	//多表连接函数
	vector<int> tableIDs;
	for (int i = 0; i < tableNames.size(); i++)
	{
		int tableID = _smm->_fromNameToID(tableNames[i]);
		if (tableID == -1)
		{
			fprintf(stderr, "Error: no such table in tablelist!\n");
			return;
		}
		for (int j = 0; j < tableIDs.size(); j++)
		{
			if (tableIDs[j] == tableID)
			{
				fprintf(stderr, "Error: cannot connect same tables!\n");
				return;
			}
		}
		tableIDs.push_back(tableID);
	}

	vector<pair<int, int>> attrIDs;
	for (int i = 0; i < cols.size(); i++)
	{
		if (cols[i]->colname == "*")
		{
			//printf("所有列\n");
			attrIDs.clear();
			cols.clear();
			for(int i = 0;i < tableNames.size();i++) {
				for(int attrID = 0;attrID < _smm->_tables[tableIDs[i]].attrNum;attrID++){
					attrIDs.push_back(make_pair(tableIDs[i], attrID));
					Tcol* tcol = new Tcol(tableNames[i],_smm->_tables[tableIDs[i]].attrs[attrID].attrName);
					cols.push_back(tcol);
				}
			}
			break;
		}
		string table = cols[i]->tablename;
		string attr = cols[i]->colname;
		int attrID = -1;

		for (int i = 0; i < tableNames.size(); i++)
		{
			if (table == tableNames[i])
			{
				attrID = _smm->_fromNameToID(attr, tableIDs[i]);
				attrIDs.push_back(make_pair(tableIDs[i], attrID));
			}
		}
		if (attrID == -1)
		{
			fprintf(stderr, "Error: invalid columns!\n");
			return;
		}
	}
	// for(int i = 0;i < attrIDs.size();i++){
	// 	printf("attrID:%d %d\n",attrIDs[i].first,attrIDs[i].second);
	// }

	// 处理where
	vector<pair<int, int>> attrID1, attrID2;
	for (int i = 0; i < relations.size(); i++)
	{
		int tableID = _smm->_fromNameToID(relations[i]->table1);
		if (tableID == -1)
		{
			fprintf(stderr, "Error: In whereClauses, invalid tables!\n");
			return;
		}
		if (!invector(tableIDs, tableID))
		{
			fprintf(stderr, "Error: In whereClauses, no such table in tablelist!\n");
			return;
		}

		int attr = _smm->_fromNameToID(relations[i]->attr1, tableID);
		if (attr == -1)
		{
			fprintf(stderr, "Error: In whereClauses, invalid columns!\n");
			return;
		}
		attrID1.push_back(make_pair(tableID, attr));
		if (relations[i]->op == CompOp::IS_NULL || relations[i]->op == CompOp::IS_NOT_NULL)
		{
			attrID2.push_back(make_pair(-1, -1));
			continue;
		}
		if (relations[i]->value->data != nullptr)
		{
			attrID2.push_back(make_pair(-1, -1));
			//类型检查与转换
			if (_smm->_tables[tableID].attrs[attr].attrType == INT || _smm->_tables[tableID].attrs[attr].attrType == DATE)
			{
				if (relations[i]->value->datatype == INT || relations[i]->value->datatype == DATE)
				{
					continue;
				}
				else if (relations[i]->value->datatype == FLOAT)
				{
					double fdata = *(double *)relations[i]->value->data;
					int *idata = new int(fdata);
					relations[i]->value->data = (BufType)idata;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else if (_smm->_tables[tableID].attrs[attr].attrType == FLOAT)
			{
				if (relations[i]->value->datatype == INT || relations[i]->value->datatype == DATE)
				{
					int idata = *(int *)relations[i]->value->data;
					double *fdata = new double(idata);
					relations[i]->value->data = (BufType)fdata;
				}
				else if (relations[i]->value->datatype == FLOAT)
				{
					continue;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else if (_smm->_tables[tableID].attrs[attr].attrType == STRING)
			{
				if (relations[i]->value->datatype == STRING)
				{
					continue;
				}
				else
				{
					printf("Error: data type does not fit!\n");
					return;
				}
			}
			else
			{
				//经过检查不允许插入NULL值域(实际上目前代码支持插入NULL),所以理论上不可能进入这里
				assert(0);
			}
			continue;
		}

		tableID = _smm->_fromNameToID(relations[i]->table2);
		if (tableID == -1)
		{
			fprintf(stderr, "Error: In whereClauses, invalid tables!\n");
			return;
		}
		if (!invector(tableIDs, tableID))
		{
			fprintf(stderr, "Error: In whereClauses, no such table in tablelist!\n");
			return;
		}
		attr = _smm->_fromNameToID(relations[i]->attr2, tableID);
		if (attr == -1)
		{
			fprintf(stderr, "Error: In whereClauses, invalid columns!\n");
			return;
		}
		attrID2.push_back(make_pair(tableID, attr));
		// 检查两个列类型是否相同
		if (_smm->_tables[attrID1[i].first].attrs[attrID1[i].second].attrType != _smm->_tables[attrID2[i].first].attrs[attrID2[i].second].attrType)
		{
			fprintf(stderr, "Error: In whereClauses, invalid comparison!\n");
			return;
		}
	}

	vector<RecManager *> filehandles;
	vector<RecManager::Iterator *> iters;
	vector<int> recordSizes;
	for (int i = 0; i < tableNames.size(); i++)
	{
		RecManager *filehandle = new RecManager(bufPageManager, _smm->_tableFileID[tableNames[i]], 0, false);
		RecManager::Iterator *iter = new RecManager::Iterator(filehandle);
		int recordSize = _smm->_tables[tableIDs[i]].recordSize;
		filehandles.push_back(filehandle);
		iters.push_back(iter);
		recordSizes.push_back(recordSize);
	}

	//printf("多表连接1\n");

	//datas[i]是第i个表中的所有数据
	vector<vector<BufType>> datas;
	for (int i = 0; i < tableNames.size(); i++)
	{
		vector<BufType> tabledatas;
		unsigned int recID;
		while (1)
		{
			BufType data = new unsigned int[recordSizes[i] >> 2];
			bool hasNext = iters[i]->next(data, recID);
			if (!hasNext)
				break;
			bool ok = true;
			unsigned long long *bitmap = (unsigned long long *)data;

			// 进行单项筛选
			for (int i = 0; i < relations.size(); i++)
			{
				if(attrID1[i].first != tableIDs[i])	continue;
				BufType data1 = data + _smm->_tables[attrID1[i].first].attrs[attrID1[i].second].offset;

				if (relations[i]->op == IS_NULL)
				{
					if ((bitmap[0] & (1ull << attrID1[i].second)) == 0)
						continue;
					else
					{
						ok = false;
						break;
					}
				}
				if (relations[i]->op == IS_NOT_NULL)
				{
					if ((bitmap[0] & (1ull << attrID1[i].second)) != 0)
						continue;
					else
					{
						ok = false;
						break;
					}
				}

				BufType data2 = relations[i]->value->data;
				if (data2 == nullptr)
				{
					continue;
				}
				if ((bitmap[0] & (1ull << attrID1[i].second)) == 0)
				{
					ok = false;
					break;
				}
				ok = _compare(data1, data2, relations[i]->op, _smm->_tables[tableIDs[i]].attrs[attrID1[i].second].attrType);
				if (!ok)
					break;
			}
			if(ok)	tabledatas.push_back(data);

		}
		datas.push_back(tabledatas);
		if(tabledatas.size() == 0){
			printf("No result.\n");
			return;
		}
		// printf("第%d个表中有%ld条被选数据\n",i,tabledatas.size());
	}

	vector<int> scanid;
	for(int i = 0; i < tableNames.size(); i++) {
		scanid.push_back(0);
	}

	//printf("多表连接2\n");
	int outcnt = 0;
	while(1) {
		vector<BufType> scandatas;
		for(int i = 0;i < tableNames.size();i++){
			scandatas.push_back(datas[i][scanid[i]]);
		}

		bool ok = true;
		//printf("多表连接3\n");
		for (int i = 0; i < relations.size(); i++)
		{
			//获取attr1的表在tableID里的位置
			int tableidx1 = -1;
			for(int j = 0;j < tableNames.size();j++){
				if(attrID1[i].first == tableIDs[j]){
					tableidx1 = j;
					break;
				}
			}
			assert(tableidx1 >= 0);
			// printf("attr1:%d,%d,tableidx:%d\n",attrID1[i].first,attrID1[i].second,tableidx1);
			//NULL检查和单项属性检查在之前已完成,这里只检查联合判定
			BufType data1 = scandatas[tableidx1] + _smm->_tables[attrID1[i].first].attrs[attrID1[i].second].offset;

			//获取attr2的表在tableID里的位置
			if(attrID2[i].first == -1 || attrID2[i].second == -1) continue;
			int tableidx2 = -1;
			for(int j = 0;j < tableNames.size();j++){
				if(attrID2[i].first == tableIDs[j]){
					tableidx2 = j;
					break;
				}
			}
			assert(tableidx2 >= 0);
			BufType data2 = scandatas[tableidx2] + _smm->_tables[attrID2[i].first].attrs[attrID2[i].second].offset;
			ok = _compare(data1, data2, relations[i]->op, _smm->_tables[attrID1[i].first].attrs[attrID1[i].second].attrType);
			if(!ok) break;
		}
		//printf("多表连接4\n");
		if (ok)
		{
			outcnt++;
			if (outcnt <= 100)
			{
				putchar('|');
				for (int i = 0; i < attrIDs.size(); i++)
				{
					int tableid = attrIDs[i].first;
					int attrid = attrIDs[i].second;
					//获取attr1的表在tableID里的位置
					int tableidx1 = -1;
					for(int j = 0;j < tableNames.size();j++){
						if(tableid == tableIDs[j]){
							tableidx1 = j;
							break;
						}
					}
					assert(tableidx1 >= 0);
					//printf("attr1:%d,%d,tableidx:%d\n",tableid,attrid,tableidx1);
					BufType out = scandatas[tableidx1] + _smm->_tables[tableid].attrs[attrid].offset;
					printf(" %s.%s ", cols[i]->tablename.c_str(),cols[i]->colname.c_str());

					unsigned long long *bitmap = (unsigned long long *)scandatas[tableidx1];
					if ((bitmap[0] & (1ull << attrid)) == 0)
					{
						printf(" NULL |");
						continue;
					}
					if (_smm->_tables[tableid].attrs[attrid].attrType == INT)
					{
						printf(" %d ", *(int *)out);
					}
					else if (_smm->_tables[tableid].attrs[attrid].attrType == FLOAT)
					{
						printf(" %.6lf ", *(double *)out);
					}
					else if (_smm->_tables[tableid].attrs[attrid].attrType == STRING)
					{
						printf(" %s ", (char *)out);
					}
					putchar('|');
				}
				putchar('\n');
			}
		}

		bool jw = true;
		int idx = scanid.size() - 1;
		while(idx >= 0) {
			// printf("scanid:第%d个--%d\n",idx,scanid[idx]);
			if(jw){
				scanid[idx] = scanid[idx] + 1;
				if(scanid[idx] >= datas[idx].size() && idx != 0){
					scanid[idx] = 0;
					jw = true;
				}else{
					jw = false;
				}
			}else{
				jw = false;
			}
			idx--;
		}
		if(scanid[0] >= datas[0].size())	break;
	}
	if (outcnt > 100)
	{
	 	puts("...");
		printf("Altogether %d records.\n", outcnt);
	}
	// delete iter1;
	// delete filehandle1;
	// delete iter2;
	// delete filehandle2;
}

	bool DataOperater::_compare(BufType data1, BufType data2, CompOp op, int type)
	{
		if (type == STRING)
		{
			int result = strcmp((char *)data1, (char *)data2);
			if (op == EQ_OP)
				return result == 0;
			if (op == NE_OP)
				return result != 0;
			if (op == LT_OP)
				return result < 0;
			if (op == GT_OP)
				return result > 0;
			if (op == LE_OP)
				return result <= 0;
			if (op == GE_OP)
				return result >= 0;
		}
		else if (type == INT)
		{
			if (op == EQ_OP)
				return *(int *)data1 == *(int *)data2;
			if (op == NE_OP)
				return *(int *)data1 != *(int *)data2;
			if (op == LT_OP)
				return *(int *)data1 < *(int *)data2;
			if (op == GT_OP)
				return *(int *)data1 > *(int *)data2;
			if (op == LE_OP)
				return *(int *)data1 <= *(int *)data2;
			if (op == GE_OP)
				return *(int *)data1 >= *(int *)data2;
		}
		else if (type == FLOAT)
		{
			if (op == EQ_OP)
				return *(double *)data1 == *(double *)data2;
			if (op == NE_OP)
				return *(double *)data1 != *(double *)data2;
			if (op == LT_OP)
				return *(double *)data1 < *(double *)data2;
			if (op == GT_OP)
				return *(double *)data1 > *(double *)data2;
			if (op == LE_OP)
				return *(double *)data1 <= *(double *)data2;
			if (op == GE_OP)
				return *(double *)data1 >= *(double *)data2;
		}
		printf("Error Operator in compare\n");
		assert(0);
		return false;
	}

	void DataOperater::Load(const string tableName, const char *line)
	{
		int tableID = _smm->_fromNameToID(tableName);
		vector<AttrInfo> attrsinfos = _smm->_tables[tableID].attrs;
		vector<Value *> rdata;
		vector<int> cols;
		int nullnub = 0;

		char *tmp = strdup(line);
		char *tok;
		/* 获取第一个子字符串 */
		tok = strtok(tmp, "|");
		vector<char *> tmps;
		int i = 0;
		while (tok != NULL)
		{
			// 	printf("%d %s\n", i, tok);
			if (i >= attrsinfos.size())
				break;
			assert(i < attrsinfos.size());
			if (attrsinfos[i].attrType == INT)
			{
				int *vi = new int(atoi(tok));
				Value *value = new Value(INT, (BufType)vi);
				rdata.push_back(value);
				cols.push_back(i);
			}
			else if (attrsinfos[i].attrType == FLOAT)
			{
				double *vf = new double(atof(tok));
				Value *value = new Value(FLOAT, (BufType)vf);
				rdata.push_back(value);
				cols.push_back(i);
			}
			else
			{
				char *buftmp = strdup(tok);
				tmps.push_back(buftmp);
				Value *value = new Value(STRING, (BufType)buftmp);
				rdata.push_back(value);
				cols.push_back(i);
			}
			tok = strtok(NULL, "|\n");
			i++;
		}
		Insert(tableName, rdata, cols, nullnub);
		rdata.clear();
		cols.clear();
		free(tmp);
		for (int j = 0; j < tmps.size(); j++)
		{
			free(tmps[j]);
		}
		tmps.clear();
	}