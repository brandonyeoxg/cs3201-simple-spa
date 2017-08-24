#include <vector>

class TNode {
public:

private:
  int lineNum;
  std::vector<TNode*> children;
  enum Type {
    assign,
    statementList
  };
  TNode* parent;
};