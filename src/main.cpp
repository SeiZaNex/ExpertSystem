//
//	Expert System Implementation
//

#include	<list>
#include	<string>
#include	<iostream>
#include	<algorithm>

#include	"Engine.hh"
#include	"Parser.hh"
#include	"System.hh"

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

void		solveQuery(std::list<esie::Values *> &rules, std::string &query)
{
  esie::Values				*start = _fetchRule(rules, query);
  std::list<esie::AObject *>		list = start->getDep();
  std::list<esie::AObject *>::iterator	it;
  esie::val				flag = esie::UNKNOWN;

  if (NULL != start)
    {
      std::cout << "Query is: [" << start->getName() << "]" << std::endl;
      if (!list.empty())
	{
	  for (it = list.begin(); it != list.end() && esie::UNKNOWN == flag; ++it)
	    flag = recDep(*it);
	  std::cout << "Query: [" << start->getName() << "] is [";
	  if (esie::TRUE == flag)
	    std::cout << "true";
	  else if (esie::FALSE == flag)
	    std::cout << "false";
	  else
	    std::cout << "unknown";
	  std::cout << "]" << std::endl;
	}
      else
	std::cerr << "Query has no dependencies" << std::endl;
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
