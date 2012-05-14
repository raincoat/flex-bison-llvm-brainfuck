#include <iostream>
#include <vector>

class NStmt;

typedef std::vector<NStmt*> StmtList;


class Node {
  public:
    virtual ~Node() {}
    virtual void codeGen() {}
};

class NStmt : public Node {
};

class NBlock : public NStmt {
  public:
    StmtList stmtList;
    NBlock(){}
    virtual void codeGen();
};

class NInp : public NStmt {
  public:
    virtual void codeGen();
};

class NOut : public NStmt {
  public:
    virtual void codeGen();
};

class NValOp: public NStmt {
  public:
    int op;
    NValOp(int op): op(op){}
    virtual void codeGen();
};

class NPosOp : public NStmt {
  public:
    int op;
    NPosOp(int op): op(op){}
    virtual void codeGen();
};

class NLoop : public NStmt {
  public:
    NBlock block;
    NLoop(NBlock& block): block(block) {}
    virtual void codeGen();
};
