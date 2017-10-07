#pragma once
#include <stack>
#include <fstream>
#include <sstream>
#include <iostream>

#include "PkbWriteOnly.h"
#include "SyntaxErrorException.h"
#include "GlobalTypeDef.h"

/**
* Represents a parser. 
* Parses the SIMPLE program and builds an ast through the PKB API.
*
* @author Brandon
* @date 8/26/2017
*
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
      m_isParsingProcedureContent(false),
      m_curProcIdx(-1){};

  ~Parser() {};
  
  /**
  * Parses the file through the filename.
  * 
  * @param t_filename filename of the file to be passed. Must be a valid readable file.
  * @return -1 if the file cannot be read or syntax error.
  */
  int parse(const std::string &t_filename) throw(); //! < returns 0 if no issue, -1 if there is a problem.
protected:
  PkbWriteOnly* m_pkbWriteOnly;
  int m_curLineNum;
  STRING_TOKEN m_nextToken;
  std::ifstream m_readStream;
  /*
  * Matches the token from the file with the expected token.
  *
  * @param t_token the expected token.
  * @return true if the token matches.
  */
  bool isMatchToken(const STRING_TOKEN& t_token);

  /*
  * Matches the tokenType from the file with the expected tokenType.
  *
  * @param t_token the expected tokenType.
  * @return true if the token matches.
  */
  bool isMatchToken(tokentype::tokenType t_type);

  /*
  * Matches the token from the file with the expected token type.
  *
  * @param t_token the expected token type.
  * @return the string of that token from the type.
  */
  STRING_TOKEN getMatchToken(const tokentype::tokenType& t_token);

  /*
  * Returns the first token in a line
  */
  STRING_TOKEN getCurrentLineToken();

  /*
  * Returns true if the token is an operator.
  *
  * @param t_token the token to be checked.
  */
  bool isOperator(const STRING_TOKEN& t_token);

  /*
  * Returns true if the token is a brace.
  *
  * @param t_token the token to be checked.
  */
  bool isBrace(const STRING_TOKEN& t_token);

  /*
  * Returns true if the token is any key delimiter like a space or a brace or operator.
  *
  * @param t_token the token to be checked.
  */
  bool isKeyDelimiter(const STRING_TOKEN& t_token);

  /*
  * Tokenises the line into tokens
  *
  * @param t_line the line to be tokenised
  */
  LIST_OF_TOKENS tokeniseLine(const STRING_TOKEN& t_line);

  /*
  * Returns true if the token is a valid name.
  * A valid name refers to LETTER(LETTER|DIGIT)+.
  */
  bool isValidName(const STRING_TOKEN& t_token);

  /*
  * Returns true if the token is a constant.
  * A constant just consists of purely digits.
  */
  bool isConstant(const STRING_TOKEN& t_token);

  /*
  * Returns true if the the statement is a non container statement.
  * Checks with m_nextToken if it is an non container statement string.
  */
  bool isNonContainerStmt(const STRING_TOKEN& t_token);

private:
  LIST_OF_STMT_NUMS m_nestedStmtLineNum;
  LIST_OF_TOKENS m_curTokens;
  bool m_isParsingProcedureContent;
  const std::string EMPTY_LINE = "";
  PROC_INDEX m_curProcIdx;

  /*
  * Parses the procedure block.
  * 
  * @return -1 if there is syntax error.
  */
  void parseForProcedure();
  
  /*
  * Parses the statement list block.
  * 
  * @param t_node the reference to the procedure node
  * @return -1 if there is syntax error.
  */
  void parseStmtLst(LIST_OF_STMT_NUMS& t_stmtInStmtLst);

  /*
  * Parses the statement.
  *
  * @param t_node the reference to the stmtLst node
  * @return -1 if there is syntax error.
  */
  void parseStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst);

  /*
  * Parses the assignment statement.
  *
  * @param t_node the reference to the stmtLst node
  * @return -1 if there is syntax error.
  */
  void parseAssignStmt();

  /*
  * Parses the call statement.
  */
  void parseCallStmt();

  /*
  * Tokenise the expr to the right. 
  * Tokenised into a vector, without spaces, each element belongs to a single term or operator.
  *
  */
  std::vector<std::string> tokeniseExpr();

  /*
  * Parses a non container statemment.
  *
  * @param t_node the reference to the stmtLst node
  */
  void parseNonContainerStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst);

  /*
   * Parses a container statement.
   *
   * @param t_node the reference to the stmtLst node
   */
  void parseContainerStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst);

  /*
  * Parses the while statement.
  *
  * @param t_node the reference to the stmtLst node
  * @return -1 if there is syntax error.
  */
  void parseWhileStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst);

  /*
  * Parses for the if and else statement.
  *
  * @param t_node the reference to the stmtLst node
  * @return -1 if there is syntax error.
  */
  void parseIfElseStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst);

  /*
  * Parses the if statement.
  *
  * @param t_node the reference to the stmtLst node
  * @return -1 if there is syntax error.
  */
  void parseIfStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst);

  /*
  * Parses the else statement.
  *
  * @param t_node the reference to the stmtLst node
  * @return -1 if there is syntax error.
  */
  void parseElseStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst);

  /*
  * Returns the the next token in the line
  */
  STRING_TOKEN getToken();
};
