#include "Grammar.h"
#include <stdio.h>

  Grammar::Grammar() {}

  int Grammar::PROC = 0;
  int Grammar::STLST = 1;
  int Grammar::STMT = 2;
  int Grammar::ASGN = 3;
  int Grammar::WHILE = 4;
  int Grammar::IF = 5;
  int Grammar::CALL = 6;
  int Grammar::VAR = 7;
  int Grammar::CONST = 8;
  int Grammar::PROGLN = 9;
  int Grammar::STMTNUM = 10;
  int Grammar::STR = 11;

  /**
   * A constructor.
   * Constructs a new Grammar object with a GType corresponding to the specified initial capacity and a string with the specified t_name.
   * @param t_type is an integer argument which corresponds to a specific GType
   * @param t_name is a string argument that is the Query's name for the Grammar.
   */
  Grammar::Grammar(int t_type, std::string t_name) {
    if (t_type == PROC) {
      m_type = queryType::GType::PROC;
    } else if (t_type == STLST) {
      m_type = queryType::GType::ST_LST;
    } else if (t_type == STMT) {
      m_type = queryType::GType::STMT;
    } else if (t_type == ASGN) {
      m_type = queryType::GType::ASGN;
    } else if (t_type == WHILE) {
      m_type = queryType::GType::WHILE;
    } else if (t_type == IF) {
      m_type = queryType::GType::IF;
    } else if (t_type == CALL) {
      m_type = queryType::GType::CALL;
    } else if (t_type == VAR) {
      m_type = queryType::GType::VAR;
    } else if (t_type == CONST) {
      m_type = queryType::GType::CONST;
    } else if (t_type == PROGLN) {
      m_type = queryType::GType::PROG_LINE;
    } else if (t_type == STMTNUM) {
      m_type = queryType::GType::STMT_NO;
    } else if (t_type == STR) {
      m_type = queryType::GType::STR;
    }
    m_name = t_name;
  }

  /**
   * A Getter that returns the type of this Grammar object
   * The returned GType is an enum type that identifies the Grammar as either a Statement or a Variable, and is able to further specify a type of Statement or Variable.
   * @return The type of this Grammar object
   */
  queryType::GType Grammar::getType() { return m_type; }

  /**
  * A Getter that returns the type of this Grammar object
  * The returned GType is an enum type that identifies the Grammar as either a Statement or a Variable, and is able to further specify a type of Statement or Variable.
  * @return The type of this Grammar object
  */
  queryType::AType Grammar::getAttr() { return m_attr; }

  /**
  * A Getter that returns the name of this Grammar object
  * The returned string is the name of the variable as specified by the query.
  * @return The name of this Grammar object
  */
  std::string Grammar::getName() { return m_name; }

  /**
  * A Getter that returns the value of this Grammar object
  * The returned string is the value of the variable as specified by the query.
  * @return The value of this Grammar object
  */
  std::string Grammar::getValue() { return m_value; }