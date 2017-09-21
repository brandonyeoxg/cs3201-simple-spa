#include "StmtListNode.h"

StmtListNode::StmtListNode(int t_lineNum) {
  m_type = TNode::Type::StatementList;
  m_lineNum = t_lineNum;
  m_parent = nullptr;
  m_children = new std::vector<TNode *>();
}
