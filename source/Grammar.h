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

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType PROC.
  * @return true if the grammar object is of GType PROC else return false.
  */
  bool isProc();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType ST_LST.
  * @return true if the grammar object is of GType ST_LST else return false.
  */
  bool isStmtLst();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType STMT.
  * @return true if the grammar object is of GType STMT else return false.
  */
  bool isStmt();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType ASGN.
  * @return true if the grammar object is of GType ASGN else return false.
  */
  bool isAssign();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType WHILE.
  * @return true if the grammar object is of GType WHILE else return false.
  */
  bool isWhile();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType IF.
  * @return true if the grammar object is of GType IF else return false.
  */
  bool isIf();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType CALL.
  * @return true if the grammar object is of GType CALL else return false.
  */
  bool isCall();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType VAR.
  * @return true if the grammar object is of GType VAR else return false.
  */
  bool isVar();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType CONST.
  * @return true if the grammar object is of GType CONST else return false.
  */
  bool isConst();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType PROG_LINE.
  * @return true if the grammar object is of GType PROG_LINE else return false.
  */
  bool isProgLine();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType STMT_NO.
  * @return true if the grammar object is of GType STMT_NO else return false.
  */
  bool isStmtNo();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType STR.
  * @return true if the grammar object is of GType STR else return false.
  */
  bool isString();

  /**
  * A public function to check the GType of the grammar object.
  * It checks whether the grammar object is of GType BOOLEAN.
  * @return true if the grammar object is of GType BOOLEAN else return false.
  */
  bool isBoolean();

  /**
  * A public function to check the AType of the grammar object.
  * It checks whether the grammar object is of AType PROC_NAME.
  * @return true if the grammar object is of AType PROC_NAME else return false.
  */
  bool isProcName();

  /**
  * A public function to check the AType of the grammar object.
  * It checks whether the grammar object is of AType VAR_NAME.
  * @return true if the grammar object is of AType VAR_NAME else return false.
  */
  bool isVarName();

  /**
  * A public function to check the AType of the grammar object.
  * It checks whether the grammar object is of AType STMT_NUM.
  * @return true if the grammar object is of AType STMT_NUM else return false.
  */
  bool isStmtNum();

  /**
  * A public function to check the AType of the grammar object.
  * It checks whether the grammar object is of AType VALUE.
  * @return true if the grammar object is of AType VALUE else return false.
  */
  bool isValue();

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
  static int BOOLEAN;
};

#endif