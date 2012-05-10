#include <iostream>
#include "node.hpp"

using namespace std;

extern NBlock* programBlock;
extern int yyparse();

int main (int argc, char** argv) {
    yyparse();
    std::cout<< programBlock << endl;
    return 0;
}
