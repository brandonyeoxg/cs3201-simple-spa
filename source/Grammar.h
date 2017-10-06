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
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType PROC.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType PROC else return false.
  */
  static bool isProc(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType ST_LST.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType ST_LST else return false.
  */
  static bool isStmtLst(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType STMT.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType STMT else return false.
  */
  static bool isStmt(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType ASGN.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType ASGN else return false.
  */
  static bool isAssign(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType WHILE.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType WHILE else return false.
  */
  static bool isWhile(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType IF.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType IF else return false.
  */
  static bool isIf(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType CALL.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType CALL else return false.
  */
  static bool isCall(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType VAR.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType VAR else return false.
  */
  static bool isVar(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType CONST.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType CONST else return false.
  */
  static bool isConst(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType PROG_LINE.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType PROG_LINE else return false.
  */
  static bool isProgLine(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType STMT_NO.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType STMT_NO else return false.
  */
  static bool isStmtNo(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType STR.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType STR else return false.
  */
  static bool isString(queryType::GType t_type);

  /**
  * A public function to check the GType of the given GType.
  * It checks whether the given GType is of GType BOOLEAN.
  * @param t_type A GType to be checked.
  * @return true if the given GType is of GType BOOLEAN else return false.
  */
  static bool isBoolean(queryType::GType t_type);

  /**
  * A public function to check the AType of the given AType.
  * It checks whether the given AType is of AType PROC_NAME.
  * @param t_type A AType to be checked.
  * @return true if the given AType is of AType PROC_NAME else return false.
  */
  static bool isProcName(queryType::AType t_type);

  /**
  * A public function to check the AType of the given AType.
  * It checks whether the given AType is of AType VAR_NAME.
  * @param t_type A AType to be checked.
  * @return true if the given AType is of AType VAR_NAME else return false.
  */
  static bool isVarName(queryType::AType t_type);

  /**
  * A public function to check the AType of the given AType.
  * It checks whether the given AType is of AType STMT_NUM.
  * @param t_type A AType to be checked.
  * @return true if the given AType is of AType STMT_NUM else return false.
  */
  static bool isStmtNum(queryType::AType t_type);

  /**
  * A public function to check the AType of the given AType.
  * It checks whether the given AType is of AType VALUE.
  * @param t_type A AType to be checked.
  * @return true if the given AType is of AType VALUE else return false.
  */
  static bool isValue(queryType::AType t_type);

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