#include "io/penyimpanan.h"

#include <fstream>
#include <string>
#include <vector>

#include "io/csv.h"

namespace kasir {
namespace {

constexpr const char* JUDUL_BARANG = "kode,nama,harga,stok";
constexpr const char* JUDUL_AKUN = "username,password,peran";
constexpr const char* JUDUL_TRANSAKSI =
    "nomor,waktu,kasir,uang_pembeli,kode_barang,nama_barang,harga_satuan,jumlah";

std::vector<std::vector<std::string>> bacaSemuaBaris(const std::string& namaBerkas) {
    std::vector<std::vector<std::string>> baris;
    std::ifstream berkas(namaBerkas);
    if (!berkas) {
        return baris;
    }
    std::string satuBaris;
    while (std::getline(berkas, satuBaris)) {
        if (!satuBaris.empty()) {
            baris.push_back(pecahBarisCsv(satuBaris));
        }
    }
    return baris;
}

std::string teksTransaksi(const Transaksi& transaksi, const BarisKeranjang& baris) {
    return gabungBarisCsv({transaksi.nomor, transaksi.waktu, transaksi.kasir,
                           std::to_string(transaksi.uangPembeli),
                           std::to_string(baris.kodeBarang), baris.nama,
                           std::to_string(baris.hargaSatuan), std::to_string(baris.jumlah)});
}

}  // namespace

bool berkasAda(const std::string& namaBerkas) {
    std::ifstream berkas(namaBerkas);
    return static_cast<bool>(berkas);
}

Katalog muatKatalog(const std::string& namaBerkas) {
    const auto semuaBaris = bacaSemuaBaris(namaBerkas);
    if (semuaBaris.empty()) {
        return Katalog::bawaan();
    }

    Katalog katalog;
    for (const auto& kolom : semuaBaris) {
        long long kode = 0;
        long long harga = 0;
        long long stok = 0;
        if (kolom.size() != 4 || !keBilangan(kolom[0], kode) || !keBilangan(kolom[2], harga) ||
            !keBilangan(kolom[3], stok)) {
            continue;  // baris judul atau baris rusak
        }
        Barang barang;
        barang.kode = static_cast<int>(kode);
        barang.nama = kolom[1];
        barang.harga = harga;
        barang.stok = static_cast<int>(stok);
        katalog.tambahDenganKode(barang);
    }
    return katalog.kosong() ? Katalog::bawaan() : katalog;
}

DaftarAkun muatAkun(const std::string& namaBerkas) {
    const auto semuaBaris = bacaSemuaBaris(namaBerkas);
    if (semuaBaris.empty()) {
        return DaftarAkun::bawaan();
    }

    DaftarAkun daftar;
    for (const auto& kolom : semuaBaris) {
        Peran peran = Peran::Kasir;
        if (kolom.size() != 3 || !peranDariTeks(kolom[2], peran)) {
            continue;  // baris judul atau baris rusak
        }
        daftar.tambah({kolom[0], kolom[1], peran});
    }
    return daftar.kosong() ? DaftarAkun::bawaan() : daftar;
}

std::vector<Transaksi> muatRiwayat(const std::string& namaBerkas) {
    std::vector<Transaksi> riwayat;
    for (const auto& kolom : bacaSemuaBaris(namaBerkas)) {
        long long uangPembeli = 0;
        long long kodeBarang = 0;
        long long hargaSatuan = 0;
        long long jumlah = 0;
        if (kolom.size() != 8 || !keBilangan(kolom[3], uangPembeli) ||
            !keBilangan(kolom[4], kodeBarang) || !keBilangan(kolom[6], hargaSatuan) ||
            !keBilangan(kolom[7], jumlah)) {
            continue;  // baris judul atau baris rusak
        }

        // Satu transaksi tersebar di beberapa baris berurutan dengan nomor sama.
        if (riwayat.empty() || riwayat.back().nomor != kolom[0]) {
            Transaksi transaksi;
            transaksi.nomor = kolom[0];
            transaksi.waktu = kolom[1];
            transaksi.kasir = kolom[2];
            transaksi.uangPembeli = uangPembeli;
            riwayat.push_back(transaksi);
        }

        BarisKeranjang baris;
        baris.kodeBarang = static_cast<int>(kodeBarang);
        baris.nama = kolom[5];
        baris.hargaSatuan = hargaSatuan;
        baris.jumlah = static_cast<int>(jumlah);
        riwayat.back().baris.push_back(baris);
    }
    return riwayat;
}

bool simpanKatalog(const std::string& namaBerkas, const Katalog& katalog) {
    std::ofstream berkas(namaBerkas, std::ios::trunc);
    if (!berkas) {
        return false;
    }
    berkas << JUDUL_BARANG << '\n';
    for (const Barang& barang : katalog.semua()) {
        berkas << gabungBarisCsv({std::to_string(barang.kode), barang.nama,
                                  std::to_string(barang.harga), std::to_string(barang.stok)})
               << '\n';
    }
    return static_cast<bool>(berkas);
}

bool simpanAkun(const std::string& namaBerkas, const DaftarAkun& daftar) {
    std::ofstream berkas(namaBerkas, std::ios::trunc);
    if (!berkas) {
        return false;
    }
    berkas << JUDUL_AKUN << '\n';
    for (const Akun& akun : daftar.semua()) {
        berkas << gabungBarisCsv({akun.username, akun.password, namaPeran(akun.peran)}) << '\n';
    }
    return static_cast<bool>(berkas);
}

bool tambahkanTransaksi(const std::string& namaBerkas, const Transaksi& transaksi) {
    const bool berkasBaru = !berkasAda(namaBerkas);
    std::ofstream berkas(namaBerkas, std::ios::app);
    if (!berkas) {
        return false;
    }
    if (berkasBaru) {
        berkas << JUDUL_TRANSAKSI << '\n';
    }
    for (const BarisKeranjang& baris : transaksi.baris) {
        berkas << teksTransaksi(transaksi, baris) << '\n';
    }
    return static_cast<bool>(berkas);
}

}  // namespace kasir
