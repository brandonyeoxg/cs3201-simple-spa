#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#ifndef PATTERN_H
#define PATTERN_H

class Pattern {
public:
  Pattern();
  Pattern(string t_left, string t_right, bool t_subtree);
  string getLeft();
  string getRight();
  bool getSubtree();
  void setLeft(string);
  void setRight(string);
  void setSubtree(bool);

private:
  string m_left;
  string m_right;
  bool m_subtree;
};

#endif PATTERN_H