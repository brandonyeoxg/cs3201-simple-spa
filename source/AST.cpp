#include "AST.h"

AST::AST() {
  m_rootNode = new ProcedureNode(DEFAULT_PROC_NAME);
}

TNode *AST::getRoot() {
  return m_rootNode;
}

