NAME = crappy 

option = -Iincludes

OBJDIR = obj

cc = g++ -Wall -Werror -Wextra

src = $(wildcard src/*.cpp)

obj = $(addprefix $(OBJDIR)/,$(addsuffix .o, $(basename $(notdir $(src)))))

all: $(NAME)

$(NAME): $(obj)
	$(cc) -o $(NAME) $(option) $^

$(OBJDIR)/%.o: src/%.cpp
	mkdir -p $(OBJDIR)
	$(cc) $(option) -c $< -o $@

clean:
	rm -rf $(obj)

fclean: clean
	rm -rf $(NAME)

re: fclean
	make all
