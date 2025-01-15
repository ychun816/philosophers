/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 14:47:59 by yilin             #+#    #+#             */
/*   Updated: 2025/01/15 17:41:57 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo.h"

int x = 2;
void *routine()
{
    x += 5;
    sleep(2);
    printf("value of x: %d\n", x);
    return (NULL);
}
void *routine2()
{
    x++;
    sleep(2);
    printf("value of x: %d\n", x);
    return (NULL);
}


//threads
int main(int ac, char *av[])
{ 
    (void)ac;
    (void)av;
    
    pthread_t   t1;
    pthread_t   t2;
    if (pthread_create(&t1, NULL, &routine, NULL))
        return 1;
    if (pthread_create(&t2, NULL, &routine2, NULL))
        return 2;
    if (pthread_join(t1, NULL))
        return 3;
    if (pthread_join(t2, NULL))
        return 4;
    return (0);    
}

//process
// int main(int ac, char *av[])
// {
//     (void)ac;
//     (void)av;
//     int x = 2;
//     int  pid = fork();

//     if (pid == -1)
//         return 1;
//     printf("process id: %d\n", getpid());

//     if (pid == 0)
//         x++;
//     sleep (2);
//     printf("value of x: %d\n", x);

//     if (pid != 0)
//         wait(NULL);

//     return 0;
// }