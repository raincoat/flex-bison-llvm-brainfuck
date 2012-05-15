#include "node.hpp"
#include "codegen.hpp"
#include "brainf.tab.hpp"
using namespace std;

void CodeGenContext::generateCode(NBlock& root)
{

  ArrayRef<Type*> argTypes;
  FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()),
                           argTypes, false);

  mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);
  BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);

  /* Push a new variable/block context */
  pushBlock(bblock);
  root.codeGen(*this); /* emit bytecode for the toplevel block */
  ReturnInst::Create(getGlobalContext(), bblock);
  popBlock();

  std::cout << "Code is generated.\n";
  PassManager pm;
  pm.add(createPrintModulePass(&outs()));
  pm.run(*module);

}

/* Executes the AST by running the main function */
GenericValue CodeGenContext::runCode()
{
  std::cout << "Running code...\n";
  ExecutionEngine *ee = EngineBuilder(module).create();
  vector<GenericValue> noargs;
  GenericValue v = ee->runFunction(mainFunction, noargs);
  std::cout << "Code was run.\n";
  return v;
}

/* Returns an LLVM type based on the identifier */

/* -- Code Generation -- */

Value* NBlock::codeGen(CodeGenContext& context)
{

  StmtList::const_iterator it;
  Value *last = NULL;
  for (it = stmtList.begin(); it != stmtList.end(); it++) {
    std::cout << "正在生成stmt" << endl;
    last = (**it).codeGen(context);
  }

  std::cout << "Creating block" << endl;
  return last;
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
  //block.codeGen(context);
}
