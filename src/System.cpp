#include	<list>
#include	<string>
#include	<iostream>
#include	<algorithm>

#include	"Engine.hh"
#include	"Parser.hh"
#include	"System.hh"

static bool	flagAnd = false;
static bool	flagOr = false;

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
  if (onAll(flag, esie::TRUE))
    return valSet(val, esie::TRUE);
  else if (onAll(flag, esie::FALSE))
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
      else if (onAny(flag, esie::FALSE))
	return esie::FALSE;
      break;
    case esie::OR:
      if (onAny(flag, esie::TRUE))
	return esie::TRUE;
      else if (onAll(flag, esie::FALSE))
	return esie::FALSE;
      break;
    default:
      break;
    }
  return esie::UNKNOWN;
}

esie::val	flagOut(esie::AObject *ptr, std::list<esie::val> &flag)
{
  std::list<esie::AObject *>::iterator	it;
  std::list<esie::AObject *>		ls;
  esie::Values				*vl = dynamic_cast<esie::Values *>(ptr);
  esie::Rules				*rl = dynamic_cast<esie::Rules *>(ptr);
  esie::val				rec;

  if (NULL != vl)
    rec = flagVal(vl, flag);
  else
    {
      rec = flagRule(rl, flag);
      if (!flagOr && onAny(flag, esie::UNKNOWN) && esie::AND == rl->getOper())
	{
	  flagAnd = true;
	  rec = recDep(rl);
	}
      if (flagOr && esie::UNKNOWN == rec && esie::OR == rl->getOper())
	{
	  flag = std::list<esie::val>();
	  flagOr = false;
	  flagAnd = true;
	  ls = rl->getDep();
	  if (!ls.empty())
	    for (it = ls.begin(); it != ls.end(); ++it)
	      {
		rec = recDep(*it);
		flag.push_back(rec);
	      }
	  rec = flagRule(rl, flag);
	}
    }
  return rec;
}

esie::val	askUserAbout(esie::Values *ptr)
{
  std::string	buff;

  std::cout << "Unknown value has been detected on: [" << ptr->getName() << "]";
  std::cout << std::endl << "Query is dependent of this value" << std::endl;
  std::cout << "Please enter new value: [true|false] for [" << ptr->getName();
  std::cout << "]" << std::endl;
  do {
    std::cin >> buff;
    if ("true" == buff)
      ptr->setVal(esie::TRUE);
    else if ("false" == buff)
      ptr->setVal(esie::FALSE);
    else
      std::cout << "Invalid value" << std::endl;
  } while ("true" != buff && "false" != buff);
  flagAnd = false;
  return ptr->getVal();
}

esie::val	recDep(esie::AObject *ptr)
{
  std::list<esie::AObject *>		list = ptr->getDep();
  std::list<esie::AObject *>::iterator	it;
  std::list<esie::val>			flag;
  esie::val				rec = esie::UNKNOWN;
  esie::Rules				*rl = dynamic_cast<esie::Rules *>(ptr);

  if (NULL != rl && esie::OR == rl->getOper())
    flagOr = true;
  if (!list.empty())
    for (it = list.begin(); it != list.end(); ++it)
      {
	rec = recDep(*it);
	flag.push_back(rec);
      }
  if (!flag.empty())
    rec = flagOut(ptr, flag);
  else
    {
      esie::Values	*vl = dynamic_cast<esie::Values *>(ptr);

      if (NULL != vl)
	rec = vl->getVal();
      if (esie::UNKNOWN == rec && flagAnd && !flagOr)
	rec = askUserAbout(vl);
    }
  return rec;
}
