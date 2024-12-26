/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yboumanz <yboumanz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 23:31:33 by yboumanz          #+#    #+#             */
/*   Updated: 2024/12/26 11:54:18 by yboumanz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	handle_one_philo(t_data *data)
{
	print_status(&data->philo[0], "has taken a fork");
	precise_sleep(data->pars.time_to_die);
	pthread_mutex_lock(&data->write);
	printf("%lld %d died\n",
		get_time_in_ms() - data->start_time,
		data->philo[0].id);
	pthread_mutex_unlock(&data->write);
}

bool	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (false);
}

int	create_threads(t_data *data)
{
	long	i;

	i = 0;
	while (i < (long)data->pars.nb_philo)
	{
		pthread_mutex_lock(&data->philo[i].last_meal_mutex);
		data->philo[i].last_meal = get_time_in_ms();
		pthread_mutex_unlock(&data->philo[i].last_meal_mutex);
		if (pthread_create
			(&data->philo[i].philo, NULL, &routine, &data->philo[i]) != 0)
		{
			while (--i >= 0)
				pthread_join(data->philo[i].philo, NULL);
			return (handle_error("Failed to create philo thread", data), 1);
		}
		i++;
	}
	if (pthread_create(&data->monitor, NULL, &routine_monitor, data) != 0)
		return (handle_error("Failed to create monitor thread", data), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data			data;
	unsigned long	i;

	if (argc < 5 || argc > 6)
		return (ft_putendl_fd("Error: Invalid number of arguments", 1), 1);
	if (init_things(&data, argv, argc) == 1)
		return (1);
	if (data.pars.nb_philo == 1)
		return (handle_one_philo(&data), 0);
	if (create_threads(&data) == 1)
		return (1);
	i = 0;
	while (i < data.pars.nb_philo)
	{
		pthread_join(data.philo[i].philo, NULL);
		i++;
	}
	pthread_join(data.monitor, NULL);
	return (0);
}
