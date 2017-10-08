#include "BracketValidator.h"
#include <assert.h>

void BracketValidator::insertBracketToken(const STRING_TOKEN& t_brace) {
  assert(t_brace == OPEN_BRACE || t_brace == CLOSE_BRACE);

  if (m_braces.empty()) {
    m_braces.push(t_brace);
    return;
  }
  if (m_braces.top() == OPEN_BRACE) {
    if (t_brace == CLOSE_BRACE) {
      m_braces.pop();
      return;
    }
  }
  m_braces.push(t_brace);
}

bool BracketValidator::isWellFormed() {
  return m_braces.empty();
}

const std::stack<STRING_TOKEN>& BracketValidator::getBraceStack() {
  return m_braces;
}