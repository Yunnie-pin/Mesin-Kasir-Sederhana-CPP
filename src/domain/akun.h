#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace kasir {

// Admin boleh mengelola barang dan melihat laporan; kasir hanya melayani
// transaksi dan mengubah akunnya sendiri.
enum class Peran { Admin, Kasir };

std::string namaPeran(Peran peran);
bool peranDariTeks(const std::string& teks, Peran& hasil);

struct Akun {
    std::string username;
    std::string password;
    Peran peran = Peran::Kasir;
};

class DaftarAkun {
public:
    // Isi awal untuk pemakaian pertama, saat berkas data belum ada.
    static DaftarAkun bawaan();

    const Akun* cari(const std::string& username) const;
    bool tambah(const Akun& akun);

    // Mengganti username sekaligus password satu akun. Gagal (false) bila akun
    // tidak ada atau username baru sudah dipakai akun lain.
    bool ubahKredensial(const std::string& usernameLama, const std::string& usernameBaru,
                        const std::string& passwordBaru);

    const std::vector<Akun>& semua() const { return daftar_; }
    bool kosong() const { return daftar_.empty(); }
    std::size_t jumlah() const { return daftar_.size(); }

private:
    std::vector<Akun> daftar_;
};

}  // namespace kasir
