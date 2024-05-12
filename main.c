#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "variable.h"

double calculate(float lat1, float lat2, float long1, float long2){
    return 2.0*r*asin(sqrt(pow(sin(((lat2-lat1)/2.0)*(PI/180.0)),2)+((cos((lat1*PI)/180.0))*(cos((lat2*PI)/180.0))*pow(sin(((long2-long1)/2.0)*(PI/180.0)),2))));
}

int main(){
    
    printf("Masukkan Nama File: ");
    scanf("%s", namafile);

    FILE* stream = fopen(namafile, "r");
    if (stream == NULL){
        printf("File tidak ditemukan");
        return 0;
    }

    //Mencari Jumlah Kota
    while(fgets(line, 255, stream)){
        jumlah_kota++;
    }
    rewind(stream);

    latitude = malloc(sizeof(float)*jumlah_kota);
    longitude = malloc(sizeof(float)*jumlah_kota);

    //Parsing
    while(fgets(line, 255, stream)){
        strcpy(tempLine, line);
        token = strtok(tempLine, ",");
        nama_kota[i] = (char*)malloc(strlen(token)+1);
        strcpy(nama_kota[i], token);
        token = strtok(NULL, ",");
        latitude[i] = atof(token);
        token = strtok(NULL, "\n");
        longitude[i] = atof(token);
        i++;
    }

    //adj graph
    adj =(double**)malloc(jumlah_kota * sizeof(double*));
    for(int i = 0; i<jumlah_kota;i++){
        adj[i] = (double*)malloc(jumlah_kota * sizeof(double));
    }

    for(int i = 0; i<jumlah_kota; i++){
        for(int j = 0; j<jumlah_kota; j++){
            if(i == j){
                adj[i][j] = 0.0;
            }
            else{
                adj[i][j] = calculate(latitude[i],latitude[j],longitude[i],longitude[j]);
            }
            printf("%f |",adj[i][j]);
        }
        printf("\n");
    }

    printf("Masukkan Kota Awal: ");
    scanf("%s", kota_awal);
}
