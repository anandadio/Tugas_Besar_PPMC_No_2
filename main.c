#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "variable.h"

// PSO Structures and Definitions
typedef struct {
    int *route;
    double fitness;
    int *pbestRoute;
    double pbestFitness;
} Particle;

Particle *swarm;
double gbestFitness;
int *gbestRoute;
int swarmSize = 50;
int maxIterations = 1000;

// Function declarations
double calculate(float lat1, float lat2, float long1, float long2);
void initializeSwarm();
void updateParticles();
double calculateTotalDistance(int *route, int n);

double calculate(float lat1, float lat2, float long1, float long2) {
    return 2.0 * r * asin(sqrt(pow(sin(((lat2 - lat1) / 2.0) * (PI / 180.0)), 2) + 
               (cos((lat1 * PI) / 180.0) * cos((lat2 * PI) / 180.0) * 
               pow(sin(((long2 - long1) / 2.0) * (PI / 180.0)), 2))));
}

double calculateTotalDistance(int *route, int n) {
    double totalDistance = 0.0;
    for (int i = 0; i < n - 1; i++) {
        totalDistance += adj[route[i]][route[i + 1]];
    }
    totalDistance += adj[route[n - 1]][route[0]]; // Return to start
    return totalDistance;
}

void initializeSwarm() {
    swarm = (Particle *) malloc(sizeof(Particle) * swarmSize);
    gbestRoute = (int *) malloc(sizeof(int) * jumlah_kota);
    gbestFitness = INFINITY;

    for (int i = 0; i < swarmSize; i++) {
        swarm[i].route = (int *) malloc(sizeof(int) * jumlah_kota);
        swarm[i].pbestRoute = (int *) malloc(sizeof(int) * jumlah_kota);
        for (int j = 0; j < jumlah_kota; j++) {
            swarm[i].route[j] = j;
        }
        for (int j = 0; j < jumlah_kota; j++) {
            int swapIdx = rand() % jumlah_kota;
            int temp = swarm[i].route[j];
            swarm[i].route[j] = swarm[i].route[swapIdx];
            swarm[i].route[swapIdx] = temp;
        }
        swarm[i].fitness = calculateTotalDistance(swarm[i].route, jumlah_kota);
        memcpy(swarm[i].pbestRoute, swarm[i].route, sizeof(int) * jumlah_kota);
        swarm[i].pbestFitness = swarm[i].fitness;
        if (swarm[i].fitness < gbestFitness) {
            gbestFitness = swarm[i].fitness;
            memcpy(gbestRoute, swarm[i].route, sizeof(int) * jumlah_kota);
        }
    }
}

void updateParticles() {
    for (int i = 0; i < swarmSize; i++) {
        int swapA = rand() % jumlah_kota;
        int swapB = rand() % jumlah_kota;
        int temp = swarm[i].route[swapA];
        swarm[i].route[swapA] = swarm[i].route[swapB];
        swarm[i].route[swapB] = temp;
        swarm[i].fitness = calculateTotalDistance(swarm[i].route, jumlah_kota);
        if (swarm[i].fitness < swarm[i].pbestFitness) {
            memcpy(swarm[i].pbestRoute, swarm[i].route, sizeof(int) * jumlah_kota);
            swarm[i].pbestFitness = swarm[i].fitness;
        }
        if (swarm[i].fitness < gbestFitness) {
            memcpy(gbestRoute, swarm[i].route, sizeof(int) * jumlah_kota);
            gbestFitness = swarm[i].fitness;
        }
    }
}

int main() {
    printf("Enter list of cities file name: ");
    scanf("%s", namafile);
    printf("Enter starting point: ");
    scanf("%s", kota_awal);

    FILE *stream = fopen(namafile, "r");
    if (stream == NULL) {
        printf("File tidak ditemukan\n");
        return 0;
    }

    // Mencari Jumlah Kota
    while (fgets(line, 255, stream)) {
        jumlah_kota++;
    }
    rewind(stream);

    latitude = malloc(sizeof(float) * jumlah_kota);
    longitude = malloc(sizeof(float) * jumlah_kota);
    adj = (double **) malloc(jumlah_kota * sizeof(double *));
    for (int i = 0; i < jumlah_kota; i++) {
        adj[i] = (double *) malloc(jumlah_kota * sizeof(double));
    }

    i = 0;
    while (fgets(line, 255, stream)) {
        strcpy(tempLine, line);
        token = strtok(tempLine, ",");
        nama_kota[i] = (char *) malloc(strlen(token) + 1);
        strcpy(nama_kota[i], token);
        token = strtok(NULL, ",");
        latitude[i] = atof(token);
        token = strtok(NULL, "\n");
        longitude[i] = atof(token);
        i++;
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

    fclose(stream);

    // Initialize and run PSO
    clock_t start_time = clock();
    initializeSwarm();
    for (int i = 0; i < maxIterations; i++) {
        updateParticles();
    }
    clock_t end_time = clock();
    double time_elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Best route found: ");
    for (int i = 0; i < jumlah_kota; i++) {
        printf("%s -> ", nama_kota[gbestRoute[i]]);
    }
    printf("%s\n", nama_kota[gbestRoute[0]]); // complete the route by returning to the start
    printf("Best route distance: %.5f km\n", gbestFitness);
    printf("Time elapsed: %.10f s\n", time_elapsed);

    return 0;
}
