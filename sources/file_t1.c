#include "../includes/file_t1.h"
#include "../includes/index_t1.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_REGISTRY_SIZE 97
#define HEADER_SIZE 182

#define NEXT_RRN_BYTEOFFSET 174
#define REMOVED_REG_BYTEOFFSET 178

//---------------------------Auxialiary Functions---------------------------//

void write_header_registry_t1(FILE *file){
    //declaring default values of the header file
    char status = '0';
    int top = -1;
    char description[] = "LISTAGEM DA FROTA DOS VEICULOS NO BRASIL";
    char descC1[] = "CODIGO IDENTIFICADOR: ";
    char descC2[] = "ANO DE FABRICACAO: ";
    char descC3[] = "QUANTIDADE DE VEICULOS: ";
    char descC4[] = "ESTADO: ";
    char codC5 = '0';
    char descC5[] = "NOME DA CIDADE: ";
    char codC6 = '1';
    char descC6[] = "MARCA DO VEICULO: ";
    char codC7 = '2';
    char descC7[] = "MODELO DO VEICULO: ";
    int nextRRN = 0;
    int remRegN = 0;

    //writing header data
    fwrite(&status, sizeof(char), 1, file);
    fwrite(&top, sizeof(int), 1, file);
    fwrite(description, sizeof(char), sizeof(description)-1, file);
    fwrite(descC1, sizeof(char), sizeof(descC1)-1, file);
    fwrite(descC2, sizeof(char), sizeof(descC2)-1, file);
    fwrite(descC3, sizeof(char), sizeof(descC3)-1, file);
    fwrite(descC4, sizeof(char), sizeof(descC4)-1, file);
    fwrite(&codC5, sizeof(char), 1, file);
    fwrite(descC5, sizeof(char), sizeof(descC5)-1, file);
    fwrite(&codC6, sizeof(char), 1, file);
    fwrite(descC6, sizeof(char), sizeof(descC6)-1, file);
    fwrite(&codC7, sizeof(char), 1, file);
    fwrite(descC7, sizeof(char), sizeof(descC7)-1, file);
    fwrite(&nextRRN, sizeof(int), 1, file);
    fwrite(&remRegN, sizeof(int), 1, file);
}

//---------------------------Main Functions---------------------------//

FILE *create_file_t1(const char *filename){
    FILE *file = fopen(filename, "wb");
    write_header_registry_t1(file);

    return file;
}

void add_registry_t1(FILE *file, registry_data_st data){
    //declaring auxiliary data
    char removed = '0';
    int next = -1;
    int initialSize = strlen(data.initials);
    int citySize = strlen(data.city);
    int brandSize = strlen(data.brand);
    int modelSize = strlen(data.model);
    int nullValue = -1;

    //variable that will store the amount of bytes that have been written to the file
    //the fwrite function returns the amount of members that have been successfully written
    //by multiplying that with the sizeof of each member we can get the amount of bytes that we've written to the file
    int writtenBytes = 0;

    //writing the registry to our file
    writtenBytes += fwrite(&removed, sizeof(char), 1, file) * sizeof(char);
    writtenBytes += fwrite(&next, sizeof(int), 1, file) * sizeof(int);
    writtenBytes += fwrite(&data.id, sizeof(int), 1, file) * sizeof(int);
    //checking if static field exist, if it doesn't, write a default value
    if(data.year == 0)
        writtenBytes += fwrite(&nullValue, sizeof(int), 1, file) * sizeof(int);
    else
        writtenBytes += fwrite(&data.year, sizeof(int), 1, file) * sizeof(int);
    if(data.amount == 0)
        writtenBytes += fwrite(&nullValue, sizeof(int), 1, file) * sizeof(int);
    else
        writtenBytes += fwrite(&data.amount, sizeof(int), 1, file) * sizeof(int);
    if(initialSize == 0)
        writtenBytes += fwrite("$$", sizeof(char), 2, file) * sizeof(char);
    else
        writtenBytes += fwrite(data.initials, sizeof(char), initialSize, file) * sizeof(char);
    //checking if dinamic field exist, if it doesn't, don't write anything
    if(citySize != 0 && strcmp(data.city, "NAO PREENCHIDO") != 0){
        writtenBytes += fwrite(&citySize, sizeof(int), 1, file) * sizeof(int);
        writtenBytes += fwrite("0", sizeof(char), 1, file) * sizeof(char);
        writtenBytes += fwrite(data.city, sizeof(char), citySize, file) * sizeof(char);
    }
    if(brandSize != 0 && strcmp(data.brand, "NAO PREENCHIDO") != 0){
        writtenBytes += fwrite(&brandSize, sizeof(int), 1, file) * sizeof(int);
        writtenBytes += fwrite("1", sizeof(char), 1, file) * sizeof(char);
        writtenBytes += fwrite(data.brand, sizeof(char), brandSize, file) * sizeof(char);
    }
    if(modelSize != 0 && strcmp(data.model, "NAO PREENCHIDO") != 0){
        writtenBytes += fwrite(&modelSize, sizeof(int), 1, file) * sizeof(int);
        writtenBytes += fwrite("2", sizeof(char), 1, file) * sizeof(char);
        writtenBytes += fwrite(data.model, sizeof(char), modelSize, file) * sizeof(char);
    }

    //getting the amount of bytes left in our registry (which should have a size of 97 bytes always)
    int remainingBytes = MAX_REGISTRY_SIZE - writtenBytes;
    char registryTrash[MAX_REGISTRY_SIZE];
    memset(registryTrash, '$', MAX_REGISTRY_SIZE);
    //writing remaining bytes as trash (represented by the character '$')
    fwrite(registryTrash, sizeof(char), remainingBytes, file);
}

registry_data_st read_registry_t1(FILE *file){
    registry_data_st registry;
    int nextRRN, stringSize, nonReadBytes = 97;
    char cod;
    int notEOF = fread(&nextRRN, sizeof(int), 1, file);  nonReadBytes -= 4;
    if(!notEOF){
        registry.id = -1;
        return registry;
    }
    fread(&registry.id, sizeof(int), 1, file); nonReadBytes -= 4;
    fread(&registry.year, sizeof(int), 1, file); nonReadBytes -= 4;
    fread(&registry.amount, sizeof(int), 1, file); nonReadBytes -= 4;
    fread(registry.initials, sizeof(char), 2, file); nonReadBytes -= 4;
    registry.initials[2] = '\0';
    strcpy(registry.brand , "NAO PREENCHIDO");
    strcpy(registry.model , "NAO PREENCHIDO");
    strcpy(registry.city , "NAO PREENCHIDO");
    //There is a little function to read variable size fields
    for(int i = 0; i < 3; i++){
        fread(&stringSize, sizeof(int), 1, file);
        fread(&cod, sizeof(char), 1, file);
        nonReadBytes -= 5;
        if(cod == '0'){
            memset(registry.city , 0, sizeof(registry.city));
            fread(registry.city, sizeof(char), stringSize, file);
            nonReadBytes -= stringSize;
        }else if(cod == '1'){
            memset(registry.brand , 0, sizeof(registry.brand));
            fread(registry.brand, sizeof(char), stringSize, file);
            nonReadBytes -= stringSize;
        }else if(cod == '2'){
            memset(registry.model , 0, sizeof(registry.model));
            fread(registry.model, sizeof(char), stringSize, file);            
            nonReadBytes -= stringSize;
        }
    }
    fseek(file, nonReadBytes+1, SEEK_CUR);

    //Now properly print everything
    return registry;
}

void read_file_t1(FILE *file){
    fseek(file, HEADER_SIZE, SEEK_SET);
    char removed; 
    int notEOF = fread(&removed, sizeof(char), 1, file);
    //if we reach EOF, then we don't have any registries
    if(!notEOF)
        printf("Registro inexistente.\n");
    //while removed is read, print the file
    while(notEOF){
        if(removed == '0') {
            //read registry
            print_registry(read_registry_t1(file));
            //read all the left garbage
            do{
                notEOF = fread(&removed, sizeof(char), 1, file);
            } while(removed == '$' && notEOF);
        }else{
            //if removed is True (1), skip 96 bytes
            // just 96 because removed was already read
            fseek(file, MAX_REGISTRY_SIZE - 1, SEEK_CUR);
            notEOF = fread(&removed, sizeof(char), 1, file);
        }
    }
}

void search_where_t1(FILE *file, Field *f, int n){
    fseek(file, HEADER_SIZE, SEEK_SET);
    char removed; 
    int found;
    int notEOF = fread(&removed, sizeof(char), 1, file); 
    //while removed is read, print the file
    while(notEOF){
        if(removed == '0') {
            //read registry
            registry_data_st registry = read_registry_t1(file);
            if(compare_registry(registry, f, n)){
                print_registry(registry);
                found++;
            }
            //read all the left garbage
            do{
                notEOF = fread(&removed, sizeof(char), 1, file);
            } while(removed == '$' && notEOF);
        }else{
            //if removed is True (1), skip 96 bytes
            printf("Registro inexistente.\n");
            // just 96 because removed was already read
            fseek(file, 96, SEEK_CUR);
            notEOF = fread(&removed, sizeof(char), 1, file);
        }
    }
    
    //checking if we found any registrys
    if(found == 0)
        printf("Registro inexistente.\n");
}

void search_rrn(FILE *file, int rrn){
    fseek(file, HEADER_SIZE + (rrn * MAX_REGISTRY_SIZE), SEEK_SET);
    //checking registry status
    char status;
    fread(&status, sizeof(char), 1, file);
    if(status != '0'){
        printf("Registro inexistente.\n");
        return;
    }

    registry_data_st data = read_registry_t1(file);
    if(data.id == -1){
        printf("Registro inexistente.\n");
        return;
    }
    print_registry(data);
}

void close_file_t1(FILE *file){
    //going to the start of the file
    fseek(file, 0, SEEK_SET);
    //updating status
    int status = '1';
    fwrite(&status, sizeof(char), 1, file);
    //closing file
    fclose(file);
}

void update_RRN(FILE *file, int finalRRN){
    fseek(file, 174, SEEK_SET);
    fwrite(&finalRRN, sizeof(int), 1, file);
}

void push_deleted_stack(FILE *binFile, int rrn){
    int nextRRN = -1;
    fseek(binFile, 1, SEEK_SET);
    do{
        fread(&nextRRN, sizeof(int), 1, binFile);
        if(nextRRN != -1) fseek(binFile, HEADER_SIZE+(nextRRN*MAX_REGISTRY_SIZE)+1, SEEK_SET);
    }while(nextRRN != -1);
    fseek(binFile, -4, SEEK_CUR);
    fwrite(&rrn, sizeof(int), 1, binFile);
    //coloca o ponteiro de volta ao 
    fseek(binFile, HEADER_SIZE+(rrn*MAX_REGISTRY_SIZE), SEEK_SET);
}

registry_data_st update_registry_t1(registry_data_st newReg, Field *update, int n){
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

int insert_into_t1(FILE *binFile, registry_data_st data){
    //definindo local de inserção
    int top;
    fread(&top, sizeof(int), 1, binFile);
    //vendo se temos registros já removidos
    if(top == -1){
        //se não tivermos, pulamos para o final do arquivo
        fseek(binFile, 0, SEEK_END);

        //escrevemos o novo registro
        add_registry_t1(binFile, data);

        //e atualizamos o cabeçalho
        //primeiro pulamos para o campo de próximo rrn
        fseek(binFile, NEXT_RRN_BYTEOFFSET, SEEK_SET);
        //depois lemos
        int headerNextRRN;
        fread(&headerNextRRN, sizeof(int), 1, binFile);
        //e atualizamos ele (incrementando 1)
        headerNextRRN++;
        fseek(binFile, NEXT_RRN_BYTEOFFSET, SEEK_SET);
        fwrite(&headerNextRRN, sizeof(int), 1, binFile);
        return headerNextRRN-1;
    }
    else{
        //se tivermos vamos para o próximo (pulando o status, portanto: +1)
        fseek(binFile, HEADER_SIZE + (top * MAX_REGISTRY_SIZE) + 1, SEEK_SET);

        //guardamos o valor do próximoRRN
        int nextRRN;
        fread(&nextRRN, sizeof(int), 1, binFile);

        //escrevemos o novo registro
        fseek(binFile, HEADER_SIZE + (top * MAX_REGISTRY_SIZE), SEEK_SET);
        add_registry_t1(binFile, data);

        //e atualizamos o campo de topo
        fseek(binFile, 1, SEEK_SET);
        fwrite(&nextRRN, sizeof(int), 1, binFile);

        //e o campo de registros removidos
        //primeiro lemos o campo
        fseek(binFile, REMOVED_REG_BYTEOFFSET, SEEK_SET);
        int removedReg;
        fread(&removedReg, sizeof(int), 1, binFile);
        //e depois decrementamos e sobrescrevemos
        removedReg--;
        fseek(binFile, REMOVED_REG_BYTEOFFSET, SEEK_SET);
        fwrite(&removedReg, sizeof(int), 1, binFile);
        return top;
    }
}

void search_rrn_b(FILE *file, int rrn){
    fseek(file, HEADER_SIZE + (rrn * MAX_REGISTRY_SIZE), SEEK_SET);
    //checking registry status
    char status;
    fread(&status, sizeof(char), 1, file);
    if(status != '0'){
        printf("Registro inexistente.\n");
        return;
    }

    registry_data_st data = read_registry_t1(file);
    if(data.id == -1){
        printf("Registro inexistente.\n");
        return;
    }
    print_registry(data);
}