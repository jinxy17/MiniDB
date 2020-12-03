#include "IndexManager.h"

IndexManager::IndexManager(FileManager *fm, BufPageManager *bpm) {
    this->fm = fm;
    this->bpm = bpm;
}

IndexManager::~IndexManager() {}

bool IndexManager::CreateIndex(const char *ixName, int ixType, int ixSize){
    // 调用fileManager创建一个名为ixName的文件
	if (!this->fm->createFile(ixName)) {
        // 文件系统的create实质是打开或创建文件(fopen)
        fprintf(stderr, "Error: Can't create index!\n");
        return false;
    }
	int fileID;
	// 调用fileManager打开indexname文件,并获得文件句柄fileID
	if (!this->fm->openFile(ixName, fileID)) {
        fprintf(stderr, "Error: Can't create index!\n");
        return false;
    }
    SIndexManager*   sim = new SIndexManager(this->bpm,fileID);
    sim->init(ixSize,ixType);
    delete sim;
    return true;
}

bool IndexManager::DeleteIndex(const char *ixName){
	bpm->close();
    const char* rm = "rm ";
    char* rmidx = (char *) malloc(strlen(rm) + strlen(ixName));
    strcpy(rmidx, rm);
    strcat(rmidx, ixName);
	system(rmidx);
	return true;
}
	
bool IndexManager::OpenIndex(const char *ixName, int& fileID){
    if (!this->fm->openFile(ixName, fileID))  {
        fprintf(stderr, "Error: Can't open index!\n");
        return false;
    }
    return true;
}

bool IndexManager::CloseIndex(SIndexManager* sim){
    if (fm->closeFile(sim->fileID)) {
        fprintf(stderr, "Error: Can't close index!\n");
        return false;
    }
    delete sim;
    return true;
}
