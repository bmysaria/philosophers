#include "philo.h"

void	hand_out_forks(t_table *table, t_philosopher *philosophers,
					   pthread_mutex_t **forks)
{
	int	i;

	i = 0;
	while (i < table->ph_threads)
	{
		philosophers[i].isfull = 0;
		philosophers[i].haseaten = 0;
		philosophers[i].id = i + 1;
		philosophers[i].left = forks[i];
		philosophers[i].right = forks[(i + 1) % table->ph_threads];
		philosophers[i].print = forks[table->ph_threads];
		philosophers[i].check_meal = forks[table->ph_threads + 1];
		philosophers[i].table = table;
		i++;
	}
	table->are_full = 0;
	table->print = forks[table->ph_threads];
	table->forks = forks;
	table->philosophers = philosophers;
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
	t_philosopher	*philosophers;

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
	philosophers = malloc(sizeof(t_philosopher) * table->ph_threads);
	if (!philosophers)
	{
		free_forks(forks, table->ph_threads);
		free(table);
		return (error(MALLOC));
	}
	hand_out_forks(table, philosophers, forks);
	return (0);
}
