/**
 * @file file_t2.h
 * @brief Functions related to type 2 registries
 */
#ifndef __FILE_T2_H__
#define __FILE_T2_H__

#include <stdio.h>
#include "../includes/csv.h"

typedef long long int lli;

/**
 * @brief Creates a file of type 2 and writes it's header
 * 
 * @param filename Name of file to be created
 * @return FILE* Pointer to file
 */
FILE *create_file_t2(const char *filename);

/**
 * @brief Calculate size of registry
 * 
 * @param data Registry
 * @return int Size of Registry
 */
int calculate_registry_size(registry_data_st data);

/**
 * @brief Adds a registry to a type 2 file
 * 
 * @param file File of type 2
 * @param data Registry data
 */
void add_registry_t2(FILE *file, registry_data_st data, int additionalSize);

/**
 * @brief Updates the byte offset field of a header from a type 2 file
 * 
 * @param file File to be updated
 * @param byteOffset New byte offset
 */
void update_byte_offset(FILE *file, lli byteOffset);

/**
 * @brief Reads a variable size registry of specified size
 * 
 * @param file File that contains the registry
 * @param registrySize size of registry to be read
 * @return csv_data_st Read data of registry
 */
registry_data_st read_registry_t2(FILE* file);

/**
 * @brief Read and print all registries in a type 2 file
 * 
 * @param file File to be read
 */
void read_file_t2(FILE *file);

/**
 * @brief Adequately closes a type 2 file
 * 
 * @param file File to be closed
 */
void close_file_t2(FILE *file);

/**
 * @brief Search and print for registries in file with Field f equivalency
 * 
 * @param file File with registries
 * @param f Array of structs to check
 * @param n Size of array f
 */
void search_where_t2(FILE *file, Field *f, int n);

/**
 * @brief Inserts a new registry in file
 * 
 * @param binFile Type 2 file
 * @param data New registry data
 */
long int insert_into_t2(FILE *binFile, registry_data_st data);

/**
 * @brief Search and print registry with specified byte offset
 * 
 * @param file File to be searched
 * @param byteOffset Byte offset of registry
 */
void search_byteOffset(FILE *file, long int byteOffset);

#endif