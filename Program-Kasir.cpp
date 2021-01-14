// Program Mesin Kasir Sederhana
//
// Aplikasi konsol untuk mencatat pembelian, menghitung total, dan mencetak
// struk. Satu berkas untuk semua sistem operasi (Windows / Linux / macOS).
//
// Kompilasi:  g++ -std=c++17 -Wall -Wextra -o kasir Program-Kasir.cpp
// Jalankan :  ./kasir        (Windows: kasir.exe)

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {

// ============================== Konstanta ==============================

// Uang memakai long long karena long hanya 32 bit di Windows.
using Uang = long long;

constexpr int MAKS_PERCOBAAN = 3;
constexpr Uang HARGA_MAKSIMAL = 100000000LL;
constexpr Uang UANG_TAK_TERBATAS = std::numeric_limits<Uang>::max();
constexpr int JUMLAH_BELI_MAKSIMAL = 1000;

// ============================== Tipe Data ==============================

// Dilempar saat aliran input tertutup (Ctrl+Z / Ctrl+D) supaya program
// berhenti dengan rapi, bukan terjebak dalam perulangan pembacaan input.
struct InputBerakhir {};

struct Barang {
    std::string nama;
    Uang harga = 0;
};

// Satu baris pada struk: barang yang dibeli beserta jumlahnya.
struct BarisStruk {
    std::string nama;
    Uang hargaSatuan = 0;
    int jumlah = 0;

    Uang subtotal() const { return hargaSatuan * jumlah; }
};

struct Akun {
    std::string username = "admin";
    std::string password = "admin";
};

struct Toko {
    Akun akun;
    std::vector<Barang> katalog{
        {"Buku", 5000},
        {"Bolpoin", 2000},
        {"Tipe-X", 1000},
        {"Pensil", 1000},
    };
};

// Menandakan apa yang harus dilakukan pemanggil setelah sebuah alur selesai.
enum class Lanjut { KeMenuUtama, KeluarProgram };

enum class AksiKasir { TambahBarang, Bayar, MenuUtama, KeluarProgram };

// =============================== Tampilan ==============================

void bersihkanLayar() {
#ifdef _WIN32
    static_cast<void>(std::system("cls"));
#else
    static_cast<void>(std::system("clear"));
#endif
}

std::string rupiah(Uang nilai) { return "Rp. " + std::to_string(nilai); }

// ================================ Input ================================

void abaikanSisaBaris() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Membuang input yang salah agar bisa dibaca ulang. Kegagalan karena input
// tertutup atau rusak tidak bisa dipulihkan, jadi program dihentikan.
void pulihkanInputAtauBerhenti() {
    if (std::cin.eof() || std::cin.bad()) {
        throw InputBerakhir{};
    }
    std::cin.clear();
    abaikanSisaBaris();
}

Uang bacaBilangan(const std::string& prompt, Uang minimum, Uang maksimum = UANG_TAK_TERBATAS) {
    for (;;) {
        std::cout << prompt;
        Uang nilai = 0;
        if (!(std::cin >> nilai)) {
            pulihkanInputAtauBerhenti();
            std::cout << "Input harus berupa angka.\n";
            continue;
        }
        abaikanSisaBaris();
        if (minimum <= nilai && nilai <= maksimum) {
            return nilai;
        }
        std::cout << "Masukkan angka antara " << minimum << " sampai " << maksimum << ".\n";
    }
}

std::string bacaKata(const std::string& prompt) {
    for (;;) {
        std::cout << prompt;
        std::string kata;
        if (std::cin >> kata) {
            abaikanSisaBaris();
            return kata;
        }
        pulihkanInputAtauBerhenti();
    }
}

bool bacaKonfirmasi(const std::string& pertanyaan) {
    std::cout << pertanyaan << '\n';
    return bacaBilangan("Ketik 1 untuk ya, 0 untuk tidak = ", 0, 1) == 1;
}

void tekanEnterUntukLanjut() {
    std::cout << "\nTekan Enter untuk melanjutkan...";
    abaikanSisaBaris();
}

// ============================== Perhitungan ============================

// Total belanja dihitung secara rekursif, sesuai rancangan awal program.
Uang hitungTotal(const std::vector<BarisStruk>& keranjang, std::size_t banyakBaris) {
    if (banyakBaris == 0) {
        return 0;
    }
    return keranjang[banyakBaris - 1].subtotal() + hitungTotal(keranjang, banyakBaris - 1);
}

Uang hitungTotal(const std::vector<BarisStruk>& keranjang) {
    return hitungTotal(keranjang, keranjang.size());
}

// ================================ Login ================================

bool login(const Akun& akun) {
    std::cout << "Masukan Username dan Password untuk melanjutkan\n";
    for (int percobaan = 1; percobaan <= MAKS_PERCOBAAN; ++percobaan) {
        const std::string username = bacaKata("Username : ");
        const std::string password = bacaKata("Password : ");
        if (username == akun.username && password == akun.password) {
            return true;
        }
        std::cout << "Username atau password salah (percobaan " << percobaan << " dari "
                  << MAKS_PERCOBAAN << ").\n";
    }
    return false;
}

// =============================== Katalog ===============================

void tampilkanKatalog(const std::vector<Barang>& katalog) {
    std::cout << "================= Database Barang =================\n";
    std::cout << std::left << std::setw(14) << "Kode Barang" << std::setw(22) << "Nama Barang"
              << "Harga Barang\n";
    for (std::size_t indeks = 0; indeks < katalog.size(); ++indeks) {
        std::cout << std::left << std::setw(14) << (std::to_string(indeks + 1) + ".")
                  << std::setw(22) << katalog[indeks].nama << rupiah(katalog[indeks].harga) << '\n';
    }
    std::cout << "===================================================\n";
}

void tambahBarangKeKatalog(std::vector<Barang>& katalog) {
    do {
        bersihkanLayar();
        std::cout << "=== Menambah Database Barang ===\n";
        tampilkanKatalog(katalog);

        Barang barang;
        std::cout << "Menambah Kode Barang ke " << katalog.size() + 1 << '\n';
        barang.nama = bacaKata("Masukkan Nama Barang (tanpa spasi) = ");
        barang.harga =
            bacaBilangan("Masukkan Harga Barang " + barang.nama + " = ", 0, HARGA_MAKSIMAL);
        katalog.push_back(barang);

        std::cout << "Selamat! " << barang.nama << " telah terdaftar di database!\n";
    } while (bacaKonfirmasi("Tambah barang lagi?"));
}

// ============================== Transaksi ==============================

void tambahBarangKeKeranjang(const std::vector<Barang>& katalog,
                             std::vector<BarisStruk>& keranjang) {
    const Uang kode = bacaBilangan("Masukkan kode barang = ", 1, static_cast<Uang>(katalog.size()));
    const Barang& barang = katalog[static_cast<std::size_t>(kode) - 1];

    const Uang jumlah = bacaBilangan("Masukkan jumlah barang '" + barang.nama + "' = ", 1,
                                     JUMLAH_BELI_MAKSIMAL);
    keranjang.push_back({barang.nama, barang.harga, static_cast<int>(jumlah)});

    std::cout << "Harga Total = " << rupiah(hitungTotal(keranjang)) << "\n\n";
}

AksiKasir pilihAksiKasir() {
    std::cout << "0  = tambah barang\n"
                 "1  = konfirmasi pembayaran\n"
                 "99 = kembali ke menu utama\n"
                 "98 = keluar dari program\n";
    for (;;) {
        switch (bacaBilangan("Masukkan nomor = ", 0, 99)) {
            case 0:
                return AksiKasir::TambahBarang;
            case 1:
                return AksiKasir::Bayar;
            case 99:
                return AksiKasir::MenuUtama;
            case 98:
                return AksiKasir::KeluarProgram;
            default:
                std::cout << "Nomor tidak dikenal, pilih 0, 1, 98, atau 99.\n";
        }
    }
}

Lanjut pilihAksiSetelahStruk() {
    std::cout << "99 = kembali ke menu utama\n"
                 "98 = keluar dari program\n";
    return bacaBilangan("Masukkan nomor = ", 98, 99) == 99 ? Lanjut::KeMenuUtama
                                                           : Lanjut::KeluarProgram;
}

// Mengembalikan jumlah uang yang diserahkan pembeli, minimal sebesar total.
Uang mintaPembayaran(Uang total) {
    for (;;) {
        bersihkanLayar();
        std::cout << "============ Konfirmasi Pembayaran ============\n";
        std::cout << "Total Harga = " << rupiah(total) << '\n';

        const Uang uangPembeli = bacaBilangan("Masukkan uang = Rp. ", total);
        std::cout << "Uang Kembalian = " << rupiah(uangPembeli - total) << '\n';

        if (bacaKonfirmasi("Lanjutkan pembayaran?")) {
            return uangPembeli;
        }
    }
}

void tampilkanStruk(const std::string& kasir, const std::vector<BarisStruk>& keranjang,
                    Uang uangPembeli) {
    const Uang total = hitungTotal(keranjang);

    bersihkanLayar();
    std::cout << "==================== STRUK BARANG ====================\n";
    std::cout << std::left << std::setw(5) << "No" << std::setw(20) << "Nama Barang" << std::right
              << std::setw(8) << "Jumlah" << std::setw(18) << "Harga/pcs" << '\n';
    for (std::size_t indeks = 0; indeks < keranjang.size(); ++indeks) {
        const BarisStruk& baris = keranjang[indeks];
        std::cout << std::left << std::setw(5) << (indeks + 1) << std::setw(20) << baris.nama
                  << std::right << std::setw(8) << baris.jumlah << std::setw(18)
                  << rupiah(baris.hargaSatuan) << '\n';
    }
    std::cout << "=====================================================\n";
    std::cout << "Kasir          = " << kasir << '\n';
    std::cout << "Harga Total    = " << rupiah(total) << '\n';
    std::cout << "Uang Pembeli   = " << rupiah(uangPembeli) << '\n';
    std::cout << "Uang Kembalian = " << rupiah(uangPembeli - total) << "\n\n";
}

Lanjut jalankanProgramKasir(const Toko& toko) {
    std::vector<BarisStruk> keranjang;
    for (;;) {
        bersihkanLayar();
        std::cout << "== Program Kasir ==\n";
        tampilkanKatalog(toko.katalog);
        tambahBarangKeKeranjang(toko.katalog, keranjang);

        switch (pilihAksiKasir()) {
            case AksiKasir::TambahBarang:
                break;  // ulangi perulangan untuk barang berikutnya
            case AksiKasir::Bayar: {
                const Uang uangPembeli = mintaPembayaran(hitungTotal(keranjang));
                tampilkanStruk(toko.akun.username, keranjang, uangPembeli);
                return pilihAksiSetelahStruk();
            }
            case AksiKasir::MenuUtama:
                return Lanjut::KeMenuUtama;
            case AksiKasir::KeluarProgram:
                return Lanjut::KeluarProgram;
        }
    }
}

// ============================== Ganti Akun =============================

bool verifikasiPassword(const Akun& akun) {
    std::cout << "=========== Verifikasi ===========\n";
    std::cout << "Hai " << akun.username << "!\n";
    std::cout << "Masukkan Password untuk verifikasi\n";
    for (int percobaan = 1; percobaan <= MAKS_PERCOBAAN; ++percobaan) {
        if (bacaKata("Password = ") == akun.password) {
            return true;
        }
        std::cout << "Password salah (percobaan " << percobaan << " dari " << MAKS_PERCOBAAN
                  << ").\n";
    }
    return false;
}

// Meminta nilai baru dua kali sampai kedua isian sama.
std::string bacaNilaiBaru(const std::string& prompt, const std::string& promptUlang) {
    for (;;) {
        const std::string nilai = bacaKata(prompt);
        if (nilai == bacaKata(promptUlang)) {
            return nilai;
        }
        std::cout << "Isian tidak sama, silakan ulangi.\n";
    }
}

void gantiUsernameDanPassword(Akun& akun) {
    bersihkanLayar();
    if (!verifikasiPassword(akun)) {
        std::cout << "Verifikasi gagal, kembali ke menu utama.\n";
        tekanEnterUntukLanjut();
        return;
    }

    std::cout << "\n====== Mengganti Username/Password ======\n";
    akun.username = bacaNilaiBaru("Masukkan Username Baru = ", "Masukkan ulang username baru = ");
    akun.password = bacaNilaiBaru("Masukkan Password Baru = ", "Masukkan ulang password baru = ");

    std::cout << "Selamat " << akun.username << ", username/password berhasil diubah.\n";
    tekanEnterUntukLanjut();
}

// ============================== Menu Utama =============================

void tampilkanMenuUtama(const std::string& username) {
    std::cout << "Selamat Datang " << username << '\n';
    std::cout << "==================== Menu ====================\n";
    std::cout << "1. Program Kasir\n";
    std::cout << "2. Tambahkan Database Barang\n";
    std::cout << "3. Ganti Username/Password\n";
    std::cout << "4. Exit Program\n";
    std::cout << "Masukkan kode angka yang sesuai lalu tekan enter\n";
}

void jalankanMenuUtama(Toko& toko) {
    for (;;) {
        bersihkanLayar();
        tampilkanMenuUtama(toko.akun.username);
        switch (bacaBilangan("Kode Angka : ", 1, 4)) {
            case 1:
                if (jalankanProgramKasir(toko) == Lanjut::KeluarProgram) {
                    return;
                }
                break;
            case 2:
                tambahBarangKeKatalog(toko.katalog);
                break;
            case 3:
                gantiUsernameDanPassword(toko.akun);
                break;
            default:  // pilihan 4, satu-satunya sisa yang diizinkan bacaBilangan
                return;
        }
    }
}

}  // namespace

int main() {
    Toko toko;
    try {
        bersihkanLayar();
        std::cout << "===================\n"
                     "PROGRAM MESIN KASIR\n"
                     "===================\n";
        if (!login(toko.akun)) {
            std::cout << "Login gagal, program berhenti.\n";
            return 1;
        }
        jalankanMenuUtama(toko);
        std::cout << "Selamat tinggal, " << toko.akun.username << "!\n";
    } catch (const InputBerakhir&) {
        std::cout << "\nInput berakhir, program berhenti.\n";
        return 1;
    }
    return 0;
}
