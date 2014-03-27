#ifndef		ESIE_PARSER_HH_
#define		ESIE_PARSER_HH_

#include		<string>
#include		<vector>
#include		<list>
#include		<map>

namespace		esie
{
  class			Rules
  {
  public:
  private:
    
  };

  class			Attributes
  {

  };

  class			Parser
  {
  public:
    Parser(std::string const & file);
    Parser(char * const & file);
    ~Parser();

    void		parse();
  private:
    std::string		_file;

    Parser();
  };
};

#endif		/* !ESIE_PARSER_HH_ */
