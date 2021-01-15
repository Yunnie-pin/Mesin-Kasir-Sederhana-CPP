#include "domain/barang.h"

#include <algorithm>
#include <cctype>

namespace kasir {
namespace {

bool samaTanpaBesarKecil(const std::string& kiri, const std::string& kanan) {
    if (kiri.size() != kanan.size()) {
        return false;
    }
    for (std::size_t posisi = 0; posisi < kiri.size(); ++posisi) {
        const int hurufKiri = std::tolower(static_cast<unsigned char>(kiri[posisi]));
        const int hurufKanan = std::tolower(static_cast<unsigned char>(kanan[posisi]));
        if (hurufKiri != hurufKanan) {
            return false;
        }
    }
    return true;
}

}  // namespace

Katalog Katalog::bawaan() {
    Katalog katalog;
    katalog.tambah("Buku", 5000, 100);
    katalog.tambah("Bolpoin", 2000, 100);
    katalog.tambah("Tipe-X", 1000, 50);
    katalog.tambah("Pensil", 1000, 100);
    return katalog;
}

int Katalog::tambah(const std::string& nama, Uang harga, int stok) {
    Barang barang;
    barang.kode = ++kodeTerakhir_;
    barang.nama = nama;
    barang.harga = harga;
    barang.stok = stok;
    daftar_.push_back(barang);
    return barang.kode;
}

bool Katalog::tambahDenganKode(const Barang& barang) {
    if (barang.kode <= 0 || barang.nama.empty() || barang.harga < 0 || barang.stok < 0) {
        return false;
    }
    if (cari(barang.kode) != nullptr) {
        return false;
    }
    daftar_.push_back(barang);
    kodeTerakhir_ = std::max(kodeTerakhir_, barang.kode);
    return true;
}

bool Katalog::ubah(int kode, const std::string& nama, Uang harga, int stok) {
    Barang* barang = cari(kode);
    if (barang == nullptr || nama.empty() || harga < 0 || stok < 0) {
        return false;
    }
    barang->nama = nama;
    barang->harga = harga;
    barang->stok = stok;
    return true;
}

bool Katalog::hapus(int kode) {
    const auto posisi = std::find_if(daftar_.begin(), daftar_.end(),
                                     [kode](const Barang& barang) { return barang.kode == kode; });
    if (posisi == daftar_.end()) {
        return false;
    }
    daftar_.erase(posisi);
    return true;
}

Barang* Katalog::cari(int kode) {
    const auto posisi = std::find_if(daftar_.begin(), daftar_.end(),
                                     [kode](const Barang& barang) { return barang.kode == kode; });
    return posisi == daftar_.end() ? nullptr : &*posisi;
}

const Barang* Katalog::cari(int kode) const {
    const auto posisi = std::find_if(daftar_.begin(), daftar_.end(),
                                     [kode](const Barang& barang) { return barang.kode == kode; });
    return posisi == daftar_.end() ? nullptr : &*posisi;
}

bool Katalog::namaDipakai(const std::string& nama, int kecualiKode) const {
    return std::any_of(daftar_.begin(), daftar_.end(), [&](const Barang& barang) {
        return barang.kode != kecualiKode && samaTanpaBesarKecil(barang.nama, nama);
    });
}

bool Katalog::kurangiStok(int kode, int jumlah) {
    Barang* barang = cari(kode);
    if (barang == nullptr || jumlah <= 0 || barang->stok < jumlah) {
        return false;
    }
    barang->stok -= jumlah;
    return true;
}

}  // namespace kasir
