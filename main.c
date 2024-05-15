#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h> // Untuk menggunakan DBL_MAX
#include <time.h>
#include "variable.h"

#define N 100 // Misalkan jumlah kota maksimum adalah 100

double calculate(float lat1, float lat2, float long1, float long2) {
    return 2.0 * r * asin(sqrt(pow(sin(((lat2 - lat1) / 2.0) * (PI / 180.0)), 2) + ((cos((lat1 * PI) / 180.0)) * (cos((lat2 * PI) / 180.0)) * pow(sin(((long2 - long1) / 2.0) * (PI / 180.0)), 2))));
}

void Menemukan_rute(double graph[N][N], int curr_path[], double curr_cost, int visited[], int level, double* rute_minimun, int final_path[], int start) {
    if (level == jumlah_kota) {
        // Menambahkan biaya kembali ke kota awal
        curr_cost += graph[curr_path[level - 1]][curr_path[0]];
        // Memperbarui jalur terpendek jika ditemukan
        if (curr_cost <= *rute_minimun) {
            *rute_minimun = curr_cost;
            for (int i = 0; i < jumlah_kota; i++) {
                final_path[i] = curr_path[i];
            }
        }
        return;
    }

    for (int i = 0; i < jumlah_kota; i++) {
        if (!visited[i]) {
            visited[i] = 1;
            curr_path[level] = i;
            double jarak_tambahan = graph[curr_path[level - 1]][curr_path[level]];
            // Memanggil fungsi rekursif untuk level berikutnya
            Menemukan_rute(graph, curr_path, curr_cost + jarak_tambahan, visited, level + 1, rute_minimun, final_path, start);
            // Backtracking
            visited[i] = 0;
        }
    }
}

void Menghitung_jarak_terpendek(double graph[N][N], int start) {
    int curr_path[N];
    int visited[N];
    int final_path[N];
    double min_cost = INFINITY; // Gunakan INFINITY dari float.h untuk inisialisasi biaya minimum

    // Inisialisasi array visited
    for (int i = 0; i < jumlah_kota; i++) {
        visited[i] = 0;
    }

    // Awalnya, kota pertama adalah kota awal
    curr_path[0] = start;
    visited[start] = 1;

    // Memanggil fungsi rekursif untuk mencari jalur terpendek
    Menemukan_rute(graph, curr_path, 0, visited, 1, &min_cost, final_path, start);

    // Output jalur terpendek
    printf("Jalur terpendek: ");
    for (int i = 0; i < jumlah_kota; i++) {
        printf("%s->", nama_kota[final_path[i]]);
    }
    printf("%s\n", kota_awal); // Kembali ke kota awal

    printf("Jarak terbaik yang ditemukan adalah: %f km\n", min_cost);
}

int main() {
    printf("Masukkan Nama File: ");
    scanf("%s", namafile);

    FILE* stream = fopen(namafile, "r");
    if (stream == NULL) {
        printf("File tidak ditemukan");
        return 0;
    }

    // Menginisialisasi jumlah kota dan variabel parsing
    jumlah_kota = 0;
    char line[255];
    char tempLine[255];
    char* token;
    int i = 0;

    // Menghitung jumlah kota dan alokasi memori untuk latitude dan longitude
    while (fgets(line, 255, stream)) {
        jumlah_kota++;
    }
    rewind(stream);

    latitude = malloc(sizeof(float) * jumlah_kota);
    longitude = malloc(sizeof(float) * jumlah_kota);

    // Parsing data kota dari file
    while (fgets(line, 255, stream)) {
        // Menghapus BOM jika ditemukan
        if (line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF') {
            memmove(line, line + 3, strlen(line) - 2);
        }
        
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

    // Membuat matriks adjasensi berdasarkan jarak antar kota
    int banyak = i;
    double adj[N][N]; // Deklarasi matriks adj sebagai double[N][N]

    // Mengisi matriks adj dengan jarak antar kota
    for (int i = 0; i < jumlah_kota; i++) {
        for (int j = 0; j < jumlah_kota; j++) {
            if (i == j) {
                adj[i][j] = 0.0;
            }   
            else {
                adj[i][j] = calculate(latitude[i], latitude[j], longitude[i], longitude[j]);
            }
            printf("%f |", adj[i][j]);
        }
        printf("\n");
    }
    int cek = -1;
    // Meminta input untuk kota awal
    printf("Masukkan Kota Awal: ");
    scanf("%s", kota_awal);
    for(int j = 0; j < banyak; j++){
        if(strcmp(nama_kota[j], kota_awal) == 0){ // Menggunakan strcmp() untuk membandingkan string
        cek = j;
        break; // Keluar dari loop jika kota ditemukan
        }
   }
    // Memanggil fungsi untuk menghitung jalur terpendek
    clock_t begin = clock();
    if(cek == -1){
        printf("Nama kota tidak ditemukan\n");
    }
    else{
        Menghitung_jarak_terpendek(adj, cek);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %f s\n", time_spent);
    return 0;
}
