#include "Relation.h"

Relation::Relation() {}

std::string Relation::FLS = "Follows";
std::string Relation::FLS_ = "Follows*";
std::string Relation::PRT = "Parent";
std::string Relation::PRT_ = "Parent*";
std::string Relation::USE = "Uses";
std::string Relation::MDF = "Modifies";
std::string Relation::CLS = "Calls";
std::string Relation::CLS_ = "Calls*";
std::string Relation::NXT = "Next";
std::string Relation::NXT_ = "Next*";
std::string Relation::AFS = "Affects";
std::string Relation::AFS_ = "Affects*";

queryType::RType Relation::getType() { return m_type; }
Grammar Relation::getG1() { return m_g1; }
Grammar Relation::getG2() { return m_g2; }

Relation::Relation(std::string t_type, Grammar t_g1, Grammar t_g2) {
  if (t_type.compare(FLS) == 0) {
    m_type = queryType::RType::FOLLOWS;
  } else if (t_type.compare(FLS_) == 0) {
    m_type = queryType::RType::FOLLOWS_;
  } else if (t_type.compare(PRT) == 0) {
    m_type = queryType::RType::PARENT;
  } else if (t_type.compare(PRT_) == 0) {
    m_type = queryType::RType::PARENT_;
  } else if (t_type.compare(USE) == 0) {
    m_type = queryType::RType::USES;
  } else if (t_type.compare(MDF) == 0) {
    m_type = queryType::RType::MODIFIES;
  } else if (t_type.compare(CLS) == 0) {
    m_type = queryType::RType::CALLS;
  } else if (t_type.compare(CLS_) == 0) {
    m_type = queryType::RType::CALLS_;
  } else if (t_type.compare(NXT) == 0) {
    m_type = queryType::RType::NEXT;
  } else if (t_type.compare(NXT_) == 0) {
    m_type = queryType::RType::NEXT_;
  } else if (t_type.compare(AFS) == 0) {
    m_type = queryType::RType::AFFECTS;
  } else if (t_type.compare(AFS_) == 0) {
    m_type = queryType::RType::AFFECTS_;
  }

  m_g1 = t_g1;
  m_g2 = t_g2;
}

bool Relation::isFollows(Relation t_relation) {
  return t_relation.getType() == queryType::RType::FOLLOWS;
}

bool Relation::isFollowsStar(Relation t_relation) {
  return t_relation.getType() == queryType::RType::FOLLOWS_;
}

bool Relation::isParent(Relation t_relation) {
  return t_relation.getType() == queryType::RType::PARENT;
}

bool Relation::isParentStar(Relation t_relation) {
  return t_relation.getType() == queryType::RType::PARENT_;
}

bool Relation::isUses(Relation t_relation) {
  return t_relation.getType() == queryType::RType::USES;
}

bool Relation::isModifies(Relation t_relation) {
  return t_relation.getType() == queryType::RType::MODIFIES;
}

bool Relation::isCalls(Relation t_relation) {
  return t_relation.getType() == queryType::RType::CALLS;
}

bool Relation::isCallsStar(Relation t_relation) {
  return t_relation.getType() == queryType::RType::CALLS_;
}

bool Relation::isNext(Relation t_relation) {
  return t_relation.getType() == queryType::RType::NEXT;
}

bool Relation::isNextStar(Relation t_relation) {
  return t_relation.getType() == queryType::RType::NEXT_;
}

bool Relation::isAffects(Relation t_relation) {
  return t_relation.getType() == queryType::RType::AFFECTS;
}

bool Relation::isAffectsStar(Relation t_relation) {
  return t_relation.getType() == queryType::RType::AFFECTS_;
}