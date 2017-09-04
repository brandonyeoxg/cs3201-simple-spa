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
Pattern::Pattern(Grammar t_grammar, std::string t_left, std::string t_right, bool t_subtree) {
  m_grammar = t_grammar;
  m_left = t_left;
  m_right = t_right;
  m_subtree = t_subtree;
}

/**
* A Getter that returns the grammar of the pattern clause.
* Returns the grammar of the pattern clause.
*/
Grammar Pattern::getGrammar() { return m_grammar; }

/**
* A Getter that returns the left string of the pattern clause.
* Returns the left string of the pattern clause.
*/
std::string Pattern::getLeft() { return m_left; }

/**
* A Getter that returns the right string of the pattern clause.
* Returns the right string of the pattern clause.
*/
std::string Pattern::getRight() { return m_right; }

/**
* A Getter that returns a boolean of whether the pattern clause includes the subtree.
* Returns a boolean of whether the pattern clause includes the subtree.
*/
bool Pattern::getSubtree() { return m_subtree; }

/**
* A Setter that sets the grammar of the pattern clause.
*/
void Pattern::setGrammar(Grammar t_grammar) {
  m_grammar = t_grammar;
}

/**
* A Setter that sets the left string of the pattern clause.
*/
void Pattern::setLeft(std::string t_left) {
  m_left = t_left;
}

/**
* A Setter that sets the right string of the pattern clause.
*/
void Pattern::setRight(std::string t_right) {
  m_right = t_right;
}

/**
* A Setter that sets the boolean of whether the pattern clause includes the subtree.
*/
void Pattern::setSubtree(bool t_subtree) {
  m_subtree = t_subtree;
}