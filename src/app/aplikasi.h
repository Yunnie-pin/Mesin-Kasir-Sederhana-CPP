#pragma once

#include <string>
#include <vector>

#include "domain/akun.h"
#include "domain/barang.h"
#include "domain/keranjang.h"
#include "domain/transaksi.h"
#include "domain/uang.h"
#include "io/penyimpanan.h"

namespace kasir {

// Menyatukan data toko, alur menu, dan penyimpanan. Data dimuat saat objek
// dibuat dan ditulis ulang setiap kali ada perubahan, sehingga tidak ada
// perubahan yang hilang walau program ditutup mendadak.
class Aplikasi {
public:
    Aplikasi();

    // Mengembalikan kode keluar untuk main().
    int jalankan();

private:
    enum class Lanjut { KeMenuUtama, KeluarProgram };

    bool login();
    void jalankanMenuUtama();

    Lanjut menuProgramKasir();
    Lanjut menuKelolaBarang();
    Lanjut menuLaporan();
    Lanjut menuGantiKredensial();
    Lanjut pilihAksiSetelahStruk();

    void tambahKeKeranjang(Keranjang& keranjang);
    void hapusDariKeranjang(Keranjang& keranjang);
    Uang mintaPembayaran(Uang total);
    void selesaikanTransaksi(const Keranjang& keranjang);

    void tambahBarangBaru();
    void ubahBarang();
    void hapusBarang();

    // Barang yang dipilih pengguna, atau nullptr bila dibatalkan / tidak ada.
    const Barang* pilihBarang(const std::string& prompt);

    void simpanKatalogAtauPeringatkan();
    void simpanAkunAtauPeringatkan();

    BerkasData berkas_;
    Katalog katalog_;
    DaftarAkun akun_;
    std::vector<Transaksi> riwayat_;
    std::string usernameAktif_;
    Peran peranAktif_ = Peran::Kasir;
};

}  // namespace kasir
