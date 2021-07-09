#include "philo.h"

void my_usleep(long long time)
{
	long long now;

	now = get_time();
	while (get_time() < time + now)
		usleep(100);
}

int	check_death(t_philosopher *philo)
{
	pthread_mutex_lock(philo->check_meal);
	if (get_time() > philo->deadline)
	{
		pthread_mutex_lock(philo->print);
		printf("%llu philo %d has died\n", get_time() - philo->start,
			   philo->id);
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

void eat(t_philosopher *philo)
{
	long long time;

	pthread_mutex_lock(philo->left);
	print_message(get_time() - philo->start, philo->id, FORK, philo);
	pthread_mutex_lock(philo->right);
	time = get_time();
	pthread_mutex_lock(philo->check_meal);
	philo->deadline = time + philo->to_die;
	pthread_mutex_unlock(philo->check_meal);
	print_message(get_time() - philo->start, philo->id, FORK, philo);
	print_message(get_time() - philo->start, philo->id, EAT, philo);
	my_usleep(philo->to_eat);
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

	philo = (t_philosopher *)args;
	pthread_mutex_lock(philo->check_meal);
	philo->deadline = get_time() + philo->to_die;
	pthread_mutex_unlock(philo->check_meal);
	if (philo->id % 2 != 0)
		my_usleep(philo->to_eat);
	while (philo->table->firstdeath == 0)
	{
		eat(philo);
		print_message(get_time() - philo->start, philo->id, SLEEP, philo);
		my_usleep(philo->to_sleep);
		print_message(get_time() - philo->start, philo->id, THINK, philo);
	}
	return (NULL);
}

int	simulation(t_table *table, pthread_t *ph_t)
{
	int	i;

	i = -1;
	while (++i < table->ph_threads)
	{
		if (pthread_create(&ph_t[i], NULL, &lifetime, &table->philo[i]))
			return (error(PTHREAD_C));
		if (pthread_detach(ph_t[i]))
			return (error(PTHREAD_D));
	}
	while (1)
	{
		i = -1;
		while (++i < table->ph_threads)
		{
			if (check_death(&table->philo[i]))
			{
				i = -1;
				while (++i < table->ph_threads + 2)
				{
					pthread_mutex_destroy(table->forks[i]);
				}
				return (1);
			}
		}
	}
}


