NAME = megaphone

option = -I

OBJDIR = obj

cc = g++ -Wall -Werror -Wextra

src = megaphone.cpp

obj = $(addprefix $(OBJDIR)/,$(addsuffix .o, $(basename $(notdir $(src)))))

all: $(NAME)

$(NAME): $(obj)
	$(cc) -o $(NAME) $^

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(OBJDIR)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj)

fclean: clean
	rm -rf $(NAME)

re: fclean
	make all
