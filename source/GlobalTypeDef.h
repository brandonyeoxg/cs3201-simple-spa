#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

/*Indexes*/
typedef int VAR_INDEX;
typedef int PROC_INDEX;
typedef int STMT_NUM;
typedef int PROG_LINE;
typedef int CONSTANT;
typedef int SYNONYM_POSITION;
typedef std::string VAR_NAME;
typedef std::string PROC_NAME;
typedef std::string CONSTANT_TERM;
typedef std::string STRING_TOKEN;
typedef std::string SYNONYM_NAME;

/*Data Structs*/
typedef std::vector<std::string> LIST_OF_SYNONYMS;
typedef std::vector<int> LIST_OF_STMT_NUMS;
typedef std::vector<std::string> LIST_OF_VAR_NAMES;
typedef std::vector<std::string> LIST_OF_PROC_NAMES;
typedef std::vector<std::string> LIST_OF_RESULTS;
typedef std::vector<std::string> LIST_OF_TOKENS;
typedef std::set<std::string> SET_OF_VAR_NAMES;
typedef std::set<int> SET_OF_STMT_NUMS;
typedef std::unordered_map<std::string, std::vector<std::string>> SET_OF_RESULTS;
typedef std::unordered_map<int, std::vector<int>> MAP_OF_VAR_INDEX_TO_STMT_NUMS;
typedef std::unordered_map<std::string, std::vector<int>> MAP_OF_VAR_NAME_TO_STMT_NUMS;
typedef std::unordered_map<STMT_NUM, VAR_NAME> MAP_OF_STMT_NUM_TO_VAR_NAME;
typedef std::vector<std::vector<std::string>> INTERMEDIATE_TABLE;
typedef std::unordered_map<SYNONYM_NAME, SYNONYM_POSITION> MAP_OF_SYNONYM_TO_TABLE_POSITION;

typedef std::multimap<PROC_NAME, VAR_NAME> MAP_OF_PROC_TO_VAR;
typedef std::unordered_set<int> VAR_HASH_SET;

const int INVALID_INDEX = -1;

const std::string OPERATOR_MULTIPLY = "*";
const std::string OPERATOR_MINUS = "-";
const std::string OPERATOR_PLUS = "+";
const std::string EMPTY_LINE = "";

/*Called in QueryPreProcessor.h, by aaron*/

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
    SELECT, /**< An enum value SELECT. */
    RELATION, /**< An enum value RELATION. */
    PATTERN, /**< An enum value PATTERN. */
    WITH /**< An enum value WITH. */
  };

  /** Represents the type of Grammar.
  *  An enum type representing the type of the component of the SIMPLE Grammar.
  *  Called in Grammar.h, by Ryan.
  */
  enum GType {
    PROC = 0, /**< An enum value representing a Procedure. */
    ST_LST = 1, /**< An enum value representing a Statement List. */
    STMT = 2, /**< An enum value representing a Statement. */
    ASGN = 3, /**< An enum value representing an Assign Statement. */
    WHILE = 4, /**< An enum value representing a While Statement. */
    IF = 5, /**< An enum value representing an If Else Statement. */
    CALL = 6, /**< An enum value representing an Expression (or operator, "+","-","*"). */
    VAR = 7, /**< An enum value representing a Variable. */
    CONST = 8, /**< An enum value representing an Integer Constant. */
    PROG_LINE = 9, /**< An enum value representing a Program Line. */
    STMT_NO = 10, /**< An enum value representing a Statement Number. */
    STR = 11, /**< An enum value representing a String. */
    BOOLEAN = 12 /**< An enum value representing a Boolean. */
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
