#include "../includes/file_t2.h"
#include <string.h>
#include <stdlib.h>

#define HEADER_SIZE 190

#define NEXT_BYTEOFF_BYTEOFFSET 178
#define REMOVED_REG_BYTEOFFSET 186

void write_header_registry_t2(FILE *file){
    //declaring default values of the header file
    char status = '0';
    lli top = -1;
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
    lli nextByteOffset = 0;
    int remRegN = 0;

    //writing header data
    fwrite(&status, sizeof(char), 1, file);
    fwrite(&top, sizeof(lli), 1, file);
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
    fwrite(&nextByteOffset, sizeof(lli), 1, file);
    fwrite(&remRegN, sizeof(int), 1, file);
}

int calculate_registry_size(registry_data_st data){
    //first fields of a registry are static in size
    int size = 22;

    //calculating size based on dinamic fields
    int citySize = strlen(data.city);
    int brandSize = strlen(data.brand);
    int modelSize = strlen(data.model);

    if(citySize != 0)
        size += citySize + 5;
    if(brandSize != 0)
        size += brandSize + 5;
    if(modelSize != 0)
        size += modelSize + 5;

    return size;
}

FILE *create_file_t2(const char *filename){
    FILE *file = fopen(filename, "wb");
    write_header_registry_t2(file);

    return file;
}

void add_registry_t2(FILE *file, registry_data_st data, int additionalSize){
    //declaring variables that will be written to the registry
    char removed = '0';
    int registrySize = calculate_registry_size(data) + additionalSize;
    lli next = -1;
    int initialSize = 2;
    int citySize = strlen(data.city);
    int brandSize = strlen(data.brand);
    int modelSize = strlen(data.model);
    int nullValue = -1;

    fwrite(&removed, sizeof(char), 1, file);
    fwrite(&registrySize, sizeof(int), 1, file);
    fwrite(&next, sizeof(lli), 1, file);
    fwrite(&data.id, sizeof(int), 1, file);
    if(data.year == 0)
        fwrite(&nullValue, sizeof(int), 1, file);
    else
        fwrite(&data.year, sizeof(int), 1, file);
    if(data.amount == 0)
        fwrite(&nullValue, sizeof(int), 1, file);
    else
        fwrite(&data.amount, sizeof(int), 1, file);
    if(initialSize == 0)
        fwrite("$$", sizeof(char), 2, file);
    else
        fwrite(data.initials, sizeof(char), 2, file);

    if(citySize != 0){
        fwrite(&citySize, sizeof(int), 1, file);
        fwrite("0", sizeof(char), 1, file);
        fwrite(data.city, sizeof(char), citySize, file);
    }
    if(brandSize != 0){
        fwrite(&brandSize, sizeof(int), 1, file);
        fwrite("1", sizeof(char), 1, file);
        fwrite(data.brand, sizeof(char), brandSize, file);
    }
    if(modelSize != 0){
        fwrite(&modelSize, sizeof(int), 1, file);
        fwrite("2", sizeof(char), 1, file);
        fwrite(data.model, sizeof(char), modelSize, file);
    }
}

void update_byte_offset(FILE *file, lli byteOffset){
    fseek(file, 178, SEEK_SET);
    fwrite(&byteOffset, sizeof(lli), 1, file);
}

registry_data_st read_registry_t2(FILE* file){
    registry_data_st registry;
    int stringSize;
    lli nextByteOffset;
    char cod, nextByte;
    int notEOF;
    fread(&nextByteOffset, sizeof(lli), 1, file);
    fread(&registry.id, sizeof(int), 1, file);
    fread(&registry.year, sizeof(int), 1, file);
    fread(&registry.amount, sizeof(int), 1, file);
    fread(registry.initials, sizeof(char), 2, file);
    registry.initials[2] = '\0';
    strcpy(registry.brand , "");
    strcpy(registry.model , "");
    strcpy(registry.city , "");
    //There is a little function to read variable size fields
    for(int i = 0; i < 3; i++){
        //if next read byte indicates removed char, 
        //print registry and return
        //otherwise, read other fields
        notEOF = fread(&nextByte, sizeof(char), 1, file);
        fseek(file, -1, SEEK_CUR);
        if(notEOF){
            if(nextByte == '1' || nextByte == '0'){
                return registry;
            }
        }else{
            fseek(file, 1, SEEK_CUR);
            return registry;
        }

        fread(&stringSize, sizeof(int), 1, file);
        fread(&cod, sizeof(char), 1, file);
        if(cod == '0'){
            memset(registry.city , 0, sizeof(registry.city));
            fread(registry.city, sizeof(char), stringSize, file);
        }else if(cod == '1'){
            memset(registry.brand , 0, sizeof(registry.brand));
            fread(registry.brand, sizeof(char), stringSize, file);
        }else if(cod == '2'){
            memset(registry.model , 0, sizeof(registry.model));
            fread(registry.model, sizeof(char), stringSize, file);
        }
    }

    return registry;
}

void read_file_t2(FILE *file){
    fseek(file, HEADER_SIZE, SEEK_SET);
    char removed; 
    int registrySize;
    int notEOF = fread(&removed, sizeof(char), 1, file);
    //while removed is read, print the file
    while(notEOF){
        fread(&registrySize, sizeof(int), 1, file);
        //if removed is True (1), skip 97 bytes
        if(removed == '0') {
            //read registry
            registry_data_st registry = read_registry_t2(file);
            //Now properly print everything
            print_registry(registry);
        }else{
            // just 96 because removed was already read
            fseek(file, registrySize, SEEK_CUR);
        }
        notEOF = fread(&removed, sizeof(char), 1, file);
    }
}

void close_file_t2(FILE *file){
    //going to the start of the file
    fseek(file, 0, SEEK_SET);
    //updating status
    int status = '1';
    fwrite(&status, sizeof(char), 1, file);
    //closing file
    fclose(file);
}

void search_where_t2(FILE *file, Field *f, int n){
    
    fseek(file, HEADER_SIZE, SEEK_SET);
    char removed;
    int registrySize;
    int found = 0;
    int notEOF = fread(&removed, sizeof(char), 1, file);
    //while removed is read, print the file
    while(notEOF){
        //getting registry size
        fread(&registrySize, sizeof(int), 1, file);
        if(removed == '0') {
            //read registry
            registry_data_st registry = read_registry_t2(file);
            if(compare_registry(registry, f, n)){
                print_registry(registry);
                found++;
            }
        }else{
            //if it's removed, we skip the rest of the registry
            printf("Registro inexistente.\n");
            fseek(file, registrySize, SEEK_CUR);
        }
        notEOF = fread(&removed, sizeof(char), 1, file);
    }

    //checking if we found any registrys
    if(found == 0)
        printf("Registro inexistente.\n");
}

long int insert_in_the_end(FILE *binFile, registry_data_st data){
    //ir para o final do arquivo
    fseek(binFile, 0, SEEK_END);
    //escrever o novo registro
    add_registry_t2(binFile, data, 0);

    //atualizar o cabeçalho
    //pulando para o campo de prox byteoffset
    fseek(binFile, NEXT_BYTEOFF_BYTEOFFSET, SEEK_SET);
    long long int headerNextByteoff, oldByteOff;
    fread(&headerNextByteoff, sizeof(long long int), 1, binFile);
    oldByteOff = headerNextByteoff;
    //atualizar somando com o tamanho do registro (e seu início)
    headerNextByteoff += calculate_registry_size(data) + 5;
    fseek(binFile, NEXT_BYTEOFF_BYTEOFFSET, SEEK_SET);
    fwrite(&headerNextByteoff, sizeof(long long int), 1, binFile);

    return oldByteOff;
}

long int insert_into_t2(FILE *binFile, registry_data_st data){
    //definindo local de inserção
    long long int top, byteOffset;
    fread(&top, sizeof(long long int), 1, binFile);
    //vendo se temos registros removidos
    if(top == -1){
        //se não tivermos, inserimos no final
        byteOffset = insert_in_the_end(binFile, data);
        return byteOffset;
    }
    else{
        //se tivermos vamos para o próximo (pulando o status, portanto: +1)
        fseek(binFile, top + 1, SEEK_SET);
        byteOffset = top;

        //vemos se no tamanho do registro removido cabe nosso registro novo
        int removedSize;
        fread(&removedSize, sizeof(int), 1, binFile);
        int registrySize = calculate_registry_size(data);
        if(removedSize < registrySize){
            //se não couber, inserimos no final
            byteOffset = insert_in_the_end(binFile, data);
            return byteOffset;
        }

        //se couber, guardamos o próximo byteoffset
        long long int nextByteoff;
        fread(&nextByteoff, sizeof(long long int), 1, binFile);

        //escrevemos o novo registro
        //primeiro pulamos ele
        fseek(binFile, top, SEEK_SET);
        //depois recalculamos o tamanho
        int additionalSize = removedSize - registrySize;
        //aí adicionamos o novo registro com o novo tamanho
        add_registry_t2(binFile, data, additionalSize);
        //por último escrevemos o lixo
        for(int i = 0; i < additionalSize; i++){
            char trash = '$';
            fwrite(&trash, sizeof(char), 1, binFile);
        }
        
        //e atualizamos o cabeçalho
        //primeiro atualizamos o topo
        fseek(binFile, 1, SEEK_SET);
        fwrite(&nextByteoff, sizeof(long long int), 1, binFile);

        //depois atualizamos o numero de registros removidos
        //primeiro lemos o campo
        fseek(binFile, REMOVED_REG_BYTEOFFSET, SEEK_SET);
        int removedReg;
        fread(&removedReg, sizeof(int), 1, binFile);
        //e depois decrementamos e sobrescrevemos
        removedReg--;
        fseek(binFile, REMOVED_REG_BYTEOFFSET, SEEK_SET);
        fwrite(&removedReg, sizeof(int), 1, binFile);
    }
    return byteOffset;
}

void search_byteOffset(FILE *file, long int byteOffset){
    fseek(file, byteOffset, SEEK_SET);
    //checking registry status
    char status;
    fread(&status, sizeof(char), 1, file);
    int registrySize;
    fread(&registrySize, sizeof(int), 1, file);
    if(status != '0'){
        printf("Registro inexistente.\n");
        return;
    }

    registry_data_st data = read_registry_t2(file);
    if(data.id == -1){
        printf("Registro inexistente.\n");
        return;
    }
    print_registry(data);
}