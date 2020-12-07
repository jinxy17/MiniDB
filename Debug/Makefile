src = ../filesystem/pf.o ../idxmgr/IndexManager.o ../idxmgr/SIndexManager.o ../sysmgr/SysManager.o
CXXFLAGS = -std=c++11
all: testfilesystem testrecmgr testidxmgr testsysmgr create drop create_rec examine_rec
testfilesystem: testfilesystem.o $(src)
	g++ -o testfilesystem testfilesystem.o $(src)
testrecmgr: testrecmgr.o $(src)
	g++ -o testrecmgr testrecmgr.o $(src)
testidxmgr: testidxmgr.o $(src)
	g++ -o testidxmgr testidxmgr.o $(src)
testsysmgr: testsysmgr.o $(src)
	g++ -o testsysmgr testsysmgr.o $(src)
create_rec: create_rec.o $(src)
	g++ -o create_rec create_rec.o $(src)
examine_rec: examine_rec.o $(src)
	g++ -o examine_rec examine_rec.o $(src)
create: ../sysmgr/create.o $(src)
	g++ -o create ../sysmgr/create.o $(src)
drop: ../sysmgr/drop.o $(src)
	g++ -o drop ../sysmgr/drop.o $(src)
clean:
	-rm testfilesystem testrecmgr testidxmgr testsysmgr create_rec examine_rec create drop create_rec.o examine_rec.o ../sysmgr/create.o ../sysmgr/drop.o *.o $(src)