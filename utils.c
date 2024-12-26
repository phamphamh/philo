/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yboumanz <yboumanz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 00:20:04 by yboumanz          #+#    #+#             */
/*   Updated: 2024/12/26 11:55:44 by yboumanz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
	{
		while (*s)
		{
			write(fd, &*s, 1);
			s++;
		}
	}
}

void	handle_error(char *message, t_data *data)
{
	unsigned long	i;

	i = -1;
	while (i++ < data->pars.nb_philo)
		pthread_mutex_destroy(&data->philo[i].left_fork);
	if (data->philo[0].philo)
	{
		i = -1;
		while (i++ < data->pars.nb_philo)
			pthread_join(data->philo[i].philo, NULL);
	}
	if (data->monitor)
		pthread_join(data->monitor, NULL);
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->dead_mutex);
	ft_putendl_fd(message, 2);
}

long long	ft_atol(const char *str)
{
	long long	res;
	int			sign;
	int			i;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	if (!(str[i] >= '0' && str[i] <= '9'))
		return (ULONG_MAX);
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
		if ((sign == 1 && res > LONG_MAX)
			|| (sign == -1 && res * sign < LONG_MIN))
			return (ULONG_MAX);
	}
	if (str[i])
		return (ULONG_MAX);
	return (res * sign);
}
