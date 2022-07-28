/**
 * @file main.c
 * @author Murillo Moraes Martins
 * @author Vítor Amorim Fróis
 * @brief File Organization final assignment 
 * @version 4.0
 * @date 2022-07
 * 
 */

#include "includes/file_t1.h"
#include "includes/file_t2.h"
#include "includes/myString.h"
#include "includes/database.h"
#include "includes/utils.h"
#include "includes/btree.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CREATE_TABLE 1
#define SELECT 2
#define SELECT_WHERE 3
#define SEARCH_RRN 4
#define CREATE_INDEX 5
#define DELETE_WHERE 6
#define INSERT_INTO 7
#define UPDATE_WHERE 8
#define CREATE_INDEX_B 9
#define SEARCH_WHERE_B 10
#define INSERT_INTO_B 11

int main(){
    int function;
    scanf("%d ", &function);
    char *fileType = readString();
    char type;
    
    if(strcmp(fileType, "tipo1") == 0){
        type = '1';
    }else if(strcmp(fileType, "tipo2") == 0){
        type = '2';
    }else{
        printf("Falha no processamento do arquivo.");
        return 1;
    }

    char *binFilename = NULL, *indexFilename = NULL, *CSVFileName = NULL;
    FILE *binFile = NULL, *indexFile = NULL;

    if(function == CREATE_TABLE){
        char *CSVFileName = readString();
        binFilename = readString();
        create_table(type, CSVFileName, binFilename);
        free(CSVFileName);
    }

    if(function == SELECT){
        binFilename = readString();
        binFile = open_file_rb(binFilename);
        if(!binFile) return 1;
        select_from(binFile, type);
    }

    if(function == SELECT_WHERE){
        binFilename = readString();
        binFile = open_file_rb(binFilename);
        if(!binFile) return 1;
        select_from_where(binFile, type);
    }

    if(function == SEARCH_RRN){
        binFilename = readString();
        binFile = open_file_rb(binFilename);
        if(!binFile) return 1;
        if(type == '1')
            select_from_RRN(binFile);
        else
            printf("Falha no processamento do arquivo.\n");
    }

    if(function == CREATE_INDEX){
        binFilename = readString();
        indexFilename = readString();
        binFile = open_file_rb(binFilename);
        if(!binFile) return 1;
        indexFile = open_file_rplusb(indexFilename);
        if(!indexFile) return 1;
        create_index(type, binFile, indexFile);    
    }

    if(function == DELETE_WHERE){
        binFilename = readString();
        indexFilename = readString();
        binFile = open_file_rplusb(binFilename);
        if(!binFile) return 1;
        indexFile = open_file_rplusb(indexFilename);
        if(!indexFile) return 1;
        delete_from_where(binFile, indexFilename, type);
    }
    

    if(function == INSERT_INTO){
        binFilename = readString();
        indexFilename = readString();
        binFile = open_file_rplusb(binFilename);
        if(!binFile) return 1;
        indexFile = open_file_rplusb(indexFilename);
        if(!indexFile) return 1;
        insert_into(binFile, indexFilename, type);
    }

    if(function == UPDATE_WHERE){
        binFilename = readString();
        indexFilename = readString();
        binFile = open_file_rplusb(binFilename);
        if(!binFile) return 1;
        indexFile = open_file_rplusb(indexFilename);
        if(!indexFile) return 1;
        update_where(binFile, indexFilename, type);
    }
    
    if(function == CREATE_INDEX_B){
        binFilename = readString();
        indexFilename = readString();
        binFile = open_file_rplusb(binFilename);
        if(!binFile) return 1;
        indexFile = open_file_rplusb(indexFilename);
        if(!indexFile) return 1;
        create_index_b(binFile, indexFile, type);
    }
    
    if(function == SEARCH_WHERE_B){
        binFilename = readString();
        indexFilename = readString();
        binFile = open_file_rplusb(binFilename);
        if(!binFile) return 1;
        indexFile = open_file_rplusb(indexFilename);
        if(!indexFile) return 1;
        search_where_b(binFile, indexFile, type);
    }
    
    if(function == INSERT_INTO_B){
        binFilename = readString();
        indexFilename = readString();
        binFile = open_file_rplusb(binFilename);
        if(!binFile) return 1;
        indexFile = open_file_rplusb(indexFilename);
        if(!indexFile) return 1;
        insert_into_b(binFile, indexFile, type);
    }

    if(indexFile) fclose(indexFile);
    if(binFile) fclose(binFile);
    if(indexFilename) free(indexFilename);
    if(binFilename) free(binFilename);    
    if(fileType) free(fileType);
    return 0;
}