/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:37:06 by yilin             #+#    #+#             */
/*   Updated: 2025/01/22 18:05:17 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isblank(char c)
{
	if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == '\v')
		return (1);
	return (0);
}

long	ft_atol(const char *nptr)
{
	int		i;
	long	sign;
	long	num;

	i = 0;
	sign = 1;
	num = 0;
	while (ft_isblank(nptr[i]) == 1)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign *= (-1);
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = ((num * 10) + (nptr[i] - '0'));
		i++;
	}
	return ((num * sign));
}

/** ft_atoi */
int	ft_atoi(const char *str)
{
	int	i;
	int	nb;
	int	sign;

	i = 0;
	nb = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = sign * (-1);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = (str[i] - 48) + (nb * 10);
		i++;
	}
	return (nb * sign);
}
