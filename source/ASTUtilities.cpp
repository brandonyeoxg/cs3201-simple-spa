#include "ASTUtilities.h"

std::string getStringFromNode(TNode *t_node) {
  switch (t_node->getType()) {
    case TNode::Type::Plus:
      return "+";

    case TNode::Type::Minus:
      return "-";

    case TNode::Type::Variable:
      return ((VariableNode *)t_node)->getVarName();

    case TNode::Type::Constant:
      return std::to_string(((ConstantNode *)t_node)->getValue());
  }
}

std::string convertTreeToString(TNode *t_node) {
  TwoChildrenNode *node = (TwoChildrenNode *)t_node;
  if (node->getLeftChild() == nullptr && node->getRightChild() == nullptr) {
    return getStringFromNode(node);
  }

  return convertTreeToString(node->getLeftChild()) + getStringFromNode(node)
    + convertTreeToString(node->getRightChild());
}

//TODO given tree should generate strings from possible subtrees
std::vector<std::string> generateStrings(TwoChildrenNode *t_node) {

}


bool ASTUtilities::matchExact(TNode *node, std::string pattern) {
  return false;
}

bool ASTUtilities::matchSubtree(TNode *node, std::string pattern) {
  return false;
}





