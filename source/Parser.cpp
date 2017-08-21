#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

#include "Parser.h"
#include "PKB.h"
#include "TNode.h"

int Parser::parse () {
  // Open file
  std::string stmtLine;
  ifstream readStream(m_filename);
  if (readStream.is_open()) {
    while (getline(readStream, stmtLine)) {
      std::cout << stmtLine << "\n";
    }
  }
	return 0;
}