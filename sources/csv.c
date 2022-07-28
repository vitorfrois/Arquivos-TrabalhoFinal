#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/csv.h"

int read_from_csv(FILE *file, registry_data_st *data){
    //declaring auxiliary variables
    char fields[7][50];
    int isEOF = 0;
    //iterating through fields
    for(int i = 0; i < 7; i++){
        //reading char and checking if it is EOF
        char readout = getc(file);
        if(readout == EOF){
            isEOF = 1;
            break;
        }

        //reading field data
        int size = 0;
        while(readout != ',' && readout != '\r' && readout != EOF){
            fields[i][size++] = readout;
            readout = getc(file);
            if(readout == EOF)
                isEOF = 1;
        }
        fields[i][size] = '\0';

        //if it has a \r, the next char will be a \n
        if(readout == '\r')
            readout = getc(file);
    }

    //storing data only if we don't find a EOF
    if(!isEOF){
        //storing data in struct
        data->id = atoi(fields[0]);
        data->year = atoi(fields[1]);
        strcpy(data->city, fields[2]);
        data->amount = atoi(fields[3]);
        strcpy(data->initials, fields[4]);
        strcpy(data->brand, fields[5]);
        strcpy(data->model, fields[6]);
    }

    return isEOF;
}