#include <vector>

class TNode {
public:

private:
  int lineNum;
  std::vector<TNode*> children;
  enum Type {
    procedure,
    statementList,
    assign,
    variable,
    whileType // 'while' will have compilation error
  };
  TNode* parent;
};