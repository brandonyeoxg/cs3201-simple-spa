#include "Grammar.h"
#include <stdio.h>
#include <string>

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
      m_type = GType::PROC;
    } else if (t_type == STLST) {
      m_type = GType::ST_LST;
    } else if (t_type == STMT) {
      m_type = GType::STMT;
    } else if (t_type == ASGN) {
      m_type = GType::ASGN;
    } else if (t_type == WHILE) {
      m_type = GType::WHILE;
    } else if (t_type == IF) {
      m_type = GType::IF;
    } else if (t_type == CALL) {
      m_type = GType::CALL;
    } else if (t_type == VAR) {
      m_type = GType::VAR;
    } else if (t_type == CONST) {
      m_type = GType::CONST;
    } else if (t_type == PROGLN) {
      m_type = GType::PROG_LINE;
    } else if (t_type == STMTNUM) {
      m_type = GType::STMT_NO;
    } else if (t_type == STR) {
      m_type = GType::STR;
    }
    m_name = t_name;
  }

  /**
   * A Getter that returns the type of this Grammar object
   * The returned GType is an enum type that identifies the Grammar as either a Statement or a Variable, and is able to further specify a type of Statement or Variable.
   * @return The type of this Grammar object
   */
  Grammar::GType Grammar::getType() { return m_type; }

  /**
  * A Getter that returns the name of this Grammar object
  * The returned string is the name of the variable as specified by the query.
  * @return The name of this Grammar object
  */
  std::string Grammar::getName() { return m_name; }

