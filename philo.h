/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yboumanz <yboumanz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 23:31:15 by yboumanz          #+#    #+#             */
/*   Updated: 2024/12/26 11:56:38 by yboumanz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdbool.h>

typedef struct s_philo
{
	int						id;
	unsigned long long		last_meal;
	pthread_mutex_t			last_meal_mutex;
	pthread_t				philo;
	pthread_mutex_t			left_fork;
	pthread_mutex_t			*right_fork;
	pthread_mutex_t			nb_meal_mutex;
	unsigned long			nb_meal;
	struct s_data			*data;
	unsigned long			time_to_think;
}	t_philo;

typedef struct s_pars
{
	unsigned long				nb_philo;
	unsigned long				time_to_die;
	unsigned long				time_to_eat;
	unsigned long				time_to_sleep;
	unsigned long				nb_eat;
}	t_pars;

typedef struct s_data
{
	unsigned long long		start_time;
	pthread_t				monitor;
	pthread_mutex_t			dead_mutex;
	bool					dead;
	t_philo					philo[200];
	t_pars					pars;
	pthread_mutex_t			write;
}	t_data;

//MAIN
int			main(int argc, char **argv);
int			create_threads(t_data *data);
bool		is_dead(t_philo *philo);

//INIT
int			init_input(t_data *data, int argc, char **argv);
int			init_philos(t_data *data);
int			init_things(t_data *data, char **argv, int argc);

//ROUTINE
void		*routine(void *arg);
int			eat(t_philo *philo);
int			philo_sleep(t_philo *philo);
int			think(t_philo *philo);
int			take_fork(t_philo *philo);

//UTILS
long long	ft_atol(const char *str);
int			ft_strcmp(const char *s1, const char *s2);
void		ft_putendl_fd(char *s, int fd);
void		ft_putstr_fd(char *s, int fd);
void		handle_error(char *message, t_data *data);

//PHILO_UTILS
long long	get_time_in_ms(void);
void		*routine_monitor(void *arg);
int			print_status(t_philo *philo, char *status);
void		precise_sleep(unsigned long time_in_ms);

#endif
