#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "variable.h"

double calculate(float lat1, float lat2, float long1, float long2){
    return 2.0*r*asin(sqrt(pow(sin(((lat2-lat1)/2.0)*(PI/180.0)),2)+((cos((lat1*PI)/180.0))*(cos((lat2*PI)/180.0))*pow(sin(((long2-long1)/2.0)*(PI/180.0)),2))));
}

int seachIndex(){
    for(int i=0; i<jumlah_kota; i++){
        if(strcmp(kota_awal, nama_kota[i]) == 0){
            return i;
        }
    }
}

double RandomNumberGenerator(){
    srand(time(NULL));
    int lower_bound = 1;
    int upper_bound = 1000;
    double val = (rand() % (upper_bound - lower_bound + 1) + lower_bound);

    return val/1000.0;       
}

int faktorial(int n){
    if(n == 1){
        return (1);
    }
    else{
        return (n * faktorial(n-1));
    }
}

void move(int node[jumlah_kota], int destination){
    for(int i=0; i<jumlah_kota; i++){
        if(node[i] == destination){
            node[i] = -1;
            break;
        }
    }
}

double probability(int alpha, int beta, double pheromone, double distance){
    return pow(pheromone, alpha) * pow(1.0/distance, beta);
}

void ACO(int alpha, int beta, float evaporate, int iterasi){

    double pheromone[jumlah_kota][jumlah_kota];
    for(int i=0; i<jumlah_kota; i++){
        for(int j=0; j<jumlah_kota; j++){
            if(i == j){
                pheromone[i][j] = 0.0;
            }
            else{
                pheromone[i][j] = 1.0;
            }
        }
    }


    int node[jumlah_kota];
    int path[jumlah_kota];
    double probabilitas[jumlah_kota];
    int kotaAsal = seachIndex();
    int current;
    int destination;
    for(int i=0; i<iterasi; i++){
        // pengisian node dengan index kota
        for(int j=0; j<jumlah_kota; j++){
            node[j] = j;
            path[j] = 0;
        }
        node[kotaAsal] = -1;
        int current = kotaAsal;
        double totalDistance = 0.0;
        for(int j=0; j<jumlah_kota; j++){
            for(int k=0; k<jumlah_kota; k++){
                probabilitas[k] = 0.0;
            }

            if(j == jumlah_kota-1){
                destination = kotaAsal;     // ketika semua kota sudah dilewati
            }
            else{
                // periksa pilihan kota yang belum dilewati
                double penyebut = 0.0;
                for(int k=0; k<jumlah_kota; k++){
                    if(node[k] == -1){
                        continue;       // kota yang sudah dilewati
                    }
                    else{
                        penyebut += pheromone[current][k]*(1.0/adj[current][k]);    // menjumlahkan semua penyebut
                    }
                }

                // menentukan probabilitas
                for(int k=0; k<jumlah_kota; k++){
                    if(node[k] == -1){
                        continue;       // kota yang sudah dilewati
                    }
                    else{
                        probabilitas[k] = (pheromone[current][k]*(1.0/adj[current][k]))/penyebut;    // menghitung probabilitas tiap jalur
                    }
                }
                printf("pros\n");
                for(int n=0; n<jumlah_kota; n++){
                    printf("%f ", probabilitas[n]);
                }
                printf("\n");

                // memilih jalur
                double random = RandomNumberGenerator();
                printf("random : %f\n",random);
                double sum = 0.0;
                for(int k=0; k<jumlah_kota; k++){
                    if(node[k] == -1){
                        continue;       // kota yang sudah dilewati
                    }
                    else{
                        sum += probabilitas[k];
                        if(sum >= random){
                            destination = k;        // menyimpan kota yang terpilih untuk dilewati
                            break;
                        }
                    }
                }
            }

            // update distance, posisi terkini, dan pheromone
            printf("%s\n",nama_kota[destination]);
            totalDistance += adj[current][destination];
            path[j] = destination;
            current = destination;
            move(node, destination);
        }

        current = kotaAsal;
        for(int j=0; j<jumlah_kota-1; j++){
            pheromone[current][path[j]] = (1.0 - evaporate) * pheromone[current][path[j]] + (1.0/totalDistance);
            current = path[j];
        }
    printf("total distance : %f\n", totalDistance);
    for(int k=0; k<jumlah_kota; k++){
        printf("%s ", nama_kota[path[k]]);
    }
    printf("\n");
    for(int k=0; k<jumlah_kota; k++){
        for(int j=0; j<jumlah_kota; j++){
            printf("%f ", pheromone[k][j]);
        }
        printf("\n");
    }
    }

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
    scanf("%s", kota_awal);     // Asumsi input selalu benar

    int alpha = 1;
    int beta = 1;
    float evaporate = 1.0;
    int iterasi = 120;

    ACO(alpha, beta, evaporate, iterasi);
    // printf("%f", RandomNumberGenerator());

}
