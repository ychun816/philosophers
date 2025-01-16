#include "philo.h"

int main(int ac, char *av[])
{
    (void)ac;
    (void)av;

    // Create and initialize a t_philo object
    t_philo philo;

    // Assuming you have a way to initialize all relevant fields in philo
    philo.id = 1;  // Set philosopher ID (example)
    philo.state = THINK;  // Set the initial state
    philo.eat_count = 0;  // Initialize the eat count
    philo.data = 0;  // Set data (you need to properly initialize this struct)
    philo.left_fork = 0;  // Initialize left fork
    philo.right_fork = 0;  // Initialize right fork

    // Pass the initialized philo object to philosopher_routine
    philosopher_routine(&philo);

    return (0);
}