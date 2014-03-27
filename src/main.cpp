//
//	Expert System Implementation
//

#include	<string>
#include	<iostream>

#include	"Parser.hh"

int		main(int ac, char *av[])
{
  esie::Parser	parser;

  if (ac > 2)
    {
      parser = esie::Parser(av[1]);
    }
  return 0;
}
