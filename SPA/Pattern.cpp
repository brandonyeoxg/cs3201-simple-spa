#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Pattern.h"

/**
* A class representing the pattern clause in the queries.
*/

class Pattern {
public:
  /**
  * A constructor.
  * Default constructor.
  */
  Pattern() {}

  /**
  * A constructor.
  * The constructor takes in the values of the pattern clause in the queries.
  */
  Pattern(string t_left, string t_right, bool t_subtree) {
    m_left = t_left;
    m_right = t_right;
    m_subtree = t_subtree;
  }

  /**
  * A Getter that returns the left string of the pattern clause.
  * Returns the left string of the pattern clause.
  */
  string getLeft() { return m_left; }

  /**
  * A Getter that returns the right string of the pattern clause.
  * Returns the right string of the pattern clause.
  */
  string getRight() { return m_right; }

  /**
  * A Getter that returns a boolean of whether the pattern clause includes the subtree.
  * Returns a boolean of whether the pattern clause includes the subtree.
  */
  bool getSubtree() { return m_subtree; }

  /**
  * A Setter that sets the left string of the pattern clause.
  */
  void setLeft(string t_left) {
    m_left = t_left;
  }

  /**
  * A Setter that sets the right string of the pattern clause.
  */
  void setRight(string t_right) {
    m_right = t_right;
  }

  /**
  * A Setter that sets the boolean of whether the pattern clause includes the subtree.
  */
  void setSubtree(bool t_subtree) {
    m_subtree = t_subtree;
  }

private:
  string m_left; /**< left string of the pattern clause */
  string m_right; /**< right string of the pattern clause */
  bool m_subtree; /**< boolean of whether the pattern clause includes the subtree*/
};