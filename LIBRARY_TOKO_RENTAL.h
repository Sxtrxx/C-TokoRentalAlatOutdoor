#ifndef LIBRARY_TOKO_RENTAL_H
#define LIBRARY_TOKO_RENTAL_H

#include <stdio.h>

#define FILE_ALAT "data_alat.txt"
#define FILE_SEWA "data_sewa.txt"
#define FILE_TEMP "temp.txt"
#define FILE_SINKRON "temp_sink.txt"

typedef struct {
    char tanggalSewa[30];
    char namaPenyewa[40];
    char namaAlat[40];
    int jumlahAlat;
    float hargaSewa, totalHargaSewa;
    char deskAlat[61];
} tambahSewaStruct;

typedef struct {
    char tanggalAlat[10];
    char namaAlat[40];
    int stokAlat;
    float hargaAlat;
    char deskAlat[60];
} tambahAlatStruct;

void dapatkanWaktu(char *waktuString);
void clear_stdin();
int validasiString(char *buffer, int input_length);
int validasiInteger(int *num);
int validasiFloat(float *num);
void judul();
void tampilkanStok();
void tampilkanStokKosong();
void tampilkanSewa();
void pencariAlatSewa(int AlatAtauSewa, FILE *file, char *yangDicari);
int cekNamaSudahAda(int AlatAtauSewa, char *namaBaru);
int cekStokMencukupi(int jumlahDibutuhkan, char *namaAlatDibutuhkan);
void sinkronNama(int keSewaAtauAlat, char *namaAlatLama, char *namaAlatBaru, char *bufferBaru);
void sinkronStok(int kondisi, int jumlahAlatSinkron, char *namaAlatDiganti, char *bufferSewa);
void sinkronHarga(int keSewaAtauAlat, float hargaBaru, char *namaAlatDiubah);
void sinkronDesk(int keSewaAtauAlat, char *deskBaru, char *namaAlatDiubah);
void gantiStokAlatKosong();
void tambahAlat();
void hapusAlat();
void hapusAlatKosong();
void gantiAlat();
void cariAlat();
void cariAlatKosong();
void cariSewa();
char tambahSewa();
void hapusSewa();
void gantiSewa();

#endif