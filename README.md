# Mesin-Kasir-Sederhana-CPP

Program mesin kasir berbasis konsol dengan C++17. Satu sumber untuk semua
sistem operasi — perbedaan Windows dan Linux/macOS (perintah bersihkan layar,
cara menyembunyikan ketikan password) ditangani lewat kompilasi bersyarat.

## Fitur

- Login dengan dua peran: **admin** (semua menu) dan **kasir** (transaksi saja)
- Transaksi: tambah barang, gabung otomatis barang yang sama, hapus item,
  pembatalan, cek stok, konfirmasi pembayaran, dan cetak struk
- Struk memuat nomor transaksi, tanggal/waktu, nama kasir, dan subtotal per baris
- Kelola database barang: tambah, ubah nama/harga/stok, hapus
- Laporan penjualan: per hari, per tanggal tertentu, seluruh riwayat, dan daftar
  transaksi terakhir — termasuk omzet dan barang terlaris
- Semua data tersimpan di berkas CSV, jadi tidak hilang saat program ditutup

## Kompilasi dan menjalankan

Dengan CMake:

```sh
cmake -B build
cmake --build build
./build/kasir            # Windows: .\build\Debug\kasir.exe
```

Tanpa CMake (g++ langsung):

```sh
g++ -std=c++17 -Wall -Wextra -Isrc -o kasir src/main.cpp src/app/*.cpp src/domain/*.cpp src/io/*.cpp
./kasir
```

## Menjalankan uji

```sh
cmake -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Uji mencakup logika yang tidak butuh konsol: format rupiah, keranjang, katalog,
transaksi, laporan, penulisan/pembacaan CSV, dan penyimpanan. Kerangka ujinya
ada di [tests/uji.h](tests/uji.h) — sengaja minimal supaya proyek ini bisa
dibangun hanya dengan compiler C++17, tanpa mengunduh pustaka apa pun.

## Login awal

| Username | Password | Peran |
| -------- | -------- | ----- |
| `admin`  | `admin`  | admin |
| `kasir`  | `kasir`  | kasir |

Ganti kedua password ini lewat menu **Ganti Username/Password** sebelum dipakai
sungguhan.

## Berkas data

Program membuat tiga berkas CSV di direktori tempat ia dijalankan:

| Berkas           | Isi                                                  |
| ---------------- | ---------------------------------------------------- |
| `barang.csv`     | katalog: kode, nama, harga, stok                     |
| `akun.csv`       | daftar akun: username, password, peran               |
| `transaksi.csv`  | riwayat transaksi, satu baris per item yang dibeli   |

Berkas boleh disunting dengan editor teks atau aplikasi spreadsheet. Baris yang
rusak dilewati saat dimuat, jadi satu baris salah tidak membuat seluruh data
hilang. Kalau `barang.csv` atau `akun.csv` belum ada, program membuatnya dengan
isi bawaan.

> **Catatan keamanan:** password disimpan sebagai teks biasa di `akun.csv`.
> Ketikan password tidak tampil di layar, tetapi berkasnya sendiri belum
> terlindungi. Penyimpanan berupa hash yang benar (bcrypt/Argon2) membutuhkan
> pustaka eksternal dan belum dipasang di proyek ini, jadi jangan pakai password
> yang juga Anda gunakan di tempat lain.

## Struktur sumber

```
src/domain/     aturan bisnis murni, tanpa input/output — bagian yang diuji
src/io/         konsol, pembacaan input, tampilan tabel, CSV, penyimpanan
src/app/        alur menu dan penggabungan semuanya
src/main.cpp    titik masuk program
tests/          program uji
```

Pemisahan ini yang membuat logika perhitungan bisa diuji tanpa mengetik apa pun
di konsol.
