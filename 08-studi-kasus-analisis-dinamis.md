# 08 — Studi Kasus: Dynamic Analysis pada Program `vault_check`

## Ringkasan

Bab ini adalah praktik dynamic analysis menggunakan program yang **sama** dengan bab static analysis sebelumnya: `vault_check`. Bedanya, kali ini program benar-benar **dieksekusi** dalam debugger (GDB) untuk mengamati langsung nilai register dan variabel saat runtime — sekaligus mengonfirmasi ulang temuan static analysis sebelumnya lewat observasi nyata, bukan sekadar pembacaan kode statis.

> Tools yang dipakai: **GDB** (GNU Debugger) di Linux, sebagai pengganti debugger lokal IDA Free yang dipakai pada course aslinya untuk platform Windows/PE. Konsep breakpoint, step, dan pengamatan register tetap identik.

## Langkah 1 — Menentukan Titik Breakpoint

Sama seperti pendekatan pada course umumnya: breakpoint paling efisien dipasang bukan di awal fungsi, melainkan tepat di titik yang relevan dengan logika verifikasi — dalam kasus ini, di dalam badan loop yang melakukan operasi checksum, dan di titik pengecekan akhir.

Dari hasil static analysis sebelumnya, dua alamat penting sudah diketahui:

- `0x4011f1` — instruksi `add DWORD PTR [rbp-0x8], 0x1` (akhir setiap iterasi loop, setelah `acc` diperbarui)
- `0x4011fb` — instruksi `cmp BYTE PTR [rbp-0x1], 0x37` (pengecekan akhir checksum)

```bash
gdb ./vault_check
```

```gdb
break *0x4011f1
break *0x4011fb
run
```

## Langkah 2 — Menjalankan Program dengan Input Uji

Saat prompt `Masukkan kode akses:` muncul di dalam sesi debugger, dimasukkan kandidat password yang sudah ditemukan dari static analysis sebelumnya: `VAULT01d`.

Begitu breakpoint pertama tercapai (`0x4011f1`, di akhir iterasi pertama loop), eksekusi program berhenti, dan nilai-nilai variabel lokal bisa diperiksa langsung lewat GDB:

```gdb
print/x *(unsigned char*)($rbp-1)    # nilai acc saat ini
print/x *(unsigned char*)($rbp-2)    # nilai running_key saat ini
info registers rax rbp rsp
```

## Langkah 3 — Mengamati Akumulator per Iterasi

Daripada menekan `continue` satu-satu sebanyak 8 kali, breakpoint pada `0x4011f1` bisa diberi instruksi otomatis (`commands`) agar GDB mencetak nilai `i`, karakter input saat itu, `running_key`, dan `acc` setiap kali breakpoint tercapai, lalu langsung melanjutkan eksekusi:

```gdb
break *0x4011f1
commands
silent
printf "i=%d  input[i]=0x%02x  running_key=0x%02x  acc=0x%02x\n", \
       *(int*)($rbp-8), *(unsigned char*)($rbp-9), \
       *(unsigned char*)($rbp-2), *(unsigned char*)($rbp-1)
continue
end
run
```

Dengan input `VAULT01d`, hasil pengamatan runtime selama 8 iterasi loop tercatat sebagai berikut:

| i | input[i] (hex) | karakter | running_key setelah +3 | acc setelah iterasi |
|---|---|---|---|---|
| 0 | 0x56 | `V` | 0x5D | 0x0C |
| 1 | 0x41 | `A` | 0x60 | 0x10 |
| 2 | 0x55 | `U` | 0x63 | 0x25 |
| 3 | 0x4C | `L` | 0x66 | 0x0A |
| 4 | 0x54 | `T` | 0x69 | 0x38 |
| 5 | 0x30 | `0` | 0x6C | 0x61 |
| 6 | 0x31 | `1` | 0x6F | 0x3C |
| 7 | 0x64 | `d` | 0x72 | **0x37** |

Nilai `acc` pada baris terakhir (`0x37`) ini **persis sama** dengan nilai checksum target yang sebelumnya ditemukan lewat pembacaan instruksi `cmp BYTE PTR [rbp-0x1], 0x37` saat static analysis — mengonfirmasi bahwa rekonstruksi logika di bab sebelumnya sudah akurat.

## Langkah 4 — Memverifikasi Hasil Pengecekan Akhir

Melanjutkan eksekusi (`continue`) sampai breakpoint kedua (`0x4011fb`) tercapai, nilai `acc` final dan `running_key` final diperiksa sekali lagi:

```gdb
print/x *(unsigned char*)($rbp-1)
```
```
$1 = 0x37
```

```gdb
print/x *(unsigned char*)($rbp-2)
```
```
$2 = 0x72
```

Karena `acc == 0x37` persis sesuai syarat di instruksi `cmp`, eksekusi berikutnya akan mengambil jalur `mov eax, 0x1` (verifikasi berhasil) — bukan jalur gagal.

## Langkah 5 — Melanjutkan Eksekusi hingga Selesai

```gdb
continue
```

Program melanjutkan eksekusi normal hingga selesai, dan keluaran yang muncul:

```
Akses Diterima
```

Hasil ini cocok dengan prediksi: karena `acc` akhir bernilai `0x37`, jalur kode yang diambil program memang jalur "berhasil".

## Insight dari Studi Kasus Ini

- Dynamic analysis tidak selalu berarti mulai dari nol — di sini ia dipakai untuk **mengonfirmasi ulang** secara langsung apa yang sudah dibongkar lewat static analysis, dengan melihat nilai-nilai sungguhan saat program berjalan.
- Breakpoint otomatis (`commands` di GDB) sangat membantu untuk loop yang berulang banyak kali, dibanding menekan `continue` manual satu per satu.
- Mengamati nilai register/variabel di titik yang **tepat** (di akhir tiap iterasi loop, dan di titik pengecekan akhir) jauh lebih efisien dibanding memasang breakpoint di awal fungsi lalu step manual sepanjang jalan.
- Kesesuaian total antara hasil prediksi static analysis (`acc == 0x37` lewat pembacaan kode) dan hasil observasi dynamic analysis (`acc == 0x37` lewat debugger) menegaskan bahwa kedua teknik ini saling melengkapi — satu untuk merekonstruksi logika, satu untuk membuktikannya secara empiris.

## Catatan Penutup

Studi kasus ini menutup pembahasan dasar dynamic analysis dengan mempraktikkan langsung bagaimana checksum berbasis XOR pada program `vault_check` diverifikasi saat runtime — menggunakan breakpoint otomatis dan pengamatan register per iterasi. Dengan metode ini, perilaku program bisa dipahami secara nyata, sekaligus menjadi bukti empiris bahwa hasil static analysis pada bab sebelumnya memang akurat.

Topik selanjutnya yang relevan untuk dipelajari adalah **Anti-Reverse Engineering** — teknik proteksi yang dipakai developer maupun malware untuk menyulitkan proses analisis, seperti eliminasi simbol (*stripping*), obfuscation string, packing, hingga penggunaan API pendeteksi debugger (*debugger present check*).
