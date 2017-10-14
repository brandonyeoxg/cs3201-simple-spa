#include "ParserDriver.h"
#include "..\StringUtil.h"
#include "..\nodes\TNode.h"
#include "..\SyntaxOpenBraceException.h"
#include "..\SyntaxNoEndOfStatementException.h"
#include "..\SyntaxUnknownCommandException.h"
#include "..\SyntaxEmptyLineException.h"

ParserDriver::~ParserDriver()
{
}

int ParserDriver::openFileStream(std::string t_filename) {
  m_readStream = std::ifstream(t_filename);
  if (!m_readStream.is_open()) {
    return -1;
  }
  m_nextToken = getCurrentLineToken();
  return 0;
}

int ParserDriver::parseProcedure() {
  if (isMatchToken("procedure")) {
    std::string procName = getMatchToken(tokentype::tokenType::PROC_NAME);
    if (!isMatchToken("{")) {
      throw SyntaxOpenBraceException(m_curLineNum);
    }
    m_curProcIdx = m_pkbWriteOnly->insertProcedure(procName);
    return 1;
  }
  return -1;
}

int ParserDriver::parseStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst) {
  //Parser::parseStmt(t_stmtInStmtLst);
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