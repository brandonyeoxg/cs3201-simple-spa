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
    StmtListNode* stmtLst = m_pkbWriteOnly->insertProcedure(procName);
    return 1;
  }
  return -1;
}

int ParserDriver::parseStmt(std::list<STMT_NUM>& t_stmtInStmtLst) {
  if (isMatchToken(EMPTY_LINE)) {
    return 1;
  }
  m_curLineNum += 1;
  m_pkbWriteOnly->insertFollowsRelation(t_stmtInStmtLst, m_curLineNum);
  return 1;
}