NAME = ircserv

CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -g3
CPPFLAGS = -MMD 

SRC = main.cpp \
Server.cpp Client.cpp Command.cpp Code.cpp Channel.cpp \
$(CMD_DIR)JOIN.cpp \
$(CMD_DIR)LIST.cpp \
$(CMD_DIR)MODE.cpp \
$(CMD_DIR)NICK.cpp \
$(CMD_DIR)PART.cpp \
$(CMD_DIR)PASS.cpp \
$(CMD_DIR)PING.cpp \
$(CMD_DIR)USER.cpp \
$(CMD_DIR)INVITE.cpp \
$(CMD_DIR)PRIVMSG.cpp \
$(CMD_DIR)NAMES.cpp \
$(CMD_DIR)NOTICE.cpp \
$(CMD_DIR)TOPIC.cpp \
$(CMD_DIR)OPER.cpp \
$(CMD_DIR)KILL.cpp \
$(CMD_DIR)KICK.cpp \
$(CMD_DIR)QUIT.cpp \
utils.cpp

CMD_DIR = command/
OBJ_DIR = obj/
OBJ = $(addprefix  $(OBJ_DIR), $(SRC:.cpp=.o))


DEPS = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS)  -o $(NAME) $(OBJ)

$(OBJ_DIR)%.o: %.cpp
	mkdir -p $(dir $@) 
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -I. -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all

-include $(DEPS)

.PHONY : all clean fclean re