#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Pattern.h"

Pattern::Pattern() {}

/**
* A constructor.
* The constructor takes in the values of the pattern clause in the queries.
*/
Pattern::Pattern(string t_left, string t_right, bool t_subtree) {
  m_left = t_left;
  m_right = t_right;
  m_subtree = t_subtree;
}

/**
* A Getter that returns the left string of the pattern clause.
* Returns the left string of the pattern clause.
*/
string Pattern::getLeft() { return m_left; }

/**
* A Getter that returns the right string of the pattern clause.
* Returns the right string of the pattern clause.
*/
string Pattern::getRight() { return m_right; }

/**
* A Getter that returns a boolean of whether the pattern clause includes the subtree.
* Returns a boolean of whether the pattern clause includes the subtree.
*/
bool Pattern::getSubtree() { return m_subtree; }

/**
* A Setter that sets the left string of the pattern clause.
*/
void Pattern::setLeft(string t_left) {
  m_left = t_left;
}

/**
* A Setter that sets the right string of the pattern clause.
*/
void Pattern::setRight(string t_right) {
  m_right = t_right;
}

/**
* A Setter that sets the boolean of whether the pattern clause includes the subtree.
*/
void Pattern::setSubtree(bool t_subtree) {
  m_subtree = t_subtree;
}