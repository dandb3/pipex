/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoh <jdoh@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:53:32 by jdoh              #+#    #+#             */
/*   Updated: 2023/01/18 20:32:05 by jdoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include "libft/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

static int	input_tmp(int fd, char *argv[])
{
	char	*str;
	int		status;

	if (write(STDOUT_FILENO, "> ", 2) == FAILURE)
		exit(perror_msg("write"));
	status = get_next_line(STDIN_FILENO, &str);
	while (status > 0)
	{
		if (ft_strncmp(str, argv[2], ft_strlen(argv[2])) == SUCCESS
			&& str[ft_strlen(argv[2])] == '\n')
		{
			free(str);
			break ;
		}
		if (write(fd, str, ft_strlen(str)) == FAILURE)
			exit(perror_msg("write"));
		free(str);
		if (write(STDOUT_FILENO, "> ", 2) == FAILURE)
			exit(perror_msg("write"));
		status = get_next_line(STDIN_FILENO, &str);
	}
	return (status);
}

static void	make_tmp(char *filename, char *argv[])
{
	char	*heredoc_err;
	int		fd;
	int		status;

	heredoc_err = "warning: here-document delimited by end-of-file\n";
	fd = open(filename, O_WRONLY | O_CREAT, 0644);
	if (fd == FAILURE)
		exit(perror_msg(filename));
	status = input_tmp(fd, argv);
	if (status == FAILURE)
	{
		unlink(filename);
		exit(perror_msg(filename));
	}
	if (close(fd) == FAILURE)
		exit(perror_msg("close"));
	if (status == 0 && write(STDOUT_FILENO, heredoc_err, 48) == FAILURE)
		exit(perror_msg("close"));
}

static void	malloc_fds(t_info *info)
{
	int	idx;

	info->fds = (int **) malloc(sizeof(int *) * (info->pid_num + 1));
	if (info->fds == NULL)
		exit(perror_msg("malloc"));
	idx = 0;
	while (idx < info->pid_num + 1)
	{
		info->fds[idx] = (int *) malloc(sizeof(int) * 2);
		if (info->fds[idx] == NULL)
			exit(perror_msg("malloc"));
		idx++;
	}
}

static void	open_fds(t_info *info)
{
	int	idx;

	info->fds[0][1] = dup(0);
	info->fds[info->pid_num][0] = dup(0);
	if (info->fds[0][1] == FAILURE || info->fds[info->pid_num][0] == FAILURE)
		exit(perror_msg("dup"));
	info->fds[0][0] = open(info->input, O_RDONLY);
	if (info->fds[0][0] == FAILURE)
		exit(perror_msg(info->input));
	if (info->heredoc == NULL)
		info->fds[info->pid_num][1] = open(info->output, O_WRONLY | O_CREAT, 0644);
	else
		info->fds[info->pid_num][1] = open(info->output, O_WRONLY | O_APPEND
			| O_CREAT, 0644);
	if (info->fds[info->pid_num][1] == FAILURE)
		exit(perror_msg(info->output));
	idx = 0;
	while (++idx < info->pid_num)
	{
		if (pipe(info->fds[idx]) == FAILURE)
			exit(perror_msg("pipe"));
	}
}

void	info_init(t_info *info, int argc, char *argv[], char *envp[])
{
	ft_memset(info, 0, sizeof(t_info));
	info->envp = envp;
	info->output = argv[argc - 1];
	if (ft_strcmp(argv[1], "here_doc") == SUCCESS)
	{
		info->heredoc = argv[2];
		info->input = "/tmp/tempfile.tmp";
		info->pid_num = argc - 4;
		make_tmp(info->input, argv);
	}
	else
	{
		info->input = argv[1];
		info->pid_num = argc - 3;
	}
	malloc_fds(info);
	open_fds(info);
}
