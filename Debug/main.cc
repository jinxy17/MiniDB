
#include "../queryparser/Executer.h"
extern int yyparse();
extern Executer * executer;
int main() {
    executer = new Executer();
    executer->init();
    int rc =  1;
    while(rc) {
        rc = yyparse();
    }
    executer->exit();
    return 0;
}