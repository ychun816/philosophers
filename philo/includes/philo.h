/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 14:47:59 by yilin             #+#    #+#             */
/*   Updated: 2025/01/15 20:27:15 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# include <sys/stat.h>
# include <sys/wait.h>
# include <pthread.h>

# define SUCCESS 0
# define FAILURE 1
# define FAILURE_VOID 2
# define BUILD_FAILURE -1

/************ STRUCTURE ************/

typedef struct  s_philo_data
{
    int philo_count;
    int eat;
    int die;
    int sleep;
    int max_meal;
    long long start_time;
    int should_end;
    int all_satisfied;
    int end_reason;
    t_mutex long_mutex;
    
    
       
}   t_philo_data;



typedef struct  s_philo
{
    int id;
    int dead;
    int meal_remain;
    long long
    
    t_philo_data    *content;
}   t_philo



/************ FUNCTIONS ************/


void *routine();
void *routine2();



#endif