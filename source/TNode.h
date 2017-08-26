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

  void setParent(TNode *t_tNode);
  void addChild(TNode *t_tNode);

  int getLineNum();
  TNode::Type getType();
  TNode *TNode::getParent();

protected:
  Type m_type;
  int m_lineNum;
  std::vector<TNode *> *m_children;
  TNode *m_parent;
  
};