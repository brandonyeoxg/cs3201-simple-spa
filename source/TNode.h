#pragma once

#include <vector>

class TNode {
public:
  static const int NO_LINE_NUM = 0;  // To be used for when there is no line number for node
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
  std::vector<TNode *> *getChildren();

  void printDataMembers();

protected:
  Type m_type;
  int m_lineNum;
  std::vector<TNode *> *m_children;
  TNode *m_parent;
  
};