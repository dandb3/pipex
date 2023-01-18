/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoh <jdoh@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:20:59 by jdoh              #+#    #+#             */
/*   Updated: 2023/01/18 20:23:27 by jdoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void	close_all(t_info *info, int size)
{
	int	idx;

	idx = -1;
	while (++idx < size)
	{
		if (close(info->fds[idx][0]) == FAILURE)
			exit(perror_msg("close"));
		if (close(info->fds[idx][1]) == FAILURE)
			exit(perror_msg("close"));
	}
}

static void	parent_process(t_info *info)
{
	int	idx;

	idx = 0;
	close_all(info, info->pid_num + 1);
	while (idx < info->pid_num)
	{
		wait(NULL);
		idx++;
	}
	if (info->heredoc != NULL && unlink(info->input) == FAILURE)
		exit(perror_msg("unlink"));
}

static void	child_process(t_info *info, int idx)
{
	int	s_out;

	s_out = dup(STDOUT_FILENO);
	if (s_out == FAILURE)
		exit(perror_msg("dup"));
	if (dup2(info->fds[idx][0], STDIN_FILENO) == FAILURE
		|| dup2(info->fds[idx + 1][1], STDOUT_FILENO) == FAILURE)
		exit(perror_msg("dup"));
	close_all(info, info->pid_num + 1);
	if (access(info->cmds[idx][0], F_OK) == FAILURE)
	{
		error_msg(info->cmds[idx][0], s_out);
		exit(error_msg(": command not found\n", s_out));
	}
	execve(info->cmds[idx][0], info->cmds[idx], info->envp);
	exit(perror_msg("execve"));
}

void	manage_process(t_info *info)
{
	pid_t	pid;
	int		idx;

	pid = 1;
	idx = 0;
	while (idx < info->pid_num && pid != 0)
	{
		pid = fork();
		if (pid == FAILURE)
			exit(perror_msg("fork"));
		if (pid == 0)
			break ;
		idx++;
	}
	if (pid != 0)
		parent_process(info);
	else
		child_process(info, idx);
}
