#include "domain/keranjang.h"

#include <algorithm>
#include <numeric>

namespace kasir {

void Keranjang::tambah(const Barang& barang, int jumlah) {
    if (jumlah <= 0) {
        return;
    }
    const auto posisi =
        std::find_if(baris_.begin(), baris_.end(), [&barang](const BarisKeranjang& baris) {
            return baris.kodeBarang == barang.kode;
        });
    if (posisi != baris_.end()) {
        posisi->jumlah += jumlah;
        return;
    }

    BarisKeranjang baris;
    baris.kodeBarang = barang.kode;
    baris.nama = barang.nama;
    baris.hargaSatuan = barang.harga;
    baris.jumlah = jumlah;
    baris_.push_back(baris);
}

bool Keranjang::hapusBaris(std::size_t nomorBaris) {
    if (nomorBaris == 0 || nomorBaris > baris_.size()) {
        return false;
    }
    baris_.erase(baris_.begin() + static_cast<std::ptrdiff_t>(nomorBaris - 1));
    return true;
}

int Keranjang::jumlahDibeli(int kodeBarang) const {
    const auto posisi =
        std::find_if(baris_.begin(), baris_.end(), [kodeBarang](const BarisKeranjang& baris) {
            return baris.kodeBarang == kodeBarang;
        });
    return posisi == baris_.end() ? 0 : posisi->jumlah;
}

Uang Keranjang::total() const {
    return std::accumulate(baris_.begin(), baris_.end(), Uang{0},
                           [](Uang jumlah, const BarisKeranjang& baris) {
                               return jumlah + baris.subtotal();
                           });
}

}  // namespace kasir
