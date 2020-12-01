#include <iostream>
#include <cstdlib>
#include <set>

#include "IndexManager.h"

set< pair<double, pair<int, int> > > s;
int testsize = 400000;

void test_insert()
{
	FileManager *fm = new FileManager();
	BufPageManager *bpm = new BufPageManager(fm);
	IndexManager *im = new IndexManager(fm, bpm);
	int fileID;
	im->CreateIndex("name", FLOAT, 8);
	im->OpenIndex("name", fileID);
	SIndexManager *sim = new SIndexManager(bpm, fileID);
	sim->Print_Tree();
	//随机插入testsize条数据
	for (int i = 1; i <= testsize; i++)
	{
		double x = (rand() % 2000) / 1000.0;
		sim->insertIx(&x, i, i);
		s.insert(make_pair(x, make_pair(i, i)));
	}
	printf("\n After insert:\n");
	sim->Print_Tree();
	bpm->close();
	im->CloseIndex(sim);
	fprintf(stderr, "close\n");
}

void test_delete()
{
	FileManager *fm = new FileManager();
	BufPageManager *bpm = new BufPageManager(fm);
	IndexManager *im = new IndexManager(fm, bpm);
	int fileID;
	im->OpenIndex("name", fileID);
	SIndexManager *sim = new SIndexManager(bpm, fileID);
	sim->Print_Tree();
	//	删除最小的1/3的数据
	set<pair<double, pair<int, int> > >::iterator iter = s.begin();
	int i = 0;
	while (iter != s.end())
	{
		pair<double, pair<int, int> > _pair = *iter;
		double x = _pair.first;
		pair<int, int> pos = _pair.second;
		int page = pos.first;
		int offset = pos.second;
		if (i++ < testsize / 3)
		{
			// printf("delete index:%.8lf,%d,%d\n",x,page,offset);
			sim->deleteIx(&x, page, offset);
		}
		iter++;
	}
	printf("\n After delete:\n");
	sim->Print_Tree();

	bpm->close();
	im->CloseIndex(sim);
	fprintf(stderr, "close\n");
}

void test_filesave()
{
	FileManager *fm = new FileManager();
	BufPageManager *bpm = new BufPageManager(fm);
	IndexManager *im = new IndexManager(fm, bpm);
	int fileID;
	im->OpenIndex("name", fileID);
	SIndexManager *sim = new SIndexManager(bpm, fileID);
	sim->Print_Tree();
	bpm->close();
	im->CloseIndex(sim);
	fprintf(stderr, "close\n");
}

void test_scan()
{
	FileManager *fm = new FileManager();
	BufPageManager *bpm = new BufPageManager(fm);
	IndexManager *im = new IndexManager(fm, bpm);
	int fileID;
	im->OpenIndex("name", fileID);
	SIndexManager *sim = new SIndexManager(bpm, fileID);
	sim->Print_Tree();

	for (int i = 0; i < 100; i++)
	{
		double x = (rand() % 2000) / 1000.0;
		printf("x: %.3lf ", x);
		set<pair<double, pair<int, int> > >::iterator it = s.lower_bound(make_pair(x, make_pair(-1, -1)));
		
		sim->OpenScan(&x, true);
		//fprintf(stderr, "set %.3lf\n", (*it).first);
		while (it != s.end())
		{
			int page = -1, slot = -1;
			sim->GetNextEntry(page, slot);
			// printf("%d %.3lf\n", (*it).second.first, (*it).first);
			if ((*it).second.first != page)
			{
				printf("Error,expect:%d,but receive:%d\n",(*it).second.first ,page);
				return;
			}
			it++;
		}
		printf("test succeed;\n");
		sim->CloseScan();
	}
	bpm->close();
	im->CloseIndex(sim);
	fprintf(stderr, "close\n");
}

int main()
{
	MyBitMap::initConst(); //新加的初始化
	// test_insert();
	// test_delete();
	// test_filesave();
	test_scan();
	return 0;
}
