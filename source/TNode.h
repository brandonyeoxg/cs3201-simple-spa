#pragma once

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
  TNode();
  int getLineNum();
  TNode::Type getType();

protected:
  Type m_type;
  int m_lineNum;
  std::vector<TNode *> *m_children;
  TNode *m_parent;
  
};