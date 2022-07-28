/**
 * @file csv.h
 * @brief Header responsible for handling interactions with csv files
 * 
 */
#ifndef __CSV_H__
#define __CSV_H__
#include <stdio.h>
#include "utils.h"

/**
 * @brief Reads a single line from a csv file
 * 
 * @param file File that will be read
 * @param data Struct that will store the data
 * @return int Returns 1 if it detects a EOF, 0 otherwise
 */
int read_from_csv(FILE *file, registry_data_st *data);

#endif