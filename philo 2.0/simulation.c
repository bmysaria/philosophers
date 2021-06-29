#include "philo.h"

int	check_death(t_philosopher *philo)
{
	pthread_mutex_lock(philo->check_meal);
	if (get_time() > philo->deadline)
	{
		pthread_mutex_lock(philo->print);
		printf("%llu philo %d has died\n", get_time() - philo->start,
			   philo->id);
		printf("%llu\n", get_time() - philo->deadline);
		philo->table->firstdeath = 1;
		return (1);
	}
	pthread_mutex_unlock(philo->check_meal);
	if (philo->table->meals !=0 && philo->table->are_full
		== philo->table->ph_threads)
	{
		pthread_mutex_lock(philo->print);
		printf("Ate %d times\n", philo->table->meals);
		return (2);
	}
	return (0);
}

void	eat(t_philosopher *philo)
{
	long long	time;

	pthread_mutex_lock(philo->left);
	print_message(get_time() - philo->start, philo->id, FORK, philo);
	pthread_mutex_lock(philo->right);
	print_message(get_time() - philo->start, philo->id, FORK, philo);
	print_message(get_time() - philo->start, philo->id, EAT, philo);
	time = get_time();
	while (get_time() < time + philo->to_eat)
		usleep(100);
	pthread_mutex_lock(philo->check_meal);
	philo->deadline = get_time() + philo->to_die;
	pthread_mutex_unlock(philo->check_meal);
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
	if (philo->table->meals != 0)
	{
		if (philo->haseaten < philo->meals)
			philo->haseaten++;
		if (philo->haseaten == philo->meals && philo->isfull == 0)
		{
			philo->isfull = 1;
			philo->table->are_full++;
		}
	}
}

void	*lifetime(void *args)
{
	t_philosopher	*philo;
	long long		time;

	philo = (t_philosopher *)args;
	pthread_mutex_lock(philo->check_meal);
	philo->deadline = get_time() + philo->to_die;
	pthread_mutex_unlock(philo->check_meal);
	if (philo->id % 2 != 0)
	{
		time = get_time();
		while (get_time() < time + philo->to_eat)
			usleep(100);
	}
	while (philo->table->firstdeath == 0)
	{
		eat(philo);
		print_message(get_time() - philo->start, philo->id, SLEEP, philo);
		time = get_time();
		while (get_time() <= time + philo->to_sleep)
			usleep(100);
		print_message(get_time() - philo->start, philo->id, THINK, philo);
	}
	return (NULL);
}

int	simulation(t_table *table, pthread_t *ph_t)
{
	int	i;
	int	res;

	i = -1;
	while (++i < table->ph_threads)
	{
		pthread_create(&ph_t[i], NULL, &lifetime, &table->philo[i]);
	}
	while (1)
	{
		i = -1;
		while (++i < table->ph_threads)
		{
			res = check_death(&table->philo[i]);
			if (res == 1 || res == 2)
				return (1);
		}
	}
}

int	threads(t_table *table)
{
	pthread_t	*ph_t;
	int			i;

	i = 0;
	ph_t = malloc(sizeof (pthread_t) * (table->ph_threads));
	if (!ph_t)
	{
		free_table(table);
		return (error(MALLOC));
	}
	table->start = get_time();
	while (i < table->ph_threads)
	{
		table->philo[i].last_meal = table->start;
		table->philo[i].deadline = table->start + table->to_die;
		table->philo[i].start = table->start;
		table->philo[i].meals = table->meals;
		table->philo[i].to_eat = table->to_eat;
		table->philo[i].to_die = table->to_die;
		table->philo[i].to_sleep = table->to_sleep;
		i++;
	}
	return (simulation(table, ph_t));
}
