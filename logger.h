/**
 * @file logger.h
 *
 * @brief Functions for logging and checking function calls.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timeUNIXConversion.h"

/**
 * @brief Record the current date and time of a function call.
 *
 * This function records the current date and time when it is called and
 * stores it in a log file named "functionCalls.log". The date and time are
 * formatted as "YYYY/MM/DD HH:MM:SS".
 */
void record_module_call();

/**
 * @brief Check if 7 days have passed since the last function call.
 *
 * This function reads the last recorded function call date from the log file,
 * converts it to Unix time, and checks if 7 days (604800 seconds) have passed
 * since that date. If the log file doesn't exist or an error occurs, it returns
 * 1 to indicate that the function can be called.
 *
 * @return 1 if 7 days have passed or an error occurs, 0 otherwise.
 */
int record_checker();

#endif // LOGGER_H
