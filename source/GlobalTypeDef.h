#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

const bool isDebugMode = false;

const int INVALID_INDEX = -1;

const std::string OPERATOR_MULTIPLY = "*";
const std::string OPERATOR_MINUS = "-";
const std::string OPERATOR_PLUS = "+";
const std::string OPERATOR_UNDERSCORE = "_";
const std::string OPERATOR_EQUAL = "=";

const std::string BRACKET_OPEN = "(";
const std::string BRACKET_CLOSE = ")";

const std::string EMPTY_LINE = "";

const std::string TRUE = "true";
const std::string FALSE = "false";

/*Data Types*/
typedef bool BOOLEAN;
typedef int INTEGER;
typedef std::string STRING;
typedef char STRING_CHARACTER;

/*Indexes*/
typedef INTEGER VAR_INDEX;
typedef INTEGER PROC_INDEX;
typedef INTEGER STMT_NUM;
typedef INTEGER PROG_LINE;
typedef INTEGER CONSTANT;
typedef INTEGER SYNONYM_POSITION;
typedef INTEGER TOTAL_NUMBER_OF_STMTS;
typedef STRING NAME;
typedef STRING VAR_NAME;
typedef STRING PROC_NAME;
typedef STRING CONSTANT_TERM;
typedef STRING STRING_TOKEN;
typedef STRING SYNONYM_NAME;
typedef STRING RESULT;

/*Data Structs*/
typedef std::list<RESULT> RESULT_LIST;
typedef std::vector<SYNONYM_NAME> LIST_OF_SYNONYMS;
typedef std::vector<INTEGER> LIST_OF_INTEGERS;
typedef std::vector<STMT_NUM> LIST_OF_STMT_NUMS;
typedef std::vector<STMT_NUM>& MUTABLE_LIST_OF_STMT_NUMS;
typedef std::vector<PROG_LINE> LIST_OF_PROG_LINES;
typedef std::vector<PROG_LINE>& MUTABLE_LIST_OF_PROG_LINES;
typedef std::vector<VAR_NAME> LIST_OF_VAR_NAMES;
typedef std::vector<PROC_NAME> LIST_OF_PROC_NAMES;
typedef std::vector<RESULT> LIST_OF_RESULTS;
typedef std::vector<std::string> LIST_OF_SELECT_RESULTS;
typedef std::vector<STRING> LIST_OF_STRINGS;
typedef std::vector<STRING_TOKEN> LIST_OF_TOKENS;
typedef std::vector<STRING_TOKEN>& MUTABLE_LIST_OF_TOKENS;
typedef std::vector<PROG_LINE> LIST_OF_PROG_LINES;
typedef std::set<VAR_NAME> SET_OF_VAR_NAMES;
typedef std::set<STMT_NUM> SET_OF_STMT_NUMS;
typedef std::unordered_set<std::string> UNORDERED_SET_OF_NAMES;
typedef std::unordered_map<STMT_NUM, std::unordered_set<std::string>> MAP_OF_STMT_NUM_TO_SET_OF_NAMES;
typedef std::unordered_map<std::string, std::unordered_set<std::string>> MAP_OF_NAME_TO_SET_OF_NAMES;
typedef std::unordered_map<std::string, std::vector<std::string>> SET_OF_RESULTS;
typedef std::unordered_map<std::string, std::vector<std::string>> SET_OF_RELATION_RESULTS;
typedef std::unordered_map<std::string, std::vector<std::string>> SET_OF_PATTERN_RESULTS;
typedef std::unordered_map<VAR_INDEX, LIST_OF_STMT_NUMS> MAP_OF_VAR_INDEX_TO_STMT_NUMS;
typedef std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> MAP_OF_VAR_NAME_TO_STMT_NUMS;
typedef std::unordered_map<STMT_NUM, VAR_NAME> MAP_OF_STMT_NUM_TO_VAR_NAME;
typedef std::vector<std::vector<std::string>> INTERMEDIATE_TABLE;
typedef std::unordered_map<SYNONYM_NAME, SYNONYM_POSITION> MAP_OF_SYNONYM_TO_TABLE_POSITION;
typedef std::unordered_map<STMT_NUM, STMT_NUM> MAP_OF_STMT_NUMS;
typedef std::unordered_map<STMT_NUM, LIST_OF_STMT_NUMS> MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS;
typedef std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES;
typedef std::unordered_map<PROC_NAME, PROC_NAME> MAP_OF_PROC_NAMES;
typedef std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES;
typedef std::unordered_map<PROC_NAME, LIST_OF_STMT_NUMS> MAP_OF_PROC_NAME_TO_LIST_OF_STMT_NUMS;
typedef std::unordered_map<STMT_NUM, PROC_NAME> MAP_OF_STMT_NUM_TO_PROC_NAME;
typedef std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS;
typedef std::list<std::string> LIST_OF_CONSTANTS;
typedef std::list<STMT_NUM> LIST_OF_ASSIGN_STMTS_FOR_PATTERN;
typedef std::unordered_map<SYNONYM_NAME, int> MAP_OF_SYNONYMS_TO_COUNTS;
typedef std::unordered_map<SYNONYM_NAME, INTEGER> MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_INTEGERS;
typedef std::unordered_map<SYNONYM_NAME, STRING> MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_STRING;
typedef std::unordered_map<SYNONYM_NAME, SYNONYM_NAME> MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_SYNONYMS;
typedef std::unordered_map<SYNONYM_NAME, LIST_OF_INTEGERS> MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_INTEGERS;
typedef std::unordered_map<SYNONYM_NAME, LIST_OF_STRINGS> MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_STRINGS;
typedef std::unordered_map<STMT_NUM, STMT_NUM> SET_OF_AFFECTS;
typedef std::vector<STMT_NUM> LIST_OF_AFFECTS_STMTS;
typedef std::pair<MAP_OF_STMT_NUMS, MAP_OF_STMT_NUMS> PAIR_OF_AFFECTS_LIST;

typedef std::multimap<PROC_NAME, VAR_NAME> MAP_OF_PROC_TO_VAR;
typedef std::unordered_set<int> VAR_HASH_SET;
typedef std::vector < std::vector<BOOLEAN>> BOOLEAN_MATRIX;

/*Called in QueryPreProcessor.h, by aaron*/

/*Called in Parser.h, by brandon*/
enum TOKEN_TYPE {
  PROC_NAME_TYPE,
  VAR_NAME_TYPE,
  CONSTANT_TYPE,
  EXPR_TYPE,
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
    STMT_NUM, /**< An enum value representing a stmt# attribute. */
    VALUE, /**< An enum value representing a value attribute. */
    NONE /**< An enum value representing no attributes. */
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
};

typedef queryType::GType SYNONYM_TYPE;

typedef std::unordered_map<STMT_NUM, queryType::GType> MAP_OF_STMT_NUM_TO_GTYPE;
typedef std::unordered_map<queryType::GType, LIST_OF_STMT_NUMS> MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS;
