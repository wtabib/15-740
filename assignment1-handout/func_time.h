/* function timer */

#ifndef FUNC_TIME_H
#define FUNC_TIME_H

typedef void (*test_funct)(void); 
long double func_time(test_funct P, long double E);
long double func_time_hw(test_funct P, long double E);

#endif /* FUNC_TIME_H */
