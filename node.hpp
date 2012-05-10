#include <iostream>
#include <vector>

class Node {
  public:
    virtual ~Node() {}
    virtual void CodeGen() {}
};

class NStmt : public Node {
};

class NBlock : public NStmt {
  public:
    virtual void CodeGen();
};

class NInp : public NStmt {
  public:
    virtual void CodeGen();
};

class NOut : public NStmt {
  public:
    virtual void CodeGen();
};

class NValOp: public NStmt {
  public:
    int op;
    NValOp(int op): op(op){}
    virtual void CodeGen();
};

class NPosOp : public NStmt {
  public:
    int op;
    NPosOp(int op): op(op){}
    virtual void CodeGen();
};

class NLoop : public NStmt {
  public:
    virtual void CodeGen();
};
