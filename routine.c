/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yboumanz <yboumanz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:34:43 by yboumanz          #+#    #+#             */
/*   Updated: 2024/12/24 12:32:02 by yboumanz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eat(t_philo *philo)
{
	if (is_dead(philo))
		return (1);
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	precise_sleep(philo->data->pars.time_to_eat);
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->nb_meal_mutex);
	philo->nb_meal++;
	if (philo->data->pars.nb_eat > 0 && philo->nb_meal
		>= philo->data->pars.nb_eat)
		return (1);
	pthread_mutex_unlock(&philo->nb_meal_mutex);
	if (is_dead(philo))
		return (1);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	if (print_status(philo, "is sleeping"))
		return (1);
	if (is_dead(philo))
		return (1);
	precise_sleep(philo->data->pars.time_to_sleep);
	return (0);
}

int	think(t_philo *philo)
{
	if (print_status(philo, "is thinking"))
		return (1);
	if (is_dead(philo))
		return (1);
	precise_sleep(philo->time_to_think);
	return (0);
}

int	take_fork(t_philo *philo)
{
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

	//if (is_dead(philo))
	//	return (1);
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
			return (pthread_mutex_unlock(right_fork), 0);
		print_status(philo, "has taken a fork");
	}
	else
	{
		if (pthread_mutex_lock(right_fork) != 0)
			return (pthread_mutex_unlock(left_fork), 0);
		print_status(philo, "has taken a fork");
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		precise_sleep(50);
	while (1)
	{
		if (is_dead(philo))
			return (NULL);
		take_fork(philo);
		if (is_dead(philo))
		{
			pthread_mutex_unlock(&philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (NULL);
		}
		if (eat(philo))
			return (NULL);
		if (philo_sleep(philo))
			return (NULL);
		if (think(philo))
			return (NULL);
	}
	return (NULL);
}
