#include "node.hpp"
#include "codegen.hpp"
#include "brainf.tab.hpp"
using namespace std;

void CodeGenContext::generateCode(NBlock& root){
  root.codeGen();
}

void NBlock::codeGen()
{
  std::cout << "正在生成Block" << endl;

  StmtList::const_iterator it;

  for (it = stmtList.begin(); it != stmtList.end(); it++) {
    std::cout << "Generating code for block "  << endl;
  }
}

void NInp::codeGen()
{
  std::cout << "正在生成Block" << endl;
}

void NOut::codeGen()
{
  std::cout << "正在生成Block" << endl;
}

void NValOp::codeGen()
{
  std::cout << "正在生成Block" << endl;
}

void NPosOp::codeGen()
{
  std::cout << "正在生成Block" << endl;
}

void NLoop::codeGen()
{
  std::cout << "正在生成Block" << endl;
}
