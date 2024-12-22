/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yboumanz <yboumanz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:34:43 by yboumanz          #+#    #+#             */
/*   Updated: 2024/12/22 15:45:16 by yboumanz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	print_status(philo, "is eating");
	precise_sleep(philo->data->pars.time_to_eat);
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_sleep(philo->data->pars.time_to_sleep);
}

void	think(t_philo *philo)
{
	unsigned long long	current_time;
	unsigned long long	time_since_last_meal;

	print_status(philo, "is thinking");
	while (1)
	{
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_lock(&philo->last_meal_mutex);
		current_time = get_time_in_ms();
		time_since_last_meal = current_time - philo->last_meal;
		if (time_since_last_meal >= philo->data->pars.time_to_die
			- philo->data->pars.time_to_eat - 10)
		{
			pthread_mutex_unlock(&philo->last_meal_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->last_meal_mutex);
		usleep(1000);
	}
}

int	take_fork(t_philo *philo)
{
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

	left_fork = &philo->left_fork;
	right_fork = philo->right_fork;
	if (philo->id % 2 == 0)
		pthread_mutex_lock(right_fork);
	else
		pthread_mutex_lock(left_fork);
	print_status(philo, "has taken a fork");
	if (philo->id % 2 == 0)
	{
		if (pthread_mutex_lock(left_fork) != 0)
			return (pthread_mutex_unlock(right_fork), 1);
		print_status(philo, "has taken a fork");
	}
	else
	{
		if (pthread_mutex_lock(right_fork) != 0)
			return (pthread_mutex_unlock(left_fork), 1);
		print_status(philo, "has taken a fork");
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		take_fork(philo);
		eat(philo);
		philo->nb_meal++;
		if (philo->data->pars.nb_eat > 0 && philo->nb_meal
			>= philo->data->pars.nb_eat)
			break ;
		philo_sleep(philo);
		think(philo);
	}
	return (NULL);
}
