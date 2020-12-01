src = ../idxmgr/IndexManager.o ../idxmgr/SIndexManager.o
CXXFLAGS = -std=c++11
all: testfilesystem testrecmgr testidxmgr
testfilesystem: testfilesystem.o $(src)
	g++ -o testfilesystem testfilesystem.o $(src)
testrecmgr: testrecmgr.o $(src)
	g++ -o testrecmgr testrecmgr.o $(src)
testidxmgr: testidxmgr.o $(src)
	g++ -o testidxmgr testidxmgr.o $(src)
clean :
	-rm testfilesystem testrecmgr testidxmgr *.o $(src)