# 03 — Dasar Assembly x86-64: Opcode, Operand, dan Kontrol Program

## Ringkasan

Setelah mengenal *kenapa* assembly penting, bab ini masuk ke detail yang lebih teknis: bagaimana sebuah instruksi assembly sebenarnya tersusun (mnemonic, opcode, operand), bagaimana CPU x86-64 mengenkode instruksi ke dalam byte, serta bagaimana instruksi-instruksi kontrol (compare & jump) menentukan alur eksekusi program. Pemahaman ini akan langsung dipakai saat membaca hasil disassembly di IDA Free pada bab-bab praktik nanti.

## Anatomi Sebuah Instruksi

Sebuah instruksi assembly pada dasarnya terdiri dari dua bagian utama:

- **Mnemonic** — nama instruksi yang mudah dibaca manusia, misalnya `MOV`, `ADD`, `CMP`, `JMP`.
- **Operand** — data atau lokasi yang dilibatkan dalam operasi: bisa berupa register, nilai immediate (konstanta), atau alamat memori.

```asm
mov eax, 5
```

Di sini `mov` adalah mnemonic (artinya "pindahkan data"), sedangkan `eax` dan `5` adalah operand — nilai `5` dipindahkan ke register `eax`.

Pada level hardware, instruksi yang sama ini dikodekan sebagai sebuah **opcode**: serangkaian byte biner yang langsung dipahami dan dieksekusi oleh CPU. Mnemonic hanyalah "nama panggilan" yang mudah diingat manusia — di balik layar, setiap mnemonic dipetakan ke satu atau lebih opcode tergantung ukuran operand, mode pengalamatan, dan prefix instruksi yang dipakai.

## Dari Mnemonic ke Opcode

Sebagai ilustrasi, instruksi:

```asm
mov eax, 5
```

akan dikodekan dalam memori sebagai urutan byte:

```
B8 05 00 00 00
```

- `B8` adalah opcode untuk "pindahkan nilai immediate 32-bit ke EAX".
- `05 00 00 00` adalah representasi little-endian dari angka `5`.

Beberapa hal penting yang perlu diingat:

- **Satu mnemonic bisa punya banyak opcode.** Mnemonic seperti `XOR` atau `AND` punya lebih dari 20 variasi opcode tergantung jenis operand yang dipakai (register ke register, register ke memori, dengan immediate, dan seterusnya).
- **Panjang instruksi bervariasi.** Instruksi x86 modern bisa terdiri dari 1 hingga 15 byte. Salah membaca panjang sebuah instruksi saat disassembly manual bisa membuat seluruh pembacaan byte-byte berikutnya jadi "geser" dan salah total — ini sering disebut *disassembly desync*.
- **Little-endian berlaku di sini juga.** Nilai multi-byte (seperti immediate atau alamat) disimpan dengan byte paling rendah lebih dulu, sama seperti yang sudah dibahas pada konsep representasi data.

Untungnya, dalam praktik sehari-hari kita tidak perlu menghitung opcode secara manual — itulah pekerjaan disassembler seperti IDA Free. Tapi memahami bahwa "di balik setiap baris assembly yang rapi, ada deretan byte opcode" membantu menjelaskan kenapa disassembler kadang salah menebak batas instruksi, terutama pada binary yang di-obfuscate.

## Jenis-Jenis Operand

Operand dalam x86/x64 umumnya berasal dari tiga sumber:

| Jenis Operand | Contoh | Keterangan |
|---|---|---|
| Register | `eax`, `rbx` | Penyimpanan tercepat, langsung di CPU |
| Immediate | `5`, `0x10` | Nilai konstan yang ditulis langsung di instruksi |
| Memory | `[rbp-0x10]` | Alamat memori, bisa dihitung dari kombinasi register + offset |

Mode pengalamatan memori bisa cukup fleksibel, contohnya:

```asm
mov eax, [ebx + ecx*4 + 8]
```

Instruksi ini mengambil nilai dari alamat yang dihitung sebagai `EBX + (ECX × 4) + 8` — pola yang umum dipakai untuk mengakses elemen array. Kombinasi *base register + index register + scale + displacement* semacam ini sering muncul saat sebuah loop mengakses array, sehingga penting dikenali saat membaca hasil disassembly.

Penanda ukuran operand seperti `dword ptr`, `qword ptr` juga sering muncul di hasil disassembly IDA — ini memberi petunjuk seberapa besar data yang sedang diakses (32-bit, 64-bit, dan seterusnya), yang berguna untuk menebak tipe data asli pada source code.

## Instruksi Kontrol: CMP dan Jump

Salah satu pasangan instruksi paling sering ditemui saat membongkar logika program — termasuk logika verifikasi password yang akan dipraktikkan di bab studi kasus nanti — adalah `CMP` diikuti instruksi *jump* bersyarat.

### CMP

`CMP` membandingkan dua operand dengan cara mengurangkan operand kedua dari operand pertama, **tanpa menyimpan hasilnya** — yang berubah hanyalah flags (CF, ZF, SF, OF, dan lainnya) di EFLAGS register.

```asm
cmp eax, ebx
```

Kalau `eax` sama dengan `ebx`, maka hasil pengurangannya nol, sehingga **Zero Flag (ZF)** akan diset ke 1.

### Jump Bersyarat

Instruksi jump bersyarat memeriksa flags hasil dari `CMP` (atau operasi lain) untuk menentukan apakah program akan "melompat" ke alamat lain atau melanjutkan secara berurutan.

```asm
cmp eax, ebx
je  label_equal      ; lompat ke label_equal jika ZF = 1 (eax == ebx)
```

Beberapa instruksi jump bersyarat yang sering ditemui:

| Instruksi | Lompat Jika | Berdasarkan Flag |
|---|---|---|
| `JE` / `JZ` | Sama / hasil nol | ZF = 1 |
| `JNE` / `JNZ` | Tidak sama / hasil tidak nol | ZF = 0 |
| `JG` | Lebih besar (signed) | kombinasi ZF, SF, OF |
| `JL` | Lebih kecil (signed) | kombinasi SF, OF |

Pola umum yang muncul saat membongkar validasi seperti pengecekan password biasanya berbentuk:

```asm
cmp  al, [rdx]      ; bandingkan karakter input dengan karakter kunci
jne  fail           ; kalau tidak sama, lompat ke rute "gagal"
inc  rdx            ; lanjut ke karakter berikutnya
jmp  loop_check
```

Pola CMP + conditional jump berulang dalam sebuah loop seperti ini adalah ciri khas dari fungsi verifikasi yang membandingkan input karakter demi karakter — sesuatu yang akan benar-benar dipraktikkan saat membongkar studi kasus password verification di bab selanjutnya.

## TEST sebagai Variasi CMP

Instruksi `TEST` mirip `CMP`, tapi melakukan operasi bitwise AND (bukan pengurangan) antar operand, juga tanpa menyimpan hasil — hanya mempengaruhi flags.

```asm
test eax, eax
jz   is_zero
```

Pola `test eax, eax` adalah cara umum untuk memeriksa apakah `eax` bernilai nol, karena AND sebuah nilai dengan dirinya sendiri akan menghasilkan nol hanya jika nilai itu sendiri nol.

## Mengapa Ini Penting untuk Tahap Selanjutnya

Pemahaman tentang opcode, operand, dan instruksi kontrol semacam CMP/jump ini akan langsung relevan ketika nanti membaca hasil disassembly di IDA Free — baik dalam bentuk *Graph View* (yang memvisualisasikan percabangan sebagai panah-panah) maupun *Text View* (yang menampilkan baris instruksi satu per satu). Mengenali pola "bandingkan lalu lompat" adalah kunci untuk menemukan logika tersembunyi seperti aturan verifikasi password, validasi input, atau mekanisme proteksi lainnya.

## Catatan Penutup

Bab ini melengkapi pemahaman dasar assembly dengan detail teknis tentang bagaimana instruksi sebenarnya tersusun — dari mnemonic yang mudah dibaca, sampai opcode biner yang dieksekusi CPU, hingga bagaimana operand dan instruksi kontrol membentuk logika program. Bab selanjutnya akan membahas struktur file binary itu sendiri: bagaimana sebuah file executable (PE) tersusun, dan bagaimana menavigasi struktur tersebut di IDA Free.
