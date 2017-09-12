#pragma once
#ifndef RELATION_H
#define RELATION_H

#include "Grammar.h"
#include <string>
/**
*  A class representing the "Pattern" clause of a query.
* @author Ryan Tan
* @date 26/08/2017
*/
class Relation
{
public:
  /** Represents the type of Relation.
  *  An enum type representing the Relation in the query.
  */
  enum RType {
    FOLLOWS, /**< An enum value representing the Follows relation. */
    FOLLOWS_, /**< An enum value representing the Follows* relation. */
    PARENT, /**< An enum value representing the Parent relation. */
    PARENT_, /**< An enum value representing the Parent* relation. */
    USES, /**< An enum value representing the Uses relation. */
    MODIFIES /**< An enum value representing the Modifies relation. */
  };

  /**
  * Default constructor.
  */
  Relation();

  /**
  * A constructor.
  * Constructs a new Design Abstraction object with a DAType corresponding to the specified initial capacity and a string with the specified t_name.
  */
  Relation(std::string t_type, Grammar t_g1, Grammar t_g2);

  /**
  * A Getter that returns the type of this Grammar object
  * Returns the type of this Grammar object. The returned GType is an enum type that identifies the Grammar as either a Statement or a Variable, and is able to further specify a type of Statement or Variable.
  */
  RType getType();

  /**
  * A Getter that returns the first Grammar object of the Dersign Abstraction.
  * Returns the first Grammar object of the Dersign Abstraction. The returned Grammar object is the first one used by the Design Abstraction. e.g. Follows(m_g1, 2)
  */
  Grammar getG1();

  /**
  * A Getter that returns the second Grammar object of the Dersign Abstraction
  * Returns the second Grammar object of the Dersign Abstraction. The returned Grammar object is the second one used by the Design Abstraction. e.g. Follows(1, m_g2)
  */
  Grammar getG2();

private:
  RType m_type; /**< Represents the type of this Design Abstraction object */
  Grammar m_g1; /**< First Grammar object useed by the Design Abstraction */
  Grammar m_g2; /**< Second Grammar object useed by the Design Abstraction */

};

#endif