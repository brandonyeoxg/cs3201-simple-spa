#pragma once
#ifndef RELATION_H
#define RELATION_H

#include <string>

#include "GlobalTypeDef.h"
#include "Grammar.h"
#include "Clause.h"

/**A class representing the "Such That" clause of a query.
*  @author Ryan Tan
*  @date 26/08/2017
*/
class Relation : public Clause {
public:
  /**
  * Default constructor.
  */
  Relation();

  /**
  * A constructor.
  * Constructs a new Relation object with a RType corresponding to the specified string and two Grammar objects between which there is a Relation.
  * @param t_type is a string representing the Design Abstraction of the query to be converted to an RType.
  * @param t_g1 is the first Grammar object of the Design Abstraction. The returned Grammar object is the first one used by the Design Abstraction. e.g. Follows(m_g1, 2)
  * @param t_g2 is the second Grammar object of the Design Abstraction. The returned Grammar object is the second one used by the Design Abstraction. e.g. Follows(1, m_g2)
  */
  Relation(std::string t_type, Grammar t_g1, Grammar t_g2);

  queryType::clauseType getClauseType();

  /**
  * A Getter that returns the type of this Relation object.
  * The returned RType is an enum type that identifies the Relation as one of the Design Abstractions.
  * @return The RType of this query.
  */
  queryType::RType getType();

  /**
  * A Getter that returns the type of this Relation object in string.
  * The returned string identifies the Relation as one of the Design Abstractions.
  * @return The RType of this query in string.
  */
  STRING getTypeInString();

  /**
  * A Getter that returns the first Grammar object of the Design Abstraction.
  * @return The first Grammar object of the Design Abstraction. The returned Grammar object is the first one used by the Design Abstraction. e.g. Follows(m_g1, 2)
  */
  Grammar getG1();

  /**
  * A Getter that returns the second Grammar object of the Design Abstraction
  * @return The second Grammar object of the Design Abstraction. The returned Grammar object is the second one used by the Design Abstraction. e.g. Follows(1, m_g2)
  */
  Grammar getG2();

  /**
  * A Setter that sets the first Grammar object of the Design Abstraction.
  */
  void setG1(Grammar t_newG1);

  /**
  * A Setter that sets the second Grammar object of the Design Abstraction
  */
  void setG2(Grammar t_newG2);

  /**
  * A public function that prints the content of this Relation object.
  */
  void toString();

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType FOLLOWS.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType FOLLOWS else return false.
  */
  static bool isFollows(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType FOLLOWS_.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType FOLLOWS_ else return false.
  */
  static bool isFollowsStar(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType PARENT.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType PARENT else return false.
  */
  static bool isParent(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType PARENT_.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType PARENT_ else return false.
  */
  static bool isParentStar(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType USES.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType USES else return false.
  */
  static bool isUses(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType MODIFIES.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType MODIFIES else return false.
  */
  static bool isModifies(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType CALLS.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType CALLS else return false.
  */
  static bool isCalls(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType CALLS_.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType CALLS_ else return false.
  */
  static bool isCallsStar(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType NEXT.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType NEXT else return false.
  */
  static bool isNext(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType NEXT_.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType NEXT_ else return false.
  */
  static bool isNextStar(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType AFFECTS.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType AFFECTS else return false.
  */
  static bool isAffects(queryType::RType t_type);

  /**
  * A public function to check the RType of the given RType.
  * It checks whether the given RType is of RType AFFECTS_.
  * @param t_type A RType to be checked.
  * @return true if the given RType is of RType AFFECTS_ else return false.
  */
  static bool isAffectsStar(queryType::RType t_type);

private:
  queryType::RType m_type; /**< Represents the type of this Design Abstraction object */
  Grammar m_g1; /**< First Grammar object useed by the Design Abstraction */
  Grammar m_g2; /**< Second Grammar object useed by the Design Abstraction */
  static std::string FLS;
  static std::string FLS_;
  static std::string PRT;
  static std::string PRT_;
  static std::string USE;
  static std::string MDF;
  static std::string CLS;
  static std::string CLS_;
  static std::string NXT;
  static std::string NXT_;
  static std::string AFS;
  static std::string AFS_;

};

#endif