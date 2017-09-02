#include "AST.h"

AST::AST() {
  m_rootNode = new ProgramNode();
}

ProgramNode *AST::getRoot() {
  return m_rootNode;
}

