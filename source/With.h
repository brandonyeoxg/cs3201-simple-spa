#pragma once
#ifndef WITH_H
#define WITH_H

#include "Grammar.h"
#include <string>
#include "GlobalTypeDef.h"

/**A class representing the "with" clause of a query.
*/
class With
{
public:
  /**
  * Default constructor.
  */
  With();

  /**
  * A constructor.
  * Constructs a new With object with two Grammar objects between which there is a Relation.
  * @param t_g1 is the first Grammar object of the Design Abstraction.
  * @param t_g2 is the second Grammar object of the Design Abstraction.
  */
  With(Grammar t_g1, Grammar t_g2);

  /**
  * A Getter that returns the first Grammar object of the Design Abstraction.
  * @return The first Grammar object of the Design Abstraction.
  */
  Grammar getG1();

  /**
  * A Getter that returns the second Grammar object of the Design Abstraction
  * @return The second Grammar object of the Design Abstraction.
  */
  Grammar getG2();

private:
  Grammar m_g1; /**< First Grammar object useed by the Design Abstraction */
  Grammar m_g2; /**< Second Grammar object useed by the Design Abstraction */
};

#endif