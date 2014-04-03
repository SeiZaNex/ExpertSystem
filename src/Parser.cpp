#include	<vector>
#include	<string>
#include	<fstream>
#include	<iostream>
#include	<sstream>

#include	"Parser.hh"

using namespace		esie;

Parser::Parser(std::string const &file) : _file(file) {}
Parser::Parser(char * const &file) : _file(file) {}
Parser::Parser() {}
Parser::~Parser() {}

inline enum elem	_isRules(std::string const &line)
{
  if ("[rules]" == line)
    return RULES;
  return ENULL;
}

inline enum elem	_isFacts(std::string const &line)
{
  if ("[facts]" == line)
    return FACTS;
  return ENULL;
}

inline enum elem	_isQuery(std::string const &line)
{
  if ("[query]" == line)
    return QUERY;
  return ENULL;
}

inline enum elem	_isParseMode(std::string const &line)
{
  enum elem	type = ENULL;

  for (int i = 0; i < 3 && ENULL == type; ++i)
    {
      switch (i)
	{
	case 0:
	  type = _isRules(line);
	  break;
	case 1:
	  type = _isFacts(line);
	  break;
	case 2:
	  type = _isQuery(line);
	  break;
	default:
	  break;
	}
    }
  return type;
}

void	Parser::_parseRules(std::string &buff)
{
  std::stringstream	ss(buff);
  std::string		str;
  Values		*value;
  Rules			*rule;

  while (ss.good())
    {
      ss >> str;
      
      std::cout << str << std::endl;
    }
  std::cout << buff << std::endl;
}

void	Parser::_parseFacts(std::string &buff)
{
  std::cout << buff << std::endl;
}

void	Parser::_parseQuery(std::string &buff)
{
  std::cout << buff << std::endl;
}

void		Parser::parse()
{
  std::ifstream		ifile(this->_file.c_str());
  std::string		buff;
  static enum elem	type = ENULL;

  if (ifile.is_open())
    {
      while (ifile.good())
	{
	  std::getline(ifile, buff);
	  if (0 != buff.size())
	    {
	      if (ENULL != _isParseMode(buff))
		{
		  type = _isParseMode(buff);
		  continue;
		}
	      if (type != ENULL)
		((*this).*_func[type])(buff);
	    }
	}
      ifile.close();
    }
}
