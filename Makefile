NAME = ircserv

CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -g3
CPPFLAGS = -MMD 

SRC = main.cpp \
Server.cpp Client.cpp

OBJ_DIR = obj/
OBJ = $(addprefix  $(OBJ_DIR), $(SRC:.cpp=.o))

DEPS = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)%.o: %.cpp
	mkdir -p $(dir $@) 
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all

-include $(DEPS)

.PHONY : all clean fclean re