#include "philo.h"

status void log_action(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->print_mutex);

    const char *messages[] = 
    {
        "is doing nothing",    // DEFAULT
        "has taken a fork",    // FORK
        "is eating",           // EAT
        "is sleeping",         // SLEEP
        "is thinking",         // THINK
        "has died",            // DIED
        "is satisfied"         // SATISFIED
    };

    printf("Time %ld: Philosopher %d %s\n",
        current_time_in_ms() - philo->data->time_started,
        philo->id,
        messages[philo->state]);

    pthread_mutex_unlock(&philo->data->print_mutex);
}

void *philosopher_routine(void *arg)
{
    (void)arg;  // Suppress unused parameter warning

    t_philo *philo = ...;  // Initialize philo as needed, probably passed in arg

    while (!philo->data->is_dead)
    {
        // Thinking
        philo->state = THINK;
        log_action(philo);

        // Picking up forks
        pthread_mutex_lock(&philo->left_fork->mutex);
        philo->state = FORK;
        log_action(philo);

        pthread_mutex_lock(&philo->right_fork->mutex);
        philo->state = FORK;
        log_action(philo);

        // Eating
        philo->state = EAT;
        philo->eat_count++;
        philo->last_meal_time = current_time_in_ms(); // Track last meal time
        log_action(philo);

        usleep(philo->data->to_eat * 1000);

        pthread_mutex_unlock(&philo->right_fork->mutex);
        pthread_mutex_unlock(&philo->left_fork->mutex);

        // Sleeping
        philo->state = SLEEP;
        log_action(philo);
        usleep(philo->data->to_sleep * 1000);
    }

    return NULL;  // Ensure the function returns a void pointer
}