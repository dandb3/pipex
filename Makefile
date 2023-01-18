# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdoh <jdoh@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/17 19:25:02 by jdoh              #+#    #+#              #
#    Updated: 2023/01/18 19:01:26 by jdoh             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	pipex
SRCS_M		=	main.c init.c error.c parsing.c pipex.c process.c
OBJS_M		=	$(SRCS_M:.c=.o)
HEADER_M	=	pipex.h
SRCS_B		=	main_bonus.c init_bonus.c error_bonus.c parsing_bonus.c \
				pipex_bonus.c process_bonus.c get_next_line_bonus.c
OBJS_B		=	$(SRCS_B:.c=.o)
HEADER_B	=	pipex_bonus.h

LIBFT		=	libft/libft.a
LIBFT_DIR	=	libft

CC			=	cc
CFLAGS		=	-Werror -Wextra -Wall
BONUS_MODE	=	$(findstring bonus, $(MAKECMDGOALS))

ifeq (bonus, $(BONUS_MODE))
	SRCS	=	$(SRCS_B)
	OBJS	=	$(OBJS_B)
	HEADER	=	$(HEADER_B)
else
	SRCS	=	$(SRCS_M)
	OBJS	=	$(OBJS_M)
	HEADER	=	$(HEADER_M)
endif

all			:	$(NAME)
bonus		:	$(NAME)

$(NAME)		:	$(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L $(LIBFT_DIR) -lft -o $@

$(LIBFT)	:
	make -C $(LIBFT_DIR) bonus

%.o			:	%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean		:
	rm -rf $(OBJS_M) $(OBJS_B)
	make -C $(LIBFT_DIR) fclean

fclean		:	clean
	rm -rf $(NAME)

re			:
	make fclean
	make all

.PHONY		:	all bonus clean fclean re