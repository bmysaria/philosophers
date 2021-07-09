#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define INVALID_ARGS -1
# define MALLOC -2
# define MUTEX_INIT -3
# define PTHREAD_C -4
# define PTHREAD_D -5

typedef struct s_table	t_table;

typedef struct s_philosopher
{
	int				id;
	int				isfull;
	long long		to_eat;
	long long		to_die;
	long long		to_sleep;
	int				meals;
	int				haseaten;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	pthread_mutex_t	*print;
	pthread_mutex_t	*check_meal;
	long long		last_meal;
	long long		deadline;
	long long		start;
	t_table			*table;
}					t_philosopher;

typedef struct s_table
{
	int				ph_threads;
	long long		to_eat;
	long long		to_die;
	long long		to_sleep;
	int				meals;
	pthread_mutex_t	**forks;
	t_philosopher	*philo;
	long long		start;
	pthread_mutex_t	*print;
	pthread_mutex_t	*check_meal;
	int				firstdeath;
	int				are_full;
}					t_table;

int					ft_atoi(const char *str);
int					all_digits(char *str);
int					error(int num);
void				print_message(long long time, int id, char *str,
						  				t_philosopher *philosopher);
long long			get_time(void);
void				hand_out_forks(t_table *table, t_philosopher *philosophers,
						   				pthread_mutex_t **forks);
void				free_forks(pthread_mutex_t **forks, int i);
int					threads(t_table *table);
void				free_table(t_table *table);
int					serve_the_table(t_table *table);
int					all_digits(char *str);
void				my_usleep(long long time);
int	simulation(t_table *table, pthread_t *ph_t);

#endif