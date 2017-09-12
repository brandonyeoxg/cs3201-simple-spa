#include "Relation.h"
#include <string>
#include "Grammar.h"

Relation::Relation() {}
Relation::RType Relation::getType() { return m_type; }
Grammar Relation::getG1() { return m_g1; }
Grammar Relation::getG2() { return m_g2; }

Relation::Relation(std::string t_type, Grammar t_g1, Grammar t_g2) {
  if (t_type.compare("Follows") == 0) {
    m_type = RType::FOLLOWS;
  }
  else if (t_type.compare("Follows*") == 0) {
    m_type = RType::FOLLOWS_;
  }
  else if (t_type.compare("Parent") == 0) {
    m_type = RType::PARENT;
  }
  else if (t_type.compare("Parent*") == 0) {
    m_type = RType::PARENT_;
  }
  else if (t_type.compare("Uses") == 0) {
    m_type = RType::USES;
  }
  else if (t_type.compare("Modifies") == 0) {
    m_type = RType::MODIFIES;
  }
  m_g1 = t_g1;
  m_g2 = t_g2;
}