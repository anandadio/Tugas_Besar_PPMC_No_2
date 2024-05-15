#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

// Eksternal global variabel
extern int jumlah_kota;
extern int *jalur_kota;
extern double jarak_total;

// Variabel global
double final_res = INT_MAX;
double epsilon = 1e-12;

// Fungsi menyalin jalur yang ditempuh
void copyToFinal(int curr_path[],int final_path[]) {
    for (int i = 0; i < jumlah_kota; i++)
        final_path[i] = curr_path[i];
    final_path[jumlah_kota] = curr_path[0]; // Kembali ke node awal
}

// Fungsi mencari jalur minimun ke-1 dari node i
double firstMin(double **adj, int i) {
    double min = INT_MAX;
    for (int k = 0; k < jumlah_kota; k++)
        if (adj[i][k] < min && i != k)
            min = adj[i][k];
    return min;
}

// Fungsi mencari jalur minimum ke-2 dari node i
double secondMin(double **adj, int i) {
    double first = INT_MAX, second = INT_MAX;
    for (int j = 0; j < jumlah_kota; j++) {
        if (i !=j){
            if (adj[i][j] <= first) {
                second = first;
                first = adj[i][j];
            } 
            else if (adj[i][j] <= second && adj[i][j] != first){
                second = adj[i][j];
            }
        }
    }
    return second;
}

// Fungsi rekursi TSP
void TSPRec(double **adj, double curr_bound, double curr_weight, int level, int curr_path[], int visited[], int final_path[]) {
    if (level == jumlah_kota) { // Sudah mencapai level akhir, leaf
        //debug
        //printf("Halo level4\n");
        if (adj[curr_path[level-1]][curr_path[0]] != 0) { // Mengecek kevalidan jalur kembali ke node awal
            double curr_res = curr_weight + adj[curr_path[level-1]][curr_path[0]];
            if (curr_res < final_res - epsilon) {
                //debug
                //printf("berat yes, %lf\n",curr_res);
                copyToFinal(curr_path, final_path);
                final_res = curr_res;
            }
        }
        return;
    }

    // Mengeksplor node pada level sekarang
    for (int i = 0; i < jumlah_kota; i++) {
        if (adj[curr_path[level-1]][i] != 0 && visited[i]!=1) {
            double temp = curr_bound;
            curr_weight += adj[curr_path[level-1]][i];

            //Menghitung pada node sekarang
            if (level == 1){ //Untuk awal
                curr_bound -= ((firstMin(adj, curr_path[level-1]) + firstMin(adj, i)) / 2.0);
                //debug
                //printf("Halo level1\n");
            }
            else{//Setelah awal, menggunakan secondMin karena firstmin sudah dihitung sebelumnya
                //debug
                //printf("Halo level%d\n",level);
                curr_bound -= ((secondMin(adj, curr_path[level-1]) + firstMin(adj, i)) / 2.0);
            }
            //Melanjutkan search, lanjut ke node selanjutnya
            if (curr_bound + curr_weight < final_res - epsilon) {
                //debug
                //printf("to node:%d\n",i);
                curr_path[level] = i;
                visited[i] = 1;
                TSPRec(adj, curr_bound, curr_weight, level + 1, curr_path, visited, final_path);
            }

            //Tidak melanjutkan search, reset
            curr_weight -= adj[curr_path[level-1]][i];
            curr_bound = temp;
            //debug
            //printf("back\n");
            memset(visited, 0, jumlah_kota * sizeof(int));
            for (int j = 0; j <= level - 1; j++)
                visited[curr_path[j]] = 1;
        }
    }
}

// Fungsi utama
void TSP(double **adj, int indeks_kota) {
    int *curr_path = (int*)malloc((jumlah_kota+1)*sizeof(int));
    int *final_path = (int*)malloc((jumlah_kota+1)*sizeof(int));
    int *visited = (int*)malloc((jumlah_kota)*sizeof(int));
    memset(curr_path, -1, (jumlah_kota + 1) * sizeof(int));
    memset(visited, 0, jumlah_kota * sizeof(int));
    double curr_bound = 0.0;

    for (int i = 0; i < jumlah_kota; i++){
        curr_bound += (firstMin(adj, i) + secondMin(adj, i));
    }
    curr_bound = curr_bound / 2.0;

    visited[indeks_kota] = 1;
    curr_path[0] = indeks_kota;

    TSPRec(adj, curr_bound, 0.0, 1, curr_path, visited, final_path);
    jarak_total = final_res;
    jalur_kota = final_path;
}
