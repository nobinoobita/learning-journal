#include <stdio.h>
#include <string.h>

/*
 * vault_check.c
 *
 * Program latihan sederhana untuk studi kasus reverse engineering.
 * Logika verifikasi: panjang input harus 8 karakter, lalu setiap
 * karakter di-XOR berurutan dengan sebuah running key yang bergeser
 * setiap iterasi. Hasil akumulasi XOR seluruh karakter harus sama
 * dengan sebuah nilai checksum target.
 */

int verify(const char *input) {
    if (strlen(input) != 8) {
        return 0;
    }

    unsigned char acc = 0x00;
    unsigned char running_key = 0x5A;

    for (int i = 0; i < 8; i++) {
        unsigned char c = (unsigned char)input[i];
        acc ^= (c ^ running_key);
        running_key += 0x03;
    }

    if (acc == 0x37) {
        return 1;
    }

    return 0;
}

int main(void) {
    char input[64];

    printf("Masukkan kode akses: ");
    if (scanf("%63s", input) != 1) {
        return 1;
    }

    if (verify(input)) {
        printf("Akses Diterima\n");
    } else {
        printf("Akses Ditolak\n");
    }

    return 0;
}
