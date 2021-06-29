#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	hand_out_forks(t_table *table, t_philosopher *philo,
					   pthread_mutex_t **forks)
{
	int	i;

	i = 0;
	while (i < table->ph_threads)
	{
		philo[i].isfull = 0;
		philo[i].haseaten = 0;
		philo[i].id = i + 1;
		philo[i].left = forks[i];
		philo[i].right = forks[(i + 1) % table->ph_threads];
		philo[i].print = forks[table->ph_threads];
		philo[i].check_meal = forks[table->ph_threads + 1];
		philo[i].table = table;
		i++;
	}
	table->are_full = 0;
	table->print = forks[table->ph_threads];
	table->forks = forks;
	table->philo = philo;
}

pthread_mutex_t	**create_forks(t_table *table)
{
	pthread_mutex_t	**forks;
	int				i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t *) * (table->ph_threads + 2));
	if (!forks)
		return (NULL);
	while (i < table->ph_threads + 2)
	{
		forks[i] = malloc(sizeof(pthread_mutex_t));
		if (!forks[i])
		{
			free_forks(forks, i);
			return (NULL);
		}
		if (pthread_mutex_init(forks[i], NULL))
		{
			free_forks(forks, i);
			return (NULL);
		}
		i++;
	}
	table->print = forks[i];
	table->check_meal = forks[i + 1];
	return (forks);
}

int	serve_the_table(t_table *table)
{
	pthread_mutex_t	**forks;
	t_philosopher	*philo;

	forks = create_forks(table);
	if (!forks)
	{
		free(table);
		return (error(MALLOC));
	}
	forks = create_forks(table);
	if (!forks)
	{
		free(table);
		return (error(MALLOC));
	}
	philo = malloc(sizeof(t_philosopher) * table->ph_threads);
	if (!philo)
	{
		free_forks(forks, table->ph_threads);
		free(table);
		return (error(MALLOC));
	}
	hand_out_forks(table, philo, forks);
	return (0);
}
