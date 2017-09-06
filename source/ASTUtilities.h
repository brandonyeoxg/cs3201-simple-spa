#pragma once

#include <string>

#include "nodes\TNode.h"
#include "nodes\TwoChildrenNode.h"
#include "nodes\VariableNode.h"
#include "nodes\ConstantNode.h"

static class ASTUtilities {
public:
  static bool matchExact(TNode *node, std::string pattern);
  static bool matchSubtree(TNode *node, std::string pattern);
};