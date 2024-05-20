#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "variable.h"
#include <time.h>


double calculate(float lat1, float lat2, float long1, float long2){
    return 2.0*r*asin(sqrt(pow(sin(((lat2-lat1)/2.0)*(PI/180.0)),2)+((cos((lat1*PI)/180.0))*(cos((lat2*PI)/180.0))*pow(sin(((long2-long1)/2.0)*(PI/180.0)),2))));
}

void tsp_greedy(){
    //Greedy Algorithm
    int visited[jumlah_kota];
    for (int i = 0 ; i < jumlah_kota ; i ++){
        visited[i] = 0;
    }
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 0 ; i < jumlah_kota ; i++){
        int ada = 0 ;
        //printf("\nnama kota: %s , kota awal : %s" , nama_kota[i], kota_awal);
        if (strcmp(nama_kota[i] , kota_awal) == 0){
            visited[i] = 1;
            idx_tinjau = i;
            idx_awal = i;
            jumlah_visited++;
            printf("Best route found: \n");
            printf("%s -> " , nama_kota[i]);
            ada = 1;
            break;
        }
    }
    jarak = 0;

    while (jumlah_visited < jumlah_kota)
    {   
        jarak_terdekat = 999999999999999;
        temp = 0;
        for (int i = 0 ; i<jumlah_kota ;i++){
            //printf("\nidx tinjau: %d" , idx_tinjau);
            //printf("\nadj : %f" , adj[idx_tinjau][i]);
            //printf("\nvisited : %d\n" , visited[i]);
            if((idx_tinjau != i)&&(adj[idx_tinjau][i] < jarak_terdekat)&& (visited[i] == 0)){
                jarak_terdekat = adj[idx_tinjau][i];
                temp = i;
            }
        }
        printf("%s -> " , nama_kota[temp]);
        idx_tinjau = temp;
        jarak += jarak_terdekat;
        visited[temp] = 1;
    
        jumlah_visited++;
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;


    jarak += adj[idx_tinjau][idx_awal];
    printf("%s",kota_awal);
    printf("\nBest route distance: %f km" , jarak);
    printf("\nTime elapsed: %f s" , cpu_time_used);
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

    //Greedy Algorithm
    tsp_greedy();
}
