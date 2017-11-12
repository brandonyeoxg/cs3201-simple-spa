#include "ParserDriver.h"
#include "../util/StringUtil.h"
#include "../exception/SyntaxOpenBraceException.h"
#include "../exception/SyntaxNoEndOfStatementException.h"
#include "../exception/SyntaxUnknownCommandException.h"
#include "../exception/SyntaxEmptyLineException.h"

ParserDriver::~ParserDriver()
{
}

int ParserDriver::openFileStream(NAME t_filename) {
  m_readStream = std::ifstream(t_filename);
  if (!m_readStream.is_open()) {
    return -1;
  }
  m_nextToken = getCurrentLineToken();
  return 0;
}

int ParserDriver::parseProcedure() {
  if (isMatchToken("procedure")) {
   PROC_NAME procName = getMatchToken(TOKEN_TYPE::PROC_NAME_TYPE);
    if (!isMatchToken("{")) {
      throw SyntaxOpenBraceException(m_curLineNum);
    }
    m_curProcIdx = m_pkbWriteOnly->insertProcedure(procName);
    return 1;
  }
  return -1;
}

int ParserDriver::parseStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst, LIST_OF_STMT_NUMS& t_progLines) {
  Parser::parseStmt(t_stmtInStmtLst, t_progLines);
  return 1;
}

bool ParserDriver::testIsValidName(std::string& t_token) {
  return isValidName(t_token);
}

bool ParserDriver::testIsConstant(std::string& t_token) {
  return isConstant(t_token);
}

bool ParserDriver::testIsNonContainerStmt(std::string t_token) {
  return isNonContainerStmt(t_token);
}