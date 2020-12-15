src = ../filesystem/pf.o ../idxmgr/IndexManager.o ../idxmgr/SIndexManager.o ../sysmgr/SysManager.o ../querypaser/DataOperater.o
CXXFLAGS = -std=c++11
all: use create drop 
# testfilesystem testrecmgr testidxmgr testsysmgr create_rec examine_rec
# testfilesystem: testfilesystem.o $(src)
# 	g++ -o testfilesystem testfilesystem.o $(src)
# testrecmgr: testrecmgr.o $(src)
# 	g++ -o testrecmgr testrecmgr.o $(src)
# testidxmgr: testidxmgr.o $(src)
# 	g++ -o testidxmgr testidxmgr.o $(src)
# testsysmgr: testsysmgr.o $(src)
# 	g++ -o testsysmgr testsysmgr.o $(src)
# create_rec: create_rec.o $(src)
# 	g++ -o create_rec create_rec.o $(src)
# examine_rec: examine_rec.o $(src)
# 	g++ -o examine_rec examine_rec.o $(src)
use: use.o $(src) ../recmgr/RecManager.h
	g++ -o use use.o $(src) 
create: ../sysmgr/create.o 
	g++ -o create ../sysmgr/create.o 
drop: ../sysmgr/drop.o 
	g++ -o drop ../sysmgr/drop.o 
clean:
	-rm testfilesystem testrecmgr testidxmgr testsysmgr create_rec examine_rec create drop use ../sysmgr/create.o ../sysmgr/drop.o *.o $(src)