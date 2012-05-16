#include "node.hpp"
#include "codegen.hpp"
#include "brainf.tab.hpp"
using namespace std;

void CodeGenContext::generateCode(NBlock& root)
{

  LLVMContext &gC = getGlobalContext(); // globalContext
  ArrayRef<Type*> argTypes;
  FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()),
                           argTypes, false);

  mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage,
                                  "main", module);

  BasicBlock *bblock = BasicBlock::Create(getGlobalContext(),
                                          "entry", mainFunction, 0);
  /* header */
  // declare void @memset
  Type *memset_params_type[] = {
    Type::getInt8PtrTy(gC),
    Type::getInt32Ty(gC)
  };
  Function *memset_func = Intrinsic::getDeclaration(module, Intrinsic::memset,
                                                    memset_params_type);

  // declare i32 @putchar(i32)

  putchar_func = cast<Function>(module->
      getOrInsertFunction("putchar", IntegerType::getInt32Ty(gC),
                          IntegerType::getInt32Ty(gC), NULL));
  // %arr = malloc i8, i32 %d
  int mem_total = 65536;
  ConstantInt *val_mem = ConstantInt::get(gC, APInt(32, mem_total));
  Type* IntPtrTy = IntegerType::getInt32Ty(gC);
  Type* Int8Ty = IntegerType::getInt8Ty(gC);
  Constant* alloc_size = ConstantExpr::getSizeOf(Int8Ty);
  alloc_size = ConstantExpr::getTruncOrBitCast(alloc_size, IntPtrTy);
  ptr_arr = CallInst::CreateMalloc(bblock, IntPtrTy, Int8Ty, alloc_size, val_mem,
                                      NULL, "arr");

  bblock->getInstList().push_back(cast<Instruction>(ptr_arr));
  // call memset
  Value*memset_params[]={
    ptr_arr,
    ConstantInt::get(gC,APInt(8,0)),
    val_mem,
    ConstantInt::get(gC,APInt(32,1)),
    ConstantInt::get(gC,APInt(1,0))
  };
  IRBuilder<> *builder = new IRBuilder<> (bblock);

  CallInst*memset_call = builder->
    CreateCall(memset_func,memset_params);
  memset_call->setTailCall(false);

  cur_head = builder->
    CreateGEP(ptr_arr, ConstantInt::get(gC, APInt(32, mem_total / 2)),
              HEAD_LABEL);

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

Value* NOut::codeGen(CodeGenContext& context)
{
  ConstantInt* const_int32_3 = ConstantInt::get(getGlobalContext(),
                                                APInt(32, StringRef("32"), 10)
                                                );
  Value* putchar_params[] = {
    const_int32_3
  };
  CallInst* putchar_call = context.currentBuilder()->CreateCall(context.putchar_func,
                                             putchar_params );
  putchar_call->setTailCall(false);
  return putchar_call;

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
