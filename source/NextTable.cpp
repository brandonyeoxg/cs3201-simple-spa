#include "NextTable.h"

NextTable::NextTable() {
  MAX_LINE_NUM = 0;
  m_afterGraph = std::unordered_map<PROG_LINE, std::vector<PROG_LINE>>();
  m_beforeGraph = std::unordered_map<PROG_LINE, std::vector<PROG_LINE>>();
}

void NextTable::insertNextRelationship(PROG_LINE t_line1, PROG_LINE t_line2) {
  if (MAX_LINE_NUM < t_line1) {
    MAX_LINE_NUM = t_line1 + 1; // offset needed as program lines are 1-based indices
  }
  if (MAX_LINE_NUM < t_line2) {
    MAX_LINE_NUM = t_line2 + 1;
  }

  // insert key if not present
  if (m_afterGraph.count(t_line1) == 0) {
    m_afterGraph.insert({ t_line1 , std::vector<PROG_LINE>() });
  }
  if (m_beforeGraph.count(t_line2) == 0) {
    m_beforeGraph.insert({ t_line2 , std::vector<PROG_LINE>() });
  }

  m_afterGraph.at(t_line1).push_back(t_line2);
  m_beforeGraph.at(t_line2).push_back(t_line1);
}

void NextTable::executeAfterAllNextInserts() {
  // initialize with max number of lines to easily set booleans at specified indices
  m_isNextTable = std::vector<std::vector<bool>>(MAX_LINE_NUM, std::vector<bool>(MAX_LINE_NUM, false));
  
  for (auto linePair : m_afterGraph) {
    for (auto lineAfter : linePair.second) {
      m_isNextTable.at(linePair.first).at(lineAfter) = true;
    }
  }
}

bool NextTable::isNext(PROG_LINE t_line1, PROG_LINE t_line2) {
  return m_isNextTable.at(t_line1).at(t_line2);
}

bool NextTable::isNextStar(PROG_LINE t_line1, PROG_LINE t_line2) {
  // no path from line1 or no path to line2, immediately false
  if (m_afterGraph.count(t_line1) == 0 || m_beforeGraph.count(t_line2) == 0) {
    return false;
  }

  if (m_isNextTable.at(t_line1).at(t_line2)) {
    return true;
  }

  return isTherePathFromLine1ToLine2(t_line1, t_line2);
}

// depth first search
bool NextTable::isTherePathFromLine1ToLine2(PROG_LINE t_line1, PROG_LINE t_line2) {
  std::vector<bool> visited = std::vector<bool>(MAX_LINE_NUM);
  std::vector<PROG_LINE> toVisit = std::vector<PROG_LINE>();

  for (auto nextLine : m_afterGraph.at(t_line1)) {
    toVisit.push_back(nextLine);
  }

  while (!toVisit.empty()) {
    PROG_LINE lineToVisit = toVisit.back(); // remove from stack
    toVisit.pop_back();
    visited.at(lineToVisit) = true;

    if (lineToVisit == t_line2) {
      return true;
    }

    // has Next() lines to visit
    if (m_afterGraph.count(lineToVisit) == 1) {
      for (auto nextLine : m_afterGraph.at(lineToVisit)) {
        if (!visited.at(nextLine)) { // line not visited yet
          toVisit.push_back(nextLine);
        }
      }
    }
  }

  return false;
}
