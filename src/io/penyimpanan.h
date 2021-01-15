#pragma once

#include <string>
#include <vector>

#include "domain/akun.h"
#include "domain/barang.h"
#include "domain/transaksi.h"

namespace kasir {

// Nama berkas data, dibuat di direktori kerja saat program dijalankan.
struct BerkasData {
    std::string barang = "barang.csv";
    std::string akun = "akun.csv";
    std::string transaksi = "transaksi.csv";
};

bool berkasAda(const std::string& namaBerkas);

// Baris yang rusak dilewati, bukan membuat program berhenti: data yang masih
// bisa dibaca tetap dipakai. Bila berkas belum ada, isi bawaan yang dipakai.
Katalog muatKatalog(const std::string& namaBerkas);
DaftarAkun muatAkun(const std::string& namaBerkas);
std::vector<Transaksi> muatRiwayat(const std::string& namaBerkas);

bool simpanKatalog(const std::string& namaBerkas, const Katalog& katalog);
bool simpanAkun(const std::string& namaBerkas, const DaftarAkun& daftar);

// Riwayat ditambahkan di akhir berkas, transaksi lama tidak ditulis ulang.
bool tambahkanTransaksi(const std::string& namaBerkas, const Transaksi& transaksi);

}  // namespace kasir
