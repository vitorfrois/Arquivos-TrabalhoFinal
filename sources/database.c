#include "../includes/database.h"
#include "../includes/file_t1.h"
#include "../includes/file_t2.h"
#include "../includes/index_t1.h"
#include "../includes/index_t2.h"
#include "../includes/myString.h"
#include "../includes/utils.h"
#include "../includes/btree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void create_table(char type, const char *CSVFileName, const char *binFileName)
{
    // opening csv file and checking if it exists
    FILE *CSVFile = fopen(CSVFileName, "r");
    if (!check_status(CSVFile))
        return;

    if (type == '1')
    {
        FILE *binFile = create_file_t1(binFileName);

        // excluding header from  csv
        registry_data_st data;
        read_from_csv(CSVFile, &data);

        int nextRRN = 0;
        // reading data and saving in binary file
        while (read_from_csv(CSVFile, &data) == 0)
        {
            add_registry_t1(binFile, data);
            nextRRN++;
        }
        update_RRN(binFile, nextRRN);

        close_file_t1(binFile);
    }
    else if (type == '2')
    {
        FILE *binFile = create_file_t2(binFileName);

        // excluding header from csv file
        registry_data_st data;
        read_from_csv(CSVFile, &data);

        // reading and saving all data in binary file and updating last byte offset
        while (read_from_csv(CSVFile, &data) == 0)
            add_registry_t2(binFile, data, 0);
        long lastByteOffset = ftell(binFile);
        update_byte_offset(binFile, lastByteOffset);

        close_file_t2(binFile);
    }
    else
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(CSVFile);
        return;
    }

    fclose(CSVFile);
    binarioNaTela(binFileName);
}

void select_from(FILE *binFile, char type)
{
    // checking file integrity
    if (!check_status(binFile))
        return;

    // checking if filetype is valid, if it is, we can execute our functionality
    if (type == '1')
        read_file_t1(binFile);
    else if (type == '2')
        read_file_t2(binFile);
    else
        printf("Falha no processamento do arquivo.\n");
}

void select_from_where(FILE *binFile, char type)
{
    if (!check_status(binFile))
        return;

    // creating and getting fields that will be searched from user
    int n;
    scanf("%d\n", &n);
    Field f[n];
    for (int i = 0; i < n; i++)
    {
        f[i].name = readString();
        f[i].value = scan_quote_string();
    }
    // checking filetype
    if (type == '1')
        search_where_t1(binFile, f, n);
    else if (type == '2')
        search_where_t2(binFile, f, n);
    else
        printf("Falha no processamento do arquivo.\n");

    // freeing memory
    for (int i = 0; i < n; i++)
    {
        free(f[i].name);
        free(f[i].value);
    }
}

void select_from_RRN(FILE *binFile)
{
    if (!check_status(binFile))
        return;

    int RRN;
    scanf("%d", &RRN);
    search_rrn(binFile, RRN);
}

void create_index(char type, FILE *binFile, FILE *indexFile)
{
    if (type == '1')
        create_index_t1(binFile, indexFile);
    else if (type == '2')
        create_index_t2(binFile, indexFile);
    else
        printf("Falha no processamento do arquivo.\n");
}

void delete_from_where(FILE *binFile, const char *indexFilename, char type)
{
    // if(!check_status(binFile) || !check_status(indexFile)) return;

    FILE *indexFile = fopen(indexFilename, "rb");
    // checking binFile status
    if (!check_status(binFile))
        return;
    if (!check_status(indexFile))
        return;

    // creating and getting fields that will be searched from user
    int n, x;
    scanf("%d\n", &n);
    if (type == '1')
    {
        index_st1 index = get_index_t1(indexFile);
        for (int i = 0; i < n; i++)
        {
            scanf("%d ", &x);
            Field *f = get_field(x);
            delete_where_t1(binFile, index, f, x);
            free_field(f, x);
        }
        fclose(indexFile);
        indexFile = fopen(indexFilename, "wb");
        write_index_file_t1(indexFile, index);
        free(index.data);
    }
    else{
        index_st2 index = get_index_t2(indexFile);
        for (int i = 0; i < n; i++)
        {
            scanf("%d ", &x);
            Field *f = get_field(x);
            delete_where_t2(binFile, index, f, x);
            free_field(f, x);
        }
        fclose(indexFile);
        indexFile = fopen(indexFilename, "wb");
        write_index_file_t2(indexFile, index);
        free(index.data);
    }

    fclose(indexFile);
}

void insert_into(FILE *binFile, const char *indexFilename, char type){

    // lendo o número de registros à serem adicionados
    int n;
    scanf(" %d\n", &n);
    for (int i = 0; i < n; i++)
    {
        // coletando input do usuário
        registry_data_st registry = read_registry_line();
        // inserindo no registro de cabeçalho
        if (type == '1')
            insert_into_t1(binFile, registry);
        else
            insert_into_t2(binFile, registry);

        // pulando o valor da integridade do arquivo
        fseek(binFile, 1, SEEK_SET);
    }

    // abrindo arquivo de indice
    FILE *indexFile = open_file_wb(indexFilename);
    // e sobrescrevendo ele
    if (type == '1')
        create_index_t1(binFile, indexFile);
    else
        create_index_t2(binFile, indexFile);

    // fechando arquivo
    fclose(indexFile);
}

void update_where(FILE *binFile, const char *indexFilename, char type)
{
    // Abre e checa integridade do arquivo
    FILE *indexFile = fopen(indexFilename, "r+b");
    if (!check_status(binFile) || !check_status(indexFile))
        return;

    int n, x, y;
    scanf("%d\n", &n);
    // Realiza o loop para cada um dos tipos
    if (type == '1')
    {
        index_st1 index = get_index_t1(indexFile);
        for (int i = 0; i < n; i++)
        {
            scanf("%d ", &x);
            Field *search = get_field(x);
            scanf("%d ", &y);
            Field *update = get_field(y);
            update_where_t1(binFile, index, search, x, update, y);
            free_field(search, x);
            free_field(update, y);
        }
        fclose(indexFile);
        indexFile = fopen(indexFilename, "wb");
        write_index_file_t1(indexFile, index);
        free(index.data);
    }
    else
    {
        index_st2 index = get_index_t2(indexFile);
        for (int i = 0; i < n; i++)
        {
            printf("\n\nn == %d\n\n", i);
            scanf("%d ", &x);
            Field *search = get_field(x);
            scanf("%d ", &y);
            Field *update = get_field(y);
            update_where_t2(binFile, index, search, x, update, y);
            free_field(search, x);
            free_field(update, y);
            printf("novo caso\n");
        }
        fclose(indexFile);
        indexFile = fopen(indexFilename, "wb");
        write_index_file_t2(indexFile, index);
        free(index.data);
    }
    fclose(indexFile);
}

void search_where_b(FILE *binFile, FILE *indexFile, char type){
    int id;
    scanf("%*s %d", &id);
    struct bTree *tree = read_tree_header(indexFile, type);
    Key foundKey = searchID(indexFile, tree->root, id, type);
    if(type == '1')
        search_rrn(binFile, foundKey.value.rrn);
    else
        search_byteOffset(binFile, foundKey.value.byteOffset);
    destroyTree(tree);
}


void insert_into_b(FILE *binFile, FILE *indexFile, char type){
    // vendo integridade do arquivo
    if (!check_status(binFile))
        return;

    int n;
    scanf(" %d\n", &n);
    for (int i = 0; i < n; i++) {
        //lendo registro à ser inserido
        registry_data_st registry = read_registry_line();

        //inserindo no arquivo de dados
        int keyRRN;
        long int keyByteOffset;
        if(type == '1')
            keyRRN = insert_into_t1(binFile, registry);
        else
            keyByteOffset = insert_into_t2(binFile, registry);
        //pulando integridade do arquivo
        fseek(binFile, 1, SEEK_SET);

        //lendo cabeçalho da árvore
        struct bTree *tree = read_tree_header(indexFile, type);
        //inicializando nossa chave
        Key key;
        key.id = registry.id;
        if(type == '1')
            key.value.rrn = keyRRN;
        else
            key.value.byteOffset = keyByteOffset;
        key.fileType = type;
        //inserindo na árvore-B
        insertKey(indexFile, tree->root, key);
        
        //liberando memória
        free(tree->root);
        free(tree);
        //pulando para o início do arquivo
        fseek(indexFile, SEEK_SET, 0);
    }
}

void create_index_b(FILE *dataFile, FILE *indexFile, char type)
{
    // vendo integridade do arquivo
    if (!check_status(dataFile)){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    // lendo dados do arquivo de dados
    if(type == '1')
        create_btree_t1(dataFile, indexFile); 
    else
        create_btree_t2(dataFile, indexFile); 

    char status = '1';
    fseek(indexFile, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, indexFile);
}