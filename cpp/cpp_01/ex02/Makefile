NAME = crappy 

option = -I.

OBJDIR = obj

cc = g++ -Wall -Werror -Wextra

src = $(wildcard *.cpp)

obj = $(addprefix $(OBJDIR)/,$(addsuffix .o, $(basename $(notdir $(src)))))

all: $(NAME)

$(NAME): $(obj)
	$(cc) $(option) -o $(NAME)  $^

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(OBJDIR)
	$(cc) $(option) -c $< -o $@

clean:
	rm -rf $(obj)

fclean: clean
	rm -rf $(NAME)

re: fclean
	make all
