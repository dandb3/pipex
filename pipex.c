/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoh <jdoh@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 11:52:04 by jdoh              #+#    #+#             */
/*   Updated: 2023/01/17 21:06:31 by jdoh             ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	pipex(int argc, char *argv[], char *envp[])
{
	t_info	info;

	info_init(&info, argc, argv, envp);
	if (parsing(&info, argv, envp) == FAILURE)
		return (FAILURE);
	manage_process(&info);
	return (SUCCESS);
}
