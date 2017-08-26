#include "TNode.h"

TNode::TNode() {} //default empty constructor


/********** Getter Functions **********/

int TNode::getLineNum() {
  return m_lineNum;
}

TNode::Type TNode::getType() {
  return m_type;
}



