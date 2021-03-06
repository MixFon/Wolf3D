# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: widraugr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/29 13:07:44 by widraugr          #+#    #+#              #
#    Updated: 2020/02/20 17:15:27 by widraugr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Wolf3D

LIBDIR = ./libft

MLXDIR = minilibx_macos

FILE_C = main.c\
		 parser.c\
		 add_shadow.c\
		 init.c\
		 check_wall.c\
		 get_wall_hieght.c\
		 press_down.c\
		 press_lift.c\
		 reduce_scale.c\
		 read_strings.c

FLAGS = -Wall -Wextra -Werror -I libft -I include -I $(MLXDIR) 

FMLXLIB = -L $(MLXDIR) -lmlx -framework OpenGL -framework AppKit

FLIB = -L ./libft -lftprintf 

DIRC = ./source/

VPATH = $(DIROBJ)

DIROBJ = ./obj/

OBJ = $(addprefix $(DIROBJ), $(FILE_C:.c=.o))

all : $(NAME)

$(NAME): $(DIROBJ) $(OBJ) 
	make -C $(LIBDIR)
	gcc $(FLAGS) $(OBJ) $(FLIB) $(FMLXLIB) -o $(NAME)
	make clean -C $(LIBDIR)

$(DIROBJ)%.o : $(DIRC)%.c
	gcc -g $(FLAGS) -c $< -o $@

$(DIROBJ):
	mkdir -p $(DIROBJ)

clean:
	/bin/rm -rf $(DIROBJ)
	
fclean: clean
	/bin/rm -f $(NAME)
	make fclean -C $(LIBDIR)
	
re: fclean all 
