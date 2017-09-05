#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>

/**
*  A class representing the multiple types of grammar used in SIMPLE. Used to store the name of the given grammar and the type of grammar specified.
*/
class Grammar
{
public:
  /** Represents the type of Grammar.
  *  An enum type representing the type of the component of the SIMPLE Grammar.
  */
  enum class GType { PROC, ST_LST, STMT, ASGN, WHILE, IF, EXPR, VAR, CONST };
  Grammar();
  Grammar(int t_type, std::string t_name);
  GType getType();
  std::string getName();

private:
  GType m_type; /**< type of this Grammar object */
  std::string m_name; /**< name of this Grammar object */
};

#endif