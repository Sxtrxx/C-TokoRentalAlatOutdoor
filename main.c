#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "LIBRARY_TOKO_RENTAL.h"

void menuAlat(){
    int pilihanMenuAlat;

    judul();
    tampilkanStok();
    pilihanMenuAlat:
    printf("\n1. Tambah alat\n2. Hapus alat\n3. Edit  alat\n4. Cari alat\n5. Kembali ke menu utama\nPilihan: ");
    

    if(validasiInteger(&pilihanMenuAlat)){
        switch(pilihanMenuAlat){
            case 1:
                tambahAlat();
                system("cls");
                menuAlat();
                break;
            case 2:
                hapusAlat();
                system("cls");
                menuAlat();
                break;
            case 3:
                gantiAlat();
                system("cls");
                menuAlat();
                break;
            case 4:
                system("cls");
                cariAlat();
                goto pilihanMenuAlat;
                break;
            case 5:
                return;
                break;
            case 00:
                system("cls");
                menuAlat();
                break;
            default:
                puts("Input tidak sesuai pilihan yang ada. Silahkan coba lagi.");
                sleep(1);
                system("cls");
                menuAlat();
                break;
        }
    } else {
        sleep(1);
        menuAlat();
    }
}

void menuAlatKosong(){
    int pilihanMenuAlatKosong;

    judul();
    tampilkanStokKosong();
    pilihanMenuAlatKosong:
    printf("\n1. Hapus alat\n2. Edit alat\n3. Cari alat\n4. Kembali ke menu utama\nPilihan: ");
    
    if(validasiInteger(&pilihanMenuAlatKosong)){
        switch(pilihanMenuAlatKosong){
            case 1:
                hapusAlatKosong();
                system("cls");
                menuAlatKosong();
                break;
            case 2:
                gantiStokAlatKosong();
                system("cls");
                menuAlatKosong();
                break;
            case 3:
                system("cls");
                cariAlatKosong();
                goto pilihanMenuAlatKosong;
                break;
            case 4:
                return;
                break;
            case 00:
                system("cls");
                break;
            default:
                puts("Input tidak sesuai pilihan yang ada. Silahkan coba lagi.");
                sleep(1);
                system("cls");
                menuAlatKosong();
                break;
        }
    } else {
        sleep(1);
        menuAlatKosong();
    }
}

int menuSewa(){
    int pilihanMenuSewa;

    judul();
    tampilkanStok();
    tampilkanSewa();
    pilihanMenuSewa:
    printf("\n1. Tambah data sewa\n2. Hapus data sewa\n3. Ganti data sewa\n4. Cari data sewa\n5. Kembali ke menu utama\nPilihan: ");
    
    if(validasiInteger(&pilihanMenuSewa)){
        switch (pilihanMenuSewa){
        case 1:
            tambahSewa();
            system("cls");
            menuSewa();
            break;
        case 2:
            hapusSewa();
            system("cls");
            menuSewa();
            break;
        case 3:
            gantiSewa();
            system("cls");
            menuSewa();
            break;
        case 4:
            system("cls");
            cariSewa();
            goto pilihanMenuSewa;
            break;
        case 5:
            return 0;
            break;
        case 00:
            system("cls");
            menuSewa();
            break;
        default:
            puts("Input tidak sesuai pilihan yang ada. Silahkan coba lagi.");
            sleep(1);
            system("cls");
            menuSewa();
            break;
        }
    } else {
        sleep(1);
        menuSewa();
    }
}

int main(){
    int pilihanMenu;

    system("cls");
    judul();
    puts("|                                                           ##  MENU UTAMA  ##                                                           |");
    puts("------------------------------------------------------------------------------------------------------------------------------------------");
    puts("                                                                                                 tips: input 00 untuk membatalkan pilihan");
    puts("\n1. MENU ALAT TERSEDIA\n2. MENU ALAT STOK KOSONG\n3. MENU SEWA\n4. KELUAR");
    printf("\nPILIHAN: ");


    if(validasiInteger(&pilihanMenu)){
        switch(pilihanMenu){
        case 1:
            system("cls");
            menuAlat();
            main();
            break;
        case 2:
            system("cls");
            menuAlatKosong();
            main();
            break;
        case 3:
            system("cls");
            menuSewa();
            main();
            break;
        case 4:
            puts("Program berhenti.");
            sleep(1);
            system("cls");
            return 0;
            break;
        case 00:
            system("cls");
            main();
            break;
        default:
            puts("Input tidak sesuai pilihan yang ada. Silahkan coba lagi.");
            sleep(1);
            system("cls");
            main();
            break;
        }
    } else {
        sleep(1);
        main();
    }

    
}