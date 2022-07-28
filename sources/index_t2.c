#include <stdio.h>
#include <stdlib.h>
#include "../includes/file_t2.h"
#include "../includes/index_t2.h"
#include "utils.h"

//-------------Variables/Structs/Defines-------------//

#define HEADER_SIZE 190

#define NEXT_RRN_BYTEOFFSET 178
#define REMOVED_RNN_BYTEOFFSET 186

//-------------Auxiliary Functions-------------//

void read_index_data_t2(FILE *dataFile, index_st2 *data){
    //inicializando struct
    data->data = (index_data_st2 *)malloc(sizeof(index_data_st2));
    data->size = 0;

    char removed;
    int registrySize;
    int totalOffset = HEADER_SIZE;
    int notEOF = fread(&removed, sizeof(char), 1, dataFile);
    while(notEOF){
        //lendo tamanho do registro
        fread(&registrySize, sizeof(int), 1, dataFile);
        //adicionando o inicio do registro no tamanho total
        registrySize += 5;
        if(removed == '0'){
            //pegando ID do arquivo de dados
            registry_data_st registry = read_registry_t2(dataFile);
            //alocando memória
            data->size++;
            data->data = (index_data_st2 *)realloc(data->data, sizeof(index_data_st2) * data->size);
            //salvando ID e byteOffset nos dados
            data->data[data->size-1].id = registry.id;
            data->data[data->size-1].byteOffset = totalOffset;
        }
        else{
            fseek(dataFile, registrySize-5, SEEK_CUR);
        }
        
        totalOffset += registrySize;
        notEOF = fread(&removed, sizeof(char), 1, dataFile);
        while(removed == '$' && notEOF)
            notEOF = fread(&removed, sizeof(char), 1, dataFile);
    }
}

int compare_t2(const void *d1, const void *d2){
    int id1 = ((index_data_st2 *)d1)->id;
    int id2 = ((index_data_st2 *)d2)->id;
    return id1 - id2;
}

//-------------Main Functions-------------//

void create_index_t2(FILE *dataFile, FILE *indexFile){
    //vendo integridade do arquivo
    if(!check_status(dataFile)){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    //pulando header
    fseek(dataFile, HEADER_SIZE, SEEK_SET);

    //lendo dados do arquivo de dados
    index_st2 index;
    read_index_data_t2(dataFile, &index);

    //ordenando dados
    qsort((void *)index.data, index.size, sizeof(index_data_st2), compare_t2);

    //criando arquivo de índice
    //escrevendo header
    write_index_file_t2(indexFile, index);
    //desalocando memória
    free(index.data);
}

void push_sorted_list(FILE *binFile, lli byteOffset){
    lli nextByteOffset = -1;
    char status = '1';
    fseek(binFile, 1, SEEK_SET);
    fread(&nextByteOffset, sizeof(lli), 1, binFile);
    int regSize = 0, nextRegSize = -1;
    // int prevRegSize = -1;
    //pega o tamanho do registro com byteoffset
    fseek(binFile, byteOffset+1, SEEK_SET);
    fread(&regSize, sizeof(int), 1, binFile);
    if(nextByteOffset == -1){
        fseek(binFile, 1, SEEK_SET);
        fwrite(&byteOffset, sizeof(lli), 1, binFile);
        fseek(binFile, byteOffset, SEEK_SET);
        fwrite(&status, sizeof(char), 1, binFile);
        return;
    }

    lli prevByteOffset = 0;
    lli top = nextByteOffset;
    fseek(binFile, 1, SEEK_SET);
    do{
        prevByteOffset = nextByteOffset;
        fread(&nextByteOffset, sizeof(lli), 1, binFile);
        if(nextByteOffset != -1){
            fseek(binFile, nextByteOffset + 1, SEEK_SET);
            fread(&nextRegSize, sizeof(int), 1, binFile);
        }
    }while((nextByteOffset != -1) && (regSize < nextRegSize));
    fseek(binFile, byteOffset, SEEK_SET);
    fwrite(&status, sizeof(char), 1, binFile);
    fseek(binFile, 4, SEEK_CUR);
    if(nextByteOffset != -1)
        fwrite(&nextByteOffset, sizeof(lli), 1, binFile);
    

    if(prevByteOffset == top)
        fseek(binFile, 1, SEEK_SET);
    else
        fseek(binFile, prevByteOffset+5, SEEK_SET);
    
    //coloca byteOffset em prox
    fwrite(&byteOffset, sizeof(lli), 1, binFile);
}

void delete_where_t2(FILE *binFile, index_st2 index, Field *search, int n){
    int idField = search_field_id(search, n);

    //A variável indexIndex irá armazenar o index do registro procurado no index
    int indexIndex;

    int nroRegRem;
    fseek(binFile, 186, SEEK_SET);
    fread(&nroRegRem, sizeof(int), 1, binFile);
    char status;
    //se qualquer campo de f é 'id', a busca deve ser feita pelo arquivo de índice.
    //caso contrário, deve ser feita pela funcionalidade 3.

    if(idField != -1){     
        indexIndex = binary_search_t2(index, atoi(search[idField].value));
        if(indexIndex != -1){ //caso o id procurado exista,
            fseek(binFile, index.data[indexIndex].byteOffset, SEEK_SET);
            fread(&status, sizeof(char), 1, binFile);
            int registrySize;
            fread(&registrySize, sizeof(int), 1, binFile);
            registry_data_st registry = read_registry_t2(binFile);
            if(status == '0' && compare_registry(registry, search, n)){
                //checa se todos os campos batem
                //insere na pilha de registros logicamente removidos
                lli byteOffset = index.data[indexIndex].byteOffset;
                push_sorted_list(binFile, byteOffset);
                //deleta logicamente no vetor
                index.data[indexIndex].id = -1;
                fseek(binFile, byteOffset + registrySize + 5, SEEK_SET);
                nroRegRem++;
            }
        }
    }else{
        //while removed is read, print the binFile
        int notEOF = fread(&status, sizeof(char), 1, binFile);
        while(notEOF){
            int registrySize;
            fread(&registrySize, sizeof(int), 1, binFile);
            //read registry
            registry_data_st registry = read_registry_t2(binFile);
            if(status == '0' && compare_registry(registry, search, n)) {
                indexIndex = binary_search_t2(index, registry.id);
                index.data[indexIndex].id = -1;
                lli byteOffset = index.data[indexIndex].byteOffset;
                push_sorted_list(binFile, byteOffset);
                fseek(binFile, byteOffset + registrySize + 5, SEEK_SET);
                nroRegRem++;
            }
            notEOF = fread(&status, sizeof(char), 1, binFile);
        };
    } 
    fseek(binFile, 1, SEEK_SET);
    lli byteOffset;
    fread(&byteOffset, sizeof(lli), 1, binFile);
    fseek(binFile, 186, SEEK_SET);
    fwrite(&nroRegRem, sizeof(int), 1, binFile);
}

void update_aux_t2(FILE *binFile, index_st2 index, registry_data_st registry, int registrySize, Field *update, int y){
    //le o registro e atualiza com os dados de update
    registry_data_st new = update_registry_t2(registry, update, y);
    printf("Pesquisa por id\n");
    int newRegSize = calculate_registry_size(new);
    if(newRegSize > registrySize){
        printf("Remove e insere\n");
        print_registry(registry);
        print_registry(new);
        long int nowAt = ftell(binFile);
        //insere na pilha de registros logicamente removidos
        int indexIndex = binary_search_t2(index, registry.id);
        lli byteOffset = index.data[indexIndex].byteOffset;
        push_sorted_list(binFile, byteOffset);
        //deleta logicamente no vetor
        index.data[indexIndex].id = -1;
        fseek(binFile, 1, SEEK_SET);
        insert_into_t2(binFile, new);
        fseek(binFile, nowAt, SEEK_SET);
    }else{
        printf("Inserção no mesmo lugar\n");
        fseek(binFile, -(registrySize+5), SEEK_CUR);
        int size_diff = registrySize - newRegSize;
        //logo após, volta o tamanho do registro e insere como novo registro
        add_registry_t2(binFile, new, size_diff);
        char garbage = '$';
        for(int i = 0; i < size_diff; i++)
            fwrite(&garbage, sizeof(char), 1, binFile);
    }
}

void update_where_t2(FILE *binFile, index_st2 index, Field *search, int x, Field *update, int y){
    int idField = search_field_id(search, x);

    //A variável indexIndex irá armazenar o index do registro procurado no index
    int indexIndex;
    fseek(binFile, HEADER_SIZE, SEEK_SET);
    // fread(binFile, )
    char status; 
    //se qualquer campo de f é 'id', o update deve ser feita pelo arquivo de índice.
    //caso contrário, deve ser feita pela funcionalidade 3.
    if(idField != -1){
        //procura pelo registro com os valores de search
        indexIndex = binary_search_t2(index, atoi(search[idField].value));
        if(indexIndex != -1){ //caso o id procurado exista,
            fseek(binFile, index.data[indexIndex].byteOffset, SEEK_SET);
            char status;
            fread(&status, sizeof(char), 1, binFile);
            int registrySize;
            fread(&registrySize, sizeof(int), 1, binFile);
            registry_data_st registry = read_registry_t2(binFile);
            // fseek(binFile, registrySize-calculate_registry_size(registry), SEEK_CUR);

            printf("reg atual\n");
            if(status == '0' && compare_registry(registry, search, x))
                update_aux_t2(binFile, index, registry, registrySize, update, y);

        }
    }else{
        int notEOF = fread(&status, sizeof(char), 1, binFile); 
        while(notEOF){
            //read registry
            printf("status = %c\n", status);
            int registrySize;
            fread(&registrySize, sizeof(int), 1, binFile);
            registry_data_st registry = read_registry_t2(binFile);
            //atualiza o registro com valores de update
            if(status == '0' && compare_registry(registry, search, x))
                update_aux_t2(binFile, index, registry, registrySize, update, y);

            notEOF = fread(&status, sizeof(char), 1, binFile);
        }
    } 
}

registry_data_st update_registry_t2(registry_data_st newReg, Field *update, int n){
    for(int i = 0; i < n; i++){
        if(strcmp(update[i].name, "id") == 0)
            newReg.id = atoi(update[i].value);

        if(strcmp(update[i].name, "ano") == 0)
            newReg.year = atoi(update[i].value);

        if(strcmp(update[i].name, "qtt") == 0)
            newReg.amount = atoi(update[i].value);

        if(strcmp(update[i].name, "cidade") == 0){
            memset(newReg.city , 0, sizeof(newReg.city));
            strcpy(newReg.city, update[i].value);
        }

        if(strcmp(update[i].name, "marca") == 0){
            memset(newReg.brand , 0, sizeof(newReg.brand));
            strcpy(newReg.brand, update[i].value);
        }

        if(strcmp(update[i].name, "modelo") == 0){
            memset(newReg.model , 0, sizeof(newReg.model));
            strcpy(newReg.model, update[i].value);
        }

        if(strcmp(update[i].name, "sigla") == 0){
            memset(newReg.initials , 0, sizeof(newReg.initials));
            strcpy(newReg.initials, update[i].value);
        }
    }
    return newReg;
}


int binary_search_t2(index_st2 index, int id){
    int inf = 0;
    int sup = index.size-1;
    int mid;
    while(inf <= sup){
        mid = (inf+sup)/2;
        if(index.data[mid].id == id)
            return mid;
        else if(index.data[mid].id > id)
            sup = mid - 1;
        else if(index.data[mid].id <= id) //caso variação seja 0, o valor procurado não existe
            inf = mid + 1;
    }
    return -1;
}

index_st2 get_index_t2(FILE *indexFile){
	index_st2 index;
    int notEOF = 0;
    index.data = malloc(sizeof(index_data_st2));
    index.size = 0;
    // fseek(indexFile, 4, SEEK_CUR);
    do{
        index.data = realloc(index.data, sizeof(index_data_st2) * (index.size+1));
        fread(&index.data[index.size].id, sizeof(int), 1, indexFile);
        notEOF = fread(&index.data[index.size].byteOffset, sizeof(lli), 1, indexFile);
        index.size++;
    }while(notEOF);
    index.data = realloc(index.data, sizeof(index_data_st2) * --index.size);
	return index;
}

void write_index_file_t2(FILE *indexFile, index_st2 index){
    char status = '0';
    fwrite(&status, sizeof(char), 1, indexFile);
    //salvando índices
    for(int i = 0; i < index.size; i++){
        if(index.data[i].id != -1){
            fwrite(&index.data[i].id, sizeof(int), 1, indexFile);
            fwrite(&index.data[i].byteOffset, sizeof(lli), 1, indexFile);
        }
    }
    //mudando status
    fseek(indexFile, 0, SEEK_SET);
    status = '1';
    fwrite(&status, sizeof(char), 1, indexFile);

}