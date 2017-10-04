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
int Grammar::BOOLEAN = 12;

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
  } else if (t_type == BOOLEAN) {
    m_type = queryType::GType::BOOLEAN;
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

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType PROC.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType PROC else return false.
*/
bool Grammar::isProc(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::PROC;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType ST_LST.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType ST_LST else return false.
*/
bool Grammar::isStmtLst(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::ST_LST;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType STMT.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType STMT else return false.
*/
bool Grammar::isStmt(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::STMT;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType ASGN.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType ASGN else return false.
*/
bool Grammar::isAssign(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::ASGN;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType WHILE.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType WHILE else return false.
*/
bool Grammar::isWhile(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::WHILE;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType IF.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType IF else return false.
*/
bool Grammar::isIf(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::IF;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType CALL.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType CALL else return false.
*/
bool Grammar::isCall(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::CALL;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType VAR.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType VAR else return false.
*/
bool Grammar::isVar(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::VAR;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType CONST.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType CONST else return false.
*/
bool Grammar::isConst(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::CONST;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType PROG_LINE.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType PROG_LINE else return false.
*/
bool Grammar::isProgLine(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::PROG_LINE;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType STMT_NO.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType STMT_NO else return false.
*/
bool Grammar::isStmtNo(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::STMT_NO;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType STR.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType STR else return false.
*/
bool Grammar::isString(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::STR;
}

/**
* A public function to check the GType of the grammar object.
* It checks whether the given grammar object is of GType BOOLEAN.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of GType BOOLEAN else return false.
*/
bool Grammar::isBoolean(Grammar t_grammar) {
  return t_grammar.getType() == queryType::GType::BOOLEAN;
}

/**
* A public function to check the AType of the grammar object.
* It checks whether the given grammar object is of AType PROC_NAME.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of AType PROC_NAME else return false.
*/
bool Grammar::isProcName(Grammar t_grammar) {
  return t_grammar.getAttr() == queryType::AType::PROC_NAME;
}

/**
* A public function to check the AType of the grammar object.
* It checks whether the given grammar object is of AType VAR_NAME.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of AType VAR_NAME else return false.
*/
bool Grammar::isVarName(Grammar t_grammar) {
  return t_grammar.getAttr() == queryType::AType::VAR_NAME;
}

/**
* A public function to check the AType of the grammar object.
* It checks whether the given grammar object is of AType STMT_NUM.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of AType STMT_NUM else return false.
*/
bool Grammar::isStmtNum(Grammar t_grammar) {
  return t_grammar.getAttr() == queryType::AType::STMT_NUM;
}

/**
* A public function to check the AType of the grammar object.
* It checks whether the given grammar object is of AType VALUE.
* @param t_grammar The grammar object to be checked.
* @return true if the grammar object is of AType VALUE else return false.
*/
bool Grammar::isValue(Grammar t_grammar) {
  return t_grammar.getAttr() == queryType::AType::VALUE;
}