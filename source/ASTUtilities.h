#pragma once

#include <string>
#include <algorithm>

#include "nodes\TNode.h"
#include "nodes\TwoChildrenNode.h"
#include "nodes\VariableNode.h"
#include "nodes\ConstantNode.h"

/** Class to hold static functions required to analyze AST
*   @author jazlyn
*/

class ASTUtilities {
public:

  /** Checks if given tree and pattern match exactly
  *   @param t_node parent node of tree (assumes all nodes populated and linked properly)
  *   @param t_pattern pattern to compare
  *   @return true if pattern matches exactly, else false
  */
  static bool matchExact(TNode *t_node, std::string t_pattern);

  /** Checks if given tree and pattern match by subtree
  *   @param t_node parent node of tree (assumes all nodes populated and linked properly)
  *   @param t_pattern pattern to compare
  *   @return true if pattern matches partial subtree of given tree, else false
  */
  static bool matchSubtree(TNode *t_node, std::string t_pattern);

  static std::vector<std::string> generateSubtreeStrings(TNode *t_node);  // exposing this to enable unit testing

private:
  static std::string getStringFromNode(TNode *t_node);
  static std::string convertTreeToString(TNode *t_node);
  static bool isNodeAnOperator(TNode * t_node);
  static std::vector<std::string> addStrIfNotDuplicate(std::vector<std::string> t_listOfStr, std::string t_str);
  static std::vector<std::string> generateStringList(TwoChildrenNode *t_node, std::vector<std::string> t_listOfStr);
  static std::string removeWhitespaces(std::string t_pattern);
};