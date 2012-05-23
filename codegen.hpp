#include <stack>
#include <typeinfo>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>
#include <llvm/LLVMContext.h>
#include <llvm/PassManager.h>
#include <llvm/Instructions.h>
#include <llvm/Intrinsics.h>
#include <llvm/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

class NBlock;

class CodeGenBlock {
  public:
    BasicBlock *block;
    IRBuilder<> *builder;
    std::map<std::string, Value *> locals;
};

static const char *HEAD_LABEL = "head";
static const char *TAPE_LABEL = "tape";
static const char *LOOP_TEST_LABEL = "testbb";
static const char *LOOP_LABEL = "loopbb";

class CodeGenContext{
  std::stack<CodeGenBlock *> blocks;
  Function *mainFunction;
  public:
    Module *module;
    Function *putchar_func;
    Function *getchar_func;
    int cur_value;
    Value* cur_head;
    Value* ptr_arr;

    CodeGenContext()
    {
      LLVMContext &gC = getGlobalContext(); // globalContext
      module = new Module("main", gC);
    }

    void generateCode(NBlock& root);
    GenericValue runCode();

    std::map<std::string, Value*>& locals() { return blocks.top()->locals; }
    BasicBlock *currentBlock() { return blocks.top()->block; }
    IRBuilder<> *currentBuilder() { return blocks.top()->builder;}

    void pushBlock(BasicBlock *block) {
      blocks.push(new CodeGenBlock());
      blocks.top()->block = block;
      blocks.top()->builder = new IRBuilder<>(blocks.top()->block);
    }

    void popBlock() {
      CodeGenBlock *top = blocks.top(); blocks.pop(); delete top;
    }
};
