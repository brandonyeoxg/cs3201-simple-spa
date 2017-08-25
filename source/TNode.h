#include <vector>

class TNode {
public:
  enum Type {
    procedure,
    statementList,
    assign,
    variable,
    whileType, // 'while' will have compilation error
    constant
  };
  TNode(Type type);

private:
  int lineNum;
  std::vector<TNode *> children;
  Type type;
  TNode *parent;
  
};