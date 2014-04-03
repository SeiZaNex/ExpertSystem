GCC	=	g++
NAME	=	esie	#Expert System of Inference Engine

SDIR	=	src/
ODIR	=	obj/
SRCS	=	$(shell find $(SDIR) -iname "*.cpp")
OBJS	=	$(subst $(SDIR),$(ODIR),$(SRCS:%.cpp=%.o))

#DEBUG	=	-ggdb3
CFLAGS	=	-std=c++0x -W -Wall -Wextra -Werror -ggdb3
IFLAGS	=	-Iheaders/

RM	=	rm -f

all	:	$(NAME)

$(NAME)	:	$(OBJS)
	$(GCC) $^ $(CFLAGS) $(IFLAGS) -o $@

$(ODIR)%.o	:	$(SDIR)%.cpp
	$(GCC) -c $< $(CFLAGS) $(IFLAGS) -o $@

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