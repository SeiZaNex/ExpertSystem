//
//	Expert System Implementation
//

#include	<string>
#include	<iostream>

#include	"Parser.hh"

int		main(int ac, char *av[])
{

  if (2 <= ac)
    {
      esie::Parser	pa(av[1]);
      pa.parse();
    }
  return 0;
}
