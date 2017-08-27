#pragma once

/**
 * Represents an extractor which extract complicated abstractions from AST.
 *
 * @author Brandon
 * @date 8/27/2017
 */

class ParentsTable;
class FollowsTable;

class DesignExtractor {
public:
  ParentsTable* obtainParentsStar(TNode *t_astRoot);
  FollowsTable* obtainFollowsStar(TNode *t_astRoot);
};