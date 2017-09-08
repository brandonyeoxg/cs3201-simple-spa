#include "Grammar.h"
#include <stdio.h>
#include <string>

  Grammar::Grammar() {}

  /**
   * A constructor.
   * Constructs a new Grammar object with a GType corresponding to the specified initial capacity and a string with the specified t_name.
   * @param t_type is an integer argument which corresponds to a specific GType
   * @param t_name is a string argument that is the Query's name for the Grammar.
   */
  Grammar::Grammar(int t_type, std::string t_name) {
    switch (t_type) {
    case 0:
      m_type = GType::PROC;
      break;
    case 1:
      m_type = GType::ST_LST;
      break;
    case 2:
      m_type = GType::STMT;
      break;
    case 3:
      m_type = GType::ASGN;
      break;
    case 4:
      m_type = GType::WHILE;
      break;
    case 5:
      m_type = GType::IF;
      break;
    case 6:
      m_type = GType::EXPR;
      break;
    case 7:
      m_type = GType::VAR;
      break;
    case 8:
      m_type = GType::CONST;
      break;
    case 9:
      m_type = GType::STR;
      break;
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

