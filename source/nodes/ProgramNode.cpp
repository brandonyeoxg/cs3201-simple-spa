#include "ProgramNode.h"

ProgramNode::ProgramNode() {
  m_type = TNode::Type::Program;
  m_lineNum = NO_LINE_NUM;
  m_parent = nullptr;
  m_children = new std::vector<TNode *>();
  m_progName = DEFAULT_PROGRAM_NAME;
}

std::string ProgramNode::getName() {
  return m_progName;
}
