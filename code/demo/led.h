#ifndef LED_H
#define LED_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sys/gpiofd.h>

#include <pthread.h>

using namespace std;

#define DIR_PATH "/dev/gpiofd"
#define LED_SMART210(x) (8 * 23 + x-1)
#define KEY_SMART210(x) (8 * 19 + x-1)

extern pthread_mutex_t lock;
extern int cancel_flag;
typedef struct Board_Num{
    int led;
    int key;
}board_num_in;



void Dir_gpio();
void *gpio_out(void *arg);
void *gpio_in(void *arg);
#endif // LED_H

