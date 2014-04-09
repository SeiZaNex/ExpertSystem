#ifndef		ESIE_PARSER_HH_
#define		ESIE_PARSER_HH_

#include	<list>
#include	<stack>
#include	<queue>
#include	<string>

#include	"Engine.hh"

#define		OPER_SYMBOL	8

namespace	esie
{
  enum		elem
    {
      RULES = 0,
      FACTS = 1,
      QUERY = 2,
      ENULL = 3
    };

  struct	s_op
  {
    const char	*sym;
    enum op	rule;
    int		param;
  };

  static const struct s_op	oper[OPER_SYMBOL]=
    {
      { "!", NOT, 1 },
      { "~", NOT, 1 },
      { "+", AND, 2 },
      { "^", AND, 2 },
      { "*", OR, 2 },
      { "v", OR, 2 },
      { "V", OR, 2 },
      { "->", EQU, 1 }
    };

  class		Parser
  {
  public:
    Parser(std::string const &file);
    Parser(char * const &file);
    ~Parser();

    void	parse();
  private:
    Parser();

    std::string		_file;
    std::list<Values *>	_facts;

    std::stack<std::string>	_fact;
    std::stack<Rules *>		_oper;
    std::queue<AObject *>	_queue;

    void	_printFacts();

    bool	_isDup(std::string &str);
    Values *	_getDup(std::string &str);

    void	_gatherRules();
    AObject *	_parseDeps(std::string &deps);

    void	_parseRules(std::string &buff);
    void	_parseFacts(std::string &buff);
    void	_parseQuery(std::string &buff);

    typedef void (Parser::*fnc)(std::string &);

    const fnc	_func[3]=
      {
	&Parser::_parseRules,
	&Parser::_parseFacts,
	&Parser::_parseQuery
      };
  };

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
    return type;
  }

  inline int	_isOper(std::string &str)
  {
    for (int i = 0; i < OPER_SYMBOL; ++i)
      if (oper[i].sym == str)
	return i;
    return -1;
  }

};

#endif		/* !ESIE_PARSER_HH_ */
