#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

class Grammar
{
public:
  enum class GType { PROC, ST_LST, STMT, ASGN, WHILE, EXPR, VAR, CONST };
  Grammar();
  Grammar(int t_type, string t_name);
  GType getType() { return m_type; }
  string getMonth() { return m_name; }

private:
  GType m_type; /**< type of this Grammar object */
  string m_name; /**< name of this Grammar object */
};

#endif