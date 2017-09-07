#pragma once

#include <string>

#include "nodes\TNode.h"
#include "nodes\TwoChildrenNode.h"
#include "nodes\VariableNode.h"
#include "nodes\ConstantNode.h"

class ASTUtilities {
public:
  static bool matchExact(TNode *node, std::string pattern);
  static bool matchSubtree(TNode *node, std::string pattern);
  static std::vector<std::string> generateStrings(TwoChildrenNode *t_node);

private:
  static std::string getStringFromNode(TNode *t_node);
  static std::string convertTreeToString(TNode *t_node);
  static bool isNodeAnOperator(TNode * t_node);
  static std::vector<std::string> addStrIfNotDuplicate(std::vector<std::string> t_listOfStr, std::string t_str);
  static std::vector<std::string> generateStringList(TwoChildrenNode *t_node, std::vector<std::string> t_listOfStr);
  //static std::vector<std::string> generateStrings(TwoChildrenNode *t_node);
};