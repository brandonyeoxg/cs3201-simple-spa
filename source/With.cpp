#include "With.h"

With::With() {}

With::With(Grammar t_g1, Grammar t_g2) {
  m_g1 = t_g1;
  m_g2 = t_g2;
}

queryType::clauseType With::getClauseType() { return queryType::clauseType::WITH; }

Grammar With::getG1() { return m_g1; }
Grammar With::getG2() { return m_g2; }

/**
* A public function that prints the content of this With object.
*/
void With::toString() {
  std::cout << "\n" << getG1().getName() << "." << getG1().getAttr() << " = ";
  std::cout << getG2().getName() << "." << getG2().getAttr() << "\n";
}
