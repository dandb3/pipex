/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoh <jdoh@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 12:44:56 by jdoh              #+#    #+#             */
/*   Updated: 2023/01/18 19:01:28 by jdoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include "libft/libft.h"
#include <unistd.h>
#include <stdlib.h>

static char	*slash_join(char *str1, char *str2)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(str1, "/");
	if (tmp == NULL)
		return (NULL);
	result = ft_strjoin(tmp, str2);
	free(tmp);
	return (result);
}

static char	**find_path(char *envp[])
{
	char	**result;

	while (*envp)
	{
		if (ft_strnstr(*envp, "PATH=", 5))
		{
			result = ft_split(*envp, ":");
			if (result == NULL)
				return (NULL);
			return (result);
		}
		envp++;
	}
	result = (char **) malloc(sizeof(char *) * 2);
	if (result == NULL)
		return (NULL);
	result[0] = ft_strdup("");
	if (result[0] == NULL)
	{
		free(result);
		return (NULL);
	}
	result[1] = NULL;
	return (result);
}

static char	**split_cmd(char *cmd, char **path)
{
	char	**result;
	char	*tmp;

	result = ft_split(cmd, " \t");
	if (result == NULL)
		return (NULL);
	while (*(++path))
	{
		tmp = slash_join(*path, result[0]);
		if (tmp == NULL)
			return ((char **)free_splitted(result, 0));
		if (access(tmp, F_OK) == SUCCESS)
		{
			free(result[0]);
			result[0] = tmp;
			if (access(tmp, X_OK) == SUCCESS)
				return (result);
			continue ;
		}
		free(tmp);
	}
	return (result);
}

int	parsing(t_info *info, char *argv[], char *envp[])
{
	char	**path;
	int		idx;
	int		offset;

	offset = 2;
	if (info->heredoc != NULL)
		offset++;
	path = find_path(envp);
	if (path == NULL)
		return (FAILURE);
	info->cmds = (char ***) malloc(sizeof(char **) * (info->pid_num + 1));
	if (info->cmds == NULL)
		return ((int)free_splitted(path, FAILURE));
	idx = -1;
	while (++idx < info->pid_num)
	{
		info->cmds[idx] = split_cmd(argv[idx + offset], path);
		if (info->cmds[idx] == NULL)
		{
			free_cmds(info->cmds, idx);
			return ((int)free_splitted(path, FAILURE));
		}
	}
	info->cmds[idx] = NULL;
	return ((int)free_splitted(path, SUCCESS));
}
