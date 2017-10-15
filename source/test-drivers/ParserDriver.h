#pragma once
#include "../Parser.h"

class ParserDriver : public Parser
{
public:
  ParserDriver::ParserDriver(PkbWriteOnly* t_pkb) : Parser(t_pkb) {}
  ~ParserDriver();

  int openFileStream(std::string);
  int parseProcedure();
  int parseStmt(LIST_OF_STMT_NUMS&, LIST_OF_STMT_NUMS&);

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

