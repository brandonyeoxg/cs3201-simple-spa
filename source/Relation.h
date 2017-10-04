#pragma once
#ifndef RELATION_H
#define RELATION_H

#include "Grammar.h"
#include <string>
#include "GlobalTypeDef.h"

/**A class representing the "Such That" clause of a query.
*  @author Ryan Tan
*  @date 26/08/2017
*/
class Relation
{
public:
  /**
  * Default constructor.
  */
  Relation();

  /**
  * A constructor.
  * Constructs a new Relation object with a RType corresponding to the specified string and two Grammar objects between which there is a Relation.
  * @param t_type is a string representing the Design Abstraction of the query to be converted to an RType.
  * @param t_g1 is the first Grammar object of the Dersign Abstraction. The returned Grammar object is the first one used by the Design Abstraction. e.g. Follows(m_g1, 2)
  * @param t_g2 is the second Grammar object of the Dersign Abstraction. The returned Grammar object is the second one used by the Design Abstraction. e.g. Follows(1, m_g2)
  */
  Relation(std::string t_type, Grammar t_g1, Grammar t_g2);

  /**
  * A Getter that returns the type of this Relation object.
  * The returned RType is an enum type that identifies the Relation as one of the Design Abstractions.
  * @return The RType of this query.
  */
  queryType::RType getType();

  /**
  * A Getter that returns the first Grammar object of the Dersign Abstraction.
  * @return The first Grammar object of the Dersign Abstraction. The returned Grammar object is the first one used by the Design Abstraction. e.g. Follows(m_g1, 2)
  */
  Grammar getG1();

  /**
  * A Getter that returns the second Grammar object of the Dersign Abstraction
  * @return The second Grammar object of the Dersign Abstraction. The returned Grammar object is the second one used by the Design Abstraction. e.g. Follows(1, m_g2)
  */
  Grammar getG2();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType FOLLOWS.
  * @return true if the relation object is of RType FOLLOWS else return false.
  */
  bool isFollows();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType FOLLOWS_.
  * @return true if the relation object is of RType FOLLOWS_ else return false.
  */
  bool isFollowsStar();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType PARENT.
  * @return true if the relation object is of RType PARENT else return false.
  */
  bool isParent();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType PARENT_.
  * @return true if the relation object is of RType PARENT_ else return false.
  */
  bool isParentStar();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType USES.
  * @return true if the relation object is of RType USES else return false.
  */
  bool isUses();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType MODIFIES.
  * @return true if the relation object is of RType MODIFIES else return false.
  */
  bool isModifies();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType CALLS.
  * @return true if the relation object is of RType CALLS else return false.
  */
  bool isCalls();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType CALLS_.
  * @return true if the relation object is of RType CALLS_ else return false.
  */
  bool isCallsStar();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType NEXT.
  * @return true if the relation object is of RType NEXT else return false.
  */
  bool isNext();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType NEXT_.
  * @return true if the relation object is of RType NEXT_ else return false.
  */
  bool isNextStar();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType AFFECTS.
  * @return true if the relation object is of RType AFFECTS else return false.
  */
  bool isAffects();

  /**
  * A public function to check the RType of the relation object.
  * It checks whether the relation object is of RType AFFECTS_.
  * @return true if the relation object is of RType AFFECTS_ else return false.
  */
  bool isAffectsStar();

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