/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yboumanz <yboumanz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 02:03:32 by yboumanz          #+#    #+#             */
/*   Updated: 2024/12/24 13:55:38 by yboumanz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	precise_sleep(unsigned long time_in_ms)
{
	unsigned long	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < time_in_ms)
		usleep(10);
}

long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	monitor_while(t_data *data, unsigned long i)
{
	pthread_mutex_lock(&data->philo[i].last_meal_mutex);
	if (get_time_in_ms() - data->philo[i].last_meal
		> data->pars.time_to_die)
	{
		pthread_mutex_unlock(&data->philo[i].last_meal_mutex);
		pthread_mutex_lock(&data->dead_mutex);
		data->dead = true;
		pthread_mutex_unlock(&data->dead_mutex);
		pthread_mutex_lock(&data->write);
		printf("%lld %d died\n",
			get_time_in_ms() - data->start_time,
			data->philo[i].id);
		pthread_mutex_unlock(&data->write);
		return (1);
	}
	else
		pthread_mutex_unlock(&data->philo[i].last_meal_mutex);
	return (0);
}

void	*routine_monitor(void *arg)
{
	t_data			*data;
	unsigned long	i;
	unsigned long	finished_eating;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		finished_eating = 0;
		while (i < data->pars.nb_philo)
		{
			if (monitor_while(data, i))
				return (NULL);
			pthread_mutex_lock(&data->philo[i].nb_meal_mutex);
			if (data->pars.nb_eat > 0 && data->philo[i].nb_meal
				>= data->pars.nb_eat)
				finished_eating++;
			pthread_mutex_unlock(&data->philo[i].nb_meal_mutex);
			i++;
		}
		if (finished_eating == data->pars.nb_philo)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

int	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->write);
	printf("%lld %d %s\n",
		get_time_in_ms() - philo->data->start_time,
		philo->id, status);
	pthread_mutex_unlock(&philo->data->write);
	return (0);
}
