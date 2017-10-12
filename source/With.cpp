#include "With.h"

With::With() {}

Grammar Relation::getG1() { return m_g1; }
Grammar Relation::getG2() { return m_g2; }

With::With(Grammar t_g1, Grammar t_g2) {
  m_g1 = t_g1;
  m_g2 = t_g2;
}
