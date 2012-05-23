#include "node.hpp"
#include "codegen.hpp"
#include "brainf.tab.hpp"
using namespace std;
//static const int MIN_ASCII = 97;
static const int MIN_ASCII = 0;

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
  // declare i32 @getchar(i32)

  getchar_func = cast<Function>(module->
      getOrInsertFunction("getchar", IntegerType::getInt32Ty(gC), NULL));

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
    ConstantInt::get(gC,APInt(8, MIN_ASCII)),
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
  BasicBlock *endblock = BasicBlock::Create(getGlobalContext(), "end", mainFunction, 0);
  builder = (*this).currentBuilder();
  builder->CreateBr(endblock);
  builder->SetInsertPoint(endblock);
  builder->CreateRetVoid();
  popBlock();

  std::cout << "Code is generated.\n";
  PassManager pm;
  pm.add(createPrintModulePass(&outs()));
  pm.run(*module);

}

/* Executes the AST by running the main function */
GenericValue CodeGenContext::runCode()
{
  std::cout << "Running code...\n"<<std::endl;
  ExecutionEngine *ee = EngineBuilder(module).create();
  vector<GenericValue> noargs;
  GenericValue v = ee->runFunction(mainFunction, noargs);
  std::cout <<std::endl<< "Code was run.\n";
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

Value* NInp::codeGen(CodeGenContext &context)
{
  IRBuilder<> *builder = context.currentBuilder();
  CallInst* getchar_call = builder->CreateCall(context.getchar_func,
                                               TAPE_LABEL );
  getchar_call->setTailCall(false);
  Value *tape_0 = getchar_call;
  Value *tape_1 = builder->
    CreateTrunc(tape_0, IntegerType::getInt8Ty(getGlobalContext()), TAPE_LABEL);

  builder->CreateStore(tape_1, context.cur_head);
  return NULL;
}

Value* NOut::codeGen(CodeGenContext &context)
{

  IRBuilder<> *builder = context.currentBuilder();
  LoadInst *tape_0 = builder->CreateLoad(context.cur_head, TAPE_LABEL);
  Value *tape_1 = builder->
    CreateSExt(tape_0, IntegerType::getInt32Ty(getGlobalContext()), TAPE_LABEL);


  Value* putchar_params[] = {
    tape_1
  };
  CallInst* putchar_call = builder->CreateCall(context.putchar_func,
                                               putchar_params );
  putchar_call->setTailCall(false);
  return putchar_call;

}

Value* NValOp::codeGen(CodeGenContext& context)
{
  IRBuilder<> *builder = context.currentBuilder();
  LoadInst *tape_0 = builder->CreateLoad(context.cur_head, TAPE_LABEL);
  Value *tape_1;

  ConstantInt *const_int8_1 = ConstantInt::get(getGlobalContext(),
                                               APInt(8, 1));

  switch(op) {
    case CHAR_INC:
      tape_1 = builder->CreateAdd(tape_0, const_int8_1, TAPE_LABEL);
      break;
    case CHAR_DEC:
      tape_1 = builder->CreateSub(tape_0, const_int8_1, TAPE_LABEL);
      break;
  }

  builder->CreateStore(tape_1, context.cur_head);
  return tape_1;
}

Value *NPosOp::codeGen(CodeGenContext& context)
{
  IRBuilder<> *builder = context.currentBuilder();
  APInt cur_value;
  switch(op){
    case POINT_INC:
      cur_value = APInt(32 , 1);
      break;
    case POINT_DEC:
      cur_value = APInt(32 , -1);
      break;
  }

  context.cur_head = builder->
    CreateGEP(context.cur_head,
              ConstantInt::get(getGlobalContext(), cur_value),
              HEAD_LABEL);

  return context.cur_head;
}

Value *NLoop::codeGen(CodeGenContext &context)
{
  std::cout << "正在生成NLoop" << std::endl;
  IRBuilder<> *builder = context.currentBuilder();
  Function *main_func = builder->GetInsertBlock()->getParent();

  BasicBlock *loopbb = BasicBlock::Create(getGlobalContext(), LOOP_LABEL,
                                          main_func);
  BasicBlock *testbb = BasicBlock::Create(getGlobalContext(), LOOP_TEST_LABEL,
                                          main_func);

  // Make Phi node
  PHINode *cur_head_phi =
    PHINode::Create(PointerType::getUnqual(IntegerType::getInt8Ty(getGlobalContext())),
        2, HEAD_LABEL, testbb);
  cur_head_phi->addIncoming(context.cur_head, context.currentBlock());
  context.cur_head = cur_head_phi;

  builder->CreateBr(loopbb);
  builder->SetInsertPoint(loopbb);

  block.codeGen(context);

  builder->CreateBr(testbb);
  builder->SetInsertPoint(testbb);

  // testbb
  {
    Value *head_0 = cur_head_phi;
    LoadInst *tape_0 = new LoadInst(head_0, TAPE_LABEL, testbb);

    //%test.%d = icmp eq i8 %tape.%d, 0
    ICmpInst *test_0 = new ICmpInst(*testbb, ICmpInst::ICMP_EQ, tape_0,
        ConstantInt::get(getGlobalContext(), APInt(8, 0)), LOOP_TEST_LABEL);

    builder->SetInsertPoint(context.currentBlock());
    cur_head_phi->addIncoming(head_0, testbb);

    BasicBlock *afterloop = BasicBlock::Create(getGlobalContext(), "entry",
                                              main_func);
    BranchInst::Create(afterloop, loopbb, test_0, testbb);
    builder->SetInsertPoint(afterloop);
  }
}
