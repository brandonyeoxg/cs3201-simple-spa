#include "ASTUtilities.h"

std::string ASTUtilities::getStringFromNode(TNode *t_node) {
  switch (t_node->getType()) {
    case TNode::Type::Plus:
      return "+";

    case TNode::Type::Minus:
      return "-";

    case TNode::Type::Variable:
      return ((VariableNode *)t_node)->getVarName();

    case TNode::Type::Constant:
      return std::to_string(((ConstantNode *)t_node)->getValue());

    default:
      return "";
  }
}

std::string ASTUtilities::convertTreeToString(TNode *t_node) {
  
  // reached an end node
  if (t_node->getType() == TNode::Type::Constant || t_node->getType() == TNode::Type::Variable) {
    return getStringFromNode(t_node);
  }

  // Operator node
  TwoChildrenNode *node = (TwoChildrenNode *)t_node;

  return convertTreeToString(node->getLeftChild()) + getStringFromNode(node)
    + convertTreeToString(node->getRightChild());
}

bool ASTUtilities::isNodeAnOperator(TNode * t_node) {
  TNode::Type type = t_node->getType();
  return (type == TNode::Type::Plus || type == TNode::Type::Minus);
}

std::vector<std::string> ASTUtilities::addStrIfNotDuplicate(
  std::vector<std::string> t_listOfStr, std::string t_str) {

  for (int i = 0; i < t_listOfStr.size(); i++) {
    if (t_listOfStr.at(i) == t_str) {
      return t_listOfStr;
    }
  }

  t_listOfStr.push_back(t_str);

  return t_listOfStr;
}

std::vector<std::string> ASTUtilities::generateStringList(TwoChildrenNode *t_node,
  std::vector<std::string> t_listOfStr) {
  if (isNodeAnOperator(t_node)) {
    t_listOfStr = addStrIfNotDuplicate(t_listOfStr, convertTreeToString(t_node));
  } else {
    return t_listOfStr;
  }

  t_listOfStr = addStrIfNotDuplicate(t_listOfStr, 
    convertTreeToString((TwoChildrenNode *)t_node->getLeftChild()));
  t_listOfStr = addStrIfNotDuplicate(t_listOfStr, 
    convertTreeToString((TwoChildrenNode *)t_node->getRightChild()));

  t_listOfStr = generateStringList((TwoChildrenNode *)t_node->getLeftChild(), t_listOfStr);
  t_listOfStr = generateStringList((TwoChildrenNode *)t_node->getRightChild(), t_listOfStr);

  return t_listOfStr;
}

//TODO given tree should generate strings from possible subtrees
std::vector<std::string> ASTUtilities::generateStrings(TwoChildrenNode *t_node) {
  std::vector<std::string> listOfStr = std::vector<std::string>();
  return generateStringList(t_node, listOfStr);
}

bool ASTUtilities::matchExact(TNode *node, std::string pattern) {
  return false;
}

bool ASTUtilities::matchSubtree(TNode *node, std::string pattern) {
  return false;
}





