//
//	Expert System Implementation
//

#include	<list>
#include	<stack>
#include	<string>
#include	<iostream>
#include	<algorithm>

#include	"Engine.hh"
#include	"Parser.hh"

inline bool	_isRule(std::list<esie::Values *> &rules, std::string &query)
{
  return std::any_of(rules.begin(), rules.end(), [query](esie::Values *i){ return (i->getName() == query); });
}

inline esie::Values	*_fetchRule(std::list<esie::Values *> &rules, std::string &query)
{
  std::list<esie::Values *>::iterator	it;
  esie::Values				*ptr;

  if (_isRule(rules, query))
    for (it = rules.begin(); it != rules.end(); ++it)
      {
	ptr = *it;
	if (ptr->getName() == query)
	  return ptr;
      }
  return NULL;
}

inline bool	onAll(std::list<esie::val> &l, esie::val x)
{
  return (std::all_of(l.begin(), l.end(), [x](esie::val y){ return x == y; }));
}

inline bool	onAny(std::list<esie::val> &l, esie::val x)
{
  return (std::any_of(l.begin(), l.end(), [x](esie::val y){ return x == y; }));
}

inline esie::val	valSet(esie::Values *&ptr, esie::val val)
{
  ptr->setVal(val);
  return val;
}

esie::val	flagVal(esie::Values *&val, std::list<esie::val> &flag)
{
  if (onAny(flag, esie::TRUE) && !onAny(flag, esie::FALSE))
    return valSet(val, esie::TRUE);
  else if (onAny(flag, esie::FALSE) && !onAny(flag, esie::TRUE))
    return valSet(val, esie::FALSE);
  else
    return esie::UNKNOWN; 
}

esie::val	flagRule(esie::Rules *rule, std::list<esie::val> &flag)
{
  switch (rule->getOper())
    {
    case esie::NOT:
      if (onAll(flag, esie::TRUE))
	return esie::FALSE;
      else if (onAll(flag, esie::FALSE))
	return esie::TRUE;
      break;
    case esie::AND:
      if (onAll(flag, esie::TRUE))
	return esie::TRUE;
      else if (onAll(flag, esie::FALSE))
	return esie::FALSE;
      break;
    case esie::OR:
      if (onAny(flag, esie::TRUE))
	return esie::TRUE;
      else if (onAll(flag, esie::UNKNOWN))
	return esie::UNKNOWN;
      return esie::FALSE;
      break;
    default:
      break;
    }
  return esie::UNKNOWN;
}

esie::val	flagOut(esie::AObject *ptr, std::list<esie::val> &flag)
{
  esie::Values	*vl = dynamic_cast<esie::Values *>(ptr);
  esie::Rules	*rl = dynamic_cast<esie::Rules *>(ptr);

  if (NULL != vl)
    return flagVal(vl, flag);
  else
    return flagRule(rl, flag);
}

esie::val	infrOut(esie::Values *&val)
{
  if (esie::UNKNOWN == val->getVal())
    {
      std::cout << "Solution is dependent of the atomic fact: [" << val->getName();
      std::cout << "]" << std::endl;
      std::cout << "But it has an unknown value." << std::endl;
    }
  return val->getVal();
}

esie::val	recDep(esie::AObject *ptr)
{
  std::list<esie::AObject *>		list = ptr->getDep();
  std::list<esie::AObject *>::iterator	it;
  std::list<esie::val>			flag;
  esie::val				rec;

  if (!list.empty())
    for (it = list.begin(); it != list.end(); ++it)
      {
	rec = recDep(*it);
	if (esie::UNKNOWN != rec)
	  flag.push_back(rec);
      }
  if (!flag.empty())
    return flagOut(ptr, flag);
  else
    {
      esie::Values	*vl = dynamic_cast<esie::Values *>(ptr);

      if (NULL != vl)
	return infrOut(vl);
    }
  std::cerr << "There has been an error within the tree" << std::endl;
  return esie::UNKNOWN;
}

void		solveQuery(std::list<esie::Values *> &rules, std::string &query)
{
  esie::Values	*start = _fetchRule(rules, query);
  esie::val	flag;

  if (NULL != start)
    {
      std::cout << "Query is: [" << start->getName() << "]" << std::endl;
      flag = recDep(start);
      std::cout << "Query: [" << start->getName() << "] is [";
      if (esie::TRUE == flag)
	std::cout << "true";
      else if (esie::FALSE == flag)
	std::cout << "false";
      else
	std::cout << "error";
      std::cout << "]" << std::endl;
    }
  else
    std::cerr << "Query is non existant within the rules." << std::endl;
  return;
}

int		main(int ac, char *av[])
{
  std::list<esie::Values *>	rules;
  std::string		query;

  if (2 <= ac)
    {
      esie::Parser	pa(av[1]);
      pa.parse();

      rules = pa.getFacts();
      query = pa.getQuery();
      solveQuery(rules, query);
    }
  return 0;
}
