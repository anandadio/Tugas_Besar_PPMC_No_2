//Input user
char namafile[255];
char kota_awal[255];
int jumlah_kota;
char line[255];
char tempLine[255];
char *token;
int i = 0;
char *nama_kota[255];
float *latitude;
float *longitude;


//Kalkulasi Jarak
double PI = 3.14159265358979323846;
float r = 6371.0;

//Matrix adj graph jarak kota
double **adj;