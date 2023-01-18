/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoh <jdoh@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 11:52:04 by jdoh              #+#    #+#             */
/*   Updated: 2023/01/18 19:01:33 by jdoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	pipex(int argc, char *argv[], char *envp[])
{
	t_info	info;

	info_init(&info, argc, argv, envp);
	if (parsing(&info, argv, envp) == FAILURE)
		return (FAILURE);
	manage_process(&info);
	return (SUCCESS);
}
