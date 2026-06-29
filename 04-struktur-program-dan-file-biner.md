# 04 — Struktur Program dan File Biner

## Ringkasan

Bab ini membahas bagaimana sebuah file executable sebenarnya tersusun — mulai dari format ELF dan PE, anatomi file PE/EXE secara umum, jenis-jenis section di dalamnya, hingga cara menavigasi struktur tersebut menggunakan IDA Free (graph view, text view, function window, names window, imports/exports, strings, dan cross-reference).

## File Executable: Format ELF vs PE

File executable pada dasarnya adalah file biner berisi instruksi mesin yang bisa langsung dijalankan sistem operasi. File ini memiliki format khusus yang mengatur bagaimana data di dalamnya disusun, sehingga loader dan linker tahu cara membaca dan mengeksekusinya.

Ada dua format file executable yang paling umum:

| Format | Sistem Operasi | Catatan |
|---|---|---|
| **ELF** (Executable and Linkable Format) | Unix-like (Linux, BSD) | Sangat fleksibel, mendukung dynamic linking, sering dipakai untuk analisis malware Linux maupun debugging kernel |
| **PE** (Portable Executable) | Windows | Berekstensi `.exe`, `.dll`, `.sys`; disebut "portable" karena formatnya konsisten di seluruh platform Windows meski detail internalnya bisa berbeda |

> Catatan fokus: pembahasan dan praktik di catatan belajar ini berfokus pada file **PE**, khususnya file `.exe` hasil kompilasi MinGW64 di Windows.

## Anatomi Dasar File EXE (PE)

Kalau sebuah file `.exe` dibongkar, struktur dasarnya terdiri dari beberapa bagian berurutan:

```
┌─────────────────────┐
│     DOS Header       │  ← warisan kompatibilitas dari sistem DOS lama
├─────────────────────┤
│      DOS Stub        │  ← pesan "This program cannot be run in DOS mode"
├─────────────────────┤
│      PE Header        │  ← info utama: tipe file, arsitektur, ukuran
├─────────────────────┤
│   Section Header(s)   │  ← daftar & info tiap section (.text, .data, dst)
├─────────────────────┤
│       Sections         │  ← isi sebenarnya: instruksi, data, resource
└─────────────────────┘
```

- **DOS Header** — bagian paling awal, warisan dari sistem DOS lama. Fungsinya lebih ke kompatibilitas, tapi tetap dipertahankan di format PE modern.
- **DOS Stub** — kode kecil yang menampilkan pesan *"This program cannot be run in DOS mode"* ketika file dijalankan di lingkungan DOS murni.
- **PE Header** — bagian penting karena menyimpan informasi utama tentang file: tipe file, arsitektur (32-bit/64-bit), ukuran, dan metadata lain.
- **Section Header** — berisi daftar dan informasi tentang tiap section di dalam file, misalnya `.text`, `.data`, atau resource.
- **Sections** — bagian utama yang berisi instruksi program, data, maupun resource sesungguhnya.

Susunan inilah yang membuat sebuah file `.exe` bisa dipahami oleh sistem operasi dan siap dijalankan.

## Jenis-Jenis Section dalam File PE

Isi section sangat beragam, tapi dalam konteks reverse engineering, section-section berikut paling sering jadi perhatian:

| Section | Isi | Relevansi untuk RE |
|---|---|---|
| `.text` | Instruksi program / machine code yang dieksekusi CPU | Fokus utama RE — di sinilah alur logika program dianalisis |
| `.data` | Variabel global dengan nilai awal (writable) | Melihat nilai awal variabel, tabel, atau buffer yang dimanipulasi program |
| `.rdata` | Read-only data: string literal, konstanta | String sering jadi petunjuk untuk menemukan fungsi penting |
| `.edata` | Informasi ekspor — fungsi yang bisa dipanggil program lain | Berguna saat menganalisis library/modul |
| `.idata` | Daftar fungsi/library yang diimpor program | Membantu memahami interaksi program dengan API eksternal, termasuk saat analisis malware |
| `.pdata` | Informasi exception handling (umum di program C/C++) | Membantu melacak alur error/exception |
| `.xdata` | Metadata tambahan untuk exception handling, biasanya berpasangan dengan `.pdata` | Penting di program besar dengan error handling kompleks |
| `.eh_frame` | Dipakai compiler (GCC/LLVM) untuk mekanisme stack unwinding saat exception | Membantu memahami detail penanganan error |
| `.bss` | Variabel global yang belum diinisialisasi | Menunjukkan buffer besar atau array yang dipakai program |
| `.tls` | Data unik untuk setiap thread | Relevan saat menganalisis malware dengan kemampuan multithreading |
| `.rsrc` | Resource program: icon, gambar, dialog, menu, string pesan | Membantu menemukan bagian UI atau pesan yang ditampilkan program |
| `.debug` | Simbol atau mapping ke source code (kalau belum dihapus/stripped) | Sangat memudahkan analisis karena memberi petunjuk nama fungsi/variabel asli |

Catatan: tidak semua section ini muncul di setiap file `.exe` — isinya sangat bergantung pada bagaimana program tersebut dikompilasi.

## Navigasi Dasar di IDA Free

Setelah memahami struktur file, langkah berikutnya adalah mengenal cara menavigasi struktur tersebut lewat IDA Free.

### Graph View

Tampilan ini menyajikan alur eksekusi program dalam bentuk blok-blok instruksi yang dihubungkan panah. Setiap warna panah punya arti:

- **Panah merah** → conditional jump yang *tidak* diambil
- **Panah hijau** → conditional jump yang diambil
- **Panah biru** → unconditional jump
- **Panah mengarah ke atas** → biasanya menandakan adanya loop

Visualisasi ini memudahkan membaca logika percabangan (`if`) atau perulangan tanpa harus membaca instruksi assembly satu per satu.

### Text View

Tekan `Space` di IDA untuk beralih ke tampilan ini. Instruksi ditampilkan baris per baris, dengan elemen-elemen:

- **Section** — tempat kode berada
- **Address** — alamat memori instruksi
- **Label** — dibuat otomatis oleh IDA
- **Instruction** — operation code (mnemonic)
- **Operand** — bagian inti instruksi (register, nilai, alamat)

IDA Free juga sering menambahkan komentar otomatis, misalnya string yang dipakai program atau referensi ke fungsi lain — sangat membantu memahami maksud instruksi tanpa harus menebak-nebak.

Kalau Graph View memudahkan melihat alur besar program, Text View memberi detail baris demi baris yang lebih presisi.

### Function Window

Menampilkan daftar semua fungsi yang terdeteksi, lengkap dengan alamat awal, panjang fungsi, dan flag tertentu. Fungsi berukuran besar biasanya lebih penting dianalisis karena kemungkinan besar berisi logika utama program.

### Names Window

Menampilkan semua alamat memori yang sudah memiliki nama — tidak hanya fungsi, tapi juga kode tertentu, data, maupun string. Sangat membantu navigasi karena bisa langsung melompat ke bagian program yang relevan tanpa mencari manual.

### Imports & Exports Window

- **Imports** — menampilkan fungsi eksternal yang dipanggil dari library. Dari sini bisa diketahui apa saja yang dimanfaatkan oleh program.
- **Exports** — menampilkan fungsi yang disediakan program agar bisa dipanggil program lain. Biasanya relevan kalau file yang dianalisis berbentuk library/modul tambahan.

### Strings Window

Menampilkan semua string dalam binary, baik di section data maupun rdata. String sering menjadi petunjuk berharga — entah itu pesan error, teks antarmuka, atau string seperti "masukkan password" yang bisa langsung mengarahkan ke fungsi tertentu.

Dengan memanfaatkan imports, exports, dan strings, pemahaman konteks program bisa lebih cepat didapat tanpa harus menelusuri semua instruksi assembly secara detail.

### Cross-Reference (Xref)

Setelah tahu fungsi eksternal apa yang dipanggil dan string apa yang tersimpan, pertanyaan berikutnya: bagian kode mana yang benar-benar memakai informasi tersebut? Di sinilah **cross-reference (Xref)** berperan.

Xref menampilkan daftar semua lokasi dalam kode yang mengakses atau memanggil suatu alamat tertentu. Misalnya: sebuah fungsi dipanggil dari mana saja, atau sebuah variabel/string dipakai di bagian mana saja.

Dengan Xref, kode tidak hanya dilihat sebagai potongan-potongan terpisah, tapi bisa dipahami kaitannya secara utuh — fitur ini akan sangat berguna saat memasuki tahap static analysis pada bab selanjutnya.

## Catatan Penutup

Bab ini sudah membahas bagaimana sebuah file executable disusun, mengenal berbagai section di dalamnya, serta memanfaatkan fitur dasar IDA Free untuk menelusuri alur program. Bab selanjutnya akan masuk ke salah satu teknik inti reverse engineering: **static analysis** — apa itu, dan bagaimana cara melakukannya.
