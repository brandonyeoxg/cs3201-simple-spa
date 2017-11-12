#include "With.h"

With::With() {}

With::With(Grammar t_g1, Grammar t_g2) {
  m_g1 = t_g1;
  m_g2 = t_g2;
  setWeights(0);
  if (Grammar::isStmtNo(t_g1.getType()) && Grammar::isStmtNo(t_g2.getType())) {
    addWeights(1);
  } else if (Grammar::isString(t_g1.getType()) && Grammar::isString(t_g2.getType())) {
    addWeights(2);
  } else if ((Grammar::isProgLine(t_g1.getType()) && Grammar::isStmtNo(t_g2.getType()))
    || (Grammar::isProgLine(t_g2.getType()) && Grammar::isStmtNo(t_g1.getType()))) {
    addWeights(3);
  } else if (Grammar::isProgLine(t_g1.getType()) && Grammar::isProgLine(t_g2.getType())) {
    addWeights(4);
  } else if (Grammar::isStmtNo(t_g1.getType()) || Grammar::isStmtNo(t_g2.getType())) {
    addWeights(5);
  } else if (Grammar::isString(t_g1.getType()) || Grammar::isString(t_g2.getType())) {
    addWeights(6);
  } else if (!Grammar::isStmtNo(t_g1.getType()) && !Grammar::isStmtNo(t_g2.getType())
    && Grammar::isString(t_g1.getType()) && Grammar::isString(t_g2.getType())) {
    addWeights(7);
  } else {
    addWeights(0);
  }
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