
// philo.h
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>
# include <string.h>

typedef enum e_state 
{
    TAKING_FORK,
    EATING,
    SLEEPING,
    THINKING,
    DIED
} t_state;

typedef struct s_philo 
{
    int             id;
    int             eat_count;
    unsigned long   last_meal_time;
    pthread_t       thread;
    pthread_mutex_t meal_mutex;
    struct s_table  *table;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
} t_philo;

typedef struct s_table 
{
    int             philo_count;
    unsigned long   time_to_die;
    unsigned long   time_to_eat;
    unsigned long   time_to_sleep;
    int             must_eat_count;
    bool            simulation_end;
    unsigned long   start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t death_mutex;
    t_philo         *philosophers;
} t_table;

// utils.c
unsigned long   get_time(void);
void            print_state(t_philo *philo, t_state state);
int             ft_atoi(const char *str);
void            smart_sleep(unsigned long duration, t_table *table);

// init.c
int             init_table(t_table *table, int argc, char **argv);
void            cleanup(t_table *table);

// monitor.c
void            *monitor_routine(void *arg);

// routine.c
void            *philosopher_routine(void *arg);

#endif

// utils.c
unsigned long get_time(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void print_state(t_philo *philo, t_state state)
{
    unsigned long   time;
    bool           is_dead;

    pthread_mutex_lock(&philo->table->print_mutex);
    pthread_mutex_lock(&philo->table->death_mutex);
    is_dead = philo->table->simulation_end;
    pthread_mutex_unlock(&philo->table->death_mutex);
    
    if (!is_dead || state == DIED)
    {
        time = get_time() - philo->table->start_time;
        if (state == TAKING_FORK)
            printf("%lu %d has taken a fork\n", time, philo->id);
        else if (state == EATING)
            printf("%lu %d is eating\n", time, philo->id);
        else if (state == SLEEPING)
            printf("%lu %d is sleeping\n", time, philo->id);
        else if (state == THINKING)
            printf("%lu %d is thinking\n", time, philo->id);
        else if (state == DIED)
            printf("%lu %d died\n", time, philo->id);
    }
    pthread_mutex_unlock(&philo->table->print_mutex);
}

void smart_sleep(unsigned long duration, t_table *table)
{
    unsigned long start;
    unsigned long current;
    bool         end;

    start = get_time();
    while (1)
    {
        pthread_mutex_lock(&table->death_mutex);
        end = table->simulation_end;
        pthread_mutex_unlock(&table->death_mutex);
        if (end)
            break;
        current = get_time();
        if ((current - start) >= duration)
            break;
        usleep(500);
    }
}

int ft_atoi(const char *str)
{
    int result;
    int sign;
    
    result = 0;
    sign = 1;
    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return (result * sign);
}

// init.c
static int init_mutexes(t_table *table)
{
    int i;

    i = 0;
    table->forks = malloc(sizeof(pthread_mutex_t) * table->philo_count);
    if (!table->forks)
        return (1);
    while (i < table->philo_count)
    {
        if (pthread_mutex_init(&table->forks[i], NULL))
            return (1);
        i++;
    }
    if (pthread_mutex_init(&table->print_mutex, NULL))
        return (1);
    if (pthread_mutex_init(&table->death_mutex, NULL))
        return (1);
    return (0);
}

static int init_philosophers(t_table *table)
{
    int i;

    i = 0;
    table->philosophers = malloc(sizeof(t_philo) * table->philo_count);
    if (!table->philosophers)
        return (1);
    while (i < table->philo_count)
    {
        table->philosophers[i].id = i + 1;
        table->philosophers[i].eat_count = 0;
        table->philosophers[i].last_meal_time = get_time();
        table->philosophers[i].table = table;
        table->philosophers[i].left_fork = &table->forks[i];
        table->philosophers[i].right_fork = &table->forks[(i + 1) % table->philo_count];
        if (pthread_mutex_init(&table->philosophers[i].meal_mutex, NULL))
            return (1);
        i++;
    }
    return (0);
}

int init_table(t_table *table, int argc, char **argv)
{
    table->philo_count = ft_atoi(argv[1]);
    table->time_to_die = ft_atoi(argv[2]);
    table->time_to_eat = ft_atoi(argv[3]);
    table->time_to_sleep = ft_atoi(argv[4]);
    table->must_eat_count = -1;
    if (argc == 6)
        table->must_eat_count = ft_atoi(argv[5]);
    table->simulation_end = false;
    if (table->philo_count < 1 || table->time_to_die < 0 || 
        table->time_to_eat < 0 || table->time_to_sleep < 0)
        return (1);
    if (init_mutexes(table))
        return (1);
    if (init_philosophers(table))
        return (1);
    return (0);
}

void cleanup(t_table *table)
{
    int i;

    i = 0;
    while (i < table->philo_count)
    {
        pthread_mutex_destroy(&table->forks[i]);
        pthread_mutex_destroy(&table->philosophers[i].meal_mutex);
        i++;
    }
    pthread_mutex_destroy(&table->print_mutex);
    pthread_mutex_destroy(&table->death_mutex);
    free(table->forks);
    free(table->philosophers);
}

// monitor.c
static bool check_death(t_philo *philo)
{
    unsigned long current_time;
    unsigned long last_meal;
    bool         is_dead;

    pthread_mutex_lock(&philo->meal_mutex);
    last_meal = philo->last_meal_time;
    pthread_mutex_unlock(&philo->meal_mutex);
    
    current_time = get_time();
    pthread_mutex_lock(&philo->table->death_mutex);
    is_dead = philo->table->simulation_end;
    if (!is_dead && (current_time - last_meal) > philo->table->time_to_die)
    {
        philo->table->simulation_end = true;
        pthread_mutex_unlock(&philo->table->death_mutex);
        print_state(philo, DIED);
        return (true);
    }
    pthread_mutex_unlock(&philo->table->death_mutex);
    return (false);
}

static bool check_meals(t_table *table)
{
    int i;
    int finished;

    if (table->must_eat_count == -1)
        return (false);
    
    i = 0;
    finished = 0;
    while (i < table->philo_count)
    {
        pthread_mutex_lock(&table->philosophers[i].meal_mutex);
        if (table->philosophers[i].eat_count >= table->must_eat_count)
            finished++;
        pthread_mutex_unlock(&table->philosophers[i].meal_mutex);
        i++;
    }
    return (finished == table->philo_count);
}

void *monitor_routine(void *arg)
{
    t_table *table;
    int     i;
    bool    end;

    table = (t_table *)arg;
    while (1)
    {
        i = 0;
        while (i < table->philo_count)
        {
            if (check_death(&table->philosophers[i]))
                return (NULL);
            if (check_meals(table))
            {
                pthread_mutex_lock(&table->death_mutex);
                table->simulation_end = true;
                pthread_mutex_unlock(&table->death_mutex);
                return (NULL);
            }
            i++;
        }
        usleep(1000);
    }
    return (NULL);
}

// routine.c
static void take_forks(t_philo *philo)
{
    if (philo->id % 2)
    {
        pthread_mutex_lock(philo->left_fork);
        print_state(philo, TAKING_FORK);
        pthread_mutex_lock(philo->right_fork);
        print_state(philo, TAKING_FORK);
    }
    else
    {
        pthread_mutex_lock(philo->right_fork);
        print_state(philo, TAKING_FORK);
        pthread_mutex_lock(philo->left_fork);
        print_state(philo, TAKING_FORK);
    }
}

static void eat(t_philo *philo)
{
    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal_time = get_time();
    philo->eat_count++;
    pthread_mutex_unlock(&philo->meal_mutex);
    
    print_state(philo, EATING);
    smart_sleep(philo->table->time_to_eat, philo->table);
    
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

void *philosopher_routine(void *arg)
{
    t_philo *philo;
    bool    end;

    philo = (t_philo *)arg;
    if (philo->id % 2)
        usleep(1000);

    while (1)
    {
        pthread_mutex_lock(&philo->table->death_mutex);
        end = philo->table->simulation_end;
        pthread_mutex_unlock(&philo->table->death_mutex);
        if (end)
            break;

        take_forks(philo);
        eat(philo);
        
        print_state(philo, SLEEPING);
        smart_sleep(philo->table->time_to_sleep, philo->table);
        
        print_state(philo, THINKING);
    }
    return (NULL);
}

// main.c
static int start_simulation(t_table *table)
{
    int         i;
    pthread_t   monitor;

    table->start_time = get_time();
    
    i = 0;
    while (i < table->philo_count)
    {
        if (pthread_create(&table->philosophers[i].thread, NULL, 
            philosopher_routine, &table->philosophers[i]))
            return (1);
        i++;
    }

    if (pthread_create(&monitor, NULL, monitor_routine, table))
        return (1);

    i = 0;
    while (i < table->philo_count)
    {
        if (pthread_join(table->philosophers[i].thread, NULL))
            return (1);
        i++;
    }
    
    if (pthread_join(monitor, NULL))
        return (1);
    
    return (0);
}

int main(int argc, char **argv)
{
    t_table table;

    if (argc != 5 && argc != 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die ", argv[0]);
        printf("time_to_eat time_to_sleep ");
        printf("[number_of_times_each_philosopher_must_eat]\n");
        return (1);
    }

    memset(&table, 0, sizeof(t_table));
    if (init_table(&table, argc, argv))
    {
        printf("Error: initialization failed\n");
        return (1);
    }

    if (start_simulation(&table))
    {
        printf("Error: simulation failed\n");
        cleanup(&table);
        return (1);
    }

    cleanup(&table);
    return (0);
}
// Made with
// Artifacts are user-generated and may contain unverified or potentially unsafe content.
// Remix Artifact