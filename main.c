#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "variable.h"

#define MAX_GENERATIONS 100
#define POPULATION_SIZE 10
#define MUTATION_RATE 0.1

struct City {
    double *distance;
};

struct Route {
    int *path;
    double fitness;
};

double calculate(float lat1, float lat2, float long1, float long2) {
    return 2.0 * r * asin(sqrt(pow(sin(((lat2 - lat1) / 2.0) * (PI / 180.0)), 2) + 
                 ((cos((lat1 * PI) / 180.0)) * (cos((lat2 * PI) / 180.0)) * 
                 pow(sin(((long2 - long1) / 2.0) * (PI / 180.0)), 2))));
}

void calculateDistance(struct Route *route, struct City *cities, int jumlah_kota, int startNode) {
    double totalDistance = 0.0;
    for (int i = 0; i < jumlah_kota - 1; ++i) {
        totalDistance += cities[route->path[i]].distance[route->path[i + 1]];
    }
    totalDistance += cities[route->path[jumlah_kota - 1]].distance[route->path[0]];
    route->fitness = totalDistance;
}

struct Route crossover(struct Route parent1, struct Route parent2, int jumlah_kota, int startNode) {
    struct Route child;
    child.path = (int*)malloc(jumlah_kota * sizeof(int));
    int startPos = rand() % (jumlah_kota - 1) + 1;

    int *used = (int*)calloc(jumlah_kota, sizeof(int));
    for (int i = 0; i < startPos; ++i) {
        child.path[i] = parent1.path[i];
        used[child.path[i]] = 1;
    }

    int childIndex = startPos;
    for (int i = 0; i < jumlah_kota; ++i) {
        if (!used[parent2.path[i]]) {
            child.path[childIndex++] = parent2.path[i];
        }
    }
    child.path[0] = startNode;
    free(used);
    return child;
}

void mutate(struct Route *route, int jumlah_kota) {
    for (int i = 1; i < jumlah_kota; ++i) {
        if ((double)rand() / RAND_MAX < MUTATION_RATE) {
            int swapIndex = rand() % (jumlah_kota - 1) + 1;
            int temp = route->path[i];
            route->path[i] = route->path[swapIndex];
            route->path[swapIndex] = temp;
        }
    }
}

struct Route findBestRoute(struct Route *population, int jumlah_kota) {
    double bestFitness = DBL_MAX;
    int bestIndex = -1;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        if (population[i].fitness < bestFitness) {
            bestFitness = population[i].fitness;
            bestIndex = i;
        }
    }
    return population[bestIndex];
}

void genetika(double **adjacent, int jumlah_kota, char **nama_kota, int startNode) {
    srand(time(NULL));
    struct City *cities = (struct City*)malloc(jumlah_kota * sizeof(struct City));
    for (int i = 0; i < jumlah_kota; ++i) {
        cities[i].distance = adjacent[i];
    }

    struct Route *population = (struct Route*)malloc(POPULATION_SIZE * sizeof(struct Route));
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].path = (int*)malloc(jumlah_kota * sizeof(int));
        population[i].path[0] = startNode;
        for (int j = 1, k = 0; j < jumlah_kota; ++j) {
            if (k == startNode) k++;
            population[i].path[j] = k++;
        }
        for (int j = 1; j < jumlah_kota; ++j) {
            int swapIndex = rand() % (jumlah_kota - 1) + 1;
            int temp = population[i].path[j];
            population[i].path[j] = population[i].path[swapIndex];
            population[i].path[swapIndex] = temp;
        }
        calculateDistance(&population[i], cities, jumlah_kota, startNode);
    }

    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        struct Route *newPopulation = (struct Route*)malloc(POPULATION_SIZE * sizeof(struct Route));
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            struct Route parent1 = findBestRoute(population, jumlah_kota);
            struct Route parent2 = findBestRoute(population, jumlah_kota);
            struct Route child = crossover(parent1, parent2, jumlah_kota, startNode);
            mutate(&child, jumlah_kota);
            calculateDistance(&child, cities, jumlah_kota, startNode);
            newPopulation[i] = child;
        }
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            free(population[i].path);
        }
        free(population);
        population = newPopulation;
    }

    struct Route bestRoute = findBestRoute(population, jumlah_kota);

    printf("Best Route: ");
    for (int i = 0; i < jumlah_kota; ++i) {
        printf("%s -> ", nama_kota[bestRoute.path[i]]);
    }
    printf("%s\n", nama_kota[startNode]);

    printf("Total Distance: %f\n", bestRoute.fitness);

    for (int i = 0; i < POPULATION_SIZE; ++i) {
        free(population[i].path);
    }
    free(population);
    free(cities);
    for (int i = 0; i < jumlah_kota; i++) {
        free(adj[i]);
        free(nama_kota[i]);
    }
    free(adj);
    free(nama_kota);
    free(latitude);
    free(longitude);
}


int main() {
    srand(42); // Fixed seed for reproducibility
    int jumlah_kota = 0;
    int startNode = -1;
    char line[255];
    char tempLine[255];
    char namafile[255];
    char *token;
    char **nama_kota;
    float *latitude, *longitude;
    double **adj;

    printf("Masukkan Nama File: ");
    scanf("%s", namafile);

    FILE* stream = fopen(namafile, "r");
    if (stream == NULL) {
        printf("File tidak ditemukan");
        return 0;
    }

    // Mencari Jumlah Kota
    while (fgets(line, 255, stream)) {
        jumlah_kota++;
    }
    rewind(stream);

    nama_kota = (char**)malloc(jumlah_kota * sizeof(char*));
    latitude = (float*)malloc(jumlah_kota * sizeof(float));
    longitude = (float*)malloc(jumlah_kota * sizeof(float));

    // Parsing
    while (fgets(line, 255, stream)) {
        strcpy(tempLine, line);
        token = strtok(tempLine, ",");
        nama_kota[i] = (char*)malloc(strlen(token) + 1);
        strcpy(nama_kota[i], token);
        token = strtok(NULL, ",");
        latitude[i] = atof(token);
        token = strtok(NULL, "\n");
        longitude[i] = atof(token);
        i++;
    }

    // adj graph
    adj = (double**)malloc(jumlah_kota * sizeof(double*));
    for (int i = 0; i < jumlah_kota; i++) {
        adj[i] = (double*)malloc(jumlah_kota * sizeof(double));
    }

    for (int i = 0; i < jumlah_kota; i++) {
        for (int j = 0; j < jumlah_kota; j++) {
            if (i == j) {
                adj[i][j] = 0.0;
            } else {
                adj[i][j] = calculate(latitude[i], latitude[j], longitude[i], longitude[j]);
            }
        }
    }

    printf("Masukkan Kota Awal: ");
    scanf("%s", kota_awal); printf("\n");
    for(int j = 0; j < jumlah_kota; j++){
        if(strcmp(nama_kota[j], kota_awal) == 0){ // Menggunakan strcmp() untuk membandingkan string
        startNode = j;
        break; // Keluar dari loop jika kota ditemukan
        }
   }
    // Memanggil fungsi untuk menghitung jalur terpendek
    clock_t start_time = clock();
    if(startNode == -1){
        printf("Nama kota tidak ditemukan\n");
    }
    else{
        genetika(adj, jumlah_kota, nama_kota, startNode);
    }
    clock_t end_time = clock();
    double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Time elapsed: %f s\n", elapsed_time);
    return 0;
}

