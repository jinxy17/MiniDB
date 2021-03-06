#include <vector>
#include "../sysmgr/SysManager.h"
using namespace std;

int def1 = 15;
char def2[] = "DEF2";
char def3[] = "DEF3";
char def4[] = "DEF4";

int main() {
	MyBitMap::initConst();   //新加的初始化
	FileManager *fileManager = new FileManager();
	BufPageManager *bufPageManager = new BufPageManager(fileManager);
	IndexManager *ixm = new IndexManager(fileManager, bufPageManager);
	SysManager *smm = new SysManager(ixm, fileManager, bufPageManager);

	smm->OpenDB("db");
	puts("==> opened db");

	// TableInfo *tab1 = new TableInfo();
	// tab1->tableName = "tab1";
	// tab1->attrNum = 3;
	// tab1->attrs.push_back((AttrInfo){"a1", INT, 4, true, false, (BufType)&def1, "", "", false, 0});
	// tab1->attrs.push_back((AttrInfo){"a2", STRING, 8, true, false, (BufType)&def2, "", "", false, 0});
	// tab1->attrs.push_back((AttrInfo){"a3", STRING, 8, true, false, (BufType)&def3, "", "", false, 0});
	// smm->CreateTable(tab1);
	// puts("==> created tab1");

	// TableInfo *tab2 = new TableInfo();
	// tab2->tableName = "tab2";
	// tab2->attrNum = 3;
	// tab2->attrs.push_back((AttrInfo){"b1", INT, 4, true, false, (BufType)&def1, "", "", false, 0});
	// tab2->attrs.push_back((AttrInfo){"b2", STRING, 8, true, false, (BufType)&def2, "", "", false, 0});
	// tab2->attrs.push_back((AttrInfo){"b3", STRING, 8, true, false, (BufType)&def3, "", "", false, 0});
	// smm->CreateTable(tab2);
	// puts("==> created tab2");

	// smm->CreateIndex("tab2", "b2");
	// puts("==> created index");
	// smm->DropIndex("tab2", "b2");
	// puts("==> dropped index");

	// smm->AddPrimaryKey("tab2", {"b2", "b3"});
	// puts("==> created primary key");
	// smm->DropPrimaryKey("tab2");
	// puts("==> dropped primary key");

	// smm->AddColumn("tab2", (AttrInfo){"b4", STRING, 8, true, false, (BufType)&def4, "", "", false, 0});
	// puts("==> added new column");
	// smm->DropColumn("tab2", "b4");
	// puts("==> dropped column");

	// smm->AddForeignKey("tab1", {"a2", "a3"}, "tab2", {"b2", "b3"});
	// puts("==> added foreign key");

	// smm->CreateIndex("index1", "table1", {"a1", "a2", "a3"});
	// smm->CreateIndex("index1", "table1", {"b1", "b2", "b3"});
	// smm->AddIndex("index1", {"a4", "a5"});
	// smm->DropIndex("index1");
	// smm->DropIndex("index1");

	smm->Show();

	// smm->DropTable("tab1");
	// smm->DropTable("tab2");
	// puts("==> dropped tab1, tab2");

	
	smm->CloseDB();
	puts("==> closed db");
	delete smm;
	delete ixm;
	delete bufPageManager;
	delete fileManager;

	return 0;
}
