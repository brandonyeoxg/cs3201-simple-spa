#include "Relation.h"

Relation::Relation() {}

std::string Relation::FLS = "Follows";
std::string Relation::FLS_ = "Follows*";
std::string Relation::PRT = "Parent";
std::string Relation::PRT_ = "Parent*";
std::string Relation::USE = "Uses";
std::string Relation::MDF = "Modifies";

queryType::RType Relation::getType() { return m_type; }
Grammar Relation::getG1() { return m_g1; }
Grammar Relation::getG2() { return m_g2; }

Relation::Relation(std::string t_type, Grammar t_g1, Grammar t_g2) {
  if (t_type.compare(FLS) == 0) {
    m_type = queryType::RType::FOLLOWS;
  }
  else if (t_type.compare(FLS_) == 0) {
    m_type = queryType::RType::FOLLOWS_;
  }
  else if (t_type.compare(PRT) == 0) {
    m_type = queryType::RType::PARENT;
  }
  else if (t_type.compare(PRT_) == 0) {
    m_type = queryType::RType::PARENT_;
  }
  else if (t_type.compare(USE) == 0) {
    m_type = queryType::RType::USES;
  }
  else if (t_type.compare(MDF) == 0) {
    m_type = queryType::RType::MODIFIES;
  }
  m_g1 = t_g1;
  m_g2 = t_g2;
}