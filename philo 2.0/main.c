#include "philo.h"

int	error(int num)
{
	if (num == INVALID_ARGS)
		printf("Error: invalid number of arguments\n");
	else if (num == MALLOC)
		printf("Error: allocation failed\n");
	return (-1);
}

int	parse_args(t_table *table, int argc, char **argv)
{
	table->ph_threads = ft_atoi(argv[1]);
	table->to_die = ft_atoi(argv[2]);
	table->to_eat = ft_atoi(argv[3]);
	table->to_sleep = ft_atoi(argv[4]);
	if (table->ph_threads < 0 || table->to_die < 0 || table->to_eat < 0
		|| table->to_sleep < 0)
		return (error(INVALID_ARGS));
	if (argc == 6)
	{
		table->meals = ft_atoi(argv[5]);
		if (table->meals < 0)
			return (error(INVALID_ARGS));
	}
	else
		table->meals = 0;
	return (0);
}

int	check_args(int argc, char **argv)
{
	if (all_digits(argv[1]) || all_digits(argv[2]) || all_digits(argv[3])
		|| all_digits(argv[4]))
		return (error(INVALID_ARGS));
	if (argc == 6)
		if (all_digits(argv[5]))
			return (error(INVALID_ARGS));
	return (0);
}

t_table	*create_table(int argc, char **argv)
{
	t_table	*table;

	if (check_args(argc, argv))
		return (NULL);
	table = malloc(sizeof(t_table));
	if (!table)
	{
		error(MALLOC);
		return (NULL);
	}
	if (parse_args(table, argc, argv))
	{
		free(table);
		error(INVALID_ARGS);
		return (NULL);
	}
	return (table);
}

int	main(int argc, char **argv)
{
	t_table	*table;

	if (argc < 5 || argc > 6)
		return (error(INVALID_ARGS));
	table = create_table(argc, argv);
	if (!table)
		return (-1);
	if (serve_the_table(table))
		return (-1);
	return (threads(table));
}
