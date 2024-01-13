/**
 * @file timeUNIXConversion.h
 *
 * @brief Functions for converting timestamps to Unix time.
 */

#ifndef TIME_UNIX_CONVERSION_H
#define TIME_UNIX_CONVERSION_H

#define _XOPEN_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Convert a timestamp to Unix time.
 *
 * This function converts the timestamp to Unix time.
 * If successful, it returns the Unix time as a string; otherwise, it returns NULL.
 *
 * @param formattedDate The input timestamp or a dynamic date specifier.
 * @return A string representing Unix time or NULL on failure.
 */
char *date_unix_conversion(const char *formattedDate);

#endif