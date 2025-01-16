#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <errno.h>
# include <limits.h>

# include <pthread.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/time.h>

/***** DEFINES *****/
# define SUCCESS 0
# define FAILURE 1


/***** STRUCTURES *****/
typedef enum e_state
{
    DEFAULT,    // Default state, before any action
    FORK,       // Picking up a fork
    EAT,        // Eating
    SLEEP,      // Sleeping
    THINK,      // Thinking
    DIED,       // Philosopher has died
    FULL   // Philosopher has finished all meals
}   t_state;

typedef struct s_fork
{
    pthread_mutex_t mutex;          // Mutex for the fork
}   t_fork;

typedef struct s_philo
{
    int             id;             // Philosopher ID
    int             eat_count;      // Number of meals eaten
    bool            inited;    // Initialization status
    t_state        state;          // Current action of the philosopher
    pthread_t       thread;         // Thread for the philosopher
    t_state          *data;          // Pointer to shared simulation data
    t_fork          *left_fork;     // Left fork assigned to this philosopher
    t_fork          *right_fork;    // Right fork assigned to this philosopher
    struct s_philo  *left_philo;    // Pointer to the left philosopher
    struct s_philo  *right_philo;   // Pointer to the right philosopher
}                   t_philo;

typedef struct s_table
{
    unsigned int    to_die;    // Time until a philosopher dies if not eating
    unsigned int    to_eat;    // Time a philosopher spends eating
    unsigned int    to_sleep;  // Time a philosopher spends sleeping
    int             nb_philo;   // Number of philosophers
    int             nb_must_eat; // Meals required to be satisfied
    bool            is_dead;        // Flag indicating if a philosopher has died
    bool            is_started;     // Flag to start the simulation
    unsigned long   time_started;   // Simulation start time
    pthread_mutex_t mutex;          // General mutex for shared resources
    pthread_mutex_t print_mutex;    // Mutex for printing actions
}   t_table;

/***** FUNCTIONS *****/
// void log_action(t_philo *philo);
// void    *philosopher_routine(void *arg)


#endif