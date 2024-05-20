#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

extern char *nama_kota[255];
extern int jumlah_kota;
extern char kota_awal[255];
extern double **adj;

int seachIndex(){
    // fungsi untuk mencari index dari kota yang diinputkan oleh user
    for(int i=0; i<jumlah_kota; i++){
        if(strcmp(kota_awal, nama_kota[i]) == 0){
            return i;
        }
    }
}

double RandomNumberGenerator(){
    // fungsi untuk mengenerate angka random dari 0.0 sampai 1.0
    int lower_bound = 1;
    int upper_bound = 1000;
    double val = (rand() % (upper_bound - lower_bound + 1) + lower_bound);

    return val/1000.0;       
}

void pheromoneMaking(double pheromone[jumlah_kota][jumlah_kota]){
    // fungsi untuk membuat matriks pheromone
    for(int i=0; i<jumlah_kota; i++){
        for(int j=0; j<jumlah_kota; j++){
            if(i == j){
                pheromone[i][j] = 0.0;      // mengisi matriks diagonal dengan 0
            }
            else{
                pheromone[i][j] = 1.0;      // mengisi pheromone tiap jalur dengan initial value 1
            }
        }
    }
}

void menentukanProbabilitas(double probabilitas[jumlah_kota], int node[jumlah_kota],
                            double pheromone[jumlah_kota][jumlah_kota], int current,
                            float alpha, float beta){
    // fungsi untuk menghitung probabilitas setiap jalur yang akan dilewati
    // periksa pilihan kota yang belum dilewati
    double penyebut = 0.0;
    for(int i=0; i<jumlah_kota; i++){
        if(node[i] == -1){
            continue;       // kota yang sudah dilewati
        }
        else{
            penyebut += pow(pheromone[current][i],alpha)*pow((1.0/adj[current][i]),beta);    // menjumlahkan semua penyebut
        }
    }

    // menentukan probabilitas
    for(int i=0; i<jumlah_kota; i++){
        if(node[i] == -1){
            continue;       // kota yang sudah dilewati
        }
        else{
            probabilitas[i] = (pow(pheromone[current][i],alpha)*pow((1.0/adj[current][i]),beta))/penyebut;    // menghitung probabilitas tiap jalur
        }
    }
}

void memilihJalur(int node[jumlah_kota], double probabilitas[jumlah_kota], int *destination){
    // fungsi untuk memilih jalur yang akan dilewati
    double random = RandomNumberGenerator();        // generate angka random dari 0.0 sampai 1.0
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
    // update distance setiap melewati kota dan posisi terkini dari semutnya
    *totalDistance += adj[*current][destination];
    path[j] = destination;
    node[destination] = -1;
    *current = destination;
}

void updatePheromone(double pheromone[jumlah_kota][jumlah_kota], int path[jumlah_kota],
                    int kotaAsal, int *current, double evaporate, double totalDistance){
    // fungsi untuk update pheromone setelah semut selesai sampai tujuan
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

int checkPath(int path[jumlah_kota]){
    // fungsi untuk memeriksa apakah ada kota yang dilwati lebih dari 1 kali
    for(int i=0; i<jumlah_kota; i++){
        int jumlah = 0;
        for(int j=0; j<jumlah_kota; j++){
            if(path[j] == i){
                jumlah++;
            }
        }
        if(jumlah > 1){
            return 0;   // jika ada kota yang dilewati lebih dari 1 kali
        }
    }

    return 1;   // jika semua kota tepat dilewati 1 kali
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
    pheromoneMaking(pheromone);     // membuat matriks pheromone


    int node[jumlah_kota];
    int path[jumlah_kota];
    double probabilitas[jumlah_kota];
    int current;
    int destination;
    double shortestDistance = 999999999999999.0;
    int shortestPath[jumlah_kota];
    int kotaAsal = seachIndex();    // mencari index dari kota awal yang diinputkan

    for(int i=0; i<iterasi; i++){
        // pengisian node dengan index kota
        for(int j=0; j<jumlah_kota; j++){
            node[j] = j;
            path[j] = 0;
        }
        node[kotaAsal] = -1;        // mengeliminasi kota awal
        int current = kotaAsal;
        double totalDistance = 0.0;
        for(int j=0; j<jumlah_kota; j++){
            for(int k=0; k<jumlah_kota; k++){
                probabilitas[k] = 0.0;      // mengisi array probabilitas dengan 0
            }

            if(j == jumlah_kota-1){
                destination = kotaAsal;     // ketika semua kota sudah dilewati
            }
            else{
                // menghitung probabilitas tiap jalur
                menentukanProbabilitas(probabilitas, node, pheromone, current, alpha, beta);

                // memilih jalur yang tersedia sesuai dengan probabilitas
                memilihJalur(node, probabilitas, &destination);
            }

            // update distance
            updateDistance(node, &current, path, &totalDistance, destination, j);
        }

        // jika ditemukan jalur terpendek
        if(totalDistance < shortestDistance && checkPath(path) == 1){
            shortestDistance = totalDistance;
            for(int j=0; j<jumlah_kota; j++){
                shortestPath[j] = path[j];
            }
            // update pheromone
            updatePheromone(pheromone, path, kotaAsal, &current, evaporate, totalDistance);
        }

    }
    
    // mencetak kota yang dilewati berdasarkan jarak terpendek
    printPath(shortestPath, shortestDistance, kotaAsal);
    
    // mencetak waktu yang dibutuhkan untuk mendapatkan hasil jalur terpendek
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %f s\n", time_spent);
}