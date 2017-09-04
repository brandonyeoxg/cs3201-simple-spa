#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Grammar.h"

#ifndef PATTERN_H
#define PATTERN_H

class Pattern {
public:
  Pattern();
  Pattern(Grammar t_grammar, std::string t_left, std::string t_right, bool t_subtree);
  Grammar getGrammar();
  std::string getLeft();
  std::string getRight();
  bool getSubtree();
  void setGrammar(Grammar);
  void setLeft(std::string);
  void setRight(std::string);
  void setSubtree(bool);

private:
  Grammar m_grammar; /**< grammar of the pattern clause */
  std::string m_left; /**< left hand side of the pattern clause */
  std::string m_right; /**< right hand side of the pattern clause */
  bool m_subtree; /**< true if m_right can be a sub-expression of the expression on the right hand side
                       false if the expression on the right hand side has to be the exact match as m_right*/
};

#endif PATTERN_H