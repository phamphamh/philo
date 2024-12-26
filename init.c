/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yboumanz <yboumanz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:56:09 by yboumanz          #+#    #+#             */
/*   Updated: 2024/12/26 13:02:39 by yboumanz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_ttt(t_data *data, int i)
{
	if (data->pars.time_to_eat == data->pars.time_to_sleep)
		data->philo[i].time_to_think = ((data->pars.time_to_sleep * 30) / 100) / 2;
	else if (data->pars.time_to_eat > data->pars.time_to_sleep)
	{
		data->philo[i].time_to_think
			= (data->pars.time_to_eat * 30) / 100
			- (data->pars.time_to_sleep * 30) / 100;
	}
	else
	{
		data->philo[i].time_to_think
			= (data->pars.time_to_sleep * 30) / 100
			- (data->pars.time_to_eat * 30) / 100;
	}
	//if (data->pars.nb_philo % 2 != 0 && data->philo[i].id % 2 != 0)
	//	data->philo[i].time_to_think += data->pars.time_to_eat;
}

int	init_input(t_data *data, int argc, char **argv)
{
	data->pars.nb_philo = ft_atol(argv[1]);
	if (data->pars.nb_philo == ULONG_MAX || data->pars.nb_philo > 200
		|| data->pars.nb_philo <= 0)
		return (ft_putendl_fd("Wrong nb of philos or bad arg syntax", 1), 1);
	data->pars.time_to_die = ft_atol(argv[2]);
	if (data->pars.time_to_die == ULONG_MAX || data->pars.time_to_die <= 0)
		return (ft_putendl_fd("Wrong time to die or bad arg syntax", 1), 1);
	data->pars.time_to_eat = ft_atol(argv[3]);
	if (data->pars.time_to_eat == ULONG_MAX || data->pars.time_to_eat <= 0)
		return (ft_putendl_fd("Wrong time to eat or bad arg syntax", 1), 1);
	data->pars.time_to_sleep = ft_atol(argv[4]);
	if (data->pars.time_to_sleep == ULONG_MAX || data->pars.time_to_sleep <= 0)
		return (ft_putendl_fd("Wrong time to sleep or bad arg syntax", 1), 1);
	if (argc == 6)
	{
		data->pars.nb_eat = ft_atol(argv[5]);
		if (data->pars.nb_eat == ULONG_MAX || data->pars.nb_eat <= 0)
			return (ft_putendl_fd("Wrong nb of eat round table", 1), 1);
	}
	else
		data->pars.nb_eat = 0;
	return (0);
}

int	init_philos(t_data *data)
{
	unsigned long	i;

	i = 0;
	while (i < data->pars.nb_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].last_meal = get_time_in_ms();
		data->philo[i].nb_meal = 0;
		data->philo[i].data = data;
		data->philo[i].data->dead = false;
		set_ttt(data, i);
		if (pthread_mutex_init(&data->philo[i].left_fork, NULL) != 0
			|| pthread_mutex_init(&data->philo[i].last_meal_mutex, NULL) != 0
			|| pthread_mutex_init(&data->philo[i].nb_meal_mutex, NULL) != 0)
		{
			while (i-- > 0)
			{
				pthread_mutex_destroy(&data->philo[i].left_fork);
				pthread_mutex_destroy(&data->philo[i].last_meal_mutex);
			}
			return (ft_putendl_fd("Mutex init error", 2), 1);
		}
		i++;
	}
	return (0);
}

int	init_things(t_data *data, char **argv, int argc)
{
	unsigned long	i;

	i = 0;
	data->start_time = get_time_in_ms();
	data->dead = false;
	if (init_input(data, argc, argv))
		return (1);
	if (init_philos(data))
		return (1);
	while (i < data->pars.nb_philo)
	{
		data->philo[i].right_fork = &data->philo[(i + 1)
			% data->pars.nb_philo].left_fork;
		i++;
	}
	if (pthread_mutex_init(&data->write, NULL) != 0
		|| pthread_mutex_init(&data->dead_mutex, NULL) != 0)
	{
		i = 0;
		while (i < data->pars.nb_philo)
			pthread_mutex_destroy(&data->philo[i++].left_fork);
		return (ft_putendl_fd("Write or Dead mutex init error", 2), 1);
	}
	return (0);
}
