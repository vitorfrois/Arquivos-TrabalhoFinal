/**
 * @file database.h
 * @brief Functions to pre process functions 1-11
 */

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stdio.h>

/**
 * @brief Function 1. Reads a CSV and creates a new file with its content
 * 
 * @param type Type of file. 1 for fixed size and 2 for variable size
 * @param CSVFilename Name of CSV file
 * @param binFile Name of new file
 */
void create_table(char type, const char *CSVFileName, const char *binFileName);

/**
 * @brief Function 2. Prints whole binary file
 */
void select_from(FILE *binFile, char type);

/**
 * @brief Function 3. Select some registries from file and print them
 * 
 * @param binFile File
 */
void select_from_where(FILE *binFile, char type);

/**
* @brief Function 4. Select a registry with specified RRN and print it
 * Type 1 function.
 * 
 * @param binFile
 */
void select_from_RRN(FILE *binFile);

/**
 * @brief Function 5. Create a index file based on binFile
 * 
 * @param type Type of file. 1 for fixed size and 2 for variable size
 * @param binFile File with registries
 * @param indexFile Index File
 */
void create_index(char type, FILE *binFile, FILE *indexFile);

/**
 * @brief Function 6. Delete registries with specified fields
 * 
 * @param binFile binFile
 * @param indexFilename indexFile
 * @param type Type of file. 1 for fixed size and 2 for variable size
 */
void delete_from_where(FILE *binFile, const char *indexFilename, char type);

/**
 * @brief Function 7. Insert a new registry in binFile
 * 
 * @param binFile binFile
 * @param indexFilename indexFilename
 * @param type Type of file. 1 for fixed size and 2 for variable size
 */
void insert_into(FILE *binFile, const char *indexFilename, char type);

/**
 * @brief Function 8. Update fields of specified registries
 * 
 * @param binFile binFile
 * @param indexFilename indexFilename
 * @param type Type of file. 1 for fixed size and 2 for variable size
 */
void update_where(FILE *binFile, const char *indexFilename, char type);

/**
 * @brief Function 9. B-Tree index based search
 * 
 * @param binFile binFile
 * @param indexFile indexFile
 * @param type Type of file. 1 for fixed size and 2 for variable size
 */
void search_where_b(FILE *binFile, FILE *indexFile, char type);

/**
 * @brief Function 10. Insert a new registry in binFile and B-Tree
 * 
 * @param binFile binFile 
 * @param indexFile indexFile
 * @param type Type of file. 1 for fixed size and 2 for variable size
 */
void insert_into_b(FILE *binFile, FILE *indexFile, char type);

/**
 * @brief Function 11. Create a B-Tree index based on binary file
 * 
 * @param binFile binFile 
 * @param indexFile indexFile
 * @param type Type of file. 1 for fixed size and 2 for variable size
 */
void create_index_b(FILE *binFile, FILE *indexFile, char type);

#endif