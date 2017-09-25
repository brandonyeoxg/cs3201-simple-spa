#pragma once
#include "Parser.h"

class ParserDriver : public Parser
{
public:
  ParserDriver::ParserDriver(PkbWriteOnly* t_pkb) : Parser(t_pkb) {}
  ~ParserDriver();

  int openFileStream(std::string);
  int parseProcedure();

};

