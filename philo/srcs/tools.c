/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:57:49 by yilin             #+#    #+#             */
/*   Updated: 2025/01/22 18:05:23 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died
Replace timestamp_in_ms with the current timestamp in milliseconds
and X with the philosopher number.
 */
/** GET TIME*/
unsigned long	get_current_time(void)
{
	t_timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/** ft_putstr_fd */
void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

/** memset */
void	*ft_memset(void *block, int value, size_t n)
{
	char	*ptr;

	ptr = (char *)block;
	while (n > 0)
	{
		ptr[n - 1] = value;
		n--;
	}
	return (block);
}

/** ft_isdigit */
int	ft_isdigit(int i)
{
	if (i >= '0' && i <= '9')
		return (1);
	return (0);
}
