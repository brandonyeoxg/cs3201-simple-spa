#pragma once

#include <vector>
#include <unordered_map>

typedef int VAR_INDEX;
typedef int PROC_INDEX;
typedef int STMT_NUM;
typedef std::string VAR_NAME;
typedef int CONSTANT;
typedef std::vector<int> LIST_OF_STMT_NUMS;
typedef std::vector<std::string> LIST_OF_VAR_NAMES;
typedef std::vector<std::string> LIST_OF_RESULTS;
typedef std::unordered_map<std::string, std::vector<std::string>> SET_OF_RESULTS;

/*Called in QueryPreProcessor.h, by aaron*/
const int m_procedure = 0;
const int m_statementList = 1;
const int m_statement = 2;
const int m_assign = 3;
const int m_while = 4;
const int m_if = 5;
const int m_call = 6;
const int m_variable = 7;
const int m_constant = 8;
const int m_progline = 9;
const int m_statementNumber = 10;
const int m_string = 11;

const int INVALID_INDEX = -1;

/*Called in Parser.h, by brandon*/
namespace tokentype {
  enum tokenType {
    PROC_NAME,
    VAR_NAME,
    CONSTANT,
    EXPR,
  };

};

/*Called in ProcTable.h, by brandon*/
const PROC_INDEX INVALID_PROC_NO = -1;

namespace queryType {
  /**
  * Represents a clauseType.
  * An enum type representing the clauseType.
  * Called in QueryEvaluator.h, by verbena.
  */
  enum clauseType {
    SELECT, /**< enum value SELECT. */
    RELATION, /**< enum value RELATION. */
    PATTERN, /**< enum value PATTERN. */
    WITH /**< enum value WITH. */
  };

  /** Represents the type of Grammar.
  *  An enum type representing the type of the component of the SIMPLE Grammar.
  *  Called in Grammar.h, by Ryan.
  */
  enum GType {
    PROC, /**< An enum value representing a Procedure. */
    ST_LST, /**< An enum value representing a Statement List. */
    STMT, /**< An enum value representing a Statement. */
    ASGN, /**< An enum value representing an Assign Statement. */
    WHILE, /**< An enum value representing a While Statement. */
    IF, /**< An enum value representing an If Else Statement. */
    CALL, /**< An enum value representing an Expression (or operator, "+","-","*"). */
    VAR, /**< An enum value representing a Variable. */
    CONST, /**< An enum value representing an Integer Constant. */
    PROG_LINE, /**< An enum value representing a Program Line. */
    STMT_NO, /**< An enum value representing a Statement Number. */
    STR /**< An enum value representing a String. */
  };

  /** Represents the type of Attribute.
  *  An enum type representing the type of the attribute of the Grammar.
  *  Called in Grammar.h, by Ryan.
  */
  enum AType {
    PROC_NAME, /**< An enum value representing a procName attribute. */
    VAR_NAME, /**< An enum value representing a varName attribute. */
    STMT_NUM, /**< An enum value representing a stmt# or prog_line# attribute. */
    VALUE /**< An enum value representing a value attribute. */
  };

  /** Represents the type of Relation.
  *  An enum type representing the Relation in the query.
  *  Called in Relation.h, by Ryan.
  */
  enum RType {
    FOLLOWS, /**< An enum value representing the Follows relation. */
    FOLLOWS_, /**< An enum value representing the Follows* relation. */
    PARENT, /**< An enum value representing the Parent relation. */
    PARENT_, /**< An enum value representing the Parent* relation. */
    USES, /**< An enum value representing the Uses relation. */
    MODIFIES, /**< An enum value representing the Modifies relation. */
    CALLS, /**< An enum value representing the Calls relation. */
    CALLS_, /**< An enum value representing the Calls* relation. */
    NEXT, /**< An enum value representing the Next relation. */
    NEXT_, /**< An enum value representing the Next* relation. */
    AFFECTS, /**< An enum value representing the Affects relation. */
    AFFECTS_ /**< An enum value representing the Affects* relation. */
  };
}
