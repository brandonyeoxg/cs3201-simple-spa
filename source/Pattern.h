#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#ifndef PATTERN_H
#define PATTERN_H

class Pattern {
public:
  Pattern();
  Pattern(std::string t_left, std::string t_right, bool t_subtree);
  std::string getLeft();
  std::string getRight();
  bool getSubtree();
  void setLeft(std::string);
  void setRight(std::string);
  void setSubtree(bool);

private:
  std::string m_left;
  std::string m_right;
  bool m_subtree;
};

#endif PATTERN_H