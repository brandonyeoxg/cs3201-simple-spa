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

queryType::clauseType Relation::getClauseType() { return queryType::clauseType::RELATION;  }
queryType::RType Relation::getType() { return m_type; }
Grammar Relation::getG1() { return m_g1; }
Grammar Relation::getG2() { return m_g2; }
void Relation::setG1(Grammar t_g1) {
  m_g1 = t_g1;
}
void Relation::setG2(Grammar t_g2) {
  m_g2 = t_g2;
}

/**
* A public function that prints the content of this Relation object.
*/
void Relation::toString() {
  std::cout << "\n" << getType() << " (";
  std::cout << getG1().getName() << ", ";
  std::cout << getG2().getName() << ")\n";
}

Relation::Relation(std::string t_type, Grammar t_g1, Grammar t_g2) {
  if (t_type.compare(FLS) == 0) {
    m_type = queryType::RType::FOLLOWS;
    setWeights(6);
  } else if (t_type.compare(FLS_) == 0) {
    m_type = queryType::RType::FOLLOWS_;
    setWeights(7);
  } else if (t_type.compare(PRT) == 0) {
    m_type = queryType::RType::PARENT;
    setWeights(4);
  } else if (t_type.compare(PRT_) == 0) {
    m_type = queryType::RType::PARENT_;
    setWeights(5);
  } else if (t_type.compare(USE) == 0) {
    m_type = queryType::RType::USES;
    setWeights(8);
  } else if (t_type.compare(MDF) == 0) {
    m_type = queryType::RType::MODIFIES;
    setWeights(3);
  } else if (t_type.compare(CLS) == 0) {
    m_type = queryType::RType::CALLS;
    setWeights(1);
  } else if (t_type.compare(CLS_) == 0) {
    m_type = queryType::RType::CALLS_;
    setWeights(2);
  } else if (t_type.compare(NXT) == 0) {
    m_type = queryType::RType::NEXT;
    setWeights(9);
  } else if (t_type.compare(NXT_) == 0) {
    m_type = queryType::RType::NEXT_;
    setWeights(10);
  } else if (t_type.compare(AFS) == 0) {
    m_type = queryType::RType::AFFECTS;
    setWeights(11);
  } else if (t_type.compare(AFS_) == 0) {
    m_type = queryType::RType::AFFECTS_;
    setWeights(12);
  }

  m_g1 = t_g1;
  m_g2 = t_g2;

  if (QueryUtil::isAllUnderscores(t_g1, t_g2)) {
    addWeights(1);
  } else if ((StringUtil::isUnderscore(t_g1.getName()) && Grammar::isStmtNo(t_g2.getType()))
    || (Grammar::isStmtNo(t_g1.getType()) && StringUtil::isUnderscore(t_g2.getName()))) {
    addWeights(2);
  } else if (QueryUtil::hasNoSynonyms(t_g1, t_g2)) {
    addWeights(3);
  } else if (QueryUtil::hasOneLeftSynonym(t_g1, t_g2) || QueryUtil::hasOneRightSynonym(t_g1, t_g2)) {
    if (Grammar::isStmtNo(t_g1.getType()) || Grammar::isStmtNo(t_g2.getType())) {
      addWeights(4);
    } else {
      addWeights(5);
    }
  } else if (QueryUtil::hasTwoSynonyms(t_g1, t_g2)) {
    addWeights(6);
  } else {
    addWeights(0);
  } 
}

STRING Relation::getTypeInString() {
  if (m_type == queryType::RType::FOLLOWS) {
    return FLS;
  } else if (m_type == queryType::RType::FOLLOWS_) {
    return FLS_;
  } else if (m_type == queryType::RType::PARENT) {
    return PRT;
  } else if (m_type == queryType::RType::PARENT_) {
    return PRT_;
  } else if (m_type == queryType::RType::USES) {
    return USE;
  } else if (m_type == queryType::RType::MODIFIES) {
    return MDF;
  } else if (m_type == queryType::RType::CALLS) {
    return CLS;
  } else if (m_type == queryType::RType::CALLS_) {
    return CLS_;
  } else if (m_type == queryType::RType::NEXT) {
    return NXT;
  } else if (m_type == queryType::RType::NEXT_) {
    return NXT_;
  } else if (m_type == queryType::RType::AFFECTS) {
    return AFS;
  } else if (m_type == queryType::RType::AFFECTS_) {
    return AFS_;
  }

  return "";
}

bool Relation::isFollows(queryType::RType t_type) {
  return t_type == queryType::RType::FOLLOWS;
}

bool Relation::isFollowsStar(queryType::RType t_type) {
  return t_type == queryType::RType::FOLLOWS_;
}

bool Relation::isParent(queryType::RType t_type) {
  return t_type == queryType::RType::PARENT;
}

bool Relation::isParentStar(queryType::RType t_type) {
  return t_type == queryType::RType::PARENT_;
}

bool Relation::isUses(queryType::RType t_type) {
  return t_type == queryType::RType::USES;
}

bool Relation::isModifies(queryType::RType t_type) {
  return t_type == queryType::RType::MODIFIES;
}

bool Relation::isCalls(queryType::RType t_type) {
  return t_type == queryType::RType::CALLS;
}

bool Relation::isCallsStar(queryType::RType t_type) {
  return t_type == queryType::RType::CALLS_;
}

bool Relation::isNext(queryType::RType t_type) {
  return t_type == queryType::RType::NEXT;
}

bool Relation::isNextStar(queryType::RType t_type) {
  return t_type == queryType::RType::NEXT_;
}

bool Relation::isAffects(queryType::RType t_type) {
  return t_type == queryType::RType::AFFECTS;
}

bool Relation::isAffectsStar(queryType::RType t_type) {
  return t_type == queryType::RType::AFFECTS_;
}