src = ../filesystem/pf.o ../idxmgr/IndexManager.o ../idxmgr/SIndexManager.o ../sysmgr/SysManager.o ../queryparser/DataOperater.o ../queryparser/Executer.o
CXXFLAGS = -std=c++17
all: main
test: testfilesystem testrecmgr testidxmgr testsysmgr create_rec examine_rec
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
main: main.o $(src) ../recmgr/RecManager.h ../queryparser/Stmt.h ../queryparser/lex.l ../queryparser/yacc.y 
	flex ../queryparser/lex.l
	yacc -d ../queryparser/yacc.y 
	g++ -o main main.o y.tab.c lex.yy.c $(src)
clean:
	-rm testfilesystem testrecmgr testidxmgr testsysmgr create_rec examine_rec main lex.yy.c y.tab.c y.tab.h *.o $(src)