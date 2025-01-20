/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/16 20:44:43 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** check if stop 
 * Lock the death_mutex to safely access feast_stop 
 * Get the current state of the simulation(feast)
 * @return
 * - true if the feast should stop
 * - false if it should continue
*/
bool    check_feast_stop(t_table *table)
{
    bool is_stop;

    pthread_mutex_lock(&table->stop_mutex);
    is_stop = table->feast_stop;
    pthread_mutex_unlock(&table->stop_mutex);
    return (is_stop);
}
/* int ver: 
int check_feast_stop(t_table *table)
{
    int is_stop;

    pthread_mutex_lock(&table->stop_mutex);
    is_stop = table->feast_stop;
    pthread_mutex_unlock(&table->stop_mutex);
    return (is_stop);  // Return value: 1 (true) if the feast should stop, 0 (false) if it should continue
}
*/


/** check dead 
 * This function checks if a philosopher has died.
 * The philosopher's life is determined by the time since their last meal.
 * 
*/
bool    check_has_dead(t_philo *philo)
{


    return ();
}

/** check  meals
 *  Check if all philosophers have eaten the required number of times (if must_eat_count is set)
 * 
 * 
*/
bool    check_has_allmeal(t_philo *philo)
{


    return ();
}

/** supervisor (another extra thread!)
 * main monitor function that runs in a separate thread!
 * Continuously check the status of the philos by calling check_death and check_meals for each philosopher and the entire table, respectively.
 * @note while (1)
 * Important to have while(1) -> Check the whole program simotaneously
 * @note usleep(1000)
 * - Prevent busy-waiting.
 * - Allow other threads to execute.
 * - Reduce CPU usage, making the program more efficient.
*/
void    *supervisor(void *arg)
{
    t_table *table;
    int i;
    bool is_the_end;

    table = (t_table *)arg;
    while (1)
    {        
        i = -1;
        while (++i < table->nb_philo)
        {
            if (check_has_dead(&table->philos[i]))//philos[i] is value, not pointer!
                return (NULL);
            if (check_has_allmeal(&table->philos[i])
            {
                if (check_feast_stop(table))
                {
                    is_the_end = true;
                    return (NULL);
                }
            }
        }
        usleep(1000);//notneccsary , but suggested!
    }
    return (NULL);
}