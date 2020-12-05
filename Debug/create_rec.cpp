/*
 * testrecmgmr.cpp
 *
 *  Created on: 2020年11月22日
 *      Author: xuyang jin
 * 	随机insertRec/deleteRec/updateRec，用std::set检验正确性。
 * 
 */
#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../filesystem/utils/pagedef.h"
#include "../recmgr/RecManager.h"
#include <iostream>
#include <utility>
#include <set>
#include <cstdlib>
#include <string>

using namespace std;

struct Rec {
	int ph1;
	int ph2;
	int b1;
	char b2[8];
	char b3[8];
};


int main() {
	MyBitMap::initConst();   //新加的初始化
	FileManager* fm = new FileManager();
	BufPageManager* bpm = new BufPageManager(fm);
	fm->createFile("testrm"); //新建文件
	int fileID;
	fm->openFile("testrm", fileID); //打开文件，fileID是返回的文件id
	RecManager * rm = new RecManager(bpm, fileID, sizeof(Rec), true);

	cout << sizeof(Rec) << endl;
	puts("inserting data");
	for (int i = 1; i <= 30; i++) {
		int page, slot;
		unsigned int id;
		Rec *d = new Rec;
		d->b1 = 13;
		strcpy(d->b2, "HELO");
		strcpy(d->b3, ("JINXY" + to_string(i%5)).c_str());
		rm->insertRec((BufType)d, id);
		page = id >> 16;
		slot = id << 16 >> 16;
		cout << "insert: page=" << page << " slot=" << slot << " content=" << (char *)d->b2 << (char *)d->b3 << endl;
	}

	delete rm;
	fm->closeFile(fileID);
	delete bpm;
	delete fm;
	puts("test completed");
	
	return 0;
}
