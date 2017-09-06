#pragma once

#include <vector>

/** Parent class for nodes in AST
*   @author jazlyn
*/

class TNode {
public:
  static const int NO_LINE_NUM = 0;  /**< To be used for when there is no line number for node */
  enum Type {
    Assign,
    Constant,
    If,
    Minus,
    Plus,
    Procedure,
    Program,
    StatementList,
    Variable,
    While
  };

  void setParent(TNode *t_tNode);
  void addChild(TNode *t_tNode);

  int getLineNum();
  TNode::Type getType();
  TNode *TNode::getParent();
  std::vector<TNode *> *getChildren();

protected:
  Type m_type;    /**< Type of node */
  int m_lineNum;  /**< Statement line number, to be set to NO_LINE_NUM if node is not a statement */
  std::vector<TNode *> *m_children; /**< To be set to nullptr if node should have no children */
  TNode *m_parent;  /**< To be set to nullptr on instantiation to show parent is not yet set */
  
};