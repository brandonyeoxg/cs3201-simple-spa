#include "NextTable.h"

NextTable::NextTable() {
  MAX_LINE_NUM = 0;
  m_afterGraph = std::unordered_map<PROG_LINE, std::vector<PROG_LINE>>();
  m_beforeGraph = std::unordered_map<PROG_LINE, std::vector<PROG_LINE>>();
}

void NextTable::insertNextRelationship(PROG_LINE t_line1, PROG_LINE t_line2) {
  if (MAX_LINE_NUM < t_line1 + 1) {
    MAX_LINE_NUM = t_line1 + 1; // offset needed as program lines are 1-based indices
  }
  if (MAX_LINE_NUM < t_line2 + 1) {
    MAX_LINE_NUM = t_line2 + 1;
  }

  // insert key if not present
  if (!isKeyInMap(t_line1, m_afterGraph)) {
    m_afterGraph.insert({ t_line1 , std::vector<PROG_LINE>() });
  }
  if (!isKeyInMap(t_line2, m_beforeGraph)) {
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
  if (!isKeyInMap(t_line1, m_afterGraph) || !isKeyInMap(t_line2, m_beforeGraph)) {
    return false;
  }

  // check Next() relationship first, faster termination if true
  if (m_isNextTable.at(t_line1).at(t_line2)) {
    return true;
  }

  return isTherePathFromLine1ToLine2(t_line1, t_line2);
}

std::vector<PROG_LINE> NextTable::getLinesAfter(PROG_LINE t_line) {
  if (isKeyInMap(t_line, m_afterGraph)) {
    return m_afterGraph.at(t_line);
  } else {
    return {};
  }
}

std::vector<PROG_LINE> NextTable::getLinesBefore(PROG_LINE t_line) {
  if (isKeyInMap(t_line, m_beforeGraph)) {
    return m_beforeGraph.at(t_line);
  } else {
    return {};
  }
}

std::vector<PROG_LINE> NextTable::getAllLinesAfter(PROG_LINE t_line) {
  return getListOfLinesReachableFromLineInGraph(t_line, m_afterGraph);
}

std::vector<PROG_LINE> NextTable::getAllLinesBefore(PROG_LINE t_line) {
  return getListOfLinesReachableFromLineInGraph(t_line, m_beforeGraph);
}

std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> NextTable::getAllNext() {
  return m_afterGraph;
}

std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> NextTable::getAllNextStar() {
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> map = std::unordered_map<PROG_LINE, std::vector<PROG_LINE>>();
  
  for (auto iterator : m_afterGraph) {
    map.insert({ iterator.first, getListOfLinesReachableFromLineInGraph(iterator.first, m_afterGraph) });
  }

  return map;
}

std::vector<PROG_LINE> NextTable::getAllLinesAfterAnyLine() {
  std::vector<PROG_LINE> list = std::vector<PROG_LINE>();
  std::vector<bool> visited = std::vector<bool>(MAX_LINE_NUM);

  for (auto iterator : m_afterGraph) {
    std::vector<PROG_LINE> listOfVisitedLines = getListOfLinesReachableFromLineInGraph(iterator.first, m_afterGraph);
    for (auto line : listOfVisitedLines) {
      visited.at(line) = true;
    }
  }

  for (int i = 0; i < (int)visited.size(); i++) {
    if (visited.at(i)) {
      list.push_back(i);
    }
  }

  return list;
}

std::vector<PROG_LINE> NextTable::getAllLinesBeforeAnyLine() {
  std::vector<PROG_LINE> list = std::vector<PROG_LINE>();
  std::vector<bool> visited = std::vector<bool>(MAX_LINE_NUM);

  for (auto iterator : m_beforeGraph) {
    std::vector<PROG_LINE> listOfVisitedLines = getListOfLinesReachableFromLineInGraph(iterator.first, m_beforeGraph);
    for (auto line : listOfVisitedLines) {
      visited.at(line) = true;
    }
  }

  for (int i = 0; i < (int)visited.size(); i++) {
    if (visited.at(i)) {
      list.push_back(i);
    }
  }

  return list;
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
    if (isKeyInMap(lineToVisit, m_afterGraph)) {
      for (auto nextLine : m_afterGraph.at(lineToVisit)) {
        if (!visited.at(nextLine)) { // line not visited yet
          toVisit.push_back(nextLine);
        }
      }
    }
  }

  return false;
}

std::vector<PROG_LINE> NextTable::getListOfLinesReachableFromLineInGraph(PROG_LINE t_line, 
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> t_graph) {

  if (!isKeyInMap(t_line, t_graph)) {
    return {};
  }

  std::vector<PROG_LINE> list = std::vector<PROG_LINE>();
  std::vector<bool> visited = std::vector<bool>(MAX_LINE_NUM);
  std::vector<PROG_LINE> toVisit = std::vector<PROG_LINE>();

  for (auto nextLine : t_graph.at(t_line)) {
    toVisit.push_back(nextLine);
  }

  while (!toVisit.empty()) {
    PROG_LINE lineToVisit = toVisit.back(); // remove from stack
    toVisit.pop_back();
    visited.at(lineToVisit) = true;

    // has Next() lines to visit
    if (isKeyInMap(lineToVisit, t_graph)) {
      for (auto nextLine : t_graph.at(lineToVisit)) {
        if (!visited.at(nextLine)) { // line not visited yet
          toVisit.push_back(nextLine);
        }
      }
    }
  }

  for (int i = 0; i < (int) visited.size(); i++) {
    if (visited.at(i)) {
      list.push_back(i);
    }
  }

  return list;
}
