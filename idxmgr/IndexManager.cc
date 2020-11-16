#include "IndexManager.h"

IndexManager::IndexManager(FileManager *fm, BufPageManager *bpm) {
    this->fm = fm;
    this->bpm = bpm;
}

IndexManager::~IndexManager() {}

bool IndexManager::CreateIndex(const char *ixName, int ixType, int ixSize){
    // 调用fileManager创建一个名为ixName的文件
	if (!this->fm->createFile(ixName)) 
        return false;
	int fileID;
	// 调用fileManager打开indexname文件,并获得文件句柄fileID
	if (!this->fm->openFile(ixName, fileID))
        return false;
    SIndexManager* sim = new SIndexManager(this->bpm,fileID);
    sim->init(int ixSize,int ixType);
    delete sim;
    return true;
}

bool IndexManager::DeleteIndex(const char *ixName){
	bpm->close();
	system(("rm " + ixName);
	return true;
}
	
bool IndexManager::OpenIndex(const char *ixName, SIndexManager* sim){
    int fileID;
    if (!this->fm->openFile(ixName, fileID)) 
        return false;
    sim = new SIndexManager(this->bpm,fileID);
    return true;
}

bool IndexManager::CloseIndex(SIndexManager* sim){
    if (fileManager->closeFile(sim->fileID)) return false;
    return true;
}
