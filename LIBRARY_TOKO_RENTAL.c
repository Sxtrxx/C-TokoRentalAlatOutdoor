#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "LIBRARY_TOKO_RENTAL.h"

#define FILE_ALAT "data_alat.txt"
#define FILE_SEWA "data_sewa.txt"
#define FILE_TEMP "temp.txt"
#define FILE_SINKRON "temp_sink.txt"

int barisAlatStok, barisListSewa, barisAlatStokKosong, barisAlatStokBeneran, barisListSewaBeneran;

void dapatkanWaktu(char *waktuString){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(waktuString, 50, "%d-%m-%Y", tm);
}

void clear_stdin() {
  char c;
  while ((c = getchar()) != '\n' && c != EOF);
}

int validasiString(char *buffer, int input_length) {
  fgets(buffer, input_length, stdin);

  if (buffer[strlen(buffer) - 1] != '\n') {

    clear_stdin();
    printf("Input terlalu panjang, maksimal %d karakter...\n\n",
           input_length - 2);
    return 0;                
  } else {

    buffer[strlen(buffer) - 1] = '\0';
  }

  return 1;
}

int validasiInteger(int *num) {
    char buffer[100], *leftover;
    int result = 0;

    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = 0;

    // Cek apakah input kosong
    if (strcmp(buffer, "") == 0) {
        printf("\nInput tidak boleh kosong. Silahkan coba lagi.\n\n");
        return 0;
    }

    errno = 0;

    *num = strtol(buffer, &leftover, 10);

    if (*leftover != '\0') {
        printf("Input tidak valid, harus berupa angka. Silahkan coba lagi.\n");
        return 0; 
    }

     if (*num < 0) {
        puts("Input tidak boleh negatif. Silahkan coba lagi.");
        return 0; 
    }

    return 1;
}

int validasiFloat(float *num) {
    char buffer[100], *leftover;

    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = 0;

    if (strcmp(buffer, "") == 0) {
        puts("Input tidak boleh kosong. Silahkan coba lagi.");
        return 0;
    }

    errno = 0;

    *num = strtof(buffer, &leftover);

    if (*leftover != '\0') {
        puts("Input tidak valid, harus berupa angka desimal. Silahkan coba lagi.");
        return 0; 
    }

    if (*num < 0) {
        puts("Input tidak boleh negatif. Silahkan coba lagi.");
        return 0; 
    }

    return 1;
}

void judul(){
    puts("==========================================================================================================================================");
    puts("|                                                                                                                                        |");
    puts("|                                                        TOKO RENTAL ALAT OUTDOOR                                                        |");
    puts("|                                                               MAS FAJAR                                                                |");
    puts("|                                                                                                                                        |");
    puts("==========================================================================================================================================");
}

void tampilkanStok(){
    FILE *file = fopen(FILE_ALAT, "r");
    puts("\n------------------------------------------------------------------------------------------------------------------------------------------");
    puts("|                                                            DAFTAR STOK ALAT                                                            |");
    puts("|----------------------------------------------------------------------------------------------------------------------------------------|");
    
    char buffer[256];
    char read[1]; 
    int nomor=1, nomorBeneran=0;

    puts("| No |                Nama Alat                | Stok | Harga Sewa Per Hari |                      Deskripsi Alat                        |");
    puts("|----------------------------------------------------------------------------------------------------------------------------------------|");
    while (fgets(buffer, sizeof(buffer), file)){
        if ((file == NULL)){
        printf("|                                                          Belum ada stok alat.                                                          |\n");
        puts("------------------------------------------------------------------------------------------------------------------------------------------");
        return;
        }    

        if (strlen(buffer) > 1){
            int stokAlat;
            char namaAlat[30], deskAlat[100];
            float hargaAlat;

            sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
            if(stokAlat > 0){
                printf("| %2d | %-39s | %4d | Rp%17.2f | %58s |\n", nomor, namaAlat, stokAlat, hargaAlat, deskAlat);
                nomor+=1;
            } else {
                nomorBeneran++;
            }
        }
    }
    puts("------------------------------------------------------------------------------------------------------------------------------------------");
    barisAlatStok=nomor-1;
    barisAlatStokBeneran=barisAlatStok+nomorBeneran-1;
    fclose(file);
}

void tampilkanStokKosong(){
    FILE *file = fopen(FILE_ALAT, "r");
    puts("\n------------------------------------------------------------------------------------------------------------------------------------------");
    puts("|                                                        DAFTAR STOK ALAT KOSONG                                                         |");
    puts("|----------------------------------------------------------------------------------------------------------------------------------------|");
    
    char buffer[256];
    char read[1]; 
    int nomor=1;

    puts("| No |                Nama Alat                | Stok | Harga Sewa Per Hari |                      Deskripsi Alat                        |");
    puts("|----------------------------------------------------------------------------------------------------------------------------------------|");
    while (fgets(buffer, sizeof(buffer), file)){
        if ((file == NULL)){
        printf("|                                                          Belum ada stok alat.                                                          |\n");
        puts("------------------------------------------------------------------------------------------------------------------------------------------");
        return;
        }    

        if (strlen(buffer) > 1){
            int stokAlat;
            char namaAlat[30], deskAlat[100];
            float hargaAlat;

            sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
            if(stokAlat <= 0){
                printf("| %2d | %-39s | %4d | Rp%17.2f | %58s |\n", nomor, namaAlat, stokAlat, hargaAlat, deskAlat);
                nomor+=1;
            }
        }
    }
    puts("------------------------------------------------------------------------------------------------------------------------------------------");
    barisAlatStokKosong=nomor-1;
    fclose(file);
}

void tampilkanSewa(){
    FILE *file = fopen(FILE_SEWA, "r");
    puts("\n------------------------------------------------------------------------------------------------------------------------------------------");
    puts("|                                                             LIST DATA SEWA                                                             |");
    puts("------------------------------------------------------------------------------------------------------------------------------------------");
    
    char buffer[256], buffera[30]; 
    int nomor=1, banyak;

    puts("| No |           Nama Penyewa           |           Alat Disewa            | Harga Sewa per Hari | Total Sewa Per Hari | Tanggal Menyewa |");
    puts("------------------------------------------------------------------------------------------------------------------------------------------");
    rewind(file);
    while (fgets(buffer, sizeof(buffer), file)){
        if ((file == NULL)){
        printf("|                                                          Belum ada data sewa.                                                          |\n");
        puts("------------------------------------------------------------------------------------------------------------------------------------------");
        return;
        }    
    
        if (strlen(buffer) > 1){
            char namaAlat[30], namaPenyewa[30], tanggalSewa[25];
            int jumlahAlat;
            float hargaSewa, totalHargaSewa;

            sscanf(buffer, "%[^,],%d %[^,],%f,%[^,]", &namaPenyewa, &jumlahAlat, &namaAlat, &hargaSewa, &tanggalSewa);
            printf("| %2d | %-32s | %3d %-28s | Rp%17.2f | Rp%17.2f | %15s |\n", nomor, namaPenyewa, jumlahAlat, namaAlat, hargaSewa, jumlahAlat*hargaSewa, tanggalSewa);
            nomor+=1;
        }
    }
    puts("------------------------------------------------------------------------------------------------------------------------------------------");
    barisListSewa=nomor-1;

    fclose(file);
}

void pencariAlatSewa(int AlatAtauSewa, FILE *file, char *yangDicari){
    if (file == NULL){
        printf("File tidak ditemukan.\n");
        return;
    }


    system("cls");
    judul();
    if (AlatAtauSewa == 1){
        char buffer[192], namaAlat[40], deskAlat[60];
        int stokAlat, nomor=0, baris=0;
        float hargaAlat;

        puts("\n------------------------------------------------------------------------------------------------------------------------------------------");
        puts("|                                                            HASIL PENCARIAN                                                             |");
        puts("|----------------------------------------------------------------------------------------------------------------------------------------|");
        puts("| No |                Nama Alat                | Stok | Harga Sewa Per Hari |                      Deskripsi Alat                        |");
        puts("|----------------------------------------------------------------------------------------------------------------------------------------|");
        
        while (fgets(buffer,sizeof(buffer),file)){
            sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
            if(stokAlat<=0){
                continue;
            } else { nomor++; }
            if(strstr(buffer, yangDicari) != NULL){
                printf("| %2d | %-39s | %4d | Rp%17.2f | %58s |\n", nomor, namaAlat, stokAlat, hargaAlat, deskAlat);
                baris++;
            }
        }

        if(baris<=0){
            printf("|                                                       Pencarian tidak ditemukan.                                                       |\n");
        }

        puts("------------------------------------------------------------------------------------------------------------------------------------------");
        return;
    } else if (AlatAtauSewa == 2){
        char buffer[192], namaAlat[40], namaPenyewa[40], tanggalSewa[20];
        int jumlahAlat, nomor=1, baris=0;
        float hargaSewa;

        puts("\n------------------------------------------------------------------------------------------------------------------------------------------");
        puts("|                                                             LIST DATA SEWA                                                             |");
        puts("------------------------------------------------------------------------------------------------------------------------------------------");
        puts("| No |           Nama Penyewa           |           Alat Disewa            | Harga Sewa per Hari | Total Sewa Per Hari | Tanggal Menyewa |");
        puts("------------------------------------------------------------------------------------------------------------------------------------------");
        
        while (fgets(buffer,sizeof(buffer),file)){
            if(strstr(buffer, yangDicari) != NULL){
                sscanf(buffer, "%[^,],%d %[^,],%f,%[^,]", &namaPenyewa, &jumlahAlat, &namaAlat, &hargaSewa, &tanggalSewa);
                printf("| %2d | %-32s | %3d %-28s | Rp%17.2f | Rp%17.2f | %15s |\n", nomor, namaPenyewa, jumlahAlat, namaAlat, hargaSewa, jumlahAlat*hargaSewa, tanggalSewa);
                baris++;
            }
            nomor++;
        }

        if(baris<=0){
            printf("|                                                       Pencarian tidak ditemukan.                                                       |\n");
        }

        puts("------------------------------------------------------------------------------------------------------------------------------------------");
        return;
    } else if (AlatAtauSewa == 3){
        char buffer[192], namaAlat[40], deskAlat[60];
        int stokAlat, nomor=0, baris=0;
        float hargaAlat;

        puts("\n------------------------------------------------------------------------------------------------------------------------------------------");
        puts("|                                                            HASIL PENCARIAN                                                             |");
        puts("|----------------------------------------------------------------------------------------------------------------------------------------|");
        puts("| No |                Nama Alat                | Stok | Harga Sewa Per Hari |                      Deskripsi Alat                        |");
        puts("|----------------------------------------------------------------------------------------------------------------------------------------|");
        
        while (fgets(buffer,sizeof(buffer),file)){
            sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
            if(stokAlat==0){
                nomor++;
            } else {
                continue; 
            }
            if(strstr(buffer, yangDicari) != NULL){
                printf("| %2d | %-39s | %4d | Rp%17.2f | %58s |\n", nomor, namaAlat, stokAlat, hargaAlat, deskAlat);
                baris++;
            }
        }

        if(baris<=0){
            printf("|                                                       Pencarian tidak ditemukan.                                                       |\n");
        }

        puts("------------------------------------------------------------------------------------------------------------------------------------------");
        return;
    }
}

int cekNamaSudahAda(int AlatAtauSewa, char *namaBaru){
    char buffer[256];
    if(AlatAtauSewa == 1){
        FILE *file = fopen(FILE_ALAT, "r");
        while (fgets(buffer, sizeof(buffer), file)){
            if (strlen(buffer) > 1){
                int stokAlat;
                char namaAlat[30], deskAlat[100];
                float hargaAlat;

                sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
                if(strcmp(namaBaru, namaAlat)==0){
                    if(stokAlat>0){
                        fclose(file);
                        return 1;
                    } else {
                        return 2;
                    }
                }
            }
        }

        fclose(file);
        return 0;
    } else if (AlatAtauSewa ==2){
        FILE *file = fopen(FILE_SEWA, "r");
        while (fgets(buffer, sizeof(buffer), file)){
            if (strlen(buffer) > 1){
                char namaAlat[30], namaPenyewa[30], tanggalSewa[25];
                int jumlahAlat;
                float hargaSewa, totalHargaSewa;

                sscanf(buffer, "%[^,],%d %[^,],%f,%[^,]", &namaPenyewa, &jumlahAlat, &namaAlat, &hargaSewa, &tanggalSewa);
                if(strcmp(namaBaru, namaPenyewa)==0){
                    fclose(file);
                    return 1;
                }
            }
        }

        fclose(file);
        return 0;
    }
}

int cekStokMencukupi(int jumlahDibutuhkan, char *namaAlatDibutuhkan){
    char buffer[256];
    int alatDitemukan;
    FILE *file = fopen(FILE_ALAT, "r");
    while (fgets(buffer, sizeof(buffer), file)){
        if (strlen(buffer) > 1){
            int stokAlat;
            char namaAlat[30], deskAlat[100];
            float hargaAlat;

            sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
            if(strcmp(namaAlatDibutuhkan, namaAlat)==0){
                alatDitemukan=1;
                if(jumlahDibutuhkan > stokAlat){
                    fclose(file);
                    return 0;
                } else {
                    fclose(file);
                    return 1;
                }
            }
        }
    }

    if(alatDitemukan==0){
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

void sinkronNama(int keSewaAtauAlat, char *namaAlatLama, char *namaAlatBaru, char *bufferBaru){
    char buffer[256];
    int barisDitemukan=0;
    if (keSewaAtauAlat == 2){
        FILE *file = fopen(FILE_ALAT, "r");
        FILE *tempfile = fopen(FILE_SINKRON, "w");

        if (file == NULL || tempfile == NULL) {
            printf("Gagal membuka file.\n");
            return;
        }

        rewind(file);
        while (fgets(buffer, sizeof(buffer), file)){
            if (strlen(buffer) > 1){
                int stokAlat, nomor=1;
                char namaAlat[30], deskAlat[100];
                float hargaAlat;
    
                sscanf(buffer, "%[^,]", &namaAlat);
                if (strcmp(namaAlatLama, namaAlat) == 0){
                    barisDitemukan+=1;
                    sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
                    fprintf(tempfile,"%s,%d,%.2f,%s\n", namaAlatBaru, stokAlat, hargaAlat, deskAlat);
                } else {
                    fprintf(tempfile,"%s",buffer);
                }
            }
        }

        fclose(file);
        fclose(tempfile);

        remove(FILE_ALAT);
        rename(FILE_SINKRON, FILE_ALAT);
    } else if (keSewaAtauAlat == 1){
        FILE *file = fopen(FILE_SEWA, "r");
        FILE *tempfile = fopen(FILE_SINKRON, "w");
        
        if (file == NULL || tempfile == NULL) {
            printf("Gagal membuka file.\n");
            return;
        }

        rewind(file);
        while (fgets(buffer, sizeof(buffer), file)){
            if (strlen(buffer) > 1){
                char namaAlatSewa[30], namaPenyewa[30], tanggalSewa[25], deskAlat[100];
                int jumlahAlat;
                float hargaSewa, totalHargaSewa;

                sscanf(buffer, "%[^,],%d %[^,],%f,%[^,],%[^\n]", &namaPenyewa, &jumlahAlat, &namaAlatSewa, &hargaSewa, &tanggalSewa, &deskAlat);
                if (strcmp(namaAlatLama, namaAlatSewa) == 0){
                    barisDitemukan+=1;
                    fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewa, jumlahAlat, namaAlatBaru, hargaSewa, tanggalSewa, deskAlat);
                } else {
                    fprintf(tempfile,"%s",buffer);
                }
            }
        }

        fclose(file);
        fclose(tempfile);

        remove(FILE_SEWA);
        rename(FILE_SINKRON, FILE_SEWA);
    }
    
    system("cls");
	return;
}

void sinkronStok(int kondisi, int jumlahAlatSinkron, char *namaAlatDiganti, char *bufferSewa){
    int nomor=1, barisDitemukan=0;
    char buffer[256];

    FILE *file = fopen(FILE_ALAT, "r");
    FILE *tempfile = fopen(FILE_SINKRON, "w");

    if (file == NULL || tempfile == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }   

    rewind(file);
    while (fgets(buffer, sizeof(buffer), file)){
        if (strlen(buffer) > 1){
            char namaAlatCari[30];
            sscanf(buffer, "%[^,]", &namaAlatCari);
            if (strcmp(namaAlatCari, namaAlatDiganti) == 0){
                int stokAlat;
                char namaAlat[30], deskAlat[100];
                float hargaAlat;

                barisDitemukan+=1;
                sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
                switch (kondisi){
                case 1:
                    stokAlat -= jumlahAlatSinkron;
                    break;
                case 2:
                    stokAlat += jumlahAlatSinkron;
                    break;
                case 9:
                    stokAlat = jumlahAlatSinkron;
                    break;
                case 0:
                    break;
                default:
                    break;
                }

                fprintf(tempfile, "%s,%d,%.2f,%s\n", namaAlat, stokAlat, hargaAlat, deskAlat);
            } else {
                fprintf(tempfile,"%s",buffer);
            }
        }
    }

    if (barisDitemukan==0){
        char namaAlat[30], deskAlat[61  ];
        int jumlahAlat;
        float hargaSewa;
        char namaPenyewa[30], tanggalSewa[25];
        sscanf(bufferSewa, "%[^,],%d %[^,],%f,%[^,],%[^\n]", &namaPenyewa, &jumlahAlat, &namaAlat, &hargaSewa, &tanggalSewa, &deskAlat);
        fprintf(tempfile, "%s,%d,%.2f,%s\n", namaAlat, jumlahAlat, hargaSewa, deskAlat);
    }

    fclose(file);
    fclose(tempfile);

    remove(FILE_ALAT);
    rename(FILE_SINKRON, FILE_ALAT);

    system("cls");
	return;
}

void sinkronHarga(int keSewaAtauAlat, float hargaBaru, char *namaAlatDiubah){
    char buffer[256];
    int barisDitemukan=0;
    if (keSewaAtauAlat == 2){
        FILE *file = fopen(FILE_ALAT, "r");
        FILE *tempfile = fopen(FILE_SINKRON, "w");

        
        if (file == NULL || tempfile == NULL) {
            printf("Gagal membuka file.\n");
            return;
        }

        rewind(file);
        while (fgets(buffer, sizeof(buffer), file)){
            if (strlen(buffer) > 1){
                int stokAlat, nomor=1;
                char namaAlat[30], deskAlat[100];
                float hargaAlat;
    
                sscanf(buffer, "%[^,]", &namaAlat);
                if (strcmp(namaAlatDiubah, namaAlat) == 0){
                    barisDitemukan+=1;
                    sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
                    hargaAlat = hargaBaru;

                    fprintf(tempfile,"%s,%d,%.2f,%s\n", namaAlat, stokAlat, hargaAlat, deskAlat);
                } else {
                    fprintf(tempfile,"%s",buffer);
                }
            }
        }

        fclose(file);
        fclose(tempfile);

        remove(FILE_ALAT);
        rename(FILE_SINKRON, FILE_ALAT);
    } else if (keSewaAtauAlat == 1){
        FILE *file = fopen(FILE_SEWA, "r");
        FILE *tempfile = fopen(FILE_SINKRON, "w");

        
        if (file == NULL || tempfile == NULL) {
            printf("Gagal membuka file.\n");
            return;
        }

        rewind(file);
        while (fgets(buffer, sizeof(buffer), file)){
            if (strlen(buffer) > 1){
                char namaAlatSewa[30], namaPenyewa[30], tanggalSewa[25], deskAlat[100];
                int jumlahAlat;
                float hargaSewa, totalHargaSewa;

                sscanf(buffer, "%[^,],%d %[^,],%f,%[^,],%[^\n]", &namaPenyewa, &jumlahAlat, &namaAlatSewa, &hargaSewa, &tanggalSewa, &deskAlat);
                if (strcmp(namaAlatDiubah, namaAlatSewa) == 0){
                    barisDitemukan+=1;
                    fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewa, jumlahAlat, namaAlatSewa, hargaBaru, tanggalSewa, deskAlat);
                } else {
                    fprintf(tempfile,"%s",buffer);
                }
            }
        }

        fclose(file);
        fclose(tempfile);

        remove(FILE_SEWA);
        rename(FILE_SINKRON, FILE_SEWA);
    }
    
    system("cls");
	return;
}

void sinkronDesk(int keSewaAtauAlat, char *deskBaru, char *namaAlatDiubah){
    char buffer[256];
    int barisDitemukan=0;

    if(keSewaAtauAlat == 1){
        FILE *file = fopen(FILE_SEWA, "r");
        FILE *tempfile = fopen(FILE_SINKRON, "w");

            
        if (file == NULL || tempfile == NULL) {
            printf("Gagal membuka file.\n");
            return;
        }

        rewind(file);
        while (fgets(buffer, sizeof(buffer), file)){
            if (strlen(buffer) > 1){
                char namaAlatSewa[30], namaPenyewa[30], tanggalSewa[25], deskAlat[100];
                int jumlahAlat;
                float hargaSewa, totalHargaSewa;

                sscanf(buffer, "%[^,],%d %[^,],%f,%[^,],%[^\n]", &namaPenyewa, &jumlahAlat, &namaAlatSewa, &hargaSewa, &tanggalSewa, &deskAlat);
                if (strcmp(namaAlatDiubah, namaAlatSewa) == 0){
                    barisDitemukan+=1;
                    fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewa, jumlahAlat, namaAlatSewa, hargaSewa, tanggalSewa, deskBaru);
                } else {
                    fprintf(tempfile,"%s",buffer);
                }
            }
        }

        fclose(file);
        fclose(tempfile);

        remove(FILE_SEWA);
        rename(FILE_SINKRON, FILE_SEWA);
    } else if (keSewaAtauAlat == 2){
        FILE *file = fopen(FILE_ALAT, "r");
        FILE *tempfile = fopen(FILE_SINKRON, "w");

        if (file == NULL || tempfile == NULL) {
            printf("Gagal membuka file.\n");
            return;
        }

        rewind(file);
        while (fgets(buffer, sizeof(buffer), file)){
            if (strlen(buffer) > 1){
                int stokAlat, nomor=1;
                char namaAlat[30], deskAlat[100];
                float hargaAlat;
    
                sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
                if (strcmp(namaAlatDiubah, namaAlat) == 0){
                    barisDitemukan+=1;
                    fprintf(tempfile,"%s,%d,%.2f,%s\n", namaAlat, stokAlat, hargaAlat, deskBaru);
                } else {
                    fprintf(tempfile,"%s",buffer);
                }
            }
        }

        fclose(file);
        fclose(tempfile);

        remove(FILE_ALAT);
        rename(FILE_SINKRON, FILE_ALAT);
    }
}

void gantiStokAlatKosong(){
    int baris, n=1;
    char buffer[256];
    printf("\nBaris alat yang ingin diganti datanya: ");
    
    input:
    if(validasiInteger(&baris)){
    } else { puts(""); goto input; }

    if(baris>barisAlatStokKosong){
        puts("Baris tidak dapat ditemukan. Silahkan coba lagi.\n");
        goto input;
    }
    
    FILE *file = fopen(FILE_ALAT, "r");

    while (fgets(buffer, sizeof(buffer), file)){
        int stokAlat, yangDiganti=0;
        char namaAlat[30], namaAlatBaru[30], deskAlat[100];
        float hargaAlat;

        sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
        if(stokAlat>0){
           continue;
        } else if (baris != n) {
            n++;
            continue;
        }

        fclose(file);
        sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);

        puts("------------------------------------------------------------------------------------------------------------------------------------------");
        printf("| %2d | %-39s | %4d | Rp%17.2f | %58s |\n", n, namaAlat, stokAlat, hargaAlat, deskAlat);
        puts("------------------------------------------------------------------------------------------------------------------------------------------");

        apaYangDiganti:
        printf("Apa yang ingin diganti?\n1. Nama alat\n2. Stok alat\n3. Tambah 1 stok\n4. Kurangi 1 stok\n5. Harga sewa per jam\n6. Deskripsi alat\nPilihan: ");
            
        if(validasiInteger(&yangDiganti)){
        } else {
            puts(""); goto apaYangDiganti;
        }

            switch (yangDiganti){
            case 1:
                inputNamaAlatBaru:
                printf("Nama baru: ");
                
                if(validasiString(namaAlatBaru, 30)){
                } else {
                    goto inputNamaAlatBaru;
                }

                if(strcmp(namaAlatBaru, "00")==0){
                    puts(""); goto apaYangDiganti;
                }

                sinkronNama(1, namaAlat, namaAlatBaru, buffer);
                sinkronNama(2, namaAlat, namaAlatBaru, buffer);
                n++;
                continue;
                break;
            case 2:
                inputStokBaru:
                printf("Stok baru: ");
                
                if(validasiInteger(&stokAlat)){
                } else {
                    puts(""); goto inputStokBaru;
                }
                
                sinkronStok(9, stokAlat, namaAlat, buffer);
                break;
            case 3:
                sinkronStok(9, stokAlat+1, namaAlat, buffer);
                break;
            case 4:
                sinkronStok(9, stokAlat-1, namaAlat, buffer);
                break;
            case 5:
                inputHargaBaru:
                printf("Harga baru: ");
                
                if(validasiFloat(&hargaAlat)){
                } else {
                    puts(""); goto inputHargaBaru;
                }

                sinkronHarga(1, hargaAlat, namaAlat);
                sinkronHarga(2, hargaAlat, namaAlat);
                break;
            case 6:
                inputDeskripsiBaru:
                printf("Deskripsi baru: ");
                
                if(validasiString(deskAlat, 60)){
                } else {
                    goto inputDeskripsiBaru;
                }

                if(strcmp(deskAlat, "00")==0){
                    puts(""); goto apaYangDiganti;
                }

                sinkronDesk(1, deskAlat, namaAlat);
                sinkronDesk(2, deskAlat, namaAlat);
                break;
            case 0:
                break;
            default:
                puts("Input tidak sesuai pilihan yang ada. Silahkan coba lagi.\n");
                goto apaYangDiganti;
                break;
            }
            
            if(yangDiganti>=1 && yangDiganti<=6){
                puts("Data alat berhasil diganti.");
                sleep(1);
            }
    }

    system("cls");
	return;
}

void tambahAlat(){
    int banyakJenisAlat;
    
    printf("Banyak jenis alat: ");
    
    if(validasiInteger(&banyakJenisAlat)){
    } else {
        puts("");
        tambahAlat();
    }
    if(banyakJenisAlat==00){
        return;
    }

    tambahAlatStruct *alatList = (tambahAlatStruct *)malloc(banyakJenisAlat * sizeof(tambahAlatStruct));
    if (alatList == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    for (int i = 0; i < banyakJenisAlat; i++){
        printf("\n\033[34mJenis alat ke-%d:\033[0m\n", i + 1);

        inputNamaAlat:
        printf("Nama Alat: ");
        if(validasiString(alatList[i].namaAlat, 30)){
        } else {
            goto inputNamaAlat;
        }

        if(strcmp(alatList[i].namaAlat, "00")==0){
            return;
        }

        if(cekNamaSudahAda(1, alatList[i].namaAlat)==1){
            puts("Nama alat sudah ada pada data stok. Silahkan ke menu edit jika ingin mengganti stok, harga sewa, atau deskripsi.");
            goto inputNamaAlat;
        } else if (cekNamaSudahAda(1, alatList[i].namaAlat)==2){
            puts("Nama alat sudah ada pada data stok kosong. Silahkan ke menu stok kosong dari menu utama untuk menambah stok, mengganti harga sewa, atau deskripsi.");
            goto inputNamaAlat;
        }

        inputStokAlat:
        printf("Stok Alat: ");
        
        if(validasiInteger(&alatList[i].stokAlat)){
        } else {
            puts(""); goto inputStokAlat;
        }

        inputHargaSewa:
        printf("Harga Sewa Alat per Hari: ");
        
        if(validasiFloat(&alatList[i].hargaAlat)){
        } else {
            puts(""); goto inputHargaSewa;
        }

        inputDeskripsiAlat:
        printf("Deskripsi alat:");
        
        if(validasiString(alatList[i].deskAlat, 60)){
        } else {
            puts(""); goto inputDeskripsiAlat;
        }

        if(strcmp(alatList[i].deskAlat, "00")==0){
            goto inputHargaSewa;
        }
    }


    FILE *file = fopen(FILE_ALAT, "a");

    for (int i = 0; i < banyakJenisAlat; i++) {
        fprintf(file, "%s,%d,%.2f,%s\n", alatList[i].namaAlat, alatList[i].stokAlat,
        alatList[i].hargaAlat, alatList[i].deskAlat, alatList[i].tanggalAlat);
    }

    fclose(file);

    printf("Stok alat berhasil ditambah.\n");
    free(alatList);
}

void hapusAlat(){
    int baris, n=1;
    char buffer[256];
    input:
    printf("Baris alat yang ingin dihapus: ");
    
    if(validasiInteger(&baris)){
    } else { puts(""); goto input; }

    if(baris>barisAlatStok){
        puts("Baris tidak dapat ditemukan. Silahkan coba lagi.\n");
        goto input;
    }

    FILE *file = fopen(FILE_ALAT, "r");
    FILE *tempfile = fopen(FILE_TEMP, "w");

    while (fgets(buffer, sizeof(buffer), file)){
        int stokAlat, yangDiganti=0;
        char namaAlat[30], namaAlatBaru[30], deskAlat[100];
        float hargaAlat;

        sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
        if(stokAlat<=0){
            fprintf(tempfile, "%s", buffer);
        } else if (baris != n) {
            n++;
            fprintf(tempfile, "%s", buffer);
        } else {
            n++;
        }
    }
    
    fclose(file); remove(FILE_ALAT);
    fclose(tempfile); rename(FILE_TEMP, FILE_ALAT);

    puts("Data alat berhasil dihapus.");
    sleep(1);
    system("cls");
	return;
}

void hapusAlatKosong(){
    int baris, n=1;
    char buffer[256];
    
    printf("Baris alat yang ingin dihapus: ");
    
    if(validasiInteger(&baris)){
    } else { puts(""); hapusAlatKosong(); }

    if(baris>barisAlatStokKosong){
        puts("Baris tidak dapat ditemukan. Silahkan coba lagi.\n");
        hapusAlatKosong();
    }

    FILE *file = fopen(FILE_ALAT, "r");
    FILE *tempfile = fopen(FILE_TEMP, "w");

    while (fgets(buffer, sizeof(buffer), file)){
        int stokAlat, yangDiganti=0;
        char namaAlat[30], namaAlatBaru[30], deskAlat[100];
        float hargaAlat;

        sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
        if(stokAlat>0){
            fprintf(tempfile, "%s", buffer);
        } else if (baris != n) {
            n++;
            fprintf(tempfile, "%s", buffer);
        } else {
            n++;
        }
    }
    
    fclose(file); remove(FILE_ALAT);
    fclose(tempfile); rename(FILE_TEMP, FILE_ALAT);

    puts("Data alat berhasil dihapus.");
    sleep(1);
    system("cls");
	return;
}

void gantiAlat(){
    int baris, n=1;
    char buffer[256];
    input:
    printf("\nBaris alat yang ingin diganti datanya: ");
    
    if(validasiInteger(&baris)){
    } else { 
        goto input;
    }

    if(baris>barisAlatStok){
        printf("Baris tidak dapat ditemukan. Silahkan coba lagi.\n");
        goto input;
    }
    
    FILE *file = fopen(FILE_ALAT, "r");
    
    while (fgets(buffer, sizeof(buffer), file)){
        int stokAlat, yangDiganti=0;
        char namaAlat[30], namaAlatBaru[30], deskAlat[100];
        float hargaAlat;

        sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);
        if(stokAlat<=0){
           continue;
        } else if (baris != n) {
            n++;
            continue;
        }

        fclose(file);
        sscanf(buffer, "%[^,],%d,%f,%[^\n]", &namaAlat, &stokAlat, &hargaAlat, &deskAlat);

        puts("------------------------------------------------------------------------------------------------------------------------------------------");
        printf("| %2d | %-39s | %4d | Rp%17.2f | %58s |\n", n, namaAlat, stokAlat, hargaAlat, deskAlat);
        puts("------------------------------------------------------------------------------------------------------------------------------------------");

        apaYangDiganti:
        printf("Apa yang ingin diganti?\n1. Nama alat\n2. Stok alat\n3. Tambah 1 stok\n4. Kurangi 1 stok\n5. Harga sewa per jam\n6. Deskripsi alat\nPilihan: ");
            
        if(validasiInteger(&yangDiganti)){
        } else {
            puts(""); goto apaYangDiganti;
        }

            switch (yangDiganti){
            case 1:
                inputNamaAlatBaru:
                printf("Nama baru: ");
                
                if(validasiString(namaAlatBaru, 30)){
                } else {
                    goto inputNamaAlatBaru;
                }

                if(strcmp(namaAlatBaru, "00")==0){
                    puts(""); goto apaYangDiganti;
                }

                sinkronNama(1, namaAlat, namaAlatBaru, buffer);
                sinkronNama(2, namaAlat, namaAlatBaru, buffer);
                n++;
                continue;
                break;
            case 2:
                inputStokBaru:
                printf("Stok baru: ");
                
                if(validasiInteger(&stokAlat)){
                } else {
                    puts(""); goto inputStokBaru;
                }
                
                sinkronStok(9, stokAlat, namaAlat, buffer);
                break;
            case 3:
                sinkronStok(9, stokAlat+1, namaAlat, buffer);
                break;
            case 4:
                sinkronStok(9, stokAlat-1, namaAlat, buffer);
                break;
            case 5:
                inputHargaBaru:
                printf("Harga baru: ");
                
                if(validasiFloat(&hargaAlat)){
                } else {
                    puts(""); goto inputHargaBaru;
                }

                sinkronHarga(1, hargaAlat, namaAlat);
                sinkronHarga(2, hargaAlat, namaAlat);
                break;
            case 6:
                inputDeskripsiBaru:
                printf("Deskripsi baru: ");
                
                if(validasiString(deskAlat, 60)){
                } else {
                    goto inputDeskripsiBaru;
                }

                if(strcmp(deskAlat, "00")==0){
                    puts(""); goto apaYangDiganti;
                }

                sinkronDesk(1, deskAlat, namaAlat);
                sinkronDesk(2, deskAlat, namaAlat);
                break;
            case 0:
                break;
            default:
                puts("Input tidak sesuai pilihan yang ada. Silahkan coba lagi.\n");
                goto apaYangDiganti;
                break;
            }
            
            if(yangDiganti>=1 && yangDiganti<=6){
                puts("Data alat berhasil diganti.");
                sleep(1);
            }
    }
    
    system("cls");
	return;
}

void cariAlat(){
    char kataDicari[20];

    judul(); tampilkanStok();
    puts("Ketik nama, harga, atau deskripsi alat: ");
    
    if(validasiString(kataDicari,20)){
    } else {
        sleep(1);
        cariAlat(); 
    }

    FILE *file = fopen(FILE_ALAT, "r");
    pencariAlatSewa(1, file, kataDicari);
    fclose(file);
    return;
}

void cariAlatKosong(){
    char kataDicari[20];

    judul(); tampilkanStokKosong();
    puts("Ketik nama, stok, harga, atau deskripsi alat: ");

    if(validasiString(kataDicari,20)){
    } else { cariAlatKosong(); }

    FILE *file = fopen(FILE_ALAT, "r");
    pencariAlatSewa(3, file, kataDicari);
    fclose(file);
    return;
}

void cariSewa(){
    char kataDicari[20];

    judul(); tampilkanSewa();
    puts("Ketik nama, alat, harga, total harga, tanggal, atau deskripsi alat: ");

    if(validasiString(kataDicari,20)){
    } else { cariSewa(); }

    FILE *file = fopen(FILE_SEWA, "r");
    pencariAlatSewa(2, file, kataDicari);
    fclose(file);
    return;
}

char tambahSewa(){
    int banyakPenyewa, metodeCari, nomorCari;
    
    char tanggalSewa[50], buffer[256], cariNamaAlat[30];
    banyakPenyewa:
    printf("Berapa banyak penyewa: ");
    
    if(validasiInteger(&banyakPenyewa)){ 
    } else { tambahSewa(); } 
    
    if (banyakPenyewa == 00){
            return 0;
        }

    dapatkanWaktu(tanggalSewa);
    
    tambahSewaStruct *sewaList = (tambahSewaStruct *)malloc(banyakPenyewa * sizeof(tambahSewaStruct));
    if (sewaList == NULL) {
        printf("Memori tidak cukup.\n");
        return 0;
    }

    for (int i = 0; i < banyakPenyewa; i++){
        inputPenyewa:
        printf("\n\033[34mPenyewa ke-%d\033[0m\n", i+1);
        inputNamaPenyewa:
        printf("Nama Penyewa: ");

        if(validasiString(sewaList[i].namaPenyewa, 34)){
        } else { goto inputNamaPenyewa; }

        if (strcmp(sewaList[i].namaPenyewa, "00")==0){
            goto banyakPenyewa;
        }
        if (cekNamaSudahAda(2, sewaList[i].namaPenyewa)==1){
            puts("Nama penyewa sudah ada pada data. Silahkan ke menu edit jika ingin mengganti nama, jumlah alat disewa, atau harga sewa.");
            goto inputNamaPenyewa;;
        }
        metodeCari:

        /*printf("Cari Alat Berdasarkan Nomor (1) atau Nama (2): ");
        //fgets(metodeCariStr, sizeof(metodeCariStr), stdin);
        //metodeCari = validasiInteger(metodeCariStr);
        scanf("%d", &metodeCari);
        if (nomorCari == 00){
            goto inputPenyewa;
        } 
        switch (metodeCari){
            case 1:*/

        int nomor=1;
        printf("Nomor Alat: ");
        
        if(validasiInteger(&nomorCari)){
        } else { 
            puts("");
            goto metodeCari; 
        }

        if (nomorCari == 00){
            goto inputNamaPenyewa;
        } else if (nomorCari > barisAlatStok){
            puts("Alat tidak dapat ditemukan. Silahkan coba lagi.\n");
            goto metodeCari;
        }

        FILE *filecari = fopen(FILE_ALAT, "r");
        while (fgets(buffer, sizeof(buffer), filecari)){
            int stokAlat;
            char namaAlat[30], deskAlat[100];
            float hargaAlat;
            sscanf(buffer, "%[^,],%d,%f,%[^\n]", &sewaList[i].namaAlat, &stokAlat, &sewaList[i].hargaSewa, &sewaList[i].deskAlat);
            if ((strlen(buffer) > 1) && (nomor == nomorCari) && (stokAlat>0)){
                break;
            } else if ((strlen(buffer)>1) && (stokAlat>0)){
                nomor+=1; 
            }  
        }
        fclose(filecari);

            /*    break;
            case 2:
                printf("Nama Alat (Ketik Lengkap Untuk Memasukkan ke Data): ");
                break;
            case 00:
                goto metodeCari;
                break;
            default:
                puts("Input tidak sesuai. Silahkan coba lagi.");
                goto metodeCari;
                break;

        }*/

        inputJumlahAlatDisewa:
        puts("Jumlah Alat yang Disewa: ");

        if(validasiInteger(&sewaList[i].jumlahAlat)){
        } else {
            puts("");
            goto inputJumlahAlatDisewa;
        }

        if(sewaList[i].jumlahAlat == 00){
            goto metodeCari;
        }

        if(cekStokMencukupi(sewaList[i].jumlahAlat, sewaList[i].namaAlat)==0){
            puts("Stok alat yang akan dipinjam tidak cukup. Silahkan coba lagi.");
            goto inputJumlahAlatDisewa;
        }
    }

    FILE *file = fopen(FILE_SEWA, "a");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        free(sewaList);
        return 0;
    }
    
    for (int i = 0; i < banyakPenyewa; i++){
        if(i>=1){
            fprintf(file, " ");
        }
        sinkronStok(1, sewaList[i].jumlahAlat, sewaList[i].namaAlat, buffer);
        fprintf(file, "%s,%d %s,%f,%s,%s\n", sewaList[i].namaPenyewa, sewaList[i].jumlahAlat, sewaList[i].namaAlat, sewaList[i].hargaSewa, tanggalSewa, sewaList[i].deskAlat);
    }

    fclose(file);

    printf("Data sewa berhasil ditambahkan.\n");
    sleep(1);
    free(sewaList);
}

void hapusSewa(){
    int baris, n=1;
    char buffer[256];

    printf("Baris data sewa yang ingin dihapus: ");
    if(validasiInteger(&baris)){
    } else {
        hapusSewa();
    }

    if (baris == 00){
    } else if (baris > barisListSewa){
        puts("Baris tidak dapat ditemukan. Silahkan coba lagi.");
        hapusSewa();
    }

    FILE *file = fopen(FILE_SEWA, "r");
    FILE *tempfile = fopen(FILE_TEMP, "w");

    while(fgets(buffer, sizeof(buffer), file)){
        if (baris != n){
            n++;
            fprintf(tempfile, "%s", buffer);
        } else {
            char namaAlat[30], namaPenyewa[30], tanggalSewa[25], deskAlat[61];
            int jumlahAlat;
            float hargaSewa;
            sscanf(buffer, "%[^,],%d %[^,],%f,%[^,],%[^\n]", &namaPenyewa, &jumlahAlat, &namaAlat, &hargaSewa, &tanggalSewa, &deskAlat);
            sinkronStok(2, jumlahAlat, namaAlat, buffer);
            n++;
        }
    }
    
    fclose(file);
    fclose(tempfile);

    remove(FILE_SEWA);
    rename(FILE_TEMP, FILE_SEWA);

    puts("Data sewa berhasil dihapus.");
    sleep(1);
    system("cls");
    return;
    
}

void gantiSewa(){
    int baris, n=1, yangDiganti;
    char buffer[256];

    float hargaSewa;
    char namaAlat[30], namaAlatBaru[30];

    printf("\nBaris data sewa yang ingin diganti datanya: ");
    if (validasiInteger(&baris)){
    } else {
        gantiSewa();
    }

    if (baris <= 00){
    } else if (baris > barisListSewa){
        puts("Baris tidak dapat ditemukan. Silahkan coba lagi.");
        gantiSewa();
    } else {
        FILE *file = fopen(FILE_SEWA, "r");
        FILE *tempfile = fopen(FILE_TEMP, "w");

        while(fgets(buffer, sizeof(buffer), file)){
            if(n != baris){
                n++;
                fprintf(tempfile, "%s", buffer);
                continue;
            }

            int jumlahAlat, jumlahAlatBaru;
            char namaPenyewa[30], namaPenyewaBaru[30], deskAlat[100], tanggalSewa[25];
            float totalHargaSewa;

            sscanf(buffer, "%[^,],%d %[^,],%f,%[^,],%[^\n]", &namaPenyewa, &jumlahAlat, &namaAlat, &hargaSewa, &tanggalSewa, &deskAlat);
            puts("------------------------------------------------------------------------------------------------------------------------------------------");
            printf("| %2d | %-32s | %3d %-28s | Rp%17.2f | Rp%17.2f | %15s |\n", baris, namaPenyewa, jumlahAlat, namaAlat, hargaSewa, jumlahAlat*hargaSewa, tanggalSewa);
            puts("------------------------------------------------------------------------------------------------------------------------------------------");
            apayangdiganti:
            printf("Apa yang ingin diganti?\n1. Nama penyewa\n2. Nama alat yang disewa\n3. Jumlah alat disewa\n4. Tambah 1 alat\n5. Kurangi 1 alat\n6. Harga sewa baru\nPilihan: ");
            
            if(validasiInteger(&yangDiganti)){
            } else {
                puts(""); goto apayangdiganti;
            }

            switch (yangDiganti){
            case 1:
                inputNamaPenyewa:
                printf("Nama penyewa: ");
                
                if(validasiString(namaPenyewaBaru, 30)){
                } else {
                    puts(""); goto inputNamaPenyewa;
                }

                if(strcmp(namaPenyewaBaru, "00")==0){
                    puts(""); goto apayangdiganti;
                }

                fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewaBaru, jumlahAlat, namaAlat, hargaSewa, tanggalSewa, deskAlat);
                break;
            case 2:
                inputNamaAlatBaru:
                printf("Nama alat yang disewa: ");

                if(validasiString(namaAlatBaru, 30)){
                } else {
                    puts("sad"); goto inputNamaAlatBaru;
                }
                
                if(strcmp(namaAlatBaru, "00")==0){
                    goto apayangdiganti;
                }

                sinkronNama(2, namaAlat, namaAlatBaru, buffer);
                fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewa, jumlahAlat, namaAlatBaru, hargaSewa, tanggalSewa, deskAlat);
                break;
            case 3:
                inputJumlahAlatBaru:
                printf("Jumlah alat disewa: ");

                if(validasiInteger(&jumlahAlatBaru)){
                } else {
                puts(""); goto inputJumlahAlatBaru;
                }

                if(jumlahAlatBaru==00){
                    goto apayangdiganti;
                }

                if(cekStokMencukupi(0, namaAlat)==1 && jumlahAlatBaru<jumlahAlat){
                    fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewa, jumlahAlatBaru, namaAlat, hargaSewa, tanggalSewa, deskAlat);
                    sinkronStok(2, jumlahAlat-jumlahAlatBaru, namaAlat, buffer);
                    continue;
                } else if (cekStokMencukupi(jumlahAlatBaru-jumlahAlat, namaAlat)==1 && jumlahAlatBaru>jumlahAlat){
                    fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewa, jumlahAlatBaru, namaAlat, hargaSewa, tanggalSewa, deskAlat);
                    sinkronStok(2, jumlahAlat-jumlahAlatBaru, namaAlat, buffer);
                    continue;
                } else if(cekStokMencukupi(jumlahAlatBaru-jumlahAlat, namaAlat)==0 && jumlahAlatBaru>jumlahAlat){
                    puts("Stok alat tidak cukup untuk menambah jumlah dipinjam");
                    goto apayangdiganti;
                } else if(cekStokMencukupi(jumlahAlatBaru-jumlahAlat, namaAlat)==0 && jumlahAlatBaru<jumlahAlat){
                    fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewa, jumlahAlatBaru, namaAlat, hargaSewa, tanggalSewa, deskAlat);
                    snprintf(buffer, sizeof(buffer), "%s,%d %s,%f,%s,%s", namaPenyewa, jumlahAlat-jumlahAlatBaru, namaAlat, hargaSewa, tanggalSewa, deskAlat);
                    sinkronStok(2, jumlahAlatBaru, namaAlat, buffer);
                }
                break;
            case 4:
                if(cekStokMencukupi(1, namaAlat)==0){
                    puts("Stok alat tidak cukup untuk menambah jumlah dipinjam.");
                    goto apayangdiganti;
                }

                jumlahAlat++;
                fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewa, jumlahAlat, namaAlat, hargaSewa, tanggalSewa, deskAlat);
                sinkronStok(1, 1, namaAlat, buffer);
                break;
            case 5:
                if(jumlahAlat<=1){
                    puts("Jumlah alat dipinjam tidak bisa dikurangi.");
                    goto apayangdiganti;
                }

                jumlahAlat--;
                fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewa, jumlahAlat, namaAlat, hargaSewa, tanggalSewa, deskAlat);
                if(cekStokMencukupi(0, namaAlat)==0){
                    snprintf(buffer, sizeof(buffer), "%s,%d %s,%f,%s,%s", namaPenyewa, 1, namaAlat, hargaSewa, tanggalSewa, deskAlat);
                    sinkronStok(2, 1, namaAlat, buffer);
                    continue;
                }
                sinkronStok(2, 1, namaAlat, buffer);
                break;
            case 6:
                inputHargaSewa:
                printf("Harga sewa baru: ");
                
                if(validasiFloat(&hargaSewa)){
                } else {
                    puts(""); goto inputHargaSewa;
                }

                fprintf(tempfile, "%s,%d %s,%f,%s,%s\n", namaPenyewa, jumlahAlat, namaAlat, hargaSewa, tanggalSewa, deskAlat);
                sinkronHarga(2, hargaSewa, namaAlat);
                break;
            case 0:
                fprintf(tempfile,"%s",buffer);
                break;
            default:
                fprintf(tempfile,"%s",buffer);
                break;
            }

            if(yangDiganti>=1 && yangDiganti<=6){
                puts("Data sewa berhasil diganti.");
                sleep(1);
            }

            n++;
        }

        fclose(file);
        fclose(tempfile);

        remove(FILE_SEWA);
        rename(FILE_TEMP, FILE_SEWA);

        if(yangDiganti==6){
            sinkronHarga(1, hargaSewa, namaAlat);
        } else if (yangDiganti==2){
            sinkronNama(1, namaAlat, namaAlatBaru, buffer);
        }

        system("cls");
        return;
    }
}
