/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoh <jdoh@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:53:32 by jdoh              #+#    #+#             */
/*   Updated: 2023/01/18 20:28:23 by jdoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

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
	info->fds[info->pid_num][1] = open(info->output, O_WRONLY | O_CREAT, 0644);
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
	info->pid_num = argc - 3;
	info->input = argv[1];
	info->output = argv[argc - 1];
	malloc_fds(info);
	open_fds(info);
}
