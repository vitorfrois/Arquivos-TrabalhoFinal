/**
 * @file index_t1.h
 * @brief Functions to deal with type 1 index files
 */
#ifndef __INDEX_T1_H__
#define __INDEX_T1_H__

#include <stdlib.h>
#include "utils.h"
#include <stdio.h>

#define DATA_HEADER_SIZE 182

/**
 * @brief Struct that represents a single item data in the index
 */
typedef struct index_data_s1{
    int id;
    int rrn;
}index_data_st1;

/**
 * @brief Struct that stores a array of index_data_st1 and it size
 */
typedef struct index_s1{
    index_data_st1 *data;
    int size;
}index_st1;

/**
 * @brief Create a index file based on binFile
 * 
 * @param binFile binFile 
 * @param indexFile indexFile
 */
void create_index_t1(FILE *binFile, FILE *indexFile);

/**
 * @brief Write index_st1 values at index file
 * 
 * @param indexFile index file
 * @param index Array of index data
 */
void write_index_file_t1(FILE *indexFile, index_st1 index);

/**
 * @brief Read all index data in index file
 * 
 * @param indexFile Index file
 * @return index_st1 Returns a array of index data that was read
 */
index_st1 get_index_t1(FILE *indexFile);

/**
 * @brief Delete registries with specified fields
 * 
 * @param file binFile
 * @param f Array of structs to check
 * @param n Size of array
 */
void delete_where_t1(FILE *file, index_st1 index, Field *f, int n);

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
void update_where_t1(FILE *binFile, index_st1 index, Field *search, int x, Field *update, int y);

/**
 * @brief Execute binary search for id at index_st
 * 
 * @param index Struct to be searched
 * @param id ID to be found
 * @return int Found registry index
 */
int binary_search_t1(index_st1 index, int id);

#endif