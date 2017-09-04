#include "StmtListNode.h"

StmtListNode::StmtListNode() {
  m_type = TNode::Type::StatementList;
  m_lineNum = NO_LINE_NUM;
  m_parent = nullptr;
  m_children = new std::vector<TNode *>();
}
