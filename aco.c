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

void menentukanProbabilitas(double probabilitas[jumlah_kota], int node[jumlah_kota],
                            double pheromone[jumlah_kota][jumlah_kota], int current,
                            float alpha, float beta){
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
}

void memilihJalur(int node[jumlah_kota], double probabilitas[jumlah_kota], int *destination){
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
                *destination = k;        // menyimpan kota yang terpilih untuk dilewati
                break;
            }
        }
    }
}

void updateDistance(int node[jumlah_kota], int *current,
                    int path[jumlah_kota], double *totalDistance, 
                    int destination, int j){
    // update distance, posisi terkini
    *totalDistance += adj[*current][destination];
    path[j] = destination;
    node[destination] = -1;
    *current = destination;
}

void updatePheromone(double pheromone[jumlah_kota][jumlah_kota], int path[jumlah_kota],
                    int kotaAsal, int *current, double evaporate, double totalDistance){
    *current = kotaAsal;
    for(int j=0; j<jumlah_kota; j++){
        for(int k=0; k<jumlah_kota; k++){
            if(j == *current && k == path[j]){
                pheromone[*current][path[j]] = (1.0 - evaporate) * pheromone[*current][path[j]] + (1.0/totalDistance);
                *current = path[j];
            }
            else{
                pheromone[j][k] = (1.0 - evaporate) * pheromone[j][k];      
            }
        }
    }
}

void printPath(int shortestPath[jumlah_kota], double shortestDistance, int kotaAsal){
    printf("%s ", nama_kota[kotaAsal]);
    for(int j=0; j<jumlah_kota; j++){
        printf("-> %s ", nama_kota[shortestPath[j]]);
    }
    printf("\nBest route distance : %f km\n", shortestDistance);
}

void ACO(float alpha, float beta, float evaporate, int iterasi){
    clock_t begin = clock();
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
                // menghitung probabilitas tiap jalur
                menentukanProbabilitas(probabilitas, node, pheromone, current, alpha, beta);

                // memilih jalur yang tersedia
                memilihJalur(node, probabilitas, &destination);
            }

            // update distance
            updateDistance(node, &current, path, &totalDistance, destination, j);
        }

        if(totalDistance < shortestDistance){
            shortestDistance = totalDistance;
            for(int j=0; j<jumlah_kota; j++){
                shortestPath[j] = path[j];
            }
            // update pheromone
            updatePheromone(pheromone, path, kotaAsal, &current, evaporate, totalDistance);
        }

    }
    
    printPath(shortestPath, shortestDistance, kotaAsal);
    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %f s\n", time_spent);

}