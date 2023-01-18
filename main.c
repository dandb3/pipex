/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoh <jdoh@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 11:53:41 by jdoh              #+#    #+#             */
/*   Updated: 2023/01/18 20:45:56 by jdoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdlib.h>

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 5)
		exit(error_msg("please input 5 arguments.\n", 1));
	if (pipex(argc, argv, envp) == FAILURE)
		return (1);
	return (0);
}
