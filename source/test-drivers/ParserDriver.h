#pragma once
#include "../Parser.h"

class ParserDriver : public Parser
{
public:
  ParserDriver::ParserDriver(PkbWriteOnly* t_pkb) : Parser(t_pkb) {}
  ~ParserDriver();

  int openFileStream(std::string);
  int parseProcedure();
  int parseStmt(std::list<STMT_NUM>&);

  /*
  * Returns true if the token is an operator.
  *
  * @param t_token the token to be checked.
  */
  bool testIsOperator(const std::string& t_token);

  /*
  * Returns true if the token is a brace.
  *
  * @param t_token the token to be checked.
  */
  bool testIsBrace(const std::string& t_token);

  /*
  * Returns true if the token is any key delimiter like a space or a brace or operator.
  *
  * @param t_token the token to be checked.
  */
  bool testIsKeyDelimiter(const std::string& t_token);

  /*
  * Tokenises the line into tokens
  *
  * @param t_line the line to be tokenised
  */
  std::vector<std::string> testTokeniseLine(const std::string& t_line);

  /*
  * Returns true if the token is a valid name.
  * A valid name refers to LETTER(LETTER|DIGIT)+.
  */
  bool testIsValidName(std::string& t_token);

  /*
  * Returns true if the token is a constant.
  * A constant just consists of purely digits.
  */
  bool testIsConstant(std::string& t_token);

  /*
  * Returns true if the the statement is a non container statement.
  * Checks with m_nextToken if it is an non container statement string.
  */
  bool testIsNonContainerStmt(std::string t_token);

private:
  const std::string EMPTY_LINE = "";
};

