#pragma once
#include <stack>
#include <fstream>
#include <sstream>
#include <iostream>

#include "PkbWriteOnly.h"
#include "SyntaxErrorException.h"
#include "GlobalTypeDef.h"

/*
* Represents a parser which parses the SIMPLE program and builds an ast through the PKB API.
* The Parser will throw exceptions when there are problems with the SIMPLE program.
*
* @author Brandon
* @date 8/10/2017
*/
class Parser {
public:
  /*
  * Constructs parser using pkb.
  * Also initialises other fields  
  */
  Parser(PkbWriteOnly *t_pkbWriteOnly)
    : m_pkbWriteOnly(t_pkbWriteOnly), 
      m_curLineNum(0),
      m_nextToken(""),
      m_curProcIdx(-1) {};

  ~Parser() {};
  
  /**
  * Parses the file through the filename.
  * 
  * @param t_filename filename of the file to be passed. Must be a valid readable file.
  * @return -1 if the file cannot be read or syntax error.
  */
  int parse(NAME t_filename); //! < returns 0 if no issue, -1 if there is a problem.
protected:
  PkbWriteOnly* m_pkbWriteOnly;
  int m_curLineNum;
  STRING_TOKEN m_nextToken;
  std::ifstream m_readStream;
  PROC_INDEX m_curProcIdx;

  /*
  * Matches the token from the file with the expected token.
  *
  * @param t_token the expected token.
  * @return true if the token matches.
  */
  BOOLEAN isMatchToken(STRING_TOKEN t_token);

  /*
  * Matches the tokenType from the file with the expected tokenType.
  *
  * @param t_token the expected TOKEN_TYPE.
  * @return true if the token matches.
  */
  BOOLEAN isMatchToken(TOKEN_TYPE t_type);

  /*
  * Returns the token is it matches with the TOKEN_TYPE.
  *
  * @param t_token the expected token type.
  */
  STRING_TOKEN getMatchToken(TOKEN_TYPE t_token);

  /*
  * Returns the first token of a text line
  */
  STRING_TOKEN getCurrentLineToken();

  /*
  * Returns true if the token is a valid name.
  * A valid name refers to LETTER(LETTER|DIGIT)+.
  */
  BOOLEAN isValidName(STRING_TOKEN t_token);

  /*
  * Returns true if the token is a constant.
  * A constant just consists of purely digits.
  */
  BOOLEAN isConstant(STRING_TOKEN t_token);

  /*
  * Returns true if the the statement is a non container statement.
  * Checks with m_nextToken if it is an non container statement string.
  */
  BOOLEAN isNonContainerStmt(STRING_TOKEN t_token);

  /*
  * Parses the statement.
  *
  * @param t_node the reference to the stmtLst node
  * @return -1 if there is syntax error.
  */
  void parseStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, MUTABLE_LIST_OF_PROG_LINES t_progLine);

private:
  LIST_OF_STMT_NUMS m_nestedStmtLineNum;
  LIST_OF_TOKENS m_curTokens;
  LIST_OF_PROG_LINES m_ifElseNextList;
  std::unordered_set<STMT_NUM> m_ifLookUp;

  /*
  * Parses the procedure block.
  */
  void parseForProcedure();
  
  /*
  * Parses the statement list block.
  * 
  * @param t_stmtInStmtLst the statements before this statment in the statement list.
  * @param t_progLine the program lines that have been found to cross over the statement list.
  */
  void parseStmtLst(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, MUTABLE_LIST_OF_PROG_LINES t_progLine);

  /*
  * Parses the assignment statement.
  */
  void parseAssignStmt();

  /*
  * Parses the call statement.
  */
  void parseCallStmt();

  /*
  * Returns a tokenised expr belonging to the right side of an assignment statement.
  * Tokenised into a vector, without spaces, each element belongs to a single term or operator.
  * Does validating of brackets being properly formed.
  */
  LIST_OF_TOKENS parseExpr();

  /*
  * Parses each term and tokenises them to be used.
  * Used by LIST_OF_TOKENS parseExpr().
  *
  * @param t_tokens is the list of tokens to be appended in to
  */
  void parseEachTerm(MUTABLE_LIST_OF_TOKENS t_tokens);

  /*
  * Parses the expressions inside the brackets.
  * 
  * @param t_tokens is the list of tokens to be appended in to
  */
  void parseBrackets(MUTABLE_LIST_OF_TOKENS t_tokens);

  /*
  * Parses a non container statemment.
  *
  * @param t_stmtInStmtLst the statements before this statment in the statement list.
  * @param t_progLine the program lines that have been found to cross over the statement list.
  */
  void parseNonContainerStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst);

  /*
   * Parses a container statement.
   *
   * @param t_stmtInStmtLst the statements before this statment in the statement list.
   * @param t_progLine the program lines that have been found to cross over the statement list.
   */
  void parseContainerStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, MUTABLE_LIST_OF_PROG_LINES t_progLine);

  /*
  * Parses the while statement.
  *
  * @param t_stmtInStmtLst the statements before this statment in the statement list.
  * @param t_progLine the program lines that have been found to cross over the statement list.
  */
  void parseWhileStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, MUTABLE_LIST_OF_PROG_LINES t_progLines);

  /*
  * Parses for the if and else statement.
  *
  * @param t_stmtInStmtLst the statements before this statment in the statement list.
  * @param t_progLine the program lines that have been found to cross over the statement list.
  */
  void parseIfElseStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, MUTABLE_LIST_OF_PROG_LINES t_progLine);

  /*
  * Parses the if statement.
  *
  * @param t_stmtInStmtLst the statements before this statment in the statement list.
  * @param t_ifStmtNum statement number belonging to the start of the if statement.
  * @param t_progLine the program lines that have been found to cross over the statement list.
  */
  void parseIfStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, STMT_NUM t_ifStmtNum, MUTABLE_LIST_OF_PROG_LINES t_progLine);

  /*
  * Parses the else statement.
  *
  * @param t_stmtInStmtLst the statements before this statment in the statement list.
  * @param t_ifStmtNum statement number belonging to the start of the if statement.
  * @param t_progLine the program lines that have been found to cross over the statement list.
  */
  void parseElseStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, STMT_NUM t_ifStmtNum, MUTABLE_LIST_OF_PROG_LINES t_progLine);

  /*
  * Returns the the next token in the line
  */
  STRING_TOKEN getToken();

  /*
  * Handles the insertion api call to pkb based on the token.
  * If the token is constant, a constant is inserted into PKB and if the token is 
  * a validName a Uses relation is inserted into PKB.
  *
  * @param t_term the term that is in the assignment expression
  */
  void handleInsertionOfTermByPkb(STRING_TOKEN t_term);
};
