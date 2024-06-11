// philo.c
#include "philo.h"

long	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_status(t_philosopher *philo, char *status)
{
	pthread_mutex_lock(&philo->table->write_mutex);
	printf("%ld %d %s\n", current_time() - philo->table->start_time, philo->id,
		status);
	pthread_mutex_unlock(&philo->table->write_mutex);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		print_status(philo, "is eating");
		philo->last_meal_time = current_time();
		usleep(philo->table->time_to_eat * 1000);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		print_status(philo, "is sleeping");
		usleep(philo->table->time_to_sleep * 1000);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

void	initialize_philosophers(t_table *table)
{
	for (int i = 0; i < table->number_of_philosophers; i++)
	{
		table->philosophers[i].id = i + 1;
		table->philosophers[i].meals_eaten = 0;
		table->philosophers[i].left_fork = &table->forks[i];
		table->philosophers[i].right_fork = &table->forks[(i + 1)
			% table->number_of_philosophers];
		table->philosophers[i].table = table;
	}
}

void	start_simulation(t_table *table)
{
	table->start_time = current_time();
	for (int i = 0; i < table->number_of_philosophers; i++)
	{
		pthread_create(&table->philosophers[i].thread, NULL,
			philosopher_routine, &table->philosophers[i]);
	}
	for (int i = 0; i < table->number_of_philosophers; i++)
	{
		pthread_join(table->philosophers[i].thread, NULL);
	}
}

void	cleanup(t_table *table)
{
	for (int i = 0; i < table->number_of_philosophers; i++)
	{
		pthread_mutex_destroy(&table->forks[i]);
	}
	free(table->forks);
	free(table->philosophers);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc != 5 && argc != 6)
	{
		printf("Usage:
			%s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n",
			argv[0]);
		return (1);
	}
	table.number_of_philosophers = atoi(argv[1]);
	table.time_to_die = atol(argv[2]);
	table.time_to_eat = atol(argv[3]);
	table.time_to_sleep = atol(argv[4]);
	table.number_of_times_each_philosopher_must_eat = (argc == 6) ? atoi(argv[5]) :
		-1;
	table.all_philosophers_ate = 0;
	table.forks = malloc(sizeof(pthread_mutex_t)
			* table.number_of_philosophers);
	table.philosophers = malloc(sizeof(t_philosopher)
			* table.number_of_philosophers);
	for (int i = 0; i < table.number_of_philosophers; i++)
	{
		pthread_mutex_init(&table.forks[i], NULL);
	}
	pthread_mutex_init(&table.write_mutex, NULL);
	initialize_philosophers(&table);
	start_simulation(&table);
	cleanup(&table);
	return (0);
}
