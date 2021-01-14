# Mesin-Kasir-Sederhana-CPP

Program mesin kasir sederhana berbasis konsol dengan C++. Fitur: login,
katalog barang, pencatatan pembelian, konfirmasi pembayaran, cetak struk,
penambahan barang ke database, dan penggantian username/password.

Seluruh program ada pada satu berkas: [Program-Kasir.cpp](Program-Kasir.cpp).
Perbedaan perintah bersihkan layar antara Windows (`cls`) dan Linux/macOS
(`clear`) ditangani otomatis melalui `#ifdef _WIN32`.

## Kompilasi dan menjalankan

```sh
g++ -std=c++17 -Wall -Wextra -o kasir Program-Kasir.cpp
./kasir          # Windows: kasir.exe
```

## Login awal

| Username | Password |
| -------- | -------- |
| `admin`  | `admin`  |

Username dan password dapat diubah dari menu **3. Ganti Username/Password**.
Perubahan hanya berlaku selama program berjalan karena data disimpan di memori,
bukan di berkas.
