NAME        =   lists_by_sign

RM          =   rm -f
CC          =   gcc
CCFLAGS     =   -Wall -Werror -Wextra -g3 -pthread \
                -D_POSIX_C_SOURCE=200809L
SAN			=	-fsanitize=address,undefined 
SRC_FILES   =   main program routine
SRC_DIR     =   src/
OBJ_DIR     =   obj/

SRC         =   $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ         =   $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(OBJ) $(CCFLAGS) $(SAN) -o $(NAME)
	@echo "lists_by_sign compilado!"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compilando: $<"
	@$(CC) $(CCFLAGS) -c $< -o $@

clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "Eliminando objetos..."

fclean: clean
	@$(RM) $(NAME)
	@echo "Eliminando ejecutable..."

re: fclean all
