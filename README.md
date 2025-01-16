## Brief for Philo
This is the Dining Philosophers problem, a classic computer science problem that demonstrates synchronization issues and deadlock handling in concurrent programming. 

This program simulates philosophers sitting at a round table where each philosopher alternates between thinking and eating. Each philosopher needs two forks to eat, and forks are shared between adjacent philosophers. The challenge is to prevent deadlocks (where everyone is waiting for resources) and ensure fair resource allocation.

## Key components
+ Each philosopher is a separate thread
+ Forks are represented by mutexes (mutual exclusion locks)
+ A monitoring system checks for deaths or completion
+ A structured approach to prevent deadlocks by implementing fork priority

## Steps for Implementation
1. Set up structures
2. Get the basic structure working (parsing, initialization, cleanup)
3. Then implement the philosopher routine (thinking, eating, sleeping)
4. Add the monitoring system to check for deaths
5. Finally, implement deadlock prevention
