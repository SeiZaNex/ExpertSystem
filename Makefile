GCC	=	g++
NAME	=	esie	#Expert System of Inference Engine

SDIR	=	src/
ODIR	=	obj/
SRCS	=	$(shell find $(SDIR) -iname "*.cpp")
OBJS	=	$(subst $(SDIR),$(ODIR),$(SRCS:%.cpp=%.o))

DEBUG	=	-ggdb3
CFLAGS	=	-W -Wall -Wextra -Werror
IFLAGS	=	-Iheaders

RM	=	rm -f

all	:	$(NAME)

$(NAME)	:	$(OBJS)
	$(GCC) $^ -o $@

%.o	:	%.cpp
	$(GCC) $< $(CFLAGS) $(IFLAGS) -o $@

$(OBJS)	:	| $(ODIR)

$(ODIR)	:
	mkdir -p $(ODIR)

clean	:
	$(RM) $(OBJS)
	$(RM) -r $(ODIR)

fclean	:	clean
	$(RM) $(NAME)

re	:	fclean all

.PHONY	:	all clean fclean re