#include <iostream>
#include <vector>
#include <llvm/Value.h>

class NStmt;
class CodeGenContext;

typedef std::vector<NStmt*> StmtList;


class Node {
  public:
    virtual ~Node() {}
    virtual llvm::Value *codeGen(CodeGenContext &context) {}
};

class NStmt : public Node {
};

class NBlock : public NStmt {
  public:
    StmtList stmtList;
    NBlock(){}
    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NInp : public NStmt {
  public:
    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NOut : public NStmt {
  public:
    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NValOp: public NStmt {
  public:
    int op;
    NValOp(int op): op(op){}
    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NPosOp : public NStmt {
  public:
    int op;
    NPosOp(int op): op(op){}
    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NLoop : public NStmt {
  public:
    NBlock block;
    NLoop(NBlock& block): block(block) {}
    virtual llvm::Value *codeGen(CodeGenContext &context);
};
