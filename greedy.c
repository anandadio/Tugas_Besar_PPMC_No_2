#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

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