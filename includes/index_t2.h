/**
 * @file index_t2.h
 * @brief Functions to deal with type 1 index files
 */
#ifndef __INDEX_T2_H__
#define __INDEX_T2_H__

#include <stdio.h>
#include "../includes/utils.h"

typedef long long int lli;

/**
 * @brief Struct that represents a single item data in the index
 */
typedef struct index_data_s2{
    int id;
    long long int byteOffset;
}index_data_st2;

/**
 * @brief Struct that stores a array of index_data_st1 and it size
 */
typedef struct index_s2{
    index_data_st2 *data;
    int size;
}index_st2;

/**
 * @brief Create a index file based on binFile
 * 
 * @param binFile binFile 
 * @param indexFile indexFile
 */
void create_index_t2(FILE *dataFile, FILE *indexFile);

/**
 * @brief Delete registries with specified fields
 * 
 * @param file binFile
 * @param f Array of structs to check
 * @param n Size of array
 */
void delete_where_t2(FILE *binFile, index_st2 index, Field *search, int n);

/**
 * @brief Updates a single registry
 * 
 * @param newReg Registry to be updates
 * @param update Field of new values
 * @param n Size of update
 * @return registry_data_st Updated registry 
 */
registry_data_st update_registry_t2(registry_data_st newReg, Field *update, int n);

/**
 * @brief Execute binary search for id at index_st
 * 
 * @param index Struct to be searched
 * @param id ID to be found
 * @return int Found registry index
 */
int binary_search_t2(index_st2 index, int id);

/**
 * @brief Read all index data in index file
 * 
 * @param indexFile Index file
 * @return index_st1 Returns a array of index data that was read
 */
index_st2 get_index_t2(FILE *indexFile);

/**
 * @brief Write index_st1 values at index file
 * 
 * @param indexFile index file
 * @param index Array of index data
 */
void write_index_file_t2(FILE *indexFile, index_st2 index);

/**
 * @brief Function 8-1. Update fields of specified registries
 * 
 * @param binFile binFile
 * @param indexFile indexFile
 * @param search Array of fields present in registries that should be updated
 * @param x Size of search
 * @param update Array of fields to update
 * @param y Size of update
 */
void update_where_t2(FILE *binFile, index_st2 index, Field *search, int x, Field *update, int y);
#endif