#include "io/tampilan.h"

#include <iomanip>
#include <iostream>

#include "io/konsol.h"

namespace kasir {
namespace {

constexpr int LEBAR_KODE = 6;
constexpr int LEBAR_NAMA = 22;
constexpr int LEBAR_ANGKA = 10;
constexpr int LEBAR_UANG = 16;
constexpr std::size_t LEBAR_TABEL = 64;

void cetakGaris() { std::cout << garis('=', LEBAR_TABEL) << '\n'; }

void cetakJudulTabel(const std::string& judul) {
    cetakGaris();
    std::cout << judul << '\n';
    cetakGaris();
}

}  // namespace

void tampilkanJudulProgram() {
    std::cout << garis('=', 19) << '\n';
    std::cout << "PROGRAM MESIN KASIR\n";
    std::cout << garis('=', 19) << '\n';
}

void tampilkanKatalog(const Katalog& katalog) {
    cetakJudulTabel("DATABASE BARANG");
    std::cout << std::left << std::setw(LEBAR_KODE) << "Kode" << std::setw(LEBAR_NAMA) << "Nama"
              << std::right << std::setw(LEBAR_UANG) << "Harga" << std::setw(LEBAR_ANGKA) << "Stok"
              << '\n';
    if (katalog.kosong()) {
        std::cout << "(belum ada barang di database)\n";
    }
    for (const Barang& barang : katalog.semua()) {
        std::cout << std::left << std::setw(LEBAR_KODE) << barang.kode << std::setw(LEBAR_NAMA)
                  << ringkasTeks(barang.nama, LEBAR_NAMA - 1) << std::right
                  << std::setw(LEBAR_UANG) << formatRupiah(barang.harga)
                  << std::setw(LEBAR_ANGKA) << (barang.stok == 0 ? "habis" : std::to_string(barang.stok))
                  << '\n';
    }
    cetakGaris();
}

void tampilkanKeranjang(const Keranjang& keranjang) {
    if (keranjang.kosong()) {
        return;
    }
    cetakJudulTabel("KERANJANG");
    std::cout << std::left << std::setw(LEBAR_KODE) << "No" << std::setw(LEBAR_NAMA) << "Nama"
              << std::right << std::setw(LEBAR_ANGKA) << "Jumlah" << std::setw(LEBAR_UANG)
              << "Subtotal" << '\n';
    const std::vector<BarisKeranjang>& baris = keranjang.baris();
    for (std::size_t indeks = 0; indeks < baris.size(); ++indeks) {
        std::cout << std::left << std::setw(LEBAR_KODE) << indeks + 1 << std::setw(LEBAR_NAMA)
                  << ringkasTeks(baris[indeks].nama, LEBAR_NAMA - 1) << std::right
                  << std::setw(LEBAR_ANGKA) << baris[indeks].jumlah << std::setw(LEBAR_UANG)
                  << formatRupiah(baris[indeks].subtotal()) << '\n';
    }
    cetakGaris();
    std::cout << "Total sementara = " << formatRupiah(keranjang.total()) << "\n\n";
}

void tampilkanStruk(const Transaksi& transaksi) {
    cetakJudulTabel("STRUK PEMBELIAN");
    std::cout << "Nomor  : " << transaksi.nomor << '\n';
    std::cout << "Waktu  : " << transaksi.waktu << '\n';
    std::cout << "Kasir  : " << transaksi.kasir << '\n';
    cetakGaris();
    std::cout << std::left << std::setw(LEBAR_NAMA) << "Nama" << std::right
              << std::setw(LEBAR_ANGKA) << "Jumlah" << std::setw(LEBAR_UANG) << "Harga/pcs"
              << std::setw(LEBAR_UANG) << "Subtotal" << '\n';
    for (const BarisKeranjang& baris : transaksi.baris) {
        std::cout << std::left << std::setw(LEBAR_NAMA) << ringkasTeks(baris.nama, LEBAR_NAMA - 1)
                  << std::right << std::setw(LEBAR_ANGKA) << baris.jumlah << std::setw(LEBAR_UANG)
                  << formatRupiah(baris.hargaSatuan) << std::setw(LEBAR_UANG)
                  << formatRupiah(baris.subtotal()) << '\n';
    }
    cetakGaris();
    std::cout << "Harga Total    = " << formatRupiah(transaksi.total()) << '\n';
    std::cout << "Uang Pembeli   = " << formatRupiah(transaksi.uangPembeli) << '\n';
    std::cout << "Uang Kembalian = " << formatRupiah(transaksi.kembalian()) << "\n\n";
    std::cout << "Terima kasih atas kunjungan Anda!\n";
}

void tampilkanLaporan(const Laporan& laporan) {
    cetakJudulTabel(laporan.tanggal.empty() ? "LAPORAN PENJUALAN (SELURUH RIWAYAT)"
                                            : "LAPORAN PENJUALAN " + laporan.tanggal);
    std::cout << "Jumlah transaksi = " << laporan.jumlahTransaksi << '\n';
    std::cout << "Omzet            = " << formatRupiah(laporan.omzet) << '\n';

    if (laporan.perBarang.empty()) {
        std::cout << "\n(belum ada transaksi pada rentang ini)\n";
        cetakGaris();
        return;
    }

    cetakGaris();
    std::cout << "Barang terlaris:\n";
    std::cout << std::left << std::setw(LEBAR_KODE) << "No" << std::setw(LEBAR_NAMA) << "Nama"
              << std::right << std::setw(LEBAR_ANGKA) << "Terjual" << std::setw(LEBAR_UANG)
              << "Omzet" << '\n';
    for (std::size_t indeks = 0; indeks < laporan.perBarang.size(); ++indeks) {
        const RingkasanBarang& ringkasan = laporan.perBarang[indeks];
        std::cout << std::left << std::setw(LEBAR_KODE) << indeks + 1 << std::setw(LEBAR_NAMA)
                  << ringkasTeks(ringkasan.nama, LEBAR_NAMA - 1) << std::right
                  << std::setw(LEBAR_ANGKA) << ringkasan.jumlahTerjual << std::setw(LEBAR_UANG)
                  << formatRupiah(ringkasan.omzet) << '\n';
    }
    cetakGaris();
}

void tampilkanRiwayat(const std::vector<Transaksi>& riwayat, std::size_t jumlahTerakhir) {
    cetakJudulTabel("RIWAYAT TRANSAKSI");
    if (riwayat.empty()) {
        std::cout << "(belum ada transaksi tersimpan)\n";
        cetakGaris();
        return;
    }

    const std::size_t mulai = (jumlahTerakhir == 0 || jumlahTerakhir >= riwayat.size())
                                  ? 0
                                  : riwayat.size() - jumlahTerakhir;
    std::cout << std::left << std::setw(20) << "Nomor" << std::setw(21) << "Waktu"
              << std::setw(12) << "Kasir" << std::right << std::setw(LEBAR_UANG) << "Total" << '\n';
    for (std::size_t indeks = mulai; indeks < riwayat.size(); ++indeks) {
        const Transaksi& transaksi = riwayat[indeks];
        std::cout << std::left << std::setw(20) << transaksi.nomor << std::setw(21)
                  << transaksi.waktu << std::setw(12) << ringkasTeks(transaksi.kasir, 11)
                  << std::right << std::setw(LEBAR_UANG) << formatRupiah(transaksi.total()) << '\n';
    }
    cetakGaris();
}

}  // namespace kasir
