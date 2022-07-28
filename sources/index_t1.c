#include "../includes/index_t1.h"
#include "../includes/file_t1.h"
#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_REGISTRY_SIZE 97
#define HEADER_SIZE 182

#define NEXT_RRN_BYTEOFFSET 174
#define REMOVED_RNN_BYTEOFFSET 178

//-------------Auxiliary Functions-------------//

void read_index_data_t1(FILE *dataFile, index_st1 *data)
{
    // inicializando struct
    data->data = (index_data_st1 *)malloc(sizeof(index_data_st1));
    data->size = 0;

    char removed;
    int currRRN = 0;
    int notEOF = fread(&removed, sizeof(char), 1, dataFile);
    while (notEOF)
    {
        if (removed == '0')
        {
            // pegando ID do arquivo de dados
            registry_data_st registry = read_registry_t1(dataFile);
            // alocando memória
            data->size++;
            data->data = (index_data_st1 *)realloc(data->data, sizeof(index_data_st1) * data->size);
            // salvando ID e RRN nos dados
            data->data[data->size - 1].id = registry.id;
            data->data[data->size - 1].rrn = currRRN;
            // pulando lixo
            do
            {
                notEOF = fread(&removed, sizeof(char), 1, dataFile);
            } while (removed == '$' && notEOF);
        }
        else
        {
            // pulando o registro caso ele esteja marcado como removido
            fseek(dataFile, 96, SEEK_CUR);
            notEOF = fread(&removed, sizeof(char), 1, dataFile);
        }
        currRRN++;
    }
}

int compare_t1(const void *d1, const void *d2)
{
    int id1 = ((index_data_st1 *)d1)->id;
    int id2 = ((index_data_st1 *)d2)->id;
    return id1 - id2;
}

//-------------Main Functions-------------//

void create_index_t1(FILE *dataFile, FILE *indexFile)
{
    // vendo integridade do arquivo
    if (!check_status(dataFile))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    // pulando header
    fseek(dataFile, DATA_HEADER_SIZE, SEEK_SET);

    // lendo dados do arquivo de dados
    index_st1 index;
    read_index_data_t1(dataFile, &index);

    // ordenando dados
    qsort((void *)index.data, index.size, sizeof(index_data_st1), compare_t1);

    write_index_file_t1(indexFile, index);

    // desalocando memória
    free(index.data);
}

void write_index_file_t1(FILE *indexFile, index_st1 index)
{
    // criando arquivo de índice
    // escrevendo header
    char status = '0';
    fwrite(&status, sizeof(char), 1, indexFile);
    // salvando índices
    for (int i = 0; i < index.size; i++)
    {
        if (index.data[i].id != -1)
        {
            fwrite(&index.data[i].id, sizeof(int), 1, indexFile);
            fwrite(&index.data[i].rrn, sizeof(int), 1, indexFile);
        }
    }
    // mudando status
    fseek(indexFile, 0, SEEK_SET);
    status = '1';
    fwrite(&status, sizeof(char), 1, indexFile);
}

index_st1 get_index_t1(FILE *indexFile)
{
    index_st1 index;
    int notEOF = 0;
    index.data = malloc(sizeof(index_data_st1));
    index.size = 0;
    // fseek(indexFile, 4, SEEK_CUR);
    do
    {
        index.data = realloc(index.data, sizeof(index_data_st1) * (index.size + 1));
        fread(&index.data[index.size].id, sizeof(int), 1, indexFile);
        notEOF = fread(&index.data[index.size].rrn, sizeof(int), 1, indexFile);
        index.size++;
    } while (notEOF);
    index.data = realloc(index.data, sizeof(index_data_st1) * --index.size);
    return index;
}

void delete_where_t1(FILE *binFile, index_st1 index, Field *search, int n)
{
    int idField = search_field_id(search, n);

    int nroRegRem;

    //A variável indexIndex irá armazenar o index do registro procurado no index
    int indexIndex;
    fseek(binFile, 178, SEEK_SET);
    fread(&nroRegRem, sizeof(int), 1, binFile);
    // se qualquer campo de f é 'id', a busca deve ser feita pelo arquivo de índice.
    // caso contrário, deve ser feita pela funcionalidade 3.
    if(idField != -1){     

        indexIndex = binary_search_t1(index, atoi(search[idField].value));
        //caso o id procurado exista,
        fseek(binFile, HEADER_SIZE+(index.data[indexIndex].rrn *MAX_REGISTRY_SIZE)+1, SEEK_SET);
        registry_data_st registry = read_registry_t1(binFile);
        if(indexIndex != -1 && compare_registry(registry, search, n)){
            //checa se todos os campos batem
            //insere na pilha de registros logicamente removidos
            push_deleted_stack(binFile, indexIndex);
            //além de buscar por ele no arquivo e o deletar logicamente
            logical_deletion(binFile, 0);  
            //deleta logicamente no vetor
            index.data[indexIndex].id = -1;
            nroRegRem++;
        }
    }
    else
    {
        // while removed is read, print the binFile
        char removed;
        int notEOF = fread(&removed, sizeof(char), 1, binFile);
        int rrn = 0;
        while(notEOF){
            //read registry
            registry_data_st registry = read_registry_t1(binFile);
            if(removed == '0' && compare_registry(registry, search, n)) {
                logical_deletion(binFile, -97);
                push_deleted_stack(binFile, rrn);
                indexIndex = binary_search_t1(index, registry.id);
                index.data[indexIndex].id = -1;
                nroRegRem++;
                //go to the next registry
                fseek(binFile, 97, SEEK_CUR);
            }
            notEOF = fread(&removed, sizeof(char), 1, binFile);
            rrn++;
        }
    }
    fseek(binFile, 178, SEEK_SET);
    fwrite(&nroRegRem, sizeof(int), 1, binFile);
}

void update_where_t1(FILE *binFile, index_st1 index, Field *search, int x, Field *update, int y)
{
    int idField = search_field_id(search, x);
    //A variável indexIndex irá armazenar o index do registro procurado no index
    int indexIndex;
    fseek(binFile, HEADER_SIZE, SEEK_SET);
    char status; 
    int notEOF = fread(&status, sizeof(char), 1, binFile); 
    //se qualquer campo de f é 'id', a busca deve ser feita pelo arquivo de índice.
    //caso contrário, deve ser feita pela funcionalidade 3.
    if(idField != -1){
        //procura pelo registro com os valores de search
        indexIndex = binary_search_t1(index, atoi(search[idField].value));
        fseek(binFile, HEADER_SIZE+(indexIndex*MAX_REGISTRY_SIZE), SEEK_SET);
        fread(&status, sizeof(char), 1, binFile);
        registry_data_st registry = read_registry_t1(binFile);
        if(status == '0' && compare_registry(registry, search, x)){    
            //le o registro e atualiza com os dados de update
            registry_data_st new = update_registry_t1(registry, update, y);
            index.data[indexIndex].id = new.id;
            //logo após, volta o tamanho do registro e insere como novo registro
            fseek(binFile, -MAX_REGISTRY_SIZE, SEEK_CUR);
            add_registry_t1(binFile, new);
        }
    }else{
        while(notEOF){
            //read registry
            registry_data_st registry = read_registry_t1(binFile);
            if(status == '0' && compare_registry(registry, search, x)) {
                //atualiza o registro com valores de update
                registry_data_st new = update_registry_t1(registry, update, y);
                indexIndex = binary_search_t1(index, registry.id);
                index.data[indexIndex].id = new.id;
                //logo após, volta o tamanho do registro e insere como novo registro
                fseek(binFile, -MAX_REGISTRY_SIZE, SEEK_CUR);
                add_registry_t1(binFile, new);
            }
            notEOF = fread(&status, sizeof(char), 1, binFile);
        }
    } 
}

int binary_search_t1(index_st1 index, int id)
{
    int inf = 0;
    int sup = index.size - 1;
    int mid;
    while (inf <= sup)
    {
        mid = (inf + sup) / 2;
        if (index.data[mid].id == id)
            return mid;
        else if (index.data[mid].id > id)
            sup = mid - 1;
        else if (index.data[mid].id <= id) // caso variação seja 0, o valor procurado não existe
            inf = mid + 1;
    }
    return -1;
}