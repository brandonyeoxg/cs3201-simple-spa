#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Pattern.h"

/**
* Default Constructor
*/
Pattern::Pattern() {}

/**
* A constructor.
* The constructor takes in the values of the pattern clause in the queries.
*/
Pattern::Pattern(Grammar t_clause, Grammar t_left, Grammar t_right, bool t_subtree) {
  setStmt(t_clause);
  setLeft(t_left);
  setRight(t_right);
  setSubtree(t_subtree);
}

queryType::clauseType Pattern::getClauseType() { return queryType::clauseType::PATTERN; }

/**
* A Getter that returns the grammar of the pattern clause.
* Returns the grammar of the pattern clause.
*/
Grammar Pattern::getStmt() { return m_statement; }

/**
* A Getter that returns the left string of the pattern clause.
* Returns the left string of the pattern clause.
*/
Grammar Pattern::getLeft() { return m_left; }

/**
* A Getter that returns the right string of the pattern clause.
* Returns the right string of the pattern clause.
*/
Grammar Pattern::getRight() { return m_right; }

/**
* A Getter that returns a boolean of whether the pattern clause includes the subtree.
* Returns a boolean of whether the pattern clause includes the subtree.
*/
bool Pattern::isSubtree() { return m_subtree; }

/**
* A Setter that sets the grammar of the pattern clause.
*/
void Pattern::setStmt(Grammar t_stmt) {
  m_statement = t_stmt;
}

/**
* A Setter that sets the left string of the pattern clause.
*/
void Pattern::setLeft(Grammar t_left) {
  m_left = t_left;
}

/**
* A Setter that sets the right string of the pattern clause.
*/
void Pattern::setRight(Grammar t_right) {
  m_right = t_right;
}

/**
* A Setter that sets the boolean of whether the pattern clause includes the subtree.
*/
void Pattern::setSubtree(bool t_subT) {
  m_subtree = t_subT;
}

/**
* A public function that prints the content of this Pattern object.
*/
void Pattern::toString() {
  std::cout << "\n" << getStmt().getName() << " (";
  std::cout << getLeft().getName() << ", ";
  std::cout << getRight().getName() << ") ";
  std::cout << isSubtree() << "\n";
}