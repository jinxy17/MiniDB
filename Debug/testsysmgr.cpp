#include <vector>
#include "../sysmgr/SysManager.h"
using namespace std;

int def1 = 15;
char def2[] = "HELO";

int main() {
	MyBitMap::initConst();   //新加的初始化
	FileManager *fileManager = new FileManager();
	int fileID, f2;
	// fileManager->createFile("1"); //新建文件
	// fileManager->openFile("1", fileID); //打开文件，fileID是返回的文件id
	// fileManager->createFile("2");
    // fileManager->openFile("2", f2);
	BufPageManager *bufPageManager = new BufPageManager(fileManager);
	IndexManager *ixm = new IndexManager(fileManager, bufPageManager);
	SysManager *smm = new SysManager(ixm, fileManager, bufPageManager);

	smm->OpenDB("db");
	puts("==> opened db");

	TableInfo *tab1 = new TableInfo();
	tab1->tableName = "tab1";
	tab1->attrNum = 2;
	tab1->attrs.push_back((AttrInfo){"a1", INT, 4, true, false, (BufType)&def1, "", "", false, 0});
	tab1->attrs.push_back((AttrInfo){"a2", STRING, 5, true, false, (BufType)&def2, "", "", false, 0});
	smm->CreateTable(tab1);
	puts("==> created tab1");

	TableInfo *tab2 = new TableInfo();
	tab2->tableName = "tab2";
	tab2->attrNum = 2;
	tab2->attrs.push_back((AttrInfo){"b1", INT, 4, true, false, (BufType)&def1, "", "", false, 0});
	tab2->attrs.push_back((AttrInfo){"b2", STRING, 5, true, false, (BufType)&def2, "", "", false, 0});
	smm->CreateTable(tab2);
	puts("==> created tab2");

	TableInfo *tab3 = new TableInfo();
	tab3->tableName = "tab3";
	tab3->attrNum = 2;
	tab3->attrs.push_back((AttrInfo){"c1", INT, 4, true, false, (BufType)&def1, "", "", false, 0});
	tab3->attrs.push_back((AttrInfo){"c2", STRING, 5, true, false, (BufType)&def2, "", "", false, 0});
	smm->CreateTable(tab3);
	puts("==> created tab3");

	// smm->Show();

	// vector<string> t; t.push_back("b2");
	// smm->CreateIndex("tab2", t);
	// smm->Show();

	// smm->DropTable("tab2");
	// puts("==> dropped tab2");
	// smm->DropTable("tab1");
	// puts("==> dropped tab1");
	smm->CloseDB();
	puts("==> closed db");
	return 0;
}
