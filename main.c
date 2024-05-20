#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "variable.h"

// Struktur dan Definisi PSO
typedef struct {
    int *route; // Rute yang diambil oleh partikel
    double fitness; // Nilai fitness dari rute
    int *pbestRoute; // Rute terbaik pribadi partikel
    double pbestFitness; // Nilai fitness terbaik pribadi partikel
} Particle;

Particle *swarm; // Array partikel dalam swarm
double gbestFitness; // Nilai fitness terbaik global
int *gbestRoute; // Rute terbaik global
int swarmSize = 50; // Ukuran swarm
int iMax = 1000; // Jumlah iterasi maksimal

// Deklarasi fungsi
double calculate(float lat1, float lat2, float long1, float long2);
void initSwarm(int startIdx);
void updateParticles();
void twoOptSwap(int *route, int n);
double calculateTotalDistance(int *route, int n);

// Fungsi untuk menghitung jarak antara dua koordinat
double calculate(float lat1, float lat2, float long1, float long2) {
    return 2.0 * r * asin(sqrt(pow(sin(((lat2 - lat1) / 2.0) * (PI / 180.0)), 2) + 
               (cos((lat1 * PI) / 180.0) * cos((lat2 * PI) / 180.0) * 
               pow(sin(((long2 - long1) / 2.0) * (PI / 180.0)), 2))));
}

// Fungsi untuk menghitung total jarak dari suatu rute
double calculateTotalDistance(int *route, int n) {
    double totalDistance = 0.0;
    for (int i = 0; i < n - 1; i++) {
        totalDistance += adj[route[i]][route[i + 1]];
    }
    totalDistance += adj[route[n - 1]][route[0]]; // Kembali ke titik awal
    return totalDistance;
}

// Fungsi untuk melakukan 2-opt swap pada rute
void twoOptSwap(int *route, int n) {
    int i = 1 + rand() % (n - 1);
    int j = 1 + rand() % (n - 1);
    if (i > j) {
        int temp = i;
        i = j;
        j = temp;
    }
    while (i < j) {
        int temp = route[i];
        route[i] = route[j];
        route[j] = temp;
        i++;
        j--;
    }
}

// Fungsi untuk menginisialisasi swarm
void initSwarm(int startIdx) {
    swarm = (Particle *) malloc(sizeof(Particle) * swarmSize);
    gbestRoute = (int *) malloc(sizeof(int) * jumlah_kota);
    gbestFitness = INFINITY;

    for (int i = 0; i < swarmSize; i++) {
        swarm[i].route = (int *) malloc(sizeof(int) * jumlah_kota);
        swarm[i].pbestRoute = (int *) malloc(sizeof(int) * jumlah_kota);

        // Inisialisasi rute dengan semua kota
        for (int j = 0; j < jumlah_kota; j++) {
            swarm[i].route[j] = j;
        }

        // Pastikan titik awal benar
        if (swarm[i].route[0] != startIdx) {
            for (int j = 0; j < jumlah_kota; j++) {
                if (swarm[i].route[j] == startIdx) {
                    int temp = swarm[i].route[0];
                    swarm[i].route[0] = swarm[i].route[j];
                    swarm[i].route[j] = temp;
                    break;
                }
            }
        }

        // Acak urutan kota lainnya 
        for (int j = jumlah_kota - 1; j > 1; j--) {
            int k = 1 + rand() % j; // Mulai acak dari elemen kedua
            int temp = swarm[i].route[j];
            swarm[i].route[j] = swarm[i].route[k];
            swarm[i].route[k] = temp;
        }

        // Hitung nilai fitness untuk rute awal
        swarm[i].fitness = calculateTotalDistance(swarm[i].route, jumlah_kota);
        memcpy(swarm[i].pbestRoute, swarm[i].route, sizeof(int) * jumlah_kota);
        swarm[i].pbestFitness = swarm[i].fitness;

        // Perbarui global best jika perlu
        if (swarm[i].fitness < gbestFitness) {
            gbestFitness = swarm[i].fitness;
            memcpy(gbestRoute, swarm[i].route, sizeof(int) * jumlah_kota);
        }
    }
}

// Fungsi untuk memperbarui partikel dalam swarm
void updateParticles() {
    for (int i = 0; i < swarmSize; i++) {
        // 2-opt swap untuk mengeksplorasi lingkungan
        twoOptSwap(swarm[i].route, jumlah_kota);

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
    // Meminta pengguna memasukkan nama file daftar kota
    printf("Enter list of cities file name: ");
    scanf("%s", namafile);
    getchar(); // Menghilangkan karakter newline yang tertinggal oleh scanf

    FILE *stream = fopen(namafile, "r");
    if (stream == NULL) {
        printf("File tidak ditemukan\n");
        return 0;
    }

    // Mencari jumlah kota
    jumlah_kota = 0;
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
        // Menghapus BOM jika ditemukan
        if (i == 0 && line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF') {
            memmove(line, line + 3, strlen(line) - 3 + 1);
        }
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

    // Menggunakan fgets untuk membaca input kota awal
    printf("Enter starting point: ");
    fgets(kota_awal, sizeof(kota_awal), stdin);
    
    // Menghilangkan karakter newline dari input
    kota_awal[strcspn(kota_awal, "\n")] = 0;

    int startIdx = -1;
    for (int j = 0; j < jumlah_kota; j++) {
        if (strcmp(nama_kota[j], kota_awal) == 0) { // Menggunakan strcmp() untuk membandingkan string
            startIdx = j;
            break; // Keluar dari loop jika kota ditemukan
        }
    }

    if (startIdx == -1) {
        printf("Kota awal tidak ditemukan dalam daftar.\n");
        return 0;
    }

    // Inisialisasi dan jalankan PSO
    clock_t start = clock();
    initSwarm(startIdx);
    for (int i = 0; i < iMax; i++) {
        updateParticles();
    }
    clock_t end = clock();
    double time_elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    // Menampilkan rute terbaik yang ditemukan
    printf("Best route found: ");
    printf("\n");
    for (int i = 0; i < jumlah_kota; i++) {
        printf("%s -> ", nama_kota[gbestRoute[i]]);
    }
    printf("%s\n", nama_kota[gbestRoute[0]]); // Menyelesaikan rute dengan kembali ke awal
    printf("Best route distance: %.5f km\n", gbestFitness);
    printf("Time elapsed: %.10f s\n", time_elapsed);

    return 0;
}
