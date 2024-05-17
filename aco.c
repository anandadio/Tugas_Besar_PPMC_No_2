#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// extern int i = 0;
extern char *nama_kota[255];
extern int jumlah_kota;
extern char kota_awal[255];
extern double **adj;

int seachIndex(){
    for(int i=0; i<jumlah_kota; i++){
        if(strcmp(kota_awal, nama_kota[i]) == 0){
            return i;
        }
    }
}

double RandomNumberGenerator(){
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


void ACO(float alpha, float beta, float evaporate, int iterasi){
    srand(time(NULL));

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
    double shortestDistance = 999999999999999.0;
    int shortestPath[jumlah_kota];
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
                        penyebut += pow(pheromone[current][k],alpha)*pow((1.0/adj[current][k]),beta);    // menjumlahkan semua penyebut
                    }
                }

                // menentukan probabilitas
                for(int k=0; k<jumlah_kota; k++){
                    if(node[k] == -1){
                        continue;       // kota yang sudah dilewati
                    }
                    else{
                        probabilitas[k] = (pow(pheromone[current][k],alpha)*pow((1.0/adj[current][k]),beta))/penyebut;    // menghitung probabilitas tiap jalur
                    }
                }
                // printf("pros\n");
                // for(int n=0; n<jumlah_kota; n++){
                //     printf("%f ", probabilitas[n]);
                // }
                // printf("\n");

                // memilih jalur
                double random = RandomNumberGenerator();
                // printf("random : %f\n",random);
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
            // printf("%s\n",nama_kota[destination]);
            totalDistance += adj[current][destination];
            path[j] = destination;
            current = destination;
            node[destination] = -1;
        }

        if(totalDistance < shortestDistance){
            shortestDistance = totalDistance;
            for(int j=0; j<jumlah_kota; j++){
                shortestPath[j] = path[j];
            }
            // update pheromone
            current = kotaAsal;
            for(int j=0; j<jumlah_kota; j++){
                for(int k=0; k<jumlah_kota; k++){
                    if(j == current && k == path[j]){
                        pheromone[current][path[j]] = (1.0 - evaporate) * pheromone[current][path[j]] + (1.0/totalDistance);
                        current = path[j];
                    }
                    else{
                        pheromone[j][k] = (1.0 - evaporate) * pheromone[j][k];      
                    }
                }
            }
        }

    // printf("total distance : %f\n", totalDistance);
    // for(int k=0; k<jumlah_kota; k++){
    //     printf("%s ", nama_kota[path[k]]);
    // }
    // printf("\n");
    // for(int k=0; k<jumlah_kota; k++){
    //     for(int j=0; j<jumlah_kota; j++){
    //         printf("%.4f ", pheromone[k][j]);
    //     }
    //     printf("\n");
    // }


    }
    
    printf("%s ", nama_kota[kotaAsal]);
    for(int j=0; j<jumlah_kota; j++){
        printf("-> %s ", nama_kota[shortestPath[j]]);
    }
    printf("\nBest route distance : %f km\n", shortestDistance);
}