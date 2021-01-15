#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "domain/barang.h"
#include "domain/uang.h"

namespace kasir {

// Satu baris pada keranjang maupun struk. Nama dan harga disalin supaya struk
// lama tidak berubah ketika harga barang di katalog diperbarui.
struct BarisKeranjang {
    int kodeBarang = 0;
    std::string nama;
    Uang hargaSatuan = 0;
    int jumlah = 0;

    Uang subtotal() const { return hargaSatuan * jumlah; }
};

class Keranjang {
public:
    // Barang yang sudah ada di keranjang ditambah jumlahnya, bukan jadi baris
    // baru, supaya struk tidak memuat dua baris untuk barang yang sama.
    void tambah(const Barang& barang, int jumlah);

    // Nomor baris mengikuti yang tampil di layar, dimulai dari 1.
    bool hapusBaris(std::size_t nomorBaris);

    // Berapa banyak barang ini sudah masuk keranjang; dipakai untuk mengecek
    // stok yang masih tersisa.
    int jumlahDibeli(int kodeBarang) const;

    Uang total() const;

    const std::vector<BarisKeranjang>& baris() const { return baris_; }
    bool kosong() const { return baris_.empty(); }
    std::size_t jumlahBaris() const { return baris_.size(); }

private:
    std::vector<BarisKeranjang> baris_;
};

}  // namespace kasir
