# 02 — Dasar Teknis Reverse Engineering

## Ringkasan

Bab ini membahas fondasi teknis yang dipakai sepanjang proses RE: kenapa bahasa assembly begitu penting, tiga perannya dalam reverse engineering, dan tiga kategori tools utama — disassembler, decompiler, dan debugger — yang masing-masing punya cara berbeda untuk "membaca" sebuah program.

## Mengapa Bahasa Assembly Itu Penting

CPU hanya mengerti instruksi dalam bentuk biner — deretan angka 0 dan 1. Instruksi-instruksi biner inilah yang menentukan operasi sederhana seperti memindahkan data, melakukan perhitungan, atau mengendalikan alur program.

Masalahnya: kode biner murni hampir mustahil dipahami langsung oleh manusia. Di sinilah **bahasa assembly** berperan — memberikan bentuk simbolis/tekstual dari instruksi biner sehingga jauh lebih mudah dibaca.

Contoh kecil:

```asm
mov eax, 1   ; memindahkan nilai 1 ke register EAX
add eax, 2   ; menambahkan nilai 2 ke EAX
```

Perlu diingat juga: assembly bukan satu bahasa tunggal. Setiap arsitektur prosesor (x86, ARM, dan lainnya) memiliki set instruksi sendiri-sendiri, sehingga kode assembly untuk satu jenis prosesor bisa berbeda bentuknya dari prosesor lain.

## Tiga Peran Utama Assembly dalam Reverse Engineering

1. **Jembatan antara kode mesin dan manusia.** Tanpa assembly, yang terlihat hanyalah deretan angka biner yang sulit dimengerti. Dengan assembly, instruksi komputer jadi bisa dibaca dan dianalisis manusia.
2. **Mengungkap logika internal software.** Dari instruksi-instruksi sederhana seperti `mov`, `add`, atau `jmp`, kita bisa memahami bagaimana sebuah program mengambil keputusan, mengolah data, dan menjalankan fungsinya.
3. **Fondasi untuk analisis lanjutan.** Hampir semua tools RE menampilkan hasilnya dalam bentuk assembly, sehingga kemampuan membaca assembly menjadi modal dasar sebelum melangkah ke disassembler, decompiler, atau debugger yang lebih kompleks.

Dengan kata lain, memahami assembly itu sama dengan membuka pintu utama menuju analisis RE yang lebih dalam.

## Tiga Kategori Tools Utama

### 1. Disassembler

**Disassembler** adalah program yang mengubah file biner/executable menjadi file teks berisi kode assembly.

```
[ 0101 0001 1010 0011 ... ]  --(disassembly)-->  mov eax, 1
                                                  add eax, 2
```

Hasil assembly ini sangat bergantung pada jenis prosesor/arsitektur CPU yang digunakan — namun disassembler modern umumnya mendukung banyak arsitektur sekaligus sehingga cukup fleksibel. Disassembler berkualitas tinggi adalah alat esensial bagi reverse engineer, karena tanpa alat ini akan sangat sulit memahami logika program hanya dari deretan 0 dan 1.

### 2. Decompiler (Compiler dalam konteks RE)

Kalau disassembler menghasilkan kode assembly, **decompiler** melangkah lebih jauh: mengubah kode mesin menjadi representasi kode tingkat tinggi yang menyerupai bahasa C.

Kenapa ini penting? Karena membaca assembly butuh keahlian khusus dan memakan waktu. Dengan decompiler, hasil analisis bisa lebih cepat dipahami lewat *pseudo-code* yang lebih familiar.

```asm
mov  eax, [rbp-4]
cmp  eax, 10
jle  label_a
```

bisa direpresentasikan sebagai pseudo-code mirip:

```c
if (x <= 10) {
    goto label_a;
}
```

Namun perlu diingat: hasil decompiler **tidak selalu identik** dengan source code aslinya. Beberapa keterbatasannya:

- Struktur program asli bisa hilang.
- Nama variabel bisa berubah menjadi nama generik (misalnya `char_1`).
- Logika program kadang masih sulit dipahami.

Meski begitu, decompiler membantu reverse engineer menyusun gambaran besar logika program tanpa harus terjebak membaca assembly baris demi baris.

### 3. Debugger

Kalau disassembler dan decompiler memberi gambaran *statis* dari program, **debugger** memungkinkan kita mengawasi program secara *dinamis* — saat program benar-benar dijalankan.

Dengan debugger, kita bisa:

- Mengeksekusi program baris demi baris (*step by step*).
- Mengamati bagaimana instruksi berjalan di CPU.
- Memodifikasi alur eksekusi.
- Melihat perubahan nilai variabel dan register secara real-time.

Salah satu fitur terpenting dalam debugger adalah **breakpoint** — sebuah titik henti yang ditentukan pada baris/instruksi tertentu, sehingga program akan berhenti tepat di sana. Ini memungkinkan pemeriksaan keadaan program secara lebih detail: isi memori, nilai variabel, atau instruksi yang siap dijalankan berikutnya.

Banyak debugger modern menampilkan informasi dalam bentuk yang lebih mudah dipahami, kadang menyerupai source code aslinya — meski di balik layar tetap bekerja langsung dengan kode mesin. Karena itu, debugger dengan dukungan assembly yang kuat menjadi alat sangat penting: lewat debugger kita bisa memahami perilaku program bukan hanya dari kode statisnya, tapi dari cara program benar-benar berjalan.

## Ringkasan Perbandingan

| Tools | Sifat Analisis | Output |
|---|---|---|
| Disassembler | Statis | Kode assembly |
| Decompiler | Statis | Pseudo-code (mirip bahasa tingkat tinggi) |
| Debugger | Dinamis | Perilaku runtime: register, memori, alur eksekusi |

## Catatan Penutup

Bab ini sudah mengenalkan dasar-dasar teknis penting: bahasa assembly, disassembler, decompiler, dan debugger. Keempat hal ini adalah pondasi yang selalu dipakai dalam setiap aktivitas reverse engineering. Bab selanjutnya akan masuk lebih spesifik ke topik opcode dan operand dalam assembly x86-64, serta bagaimana instruksi-instruksi tersebut mengendalikan alur program.
