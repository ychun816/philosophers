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

/**
 * - Parse args
 * - Initialize structures (table, philo)
 * - Start simulation (create threads)
 * - Join threads
 * - Cleanup
}
 */
int main(int ac, char *av[])
{
    t_table *table;
    int i;

    i = 0;

    if (ac < 5 || ac > 6)
        return (ft_putstr_fd("Invalid Arguments\n"), FAILURE);
    ft_memset(&table, 0, sizeof(t_table));
    // if (parse_n_check_args(ac, av, table))
    //     ft_putstr_fd("Invalid Arguments\n");
    if (init_table(ac, av, &table))
        return (ft_putstr_fd("Initialization Failed\n"), FAILURE);
    //start table
    if (start_feast(&table))
    {
        ft_putstr_fd("Similation Failed\n");
        cleanup_table(&table);
        return (FAILURE);   
    }
    cleanup_table(&table);
    return (SUCCESS);
}

/** parse n check args 
 * use atoi to initiate structure
*/
// int parse_n_check_args(int ac, char *av[], t_table *table)
// {
// }

/** init table */
int init_table(int ac, char *av[], t_table *table)
{
    
}


/** init philo */
int start_feast(t_table *table)
{
    return (SUCCESS);
}