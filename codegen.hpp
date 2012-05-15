#include <stack>
#include <typeinfo>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>
#include <llvm/LLVMContext.h>
#include <llvm/PassManager.h>
#include <llvm/Instructions.h>
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
    std::map<std::string, Value*> locals;
};

class CodeGenContext{
  std::stack<CodeGenBlock *> blocks;
  Function *mainFunction;
  public:
    Module *module;

    Function *func_putchar;


    CodeGenContext() {
      LLVMContext &gC = getGlobalContext(); // globalContext
      module = new Module("main", gC);
      /* header */
      // declare i32 @putchar(i32)
      func_putchar = cast<Function>(module->
          getOrInsertFunction("putchar", IntegerType::getInt32Ty(gC),
                              IntegerType::getInt32Ty(gC), NULL));

    }

    void generateCode(NBlock& root);
    GenericValue runCode();

    std::map<std::string, Value*>& locals() { return blocks.top()->locals; }
    BasicBlock *currentBlock() { return blocks.top()->block; }

    void pushBlock(BasicBlock *block) {
      blocks.push(new CodeGenBlock()); blocks.top()->block = block;
    }

    void popBlock() {
      CodeGenBlock *top = blocks.top(); blocks.pop(); delete top;
    }
};
