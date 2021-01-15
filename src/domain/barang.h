#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "domain/uang.h"

namespace kasir {

struct Barang {
    int kode = 0;
    std::string nama;
    Uang harga = 0;
    int stok = 0;
};

// Daftar barang yang dijual. Kode barang tidak pernah dipakai ulang, sehingga
// struk lama tetap menunjuk barang yang sama walau katalog berubah.
class Katalog {
public:
    // Isi awal untuk pemakaian pertama, saat berkas data belum ada.
    static Katalog bawaan();

    // Mengembalikan kode barang baru.
    int tambah(const std::string& nama, Uang harga, int stok);

    // Dipakai saat memuat dari berkas: kode ditentukan pemanggil. Gagal (false)
    // bila kode tidak masuk akal atau sudah terpakai.
    bool tambahDenganKode(const Barang& barang);

    bool ubah(int kode, const std::string& nama, Uang harga, int stok);
    bool hapus(int kode);

    Barang* cari(int kode);
    const Barang* cari(int kode) const;

    // Pembandingan tanpa membedakan huruf besar/kecil agar tidak ada dua
    // "Buku" dan "buku" di katalog.
    bool namaDipakai(const std::string& nama, int kecualiKode = 0) const;

    bool kurangiStok(int kode, int jumlah);

    const std::vector<Barang>& semua() const { return daftar_; }
    bool kosong() const { return daftar_.empty(); }
    std::size_t jumlah() const { return daftar_.size(); }
    int kodeBerikutnya() const { return kodeTerakhir_ + 1; }

private:
    std::vector<Barang> daftar_;
    int kodeTerakhir_ = 0;
};

}  // namespace kasir
