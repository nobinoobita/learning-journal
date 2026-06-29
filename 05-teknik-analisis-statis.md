# 05 — Teknik Analisis Statis (Static Analysis)

## Ringkasan

Bab ini membahas konsep dasar static analysis dalam reverse engineering: apa itu, tiga pendekatan utamanya, kelebihan dan kekurangannya, kapan teknik ini paling tepat digunakan, serta komponen-komponen penting yang perlu diperhatikan saat melakukannya.

## Apa Itu Static Analysis?

**Static Analysis** adalah metode memeriksa software/program dalam kondisi diam — artinya, file program atau binary dianalisis **tanpa perlu dieksekusi**. Teknik ini memungkinkan kita memahami isi dan perilaku program hanya dengan melihat struktur internalnya saja.

Tujuannya jelas: mengetahui bagaimana sebuah program bekerja, apa saja fungsinya, dan apakah ada bagian yang mencurigakan atau tidak sesuai — semua ini dilakukan **sebelum** masuk ke tahap eksekusi (dynamic analysis, yang akan dibahas di bab selanjutnya).

Dari teknik analisis ini, biasanya bisa ditemukan:

- Struktur, sistem, dan arsitektur program
- Alur data dan alur kontrol program
- Relasi antar entitas dan dependensi yang digunakan
- Akurasi sintaks atau potensi kesalahan dalam kode

Dengan kata lain, static analysis memberi gambaran menyeluruh tentang sebuah program hanya dari kodenya saja.

## Tiga Pendekatan Utama dalam Static Analysis

### 1. Code Analysis

Memeriksa langsung kode program — baik source code, file biner, maupun hasil disassembly. Tujuannya memahami aliran data, struktur kontrol, dependensi, sampai algoritma yang dipakai program.

### 2. Documentation Analysis

Jika tersedia dokumentasi sistem seperti user manual, spesifikasi desain, atau panduan operasional, ini bisa sangat membantu. Dokumen-dokumen ini memberi gambaran tambahan tentang fungsi dan tujuan program tanpa harus menebaknya hanya dari kode.

### 3. Data Analysis

Berfokus pada data yang digunakan program — baik data terstruktur (misalnya relational database) maupun data tidak terstruktur seperti artefak software. Analisis ini sering memberi konteks tambahan tentang bagaimana program berinteraksi dengan lingkungannya.

> Dalam konteks cybersecurity, pendekatan yang paling sering dipakai adalah **code analysis** — seperti yang akan dipraktikkan langsung pada bab studi kasus berikutnya, di mana file binary sebuah program dianalisis langsung.

## Kelebihan dan Kekurangan Static Analysis

**Kelebihan:**

- Memberi wawasan jelas tentang struktur dan perilaku program tanpa perlu eksekusi.
- Relatif lebih ringan karena tidak membutuhkan sumber daya sebesar dynamic analysis.

**Kekurangan:**

- Tidak bisa mendeteksi perilaku program saat runtime, sehingga ada kemungkinan hal-hal tertentu terlewat.
- Hasil kompilasi/disassembly kadang tidak sepenuhnya akurat atau lengkap.
- Kurang efektif jika program dilengkapi proteksi seperti obfuscation atau packing.

## Kapan Static Analysis Digunakan?

Static analysis biasanya dipakai di **tahap inisialisasi** analisis malware — untuk mendapatkan gambaran umum struktur dan logika kode. Teknik ini juga sangat membantu reverse engineer yang belum familiar dengan bahasa pemrograman yang dipakai malware, karena alurnya masih bisa dipahami lewat hasil disassembly atau dekompilasi.

Singkatnya, static analysis bisa dianggap sebagai pijakan awal sebelum masuk ke analisis yang lebih mendalam.

## Komponen Penting dalam Static Analysis

| Komponen | Yang Diperiksa | Kegunaan |
|---|---|---|
| **Tipe file & format** | PE, ELF, atau format lain (cek dengan `file` di Linux, atau header inspector di Windows) | Menentukan cara file dibaca oleh sistem |
| **String yang tersimpan** | Teks ASCII/Unicode di dalam file | Petunjuk awal: pesan error, instruksi ke user, alamat IP/URL, nama fungsi API, path file |
| **Import & Export** | Fungsi eksternal yang dipanggil (import) atau disediakan (export) | Menunjukkan entry point dari aktivitas mencurigakan, terutama saat analisis malware |
| **Kode disassembly** | Instruksi assembly yang membentuk logika program | Fokus pada flow control: `jump`, `call`, `cmp` untuk memahami alur kerja program |
| **Control Flow Graph (CFG)** | Visualisasi blok-blok kode | Memudahkan memahami struktur percabangan dan loop |
| **Cross reference (Xref)** | Hubungan antar fungsi/variabel | Melacak siapa memanggil siapa, dan di mana sesuatu sering dipakai |
| **Header & metadata** | Timestamp build, compiler, arsitektur (32/64-bit) | Memberi clue tambahan, meski timestamp bisa dipalsukan |
| **Packing / obfuscation check** | Apakah file masih asli atau sudah dipacking/di-encrypt | Jika iya, perlu di-unpack dulu sebelum analisis lanjutan |

Semua komponen ini — mulai dari format file, string, import-export, disassembly, hingga packing check — adalah potongan-potongan puzzle yang jika digabungkan memberi gambaran utuh tentang cara kerja program, bahkan sebelum program tersebut dijalankan.

## Catatan Penutup

Bab ini membahas konsep dasar static analysis: definisi, tiga pendekatan utama, kelebihan-kekurangan, kapan teknik ini paling tepat dipakai, serta komponen penting yang jadi perhatian. Bab selanjutnya akan langsung mempraktikkan semua teori ini lewat sebuah studi kasus nyata: membongkar aturan verifikasi password sebuah program menggunakan IDA Free.
