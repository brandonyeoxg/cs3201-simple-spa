#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include "GlobalTypeDef.h"

/**A class representing the multiple types of grammar used in SIMPLE. It is used to store the name of the given grammar and the type of grammar specified.
*  @author Ryan Tan
*  @date 26/08/2017
*/
class Grammar {
public:
  /**
  * Default Constructor
  */
  Grammar();

  /**
  * A constructor.
  * Constructs a new Grammar object with a GType corresponding to the specified integer and a string with the specified t_name.
  * @param t_type is an integer argument which corresponds to a specific GType.
  * @param t_name is a string argument that is the Query's name for the Grammar.
  */
  Grammar(int t_type, std::string t_name);

  /**
  * A Getter that returns the type of this Grammar object.
  * The returned GType is an enum type that identifies the Grammar as either a Statement or a Variable, and is able to further specify a type of Statement or Variable.
  * @return The type of this Grammar object.
  */
  queryType::GType getType();

  /**
  * A Getter that returns the attribute of this Grammar object.
  * The returned AType is an enum type that identifies the Grammar as either a Statement or a Variable, and is able to further specify a type of Statement or Variable.
  * @return The attribute of this Grammar object.
  */
  queryType::AType getAttr();

  /**
  * A Getter that returns the name of this Grammar object.
  * The returned string is the name of the variable as specified by the query.
  * @return The name of this Grammar object.
  */
  std::string getName();

  /**
  * A Getter that returns the value of this Grammar object.
  * The returned string is the value of the variable as specified by the query.
  * @return The value of this Grammar object.
  */
  std::string getValue();

private:
  queryType::GType m_type; /**< type of this Grammar object */
  queryType::AType m_attr; /**< attribute of this Grammar object */
  std::string m_name; /**< name of this Grammar object */
  std::string m_value; /**< value of this Grammar object */
  static int PROC;
  static int STLST;
  static int STMT;
  static int ASGN;
  static int WHILE;
  static int IF;
  static int CALL;
  static int VAR;
  static int CONST;
  static int PROGLN;
  static int STMTNUM ;
  static int STR;
};

#endif