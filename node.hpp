#include <iostream>
#include <vector>

class Node {
  public:
    virtual ~Node() {}
    virtual void CodeGen() {}
};

class NBlock : public Node {
  public:
    virtual void CodeGen() {}
};

class Ninp : public Node {
  public:
    virtual void CodeGen() {}
};

class Nout : public Node {
  public:
    virtual void CodeGen() {}
};

class NValOp: public Node {
  public:
    virtual void CodeGen() {}
};

class NPosOp : public Node {
  public:
    virtual void CodeGen() {}
};

class NLoop : public Node {
  public:
    virtual void CodeGen() {}
};


