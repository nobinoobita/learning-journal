# 07 — Teknik Analisis Dinamis (Dynamic Analysis)

## Ringkasan

Bab ini membahas konsep dasar dynamic analysis: bagaimana teknik ini berbeda dari static analysis, jenis-jenis pendekatannya, kelebihan dan kekurangannya, kapan paling tepat digunakan, serta komponen-komponen penting yang perlu diperhatikan saat melakukannya.

## Apa Itu Dynamic Analysis?

Berbeda dengan static analysis yang dilakukan tanpa eksekusi, **Dynamic Analysis** adalah metode memeriksa software dengan cara **mengeksekusi program** dan mengamati perilakunya secara real-time.

Di sini, bukan hanya kode yang dilihat, melainkan benar-benar mengobservasi bagaimana program berjalan: bagaimana ia berinteraksi dengan sistem operasi, file system, registry, jaringan, bahkan penggunaan memori.

Tujuan utamanya: memahami perilaku **nyata** dari sebuah program. Misalnya, apakah program tersebut mencoba membuat koneksi jaringan mencurigakan, menulis file tersembunyi, atau memodifikasi registry tanpa izin — semua ini bisa jadi indikator aktivitas berbahaya.

Dari dynamic analysis, biasanya bisa ditemukan:

- Pola interaksi program dengan lingkungannya (input, output, network activity)
- Masalah yang muncul saat program dieksekusi
- Aktivitas mencurigakan yang tidak terlihat dari kode statis

Dengan kata lain, dynamic analysis menangkap apa yang **benar-benar dilakukan** program saat berjalan — bukan hanya apa yang tertulis di dalam kodenya.

## Jenis-Jenis Dynamic Analysis

| Jenis | Fokus | Kegunaan |
|---|---|---|
| **Protocol Reverse Engineering** | Komunikasi yang dilakukan program/malware | Membongkar bagaimana data dikirim, membuat aturan deteksi/firewall |
| **API Reverse Engineering** | API tersembunyi atau terdokumentasi yang dipanggil program | Mengetahui fungsi apa saja yang dieksploitasi |
| **Behavioral Reverse Engineering** | Perilaku program secara umum saat dijalankan, tanpa peduli detail internal | Mendeteksi aktivitas mencurigakan: koneksi aneh, modifikasi file |
| **UI Reverse Engineering** | Interface program (jika ada) | Memahami interaksi user dan mencari kelemahan untuk di-bypass |
| **Blackbox Reverse Engineering** | Hanya mengamati input dan output, tanpa tahu isi kode | Cocok untuk analisis awal malware baru yang strukturnya belum jelas |
| **Whitebox Reverse Engineering** | Audit kode internal secara mendalam | Lebih jarang dipakai untuk analisis malware, lebih sering untuk audit aplikasi security-critical; bisa menemukan backdoor, kerentanan, atau enkripsi lemah |
| **Greybox Reverse Engineering** | Akses terbatas: log, memory dump, atau potongan kode | Berada di antara blackbox dan whitebox; cocok untuk investigasi insiden saat tidak semua artefak tersedia |

Setiap jenis punya fokus dan kegunaan masing-masing. Dalam konteks cybersecurity, pendekatan yang paling sering dipakai biasanya **behavioral** dan **blackbox analysis**, karena keduanya bisa cepat memberi gambaran nyata tentang bagaimana sebuah malware/software berinteraksi dengan sistem tanpa harus langsung menyelam ke kode internalnya.

## Kelebihan dan Kekurangan Dynamic Analysis

**Kelebihan:**

- Memberikan wawasan nyata tentang perilaku program saat runtime.
- Sangat membantu mendeteksi interaksi saat runtime dan teknik anti-analisis yang mungkin dipakai malware — termasuk bagaimana malware berusaha beradaptasi atau menghindari deteksi.

**Kekurangan:**

- Bisa sangat memakan waktu dan sumber daya.
- Hasil yang diperoleh tidak selalu mencerminkan perilaku sesungguhnya di lingkungan nyata (misalnya kalau malware mendeteksi sedang dianalisis dan mengubah perilakunya).
- Risiko tinggi jika analisis tidak dilakukan di sistem yang benar-benar terisolasi — malware bisa menginfeksi lingkungan sekitarnya.

## Kapan Dynamic Analysis Digunakan?

Dynamic analysis biasanya dipakai ketika perlu benar-benar memahami perilaku program secara langsung, misalnya untuk:

- Memantau bagaimana program berinteraksi dengan sistem saat dijalankan.
- Mendeteksi teknik anti-analisis yang dipakai malware.
- Mengidentifikasi ancaman tersembunyi seperti rootkit.
- Memverifikasi proses berbahaya.
- Menemukan *indicator of compromise* (IoC) yang bisa dijadikan acuan dalam investigasi forensik.

## Komponen Penting dalam Dynamic Analysis

| Komponen | Yang Diamati | Kegunaan |
|---|---|---|
| **Breakpoint Setting** | Titik henti eksekusi tertentu | Mengamati kondisi register, memori, dan variabel pada saat dijalankan — sering jadi bagian tersulit untuk ditentukan |
| **File System Activity** | Pembuatan, penghapusan, modifikasi file/folder | Misalnya malware yang membuat file baru di direktori sistem sebagai mekanisme bertahan hidup |
| **Registry Change** | Perubahan startup entry, service, policy | Sering jadi indikator mekanisme persistence |
| **Process and Memory Behavior** | Pembuatan proses baru, injeksi ke proses lain, konsumsi memori berlebihan | Termasuk API calls yang dipakai — bisa memberi petunjuk aktivitas berbahaya |
| **Network Activity** | Koneksi keluar/masuk, alamat IP/URL yang dihubungi, data yang dikirim | Kunci untuk mendeteksi komunikasi dengan command and control server |
| **System Calls and API Usage** | Fungsi sistem yang benar-benar dipanggil saat runtime | Melacak perilaku program lebih detail, biasanya lewat debugger seperti IDA Free/Pro |
| **Evasion Techniques** | Upaya program mendeteksi debugger/VM/sandbox | Malware modern sering mencoba menyembunyikan atau mematikan perilaku berbahayanya kalau merasa sedang dianalisis |

Semua komponen ini penting, tapi yang paling sering jadi fokus utama biasanya **process & memory behavior**, **network activity**, serta **API calls** — karena ketiganya langsung menunjukkan interaksi program dengan sistem maupun server eksternal.

## Catatan Penutup

Bab ini membahas konsep, jenis, serta komponen penting dalam dynamic analysis — termasuk bagaimana teknik ini membantu memahami perilaku nyata program, beserta kelebihan dan kekurangannya. Bab selanjutnya akan langsung mempraktikkan dynamic analysis pada studi kasus yang sama dengan bab static analysis sebelumnya, untuk melihat bagaimana verifikasi password tersebut benar-benar berjalan saat program dieksekusi.
