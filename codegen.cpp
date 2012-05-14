#include "node.hpp"
#include "codegen.hpp"
#include "brainf.tab.hpp"
using namespace std;

void CodeGenContext::generateCode(NBlock& root){
  root.codeGen();
}

void NBlock::codeGen()
{

  StmtList::const_iterator it;

  for (it = stmtList.begin(); it != stmtList.end(); it++) {
    std::cout << "正在生成stmt" << endl;
    (**it).codeGen();
  }
}

void NInp::codeGen()
{
}

void NOut::codeGen()
{
}

void NValOp::codeGen()
{
}

void NPosOp::codeGen()
{
}

void NLoop::codeGen()
{
  std::cout << "正在生成NLoop" << endl;
  block.codeGen();
}
