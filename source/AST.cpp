#include "AST.h"

AST::AST() {
  m_rootNode = new ProcedureNode(0);
}

TNode *AST::getRoot() {
  return m_rootNode;
}

