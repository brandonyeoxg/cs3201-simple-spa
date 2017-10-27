#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Grammar.h"

#ifndef PATTERN_H
#define PATTERN_H

/**A class representing the "Pattern" clause of a query.
*  @author Verbena Ong
*  @date 09/09/2017
*/
class Pattern {
public:

  /**
  * Default Constructor
  */
  Pattern();

  /**
  * A constructor.
  * The constructor takes in the values of the Pattern clause in the queries.
  * @param t_statement is a Grammar object representing the type of statement the Pattern clause is on.
  * @param t_left represents the left hand side to match in the statement of the Pattern clause.
  * @param t_right represents the right hand side to match in the statement of the Pattern clause.
  * @param t_subtree is a boolean determining if the statement can be a sub-expression of or has to be an exact match as m_right
  */
  Pattern(Grammar t_statement, Grammar t_left, Grammar t_right, bool t_subtree);

  /**
  * A Getter that returns the type of statement of the pattern clause as a Grammar object.
  * @return The type of statement of the Pattern clause as a Grammar object.
  */
  Grammar getStmt();

  /**
  * A Getter that returns the left hand side of the Pattern clause.
  * @return The left hand side Grammar of the Pattern clause.
  */
  Grammar getLeft();

  /**
  * A Getter that returns the right hand side of the Pattern clause.
  * @return The right hand side Grammar of the Pattern clause.
  */
  Grammar getRight();

  /**
  * A Getter that returns a boolean of whether the statement can be a sub-expression of or has to be an exact match as m_right of the Pattern clause.
  * @return a boolean of whether the statement can be a sub-expression of or has to be an exact match as m_right of the Pattern clause.
  */
  bool isSubtree();

  /**
  * A Setter that sets a Grammar object as the left hand side of the Pattern clause.
  * @param t_left is the Grammar object to set as.
  */
  void setLeft(Grammar t_left);

  /**
  * A public function that prints the content of this Pattern object.
  */
  void toString();

private:

  /**
  * A Setter that sets a Grammar object as the type of statement of the Pattern clause.
  * @param t_stmt is the Grammar object to set as.
  */
  void setStmt(Grammar t_stmt);

  /**
  * A Setter that sets a Grammar object as the right hand side of the Pattern clause.
  * @param t_right is the Grammar object to set as.
  */
  void setRight(Grammar t_right);

  /**
  * A Setter that sets a boolean that determines if the statement can be a sub-expression of or has to be an exact match as m_right of the Pattern clause.
  * @param t_subT is the value of the boolean to set.
  */
  void setSubtree(bool t_subT);

  Grammar m_statement; /**< Grammar of the Pattern clause. */
  Grammar m_left; /**< Left hand side of the pattern clause. */
  Grammar m_right; /**< Right hand side of the pattern clause. */
  bool m_subtree; /**< true if m_right can be a sub-expression of the expression on the right hand side
                       false if the expression on the right hand side has to be the exact match as m_right.*/
};

#endif PATTERN_H