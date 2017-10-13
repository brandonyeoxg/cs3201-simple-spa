#include "With.h"

With::With() {}

With::With(Grammar t_g1, Grammar t_g2) {
  m_g1 = t_g1;
  m_g2 = t_g2;
}

Grammar With::getG1() { return m_g1; }
Grammar With::getG2() { return m_g2; }

