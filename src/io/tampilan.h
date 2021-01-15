#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "domain/barang.h"
#include "domain/keranjang.h"
#include "domain/laporan.h"
#include "domain/transaksi.h"

namespace kasir {

void tampilkanJudulProgram();
void tampilkanKatalog(const Katalog& katalog);
void tampilkanKeranjang(const Keranjang& keranjang);
void tampilkanStruk(const Transaksi& transaksi);
void tampilkanLaporan(const Laporan& laporan);

// Menampilkan sejumlah transaksi terakhir; 0 berarti seluruh riwayat.
void tampilkanRiwayat(const std::vector<Transaksi>& riwayat, std::size_t jumlahTerakhir);

}  // namespace kasir
