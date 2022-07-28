/**
 * @file myString.h
 * @brief Functions related to reading of strings
 */
#ifndef __MY_STRING_H__
#define __MY_STRING_H__
#include <stdio.h>

/**
 * @brief Retorna se a leitura da string deve ou não ser finalizada
 * 
 * @param numSeparators número de separadores
 * @param separatorArray array com todos finalizadores
 * @param c caractere lido
 * @return int 1 para leitura finalizada, caso contrário, 0
 */
int stringEnd(int numSeparators, char separatorArray[], char c);

char *readFixedSizeString(FILE *fp, int stringSize);

/**
 * @brief Função para leitura de strings
 * 
 * @return char* string
 */
char* readString();


/**
 * @brief Função mais completa para leitura de strings
 * 
 * @param num número de separadores que serão passados
 * @param ... separadores: EOF, quebra de linha etc...
 * @return char* string
 */
char* readString2(int num, ...);

#endif