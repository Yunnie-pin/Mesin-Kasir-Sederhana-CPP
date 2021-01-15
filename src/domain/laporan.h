#pragma once

#include <string>
#include <vector>

#include "domain/transaksi.h"
#include "domain/uang.h"

namespace kasir {

struct RingkasanBarang {
    std::string nama;
    int jumlahTerjual = 0;
    Uang omzet = 0;
};

struct Laporan {
    std::string tanggal;  // kosong berarti seluruh riwayat
    int jumlahTransaksi = 0;
    Uang omzet = 0;
    std::vector<RingkasanBarang> perBarang;  // urut dari yang paling laris
};

// Tanggal kosong ("") berarti seluruh riwayat diringkas, bukan satu hari saja.
Laporan buatLaporan(const std::vector<Transaksi>& riwayat, const std::string& tanggal);

}  // namespace kasir
