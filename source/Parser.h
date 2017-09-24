#pragma once
#include <stack>
#include <fstream>
#include <sstream>
#include <iostream>

#include "PKB.h"
#include "ASTBuilder.h"
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
  Parser(PKB *t_pkb) 
    : m_pkb(t_pkb), 
      m_curLineNum(0),
      m_nextToken(""),
      m_isParsingProcedureContent(false) {};

  ~Parser() {};
  
  /**
  * Parses the file through the filename.
  * 
  * @param t_filename filename of the file to be passed. Must be a valid readable file.
  * @return -1 if the file cannot be read or syntax error.
  */
  int parse(const std::string &t_filename) throw(); //! < returns 0 if no issue, -1 if there is a problem.

private:
  PKB *m_pkb;
  std::string m_nextToken;
  std::stack<std::string> m_bracesStack;
  std::list<STMT_NUM> m_nestedStmtLineNum;
  std::vector<std::string> m_curTokens;
  std::ifstream m_readStream;
  int m_curLineNum;
  bool m_isParsingProcedureContent;
  const std::string EMPTY_LINE = "";

  /*
  * Parses the procedure block.
  * 
  * @return -1 if there is syntax error.
  */
  int parseForProcedure();
  
  /*
  * Parses the statement list block.
  * 
  * @param t_node the reference to the procedure node
  * @return -1 if there is syntax error.
  */
  int parseStmtLst(StmtListNode* t_node);

  /*
  * Parses the statement.
  *
  * @param t_node the reference to the stmtLst node
  * @return -1 if there is syntax error.
  */
  int parseStmt(TNode* t_node);

  /*
  * Parses the assignment statement.
  *
  * @param t_node the reference to the stmtLst node
  * @return -1 if there is syntax error.
  */
  int parseAssignStmt(TNode* t_node);

  /*
  * Parses the expr stmt.
  *
  * @param t_expr the string representation of the expression
  * @return the completed expr's root node
  */
  TNode* parseExpr();

  /*
  * Parses the each operands to populate the respective tables in the PKB.
  *
  * @param t_expr the string representation of the expression
  */
  void parseEachOperand(std::stack<TNode *>& t_exprStack);

  /*
  * Parses a non container statemment.
  *
  * @param t_node the reference to the stmtLst node
  */
  int parseNonContainerStmt(TNode* t_node);

  /*
   * Parses a container statement.
   *
   * @param t_node the reference to the stmtLst node
   */
  int parseContainerStmt(TNode* t_node);

  /*
  * Parses the while statement.
  *
  * @param t_node the reference to the stmtLst node
  * @return -1 if there is syntax error.
  */
  int parseWhileStmt(TNode* t_node);

  /*
  * Matches the token from the file with the expected token.
  * 
  * @param t_token the expected token.
  * @return true if the token matches.
  */
  bool isMatchToken(const std::string& t_token);

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
  std::string getMatchToken(const tokentype::tokenType& t_token);

  /*
  * Returns true if the token is an operator.
  * 
  * @param t_token the token to be checked.
  */
  bool isOperator(const std::string& t_token);

  /*
  * Returns true if the token is a brace.
  *
  * @param t_token the token to be checked.
  */
  bool isBrace(const std::string& t_token);

  /*
  * Returns true if the token is any key delimiter like a space or a brace or operator.
  *
  * @param t_token the token to be checked.
  */
  bool isKeyDelimiter(const std::string& t_token);

  /*
  * Returns the first token in a line 
  */
  std::string getCurrentLineToken();

  /*
  * Returns the the next token in the line
  */
  std::string getToken();

  /*
  * Tokenises the line into tokens 
  * 
  * @param t_line the line to be tokenised
  */
  std::vector<std::string> tokeniseLine(const std::string& t_line);

  /*
  * Returns true if the token is a valid name.
  * A valid name refers to LETTER(LETTER|DIGIT)+.
  */
  bool isValidName(std::string& t_token);

  /*
  * Returns true if the token is a constant.
  * A constant just consists of purely digits.
  */
  bool isConstant(std::string& t_token);

  /*
  * Returns true if the the statement is a non container statement.
  * Checks with m_nextToken if it is an non container statement string.
  */
  bool isNonContainerStmt();
};
