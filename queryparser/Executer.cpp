#include "Executer.h"

Executer::Executer(){}

Executer::~Executer(){}

int Executer::init() {}

int Executer::exit() {}

int Executer::execStmt(Stmt* stmt) {
    switch (stmt->sttype)
    {
    case Stmt::SYS_STMT:
        printf("SYS_STMT\n");
        break;
    case Stmt::DB_STMT:
        printf("DB_STMT %d,%s\n",((dbStmt*)stmt)->dbType,((dbStmt*)stmt)->dbName.c_str());
        break;
    case Stmt::TB_STMT:
        printf("TB_STMT\n");
        break;
    case Stmt::IDX_STMT:
        printf("IDX_STMT\n");
        break;
    case Stmt::ALTER_STMT:
        printf("ALTER_STMT\n");
        break;
    default:
        break;
    }
}

