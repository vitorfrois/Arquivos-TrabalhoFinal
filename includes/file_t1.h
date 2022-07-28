/**
 * @file file_t1.h
 * @brief Functions related to type 1 registries
 */
#ifndef __FILE_T1_H__
#define __FILE_T1_H__
#include <stdio.h>
#include "csv.h"
#include "index_t1.h"

/**
 * @brief Creates a file of type 1 and writes it's header
 * 
 * @param filename Name of file to be created
 * @return FILE* Pointer to file
 */
FILE *create_file_t1(const char *filename);

/**
 * @brief Adds a registry to a type 1 file
 * 
 * @param file File of type 1
 * @param data Registry data
 */
void add_registry_t1(FILE *file, registry_data_st data);

/**
 * @brief Reads a variable size registry of specified size
 * 
 * @param file File that contains the registry
 * @param registrySize size of registry to be read
 * @return csv_data_st Read data of registry
 */
registry_data_st read_registry_t1(FILE *file);

/**
 * @brief Read and print all registries in a type 1 file
 * 
 * @param file File to be read
 */
void read_file_t1(FILE *file);

/**
 * @brief Search and print for registries in file with Field f equivalency
 * 
 * @param file File with registries
 * @param f Array of structs to check
 * @param n Size of array f
 */
void search_where_t1(FILE *file, Field *f, int n);

/**
 * @brief Adequately closes a type 1 file
 * 
 * @param file File to be closed
 */
void close_file_t1(FILE *file);

/**
 * @brief Search and print registry with specified RRN
 * 
 * @param file File to be searched
 * @param rrn RRN of registry
 */
void search_rrn(FILE *file, int rrn);

/**
 * @brief Updates the RRN field of a header from a type 1 file
 * 
 * @param file File to be updated
 * @param byteOffset New RRN
 */
void update_RRN(FILE *file, int finalRRN);

/**
 * @brief Insert new RRN in deleted stack
 * 
 * @param binFile Stack file
 * @param rrn RRN to be deleted
 */
void push_deleted_stack(FILE *binFile, int rrn);

/**
 * @brief Updates the fields of a registry
 * 
 * @param newReg Registry to be updated
 * @param update Array of new values
 * @param n Size of array
 * @return registry_data_st New created registry
 */
registry_data_st update_registry_t1(registry_data_st newReg, Field *update, int n);

/**
 * @brief Inserts a new registry in file
 * 
 * @param binFile Type 1 file
 * @param data New registry data
 */
int insert_into_t1(FILE *binFile, registry_data_st data);

#endif