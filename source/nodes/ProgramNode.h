#pragma once

#include "TNode.h"

class ProgramNode : public TNode {
public:
  const std::string DEFAULT_PROGRAM_NAME = "main program";
  ProgramNode();
  std::string getName();

private:
  std::string m_progName;
};