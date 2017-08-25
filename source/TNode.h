#include <vector>

class TNode {
public:
  enum Type {
    Procedure,
    StatementList,
    Assign,
    Variable,
    While,
    Constant,
    Add,
    If
  };
  TNode(Type type);

private:
  int m_lineNum;
  std::vector<TNode *> m_children;
  Type m_type;
  TNode *m_parent;
  
};