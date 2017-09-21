#pragma once
#include <unordered_set>
#include "nodes\ProcedureNode.h"

class ProcTableData {
public:
  ProcTableData(ProcedureNode* t_rootOfProcNode)
    : m_rootOfProcNode(t_rootOfProcNode) {};

  ProcedureNode* getProcNode();

  void insertModifiesVariable(std::string& t_modifiesVar);

  void insertUsesVariable(std::string& t_usesVar);

  bool isModifies(std::string& t_modifiesVar);

  std::list<std::string>& getModifiesVar();

  std::list<std::string>& getUsesVar();

  bool isUses(std::string& t_usesVar);

  void convertAnswersInSetToList();

private:
  ProcedureNode* m_rootOfProcNode;
  std::unordered_set<std::string> m_modifiesVariablesSet;
  std::unordered_set<std::string> m_usesVariablesSet;

  std::list<std::string> m_modifiesVariables;
  std::list<std::string> m_usesVariables;
};