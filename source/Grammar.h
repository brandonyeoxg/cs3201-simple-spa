#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>

using namespace std;

/**
*  A class representing the multiple types of grammar used in SIMPLE. Used to store the name of the given grammar and the type of grammar specified.
*/
class Grammar {
public:
  /** Represents the type of Grammar.
  *  An enum type representing the type of the component of the SIMPLE Grammar.
  */
  enum class GType { PROC, ST_LST, STMT, ASGN, WHILE, IF, EXPR, VAR, CONST };

  Grammar();

  /**
  * A constructor.
  * Constructs a new Grammar object with a GType corresponding to the specified initial capacity and a string with the specified t_name.
  * @param t_type is an integer argument which corresponds to a specific GType
  * @param t_name is a string argument that is the Query's name for the Grammar.
  */
  Grammar(int t_type, string t_name);

  /**
  * A Getter that returns the type of this Grammar object
  * The returned GType is an enum type that identifies the Grammar as either a Statement or a Variable, and is able to further specify a type of Statement or Variable.
  * @return The type of this Grammar object
  */
  GType getType();

  /**
  * A Getter that returns the name of this Grammar object
  * The returned string is the name of the variable as specified by the query.
  * @return The name of this Grammar object
  */
  string getName();

private:
  GType m_type; /**< type of this Grammar object */
  string m_name; /**< name of this Grammar object */
};

#endif