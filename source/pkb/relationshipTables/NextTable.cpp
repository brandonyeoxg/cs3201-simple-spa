#include "NextTable.h"

NextTable::NextTable() {
  MAX_LINE_NUM = 0;
  m_afterGraph = std::map<PROG_LINE, std::vector<PROG_LINE>>();
  m_beforeGraph = std::map<PROG_LINE, std::vector<PROG_LINE>>();
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

  // sort each vector mapped to each program line
  for (auto linePair : m_afterGraph) {
    std::sort(linePair.second.begin(), linePair.second.end());
  }

  for (auto linePair : m_beforeGraph) {
    std::sort(linePair.second.begin(), linePair.second.end());
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

  return isTherePath(t_line1, t_line2);
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
  return getListOfLinesReachable(t_line, m_afterGraph);
}

std::vector<PROG_LINE> NextTable::getAllLinesBefore(PROG_LINE t_line) {
  return getListOfLinesReachable(t_line, m_beforeGraph);
}

std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> NextTable::getAllNext() {
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> map;
  map.insert(m_afterGraph.begin(), m_afterGraph.end());
  return map;
}

std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> NextTable::getAllNextStar() {
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> map = std::unordered_map<PROG_LINE, std::vector<PROG_LINE>>();
  
  isCaching = true;
  m_cacheVisited = std::unordered_map<PROG_LINE, std::vector<PROG_LINE>>();

  for (auto reverseIter = m_afterGraph.rbegin(); reverseIter != m_afterGraph.rend(); reverseIter++) {

    PROG_LINE progLine = reverseIter->first;
    std::vector<PROG_LINE> list;

    //std::cout << "Checking: " << progLine << "\n";

    list = getListOfLinesReachable(progLine, m_afterGraph);
    
    if (!isKeyInMap(progLine, m_cacheVisited)) {
      //std::cout << "Caching\n";
      m_cacheVisited.insert({ progLine, list });
    }

    map.insert({ progLine , list });
  }

  isCaching = false;
  m_cacheVisited.clear();

  return map;
}

std::vector<PROG_LINE> NextTable::getAllLinesAfterAnyLine() {
  std::vector<PROG_LINE> list = std::vector<PROG_LINE>();

  for (auto iter: m_beforeGraph) {
    list.push_back(iter.first);
  }

  return list;
}

std::vector<PROG_LINE> NextTable::getAllLinesBeforeAnyLine() {
  std::vector<PROG_LINE> list = std::vector<PROG_LINE>();

  for (auto iter : m_afterGraph) {
    list.push_back(iter.first);
  }

  return list;
}

bool NextTable::hasNextRelationship() {
  return m_afterGraph.size() > 0;
}

bool NextTable::hasNextLine(PROG_LINE t_line) {
  return isKeyInMap(t_line, m_afterGraph) && m_afterGraph.at(t_line).size() != 0;
}

bool NextTable::hasLineBefore(PROG_LINE t_line) {
  return isKeyInMap(t_line, m_beforeGraph) && m_beforeGraph.at(t_line).size() != 0;
}

const std::map<PROG_LINE, std::vector<PROG_LINE>>* NextTable::getAfterGraph() {
  return &m_afterGraph;
}

// depth first search
bool NextTable::isTherePath(PROG_LINE t_line1, PROG_LINE t_line2) {
  std::vector<bool> visited = std::vector<bool>(MAX_LINE_NUM);
  std::vector<PROG_LINE> toVisit = std::vector<PROG_LINE>();

  for (auto nextLine : m_afterGraph.at(t_line1)) {
    toVisit.push_back(nextLine);
  }

  while (!toVisit.empty()) {
    PROG_LINE lineToVisit = toVisit.back(); // remove from stack
    toVisit.pop_back();
    visited.at(lineToVisit) = true;

    if (lineToVisit == t_line2) { // return true once path is found
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

std::vector<PROG_LINE> NextTable::getListOfLinesReachable(PROG_LINE t_line, 
  std::map<PROG_LINE, std::vector<PROG_LINE>> t_graph) {
  std::vector<PROG_LINE> linesVisited = std::vector<PROG_LINE>();
  std::vector<bool> visited = std::vector<bool>(MAX_LINE_NUM);

  if (isKeyInMap(t_line, t_graph)) {
    for (auto lineToVisit : t_graph.at(t_line)) {
      std::vector<PROG_LINE> result = traverseGraphDfs(lineToVisit, t_graph, visited);
      linesVisited.insert(linesVisited.end(), result.begin(), result.end());
    }
  }

  return linesVisited;
}

std::vector<PROG_LINE> NextTable::traverseGraphDfs(PROG_LINE t_line, std::map<PROG_LINE, 
  std::vector<PROG_LINE>> t_graph, std::vector<bool>& visited) {
  if (visited.at(t_line)) {
    return{};
  }

  visited.at(t_line) = true;

  std::vector<PROG_LINE> linesVisited = std::vector<PROG_LINE>();
  linesVisited.push_back(t_line);

  if (isKeyInMap(t_line, t_graph)) {
    for (auto lineToVisit : t_graph.at(t_line)) {
      if (!visited.at(lineToVisit)) {
        std::vector<PROG_LINE> result;

        if (isCaching && isKeyInMap(t_line, m_cacheVisited)) {
          //std::cout << "Taking from cache\n";
          std::vector<PROG_LINE> cached = m_cacheVisited.at(t_line);
          for (auto line : cached) {
            if (!visited.at(line)) {
              result.push_back(line);
              visited.at(line) = true;
            }
          }
        } else {
          result = traverseGraphDfs(lineToVisit, t_graph, visited);
        }

        linesVisited.insert(linesVisited.end(), result.begin(), result.end());
      }
    }
  }

  return linesVisited;
}