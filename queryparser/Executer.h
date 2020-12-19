#pragma once
#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../recmgr/RecManager.h"
#include "../idxmgr/IndexManager.h"
#include "../sysmgr/SysManager.h"
#include "DataOperater.h"
#include "Stmt.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

class Executer {
public:
    Executer();
    ~Executer();

	int execStmt(Stmt* stmt);
    int init();
    int exit();
private:
	FileManager *fileManager;
	BufPageManager *bufPageManager;
	IndexManager *indexManager;
	SysManager *smm;
	DataOperater *qlm;
};
