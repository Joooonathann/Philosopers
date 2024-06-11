// philo.h
#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_philosopher {
    int id;
    pthread_t thread;
    long last_meal_time;
    int meals_eaten;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct s_table *table;
} t_philosopher;

typedef struct s_table {
    int number_of_philosophers;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
    int all_philosophers_ate;
    long start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t write_mutex;
    t_philosopher *philosophers;
} t_table;

long current_time(void);
void initialize_philosophers(t_table *table);
void *philosopher_routine(void *arg);
void start_simulation(t_table *table);
void cleanup(t_table *table);

#endif
