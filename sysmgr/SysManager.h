#pragma once

#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../filesystem/utils/pagedef.h"
#include "../recmgr/RecManager.h"
#include "../idxmgr/IndexManager.h"
#include <string>
#include <map>
#include <vector>
#include <set>

struct AttrInfo {
	string attrName;
	int attrType;
	int attrLength; // bytes, completed by SM for INTEGER & FLOAT
	bool notNull;
	bool primary;
	BufType defaultValue;
	string reference;
	string foreignKeyName;
	bool haveIndex;
	int offset; // 4 bytes, completed by SM
};

struct TableInfo {
	string tableName;
	int attrNum, foreignNum; // foreignNum completed by SM
	int recordSize, primarySize; // bytes, completed by SM
	vector<int> primary; // completed by SM
	vector<AttrInfo> attrs;
	vector<string> foreign; // completed by SM
	set<string> foreignSet; // completed by SM
};

class SysManager {
public:
	FileManager *fileManager;
	BufPageManager *bufPageManager;
	
	SysManager(IndexManager *ixm, FileManager *_fileManager, BufPageManager *_bufPageManager);
	~SysManager();
	
	// each record is started with 8 bytes containing the NULL bitmap. 1 represents not null and 0 represents null.
	void OpenDB(const string DBName);
	void CloseDB();
	void Show();
	void CreateTable(TableInfo* table);
	void DropTable(const string tableName);
	void CreateIndex(const string tableName, const string attr);
	void DropIndex(const string tableName, const string attr);
	void AddPrimaryKey(const string tableName, const vector<string> attrs);
	void DropPrimaryKey(const string tableName);
	// void AddForeignKey(const string tableName, vector<string> attrs, const string refName, vector<string> foreigns);
	// void DropForeignKey(const string tableName, string refName);
	// void AddColumn(const string tableName, AttrInfo attr);
	// void DropColumn(const string tableName, string attrName);
	
	bool _checkForeignKeyOnTable(int tableID);
	int _fromNameToID(const string tableName);
	int _fromNameToID(const string attrName, const int tableID);
	BufType _getPrimaryKey(int tableID, BufType data);
	string _DBName;
	int _tableNum;
	std::vector<TableInfo> _tables;
	std::map<std::string, int> _tableFileID;
private:
	IndexManager *_ixm;
};
