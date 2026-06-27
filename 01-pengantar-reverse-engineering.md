# 01 — Pengantar Reverse Engineering

## Ringkasan

Catatan ini membahas konsep paling dasar dari Reverse Engineering (RE): apa bedanya dengan forward engineering, tiga tahapan umum yang dilalui saat membongkar sebuah program, alasan kenapa skill ini penting di dunia cybersecurity, beberapa studi kasus nyata, serta tools populer yang dipakai — termasuk perkenalan singkat dengan IDA Free yang akan jadi tools utama sepanjang catatan belajar ini.

## Forward Engineering vs Reverse Engineering

Sebelum bicara soal "membongkar" program, ada baiknya paham dulu arah sebaliknya: **forward engineering**. Ini adalah proses normal membangun sebuah software, mulai dari tahap perencanaan sampai implementasi.

Alurnya kira-kira begini:

```
source code (human-readable)
      │  compile
      ▼
file biner / executable
      │  load ke RAM
      ▼
proses yang berjalan di OS
```

Contoh paling sederhana: sebuah file `HelloWorld.c` ditulis dalam bahasa C, lalu dikompilasi menjadi file biner. File biner inilah yang kemudian bisa dieksekusi — instruksi-instruksinya dimuat ke RAM dan dijalankan oleh sistem operasi. Inti dari forward engineering: kita mulai dari sesuatu yang mudah dibaca manusia (source code), lalu berakhir di sesuatu yang dipahami komputer (machine code).

**Reverse Engineering** adalah kebalikannya — kadang disebut juga *backward engineering*. Ini adalah proses membongkar sebuah objek buatan manusia (dalam kasus ini: program) untuk mengungkap desain, arsitektur kode, atau pengetahuan yang tersembunyi di dalamnya, **tanpa** memiliki source code aslinya.

```
file biner / executable
      │  disassembly
      ▼
kode assembly (low-level, dekat ke instruksi mesin)
      │  decompile
      ▼
pseudo-code (lebih mudah dibaca, mirip source code asli)
```

Jadi kalau forward engineering itu "menulis lalu mengompilasi", reverse engineering itu "mengambil hasil kompilasi lalu menariknya kembali ke bentuk yang bisa dipahami manusia".

## Tiga Tahapan Umum dalam Reverse Engineering

Dalam praktiknya, RE biasanya melewati tiga tahap besar:

### 1. Information Extraction

Tahap mengambil sebuah file binary (misalnya sampel malware) dan mengekstrak informasi awal darinya — string yang tersimpan, API calls yang dipanggil, struktur file, hingga perilaku dasarnya. Informasi ini menjadi pondasi untuk analisis selanjutnya.

### 2. Modelling / Analysis

Informasi yang sudah terkumpul diolah menjadi pemahaman konseptual: bagaimana cara kerja program tersebut, bagaimana alur eksekusinya, teknik proteksi apa yang dipakai. Tahap ini adalah jantungnya RE, karena di sinilah logika di balik program benar-benar dipahami.

### 3. Review / Validation

Pemahaman yang sudah dibentuk diuji ulang — bisa lewat debugging, sandbox, atau membuat proof of concept (PoC) dari exploit/patch yang ditemukan. Tujuannya memastikan analisis yang dilakukan benar dan bisa dipertanggungjawabkan.

```
[Information Extraction] → [Modelling / Analysis] → [Review / Validation]
        strings,                 cara kerja,              debugging,
       API calls,               alur eksekusi,             sandbox,
     struktur file              teknik proteksi              PoC
```

## Mengapa Reverse Engineering Penting?

Setidaknya ada empat alasan utama:

1. **Memahami dan meningkatkan sistem.** RE memudahkan developer memahami operasi hardware/software sehingga bisa melakukan penyesuaian atau peningkatan agar sistem berjalan lebih optimal.
2. **Menemukan kelemahan keamanan.** Ahli keamanan bisa mengidentifikasi celah yang mungkin dieksploitasi penyerang, sekaligus memperoleh pengetahuan untuk meningkatkan protokol keamanan.
3. **Analisis dan pertahanan malware.** RE memungkinkan kita memahami cara kerja malware, sehingga bisa menciptakan langkah pencegahan dan perlindungan yang lebih efektif.
4. **Pemeliharaan dan pembaruan software.** RE memudahkan pembuatan patch atau update berdasarkan kelemahan yang ditemukan, menjaga keamanan sekaligus fungsionalitas software.

Singkatnya: RE bukan sekadar membongkar software, tapi jadi kunci untuk memahami, melindungi, dan meningkatkan kualitas sistem yang dipakai sehari-hari.

## Studi Kasus Nyata

Beberapa contoh penerapan RE di dunia nyata:

- **Ransomware WannaCry (2017).** RE dipakai untuk menganalisis cara kerja malware ini, bagaimana cara penyebarannya, dan dari situ dikembangkan solusi pemulihan data yang sempat terenkripsi.
- **Kebocoran data Equifax (2017).** Lewat RE, peneliti dapat menelusuri teknik yang dipakai penyerang, lalu hasilnya dipakai untuk memperbaiki kelemahan keamanan dan mencegah serangan serupa terulang.
- **Analisis firmware perangkat IoT.** Banyak perangkat IoT memiliki firmware dengan celah keamanan; peneliti melakukan RE untuk menemukan kelemahan tersebut dan mengusulkan perbaikan.

Tiga contoh ini menunjukkan bahwa RE bukan sekadar teori, melainkan benar-benar diterapkan untuk menghadapi ancaman nyata dan meningkatkan keamanan teknologi sehari-hari.

## Tools Populer untuk Reverse Engineering

| Tools | Catatan |
|---|---|
| **IDA Free / IDA Pro** | Disassembler interaktif yang sangat populer di kalangan praktisi RE |
| **Ghidra** | Dikembangkan oleh NSA, dirilis open-source dan gratis |
| **OllyDbg** | Debugger yang sering dipakai untuk analisis dinamis, khususnya di Windows |
| **radare2** | Framework RE yang mendukung berbagai arsitektur prosesor dan format file |
| **Binary Ninja** | Disassembler sekaligus decompiler dengan antarmuka yang cukup intuitif |

Setiap tools punya keunggulan masing-masing dan biasanya dipilih sesuai kebutuhan analisis.

## Mengenal IDA Free

Sepanjang catatan belajar ini, tools utama yang dipakai adalah **IDA Free** — singkatan dari *Interactive Disassembler*. Ini salah satu tools paling terkenal untuk membongkar binary code, dan banyak dipakai analis malware maupun security researcher untuk memahami cara kerja program hanya dari bentuk binernya.

**Kelebihan versi gratis:**
- Gratis digunakan
- Tersedia debugger lokal
- Ada decompiler via cloud (perlu koneksi internet)
- Visualisasi alur eksekusi program
- Cukup ramah untuk pemula

**Keterbatasan versi gratis:**
- Fitur decompiler terbatas
- Tidak mendukung remote debugging
- Arsitektur yang didukung lebih sedikit dibanding versi Pro
- Kurang fleksibel menghadapi kode yang sudah di-obfuscate atau di-encrypt

### Komponen Tampilan Utama IDA Free

Empat bagian ini akan sering ditemui sepanjang praktik:

1. **Navigation Bar** — di bagian atas, untuk melihat posisi kode dalam keseluruhan program.
2. **Function List** — di sisi kiri, daftar fungsi yang bisa dipilih dan dilihat.
3. **Disassembler View** — tempat melihat instruksi assembly dari binary yang dianalisis.
4. **Graph View** — menampilkan alur eksekusi program dalam bentuk diagram.

## Catatan Penutup

Modul pengantar ini sudah mencakup konsep dasar RE, manfaat dan aplikasinya, contoh kasus nyata, hingga tools yang akan dipakai — terutama IDA Free. Bab selanjutnya akan masuk ke hal yang lebih teknis: bagaimana bahasa assembly menjadi jembatan antara kode mesin dan pemahaman manusia, serta peran disassembler, decompiler, dan debugger dalam proses RE.
