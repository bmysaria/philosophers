#include "philo.h"

int check_death(t_philosopher *philosopher)
{
    pthread_mutex_lock(philosopher->check_meal);
    if(get_time() > philosopher->deadline)
    {
        pthread_mutex_lock(philosopher->print);
        printf("%llu philo %d has died\n", get_time() - philosopher->start, philosopher->id);
        printf("%llu\n", get_time()-philosopher->deadline);
        philosopher->table->firstdeath = 1;
        return (1);
    }
    pthread_mutex_unlock(philosopher->check_meal);
    if (philosopher->table->meals !=0 && philosopher->table->are_full == philosopher->table->meals)
    {
        printf("Ate %d times\n", philosopher->table->meals);
        return (2);
    }
    return (0);
}

void eat(t_philosopher *philosopher)
{
    pthread_mutex_lock(philosopher->left);
    print_message(get_time() - philosopher->start, philosopher->id, "has taken a fork", philosopher);
    pthread_mutex_lock(philosopher->right);
    print_message(get_time() - philosopher->start, philosopher->id, "has taken a fork", philosopher);
    print_message(get_time() - philosopher->start, philosopher->id, "is eating", philosopher);
    long long time = get_time();
    while(get_time() < time + philosopher->to_eat)
        usleep(100);
    pthread_mutex_lock(philosopher->check_meal);
    philosopher->deadline = get_time() + philosopher->to_die;
    pthread_mutex_unlock(philosopher->check_meal);
    pthread_mutex_unlock(philosopher->right);
    pthread_mutex_unlock(philosopher->left);
    if (philosopher->table->meals != 0)
    {
        if (philosopher->haseaten < philosopher->meals)
            philosopher->haseaten++;
        if (philosopher->haseaten == philosopher->meals && philosopher->isfull == 0) {
            philosopher->isfull = 1;
            philosopher->table->are_full++;
        }
    }
}


void ft_sleep(t_philosopher *philosopher)
{
    print_message(get_time() - philosopher->start, philosopher->id, "is sleeping", philosopher);
    long long time = get_time();
    while(get_time() <= time + philosopher->to_sleep)
        usleep(100);
}

void think(t_philosopher *philosopher)
{
    print_message(get_time() - philosopher->start, philosopher->id, "is thinking", philosopher);
}

void *lifetime(void *args)
{
    t_philosopher *philosopher;
    philosopher = (t_philosopher*)args;
    long long time;

    pthread_mutex_lock(philosopher->check_meal);
    philosopher->deadline = get_time() + philosopher->to_die;
    pthread_mutex_unlock(philosopher->check_meal);
    if (philosopher->id%2 != 0)
    {
        time = get_time();
        while(get_time() < time + philosopher->to_eat)
            usleep(100);
    }
    while(philosopher->table->firstdeath == 0)
    {
        eat(philosopher);
        ft_sleep(philosopher);
        think(philosopher);
    }
    return NULL;
}


int simulation(t_table *table, pthread_t *ph_t)
{
    int i;

    i = -1;
    while(++i < table->ph_threads)
    {
        pthread_create(&ph_t[i], NULL, &lifetime, &table->philosophers[i]);
    }
    int res = 0;
    while(1)
    {
        i = -1;
        while(++i < table->ph_threads)
        {
            res = check_death(&table->philosophers[i]);
            if (res == 1)
                return 1;
        }
    }
}

int threads(t_table *table)
{
    pthread_t *ph_t;
    int i;

    i = 0;
    ph_t = malloc(sizeof (pthread_t) * (table->ph_threads));
    if(!ph_t)
    {
        free_table(table);
        return(error(MALLOC));
    }
    table->start = get_time();
    while(i < table->ph_threads)
    {
        table->philosophers[i].last_meal = table->start;
        table->philosophers[i].deadline = table->start + table->to_die;
        table->philosophers[i].start = table->start;
        table->philosophers[i].meals = table->meals;
        table->philosophers[i].to_eat = table->to_eat;
        table->philosophers[i].to_die = table->to_die;
        table->philosophers[i].to_sleep = table->to_sleep;
        i++;
    }
    return (simulation(table, ph_t));
}