#pragma once

#include <string>
#include <vector>

#include "domain/keranjang.h"
#include "domain/uang.h"

namespace kasir {

// Satu transaksi yang sudah dibayar. Disimpan ke berkas sebagai riwayat dan
// menjadi dasar laporan penjualan.
struct Transaksi {
    std::string nomor;
    std::string waktu;  // "YYYY-MM-DD HH:MM:SS"
    std::string kasir;
    std::vector<BarisKeranjang> baris;
    Uang uangPembeli = 0;

    Uang total() const;
    Uang kembalian() const;
    std::string tanggal() const;  // 10 karakter pertama dari waktu
};

// Nomor berurutan per hari, misalnya "TRX-20260817-0001".
std::string buatNomorTransaksi(const std::vector<Transaksi>& riwayat, const std::string& waktu);

}  // namespace kasir
