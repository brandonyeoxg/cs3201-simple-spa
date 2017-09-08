#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>

/**
*  A class representing the multiple types of grammar used in SIMPLE. It is used to store the name of the given grammar and the type of grammar specified.
*  @author Ryan Tan
*  @date 26/08/2017
*/
class Grammar {
public:
  /** Represents the type of Grammar.
  *  An enum type representing the type of the component of the SIMPLE Grammar.
  */
  enum class GType {
    PROC, /**< An enum value representing a Process. */
    ST_LST, /**< An enum value representing a Statement List. */
    STMT, /**< An enum value representing a Statement. */
    ASGN, /**< An enum value representing an Assign Statement. */
    WHILE, /**< An enum value representing a While Statement. */
    IF, /**< An enum value representing an If Else Statement. */
    EXPR, /**< An enum value representing an Expression. */
    VAR, /**< An enum value representing a Variable. */
    CONST, /**< An enum value representing an Integer Constant. */
    STR /**< An enum value representing a String. */
  };

  /**
  * Default Constructor
  */
  Grammar();

  /**
  * A constructor.
  * Constructs a new Grammar object with a GType corresponding to the specified initial capacity and a string with the specified t_name.
  * @param t_type is an integer argument which corresponds to a specific GType.
  * @param t_name is a string argument that is the Query's name for the Grammar.
  */
  Grammar(int t_type, std::string t_name);

  /**
  * A Getter that returns the type of this Grammar object.
  * The returned GType is an enum type that identifies the Grammar as either a Statement or a Variable, and is able to further specify a type of Statement or Variable.
  * @return The type of this Grammar object.
  */
  GType getType();

  /**
  * A Getter that returns the name of this Grammar object.
  * The returned string is the name of the variable as specified by the query.
  * @return The name of this Grammar object.
  */
  std::string getName();

private:
  GType m_type; /**< type of this Grammar object */
  std::string m_name; /**< name of this Grammar object */
};

#endif