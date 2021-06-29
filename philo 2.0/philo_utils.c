#include "philo.h"

long long get_time()
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void print_message(long long time, int id, char *str, t_philosopher *philosopher)
{
    pthread_mutex_lock(philosopher->print);
    printf("%llu philo %d %s\n", time, id, str);
    pthread_mutex_unlock(philosopher->print);
}

int     all_digits(char *str)
{
    int fl;
    int i;

    fl = 0;
    i = 0;
    while(str[i])
    {
        if (str[i] < 48 && str[i] > 57)
            fl = 1;
        i++;
    }
    return (fl);
}

int		ft_atoi(const char *str)
{
    long long int	a;
    int				i;
    int				mn;

    i = 0;
    a = 0;
    mn = 1;
    while ((str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
            || str[i] == '\f' || str[i] == '\r') && str[i] != '\0')
        i++;
    if (str[i] == '-')
        mn = -1;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i] >= 48 && str[i] <= 57 && str[i] != '\0')
    {
        a = a * 10 + (str[i] - 48) * mn;
        i++;
        if (a < 0 && mn > 0)
            return (-1);
        if (a > 0 && mn < 0)
            return (0);
    }
    return (a);
}

void free_forks(pthread_mutex_t **forks, int i)
{
    int k;

    k = 0;
    while(k<i)
    {
        free(forks[k]);
        k++;
    }
    free(forks);
}

void free_table(t_table *table)
{
    int i;

    i = 0;
    free(table->philosophers);
    free_forks(table->forks, table->ph_threads);
    free(table);
}
