/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoh <jdoh@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 19:01:05 by jdoh              #+#    #+#             */
/*   Updated: 2023/01/18 19:01:09 by jdoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"

static int	free_and_ret(char **ptr1, char **ptr2, int ret)
{
	if (ptr1 != NULL)
	{
		free(*ptr1);
		*ptr1 = NULL;
	}
	if (ptr2 != NULL)
	{
		free(*ptr2);
		*ptr2 = NULL;
	}
	return (ret);
}

static int	str_join(char **remain, char *buf, ssize_t read_len)
{
	char	*tmp;
	size_t	idx;
	size_t	remain_len;

	remain_len = 0;
	if (*remain != NULL)
		remain_len = ft_strlen(*remain);
	tmp = (char *) malloc(sizeof(char) * (remain_len + read_len + 1));
	if (tmp == NULL)
		return (-1);
	idx = -1;
	while (++idx < remain_len)
		tmp[idx] = (*remain)[idx];
	while (idx < remain_len + read_len)
	{
		tmp[idx] = buf[idx - remain_len];
		idx++;
	}
	tmp[idx] = '\0';
	free(*remain);
	*remain = tmp;
	return (0);
}

static int	read_line(int fd, char *buf, char **remain)
{
	char	*newline_addr;
	ssize_t	read_len;

	newline_addr = NULL;
	if (*remain != NULL)
		newline_addr = ft_memchr(*remain, '\n', ft_strlen(*remain));
	while (newline_addr == NULL)
	{
		read_len = read(fd, buf, 42);
		if (read_len < 0)
			return (-1);
		if (*remain == NULL && read_len == 0)
			return (0);
		newline_addr = ft_memchr(buf, '\n', read_len);
		if (str_join(remain, buf, read_len) < 0)
			return (-1);
	}
	return (1);
}

static int	my_split(char **remain, char **result)
{
	char	*newline_addr;
	char	*tmp;
	size_t	remain_len;

	if (*remain == NULL)
		return (0);
	remain_len = ft_strlen(*remain);
	newline_addr = ft_memchr(*remain, '\n', remain_len);
	if (newline_addr == NULL || newline_addr - *remain + 1 == (int)remain_len)
	{
		*result = *remain;
		*remain = NULL;
		return (0);
	}
	*result = ft_substr(*remain, 0, newline_addr - *remain + 1);
	if (*result == NULL)
		return (-1);
	tmp = ft_substr(*remain, newline_addr - *remain + 1, remain_len);
	if (tmp == NULL)
		return (free_and_ret(result, NULL, -1));
	free(*remain);
	*remain = tmp;
	return (0);
}

int	get_next_line(int fd, char **result)
{
	static char	*remain;
	char		buf[42];
	int			status;

	if (fd < 0 || read(fd, buf, 0) < 0)
		return (free_and_ret(&remain, NULL, -1));
	status = read_line(fd, buf, &remain);
	if (status < 0)
		return (free_and_ret(&remain, NULL, -1));
	if (my_split(&remain, result) < 0)
		return (free_and_ret(&remain, NULL, -1));
	return (status);
}
