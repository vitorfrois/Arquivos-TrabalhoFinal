/**
 * @file utils.h
 * @brief Common functions across files in project
 */
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../includes/myString.h"

#define ARR_SIZE(x)  (sizeof(x) / sizeof((x)[0]))

/**
 * @brief Struct that stores registry data
 */
typedef struct registry_data_s{
    int id;
    int year;
    char city[50];
    int amount;
    char initials[5];
    char brand[50];
    char model[50];
}registry_data_st;

/**
 * @brief Struct that stores searching data
 *
 * The fields of this struct are strings. 
 * Name stores name of the field and Value its value.
 */
typedef struct WField{
    char *name;
    char *value;
}Field;

/**
 * @brief Check if the fields of struct f exists in struct registry 
 * 
 * @param registry Struct that stores registry data
 * @param f Array of searching values
 * @param n Size of array f
 * @return int 1 if all fields exists, 0 else.
 */
int compare_registry(registry_data_st registry, Field *f, int n);

/**
 * @brief Prints a registry
 * 
 * @param registry Registry to be printed
 */
void print_registry(registry_data_st registry);

/**
 * @brief Open a binary file in "rb" mode
 * This functino prints a error message when the file is not properly opened
 * 
 * @param filename File to be opened
 * @return FILE* Pointer to the file
 */
FILE *open_file_rb(const char *filename);

/**
 * @brief Open a binary file in "r+b" mode
 * This functino prints a error message when the file is not properly opened
 * 
 * @param filename File to be opened
 * @return FILE* Pointer to the file
 */
FILE *open_file_rplusb(const char *filename);

/**
 * @brief Open a binary file in "wb" mode
 * This functino prints a error message when the file is not properly opened
 * 
 * @param filename File to be opened
 * @return FILE* Pointer to the file
 */
FILE *open_file_wb(const char *filename);

void readline(char* string);
void binarioNaTela(const char *nomeArquivoBinario);
char *scan_quote_string();

/**
 * @brief Check file status
 * 
 * @return If the file is corrupted, returns 0. Else, 1
 */
int check_status(FILE*);

/**
 * @brief Logically deletes a registry
 * The pointer should be at the beggining of the registry
 * 
 * @param FILE* Binary file with the registry to be deleted
 * @param int Size of the registry
 */
void logical_deletion(FILE*, int);

/**
 * @brief Search for "id" in "Name" field at struct array "f"
 Procura se existe um campo de Name == id no array de structs "f"
 * 
 * @param f Struct array
 * @param n Size of "f"
 * @return int Returns index of found field. If the "id" field does not exists, returns -1
 */
int search_field_id(Field* f, int n);

/**
 * @brief Read a array of Field from stdin
 * 
 * @param x Size of the array
 * @return Field* Array of Field
 */
Field *get_field(int x);

/**
 * @brief Free an array of Field
 * 
 * @param f Array of Field
 * @param x Size of array
 */
void free_field(Field *f, int x);

registry_data_st read_registry_line();

#endif