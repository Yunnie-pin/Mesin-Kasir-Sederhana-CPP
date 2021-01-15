#include "app/aplikasi.h"

#include <cstddef>
#include <functional>
#include <iostream>
#include <limits>

#include "domain/laporan.h"
#include "domain/waktu.h"
#include "io/input.h"
#include "io/konsol.h"
#include "io/tampilan.h"

namespace kasir {
namespace {

constexpr int MAKS_PERCOBAAN = 3;
constexpr Uang HARGA_MAKSIMAL = 100000000;  // Rp. 100.000.000 per barang
constexpr Uang STOK_MAKSIMAL = 1000000;
constexpr Uang KODE_MAKSIMAL = std::numeric_limits<int>::max();
constexpr std::size_t RIWAYAT_TERAKHIR = 10;

void pesan(const std::string& teks) {
    std::cout << teks << '\n';
    tekanEnterUntukLanjut();
}

}  // namespace

Aplikasi::Aplikasi()
    : katalog_(muatKatalog(berkas_.barang)),
      akun_(muatAkun(berkas_.akun)),
      riwayat_(muatRiwayat(berkas_.transaksi)) {
    // Pemakaian pertama: tuliskan data bawaan supaya pengguna bisa melihat dan
    // menyunting berkasnya.
    if (!berkasAda(berkas_.barang)) {
        simpanKatalog(berkas_.barang, katalog_);
    }
    if (!berkasAda(berkas_.akun)) {
        simpanAkun(berkas_.akun, akun_);
    }
}

int Aplikasi::jalankan() {
    try {
        bersihkanLayar();
        tampilkanJudulProgram();
        if (!login()) {
            std::cout << "Login gagal, program berhenti.\n";
            return 1;
        }
        jalankanMenuUtama();
        std::cout << "Selamat tinggal, " << usernameAktif_ << "!\n";
    } catch (const InputBerakhir&) {
        std::cout << "\nInput berakhir, program berhenti.\n";
        return 1;
    }
    return 0;
}

bool Aplikasi::login() {
    std::cout << "Masukan Username dan Password untuk melanjutkan\n";
    for (int percobaan = 1; percobaan <= MAKS_PERCOBAAN; ++percobaan) {
        const std::string username = bacaKata("Username : ");
        const std::string password = bacaPassword("Password : ");

        const Akun* akun = akun_.cari(username);
        if (akun != nullptr && akun->password == password) {
            usernameAktif_ = akun->username;
            peranAktif_ = akun->peran;
            return true;
        }
        std::cout << "Username atau password salah (percobaan " << percobaan << " dari "
                  << MAKS_PERCOBAAN << ").\n";
    }
    return false;
}

void Aplikasi::jalankanMenuUtama() {
    struct ItemMenu {
        std::string label;
        std::function<Lanjut()> aksi;
    };

    // Menu dibangun sesuai peran, sehingga nomor yang tampil selalu sama dengan
    // nomor yang diterima dan kasir tidak bisa membuka menu admin.
    std::vector<ItemMenu> menu;
    menu.push_back({"Program Kasir", [this] { return menuProgramKasir(); }});
    if (peranAktif_ == Peran::Admin) {
        menu.push_back({"Kelola Database Barang", [this] { return menuKelolaBarang(); }});
        menu.push_back({"Laporan Penjualan", [this] { return menuLaporan(); }});
    }
    menu.push_back({"Ganti Username/Password", [this] { return menuGantiKredensial(); }});
    menu.push_back({"Exit Program", [] { return Lanjut::KeluarProgram; }});

    for (;;) {
        bersihkanLayar();
        std::cout << "Selamat Datang " << usernameAktif_ << " (" << namaPeran(peranAktif_) << ")\n";
        std::cout << garis('=', 46) << '\n';
        for (std::size_t indeks = 0; indeks < menu.size(); ++indeks) {
            std::cout << indeks + 1 << ". " << menu[indeks].label << '\n';
        }
        std::cout << garis('=', 46) << '\n';

        const Uang pilihan = bacaBilangan("Kode Angka : ", 1, static_cast<Uang>(menu.size()));
        if (menu[static_cast<std::size_t>(pilihan) - 1].aksi() == Lanjut::KeluarProgram) {
            return;
        }
    }
}

// ============================== Program kasir =============================

Aplikasi::Lanjut Aplikasi::menuProgramKasir() {
    if (katalog_.kosong()) {
        pesan("Database barang masih kosong, tidak ada yang bisa dijual.");
        return Lanjut::KeMenuUtama;
    }

    Keranjang keranjang;
    for (;;) {
        bersihkanLayar();
        std::cout << "== PROGRAM KASIR ==\n";
        tampilkanKatalog(katalog_);
        tampilkanKeranjang(keranjang);
        std::cout << "0  = tambah barang\n"
                     "1  = konfirmasi pembayaran\n"
                     "2  = hapus barang dari keranjang\n"
                     "99 = kembali ke menu utama\n"
                     "98 = keluar dari program\n";

        switch (bacaBilangan("Masukkan nomor = ", 0, 99)) {
            case 0:
                tambahKeKeranjang(keranjang);
                break;
            case 1:
                if (keranjang.kosong()) {
                    pesan("Keranjang masih kosong, tambahkan barang lebih dulu.");
                    break;
                }
                selesaikanTransaksi(keranjang);
                return pilihAksiSetelahStruk();
            case 2:
                hapusDariKeranjang(keranjang);
                break;
            case 99:
                return Lanjut::KeMenuUtama;
            case 98:
                return Lanjut::KeluarProgram;
            default:
                pesan("Nomor tidak dikenal, pilih 0, 1, 2, 98, atau 99.");
                break;
        }
    }
}

void Aplikasi::tambahKeKeranjang(Keranjang& keranjang) {
    const Barang* barang = pilihBarang("Masukkan kode barang (0 = batal) = ");
    if (barang == nullptr) {
        return;
    }

    const int sisaStok = barang->stok - keranjang.jumlahDibeli(barang->kode);
    if (sisaStok <= 0) {
        pesan("Stok " + barang->nama + " tidak mencukupi lagi.");
        return;
    }

    const Uang jumlah = bacaBilangan("Masukkan jumlah barang '" + barang->nama + "' (maks " +
                                         std::to_string(sisaStok) + ") = ",
                                     1, sisaStok);
    keranjang.tambah(*barang, static_cast<int>(jumlah));
}

void Aplikasi::hapusDariKeranjang(Keranjang& keranjang) {
    if (keranjang.kosong()) {
        pesan("Keranjang masih kosong.");
        return;
    }
    const Uang nomorBaris = bacaBilangan("Nomor baris yang dihapus (0 = batal) = ", 0,
                                         static_cast<Uang>(keranjang.jumlahBaris()));
    if (nomorBaris == 0) {
        return;
    }
    keranjang.hapusBaris(static_cast<std::size_t>(nomorBaris));
}

Uang Aplikasi::mintaPembayaran(Uang total) {
    for (;;) {
        bersihkanLayar();
        std::cout << garis('=', 46) << '\n';
        std::cout << "KONFIRMASI PEMBAYARAN\n";
        std::cout << garis('=', 46) << '\n';
        std::cout << "Total Harga = " << formatRupiah(total) << '\n';

        const Uang uangPembeli = bacaBilangan("Masukkan uang = Rp. ", total);
        std::cout << "Uang Kembalian = " << formatRupiah(uangPembeli - total) << '\n';

        if (bacaKonfirmasi("Lanjutkan pembayaran?")) {
            return uangPembeli;
        }
    }
}

void Aplikasi::selesaikanTransaksi(const Keranjang& keranjang) {
    const Uang uangPembeli = mintaPembayaran(keranjang.total());

    Transaksi transaksi;
    transaksi.waktu = waktuSekarang();
    transaksi.nomor = buatNomorTransaksi(riwayat_, transaksi.waktu);
    transaksi.kasir = usernameAktif_;
    transaksi.baris = keranjang.baris();
    transaksi.uangPembeli = uangPembeli;

    bool stokTerpotong = true;
    for (const BarisKeranjang& baris : transaksi.baris) {
        stokTerpotong = katalog_.kurangiStok(baris.kodeBarang, baris.jumlah) && stokTerpotong;
    }
    riwayat_.push_back(transaksi);

    simpanKatalogAtauPeringatkan();
    if (!tambahkanTransaksi(berkas_.transaksi, transaksi)) {
        std::cout << "PERINGATAN: transaksi gagal disimpan ke " << berkas_.transaksi << ".\n";
    }

    tampilkanStruk(transaksi);
    if (!stokTerpotong) {
        std::cout << "PERINGATAN: stok sebagian barang tidak sesuai, periksa database.\n";
    }
}

Aplikasi::Lanjut Aplikasi::pilihAksiSetelahStruk() {
    std::cout << "\n99 = kembali ke menu utama\n"
                 "98 = keluar dari program\n";
    return bacaBilangan("Masukkan nomor = ", 98, 99) == 99 ? Lanjut::KeMenuUtama
                                                           : Lanjut::KeluarProgram;
}

// =========================== Kelola database ==============================

const Barang* Aplikasi::pilihBarang(const std::string& prompt) {
    const Uang kode = bacaBilangan(prompt, 0, KODE_MAKSIMAL);
    if (kode == 0) {
        return nullptr;
    }
    const Barang* barang = katalog_.cari(static_cast<int>(kode));
    if (barang == nullptr) {
        pesan("Kode barang tidak ada di database.");
    }
    return barang;
}

Aplikasi::Lanjut Aplikasi::menuKelolaBarang() {
    for (;;) {
        bersihkanLayar();
        tampilkanKatalog(katalog_);
        std::cout << "1. Tambah barang\n"
                     "2. Ubah barang (nama/harga/stok)\n"
                     "3. Hapus barang\n"
                     "4. Kembali ke menu utama\n";

        switch (bacaBilangan("Pilihan : ", 1, 4)) {
            case 1:
                tambahBarangBaru();
                break;
            case 2:
                ubahBarang();
                break;
            case 3:
                hapusBarang();
                break;
            default:
                return Lanjut::KeMenuUtama;
        }
    }
}

void Aplikasi::tambahBarangBaru() {
    std::cout << "\n=== Menambah Barang (kode " << katalog_.kodeBerikutnya() << ") ===\n";
    const std::string nama = bacaBaris("Nama barang = ");
    if (katalog_.namaDipakai(nama)) {
        pesan("Nama '" + nama + "' sudah ada di database.");
        return;
    }
    const Uang harga = bacaBilangan("Harga barang = ", 0, HARGA_MAKSIMAL);
    const Uang stok = bacaBilangan("Stok awal = ", 0, STOK_MAKSIMAL);

    katalog_.tambah(nama, harga, static_cast<int>(stok));
    simpanKatalogAtauPeringatkan();
    pesan("Selamat! " + nama + " telah terdaftar di database.");
}

void Aplikasi::ubahBarang() {
    const Barang* barang = pilihBarang("Kode barang yang diubah (0 = batal) = ");
    if (barang == nullptr) {
        return;
    }
    const int kode = barang->kode;

    std::cout << "Nilai sekarang: " << barang->nama << ", " << formatRupiah(barang->harga)
              << ", stok " << barang->stok << '\n';
    const std::string nama = bacaBaris("Nama baru = ");
    if (katalog_.namaDipakai(nama, kode)) {
        pesan("Nama '" + nama + "' sudah dipakai barang lain.");
        return;
    }
    const Uang harga = bacaBilangan("Harga baru = ", 0, HARGA_MAKSIMAL);
    const Uang stok = bacaBilangan("Stok baru = ", 0, STOK_MAKSIMAL);

    if (!katalog_.ubah(kode, nama, harga, static_cast<int>(stok))) {
        pesan("Perubahan gagal disimpan.");
        return;
    }
    simpanKatalogAtauPeringatkan();
    pesan("Barang berhasil diperbarui.");
}

void Aplikasi::hapusBarang() {
    const Barang* barang = pilihBarang("Kode barang yang dihapus (0 = batal) = ");
    if (barang == nullptr) {
        return;
    }
    const int kode = barang->kode;
    const std::string nama = barang->nama;

    if (!bacaKonfirmasi("Hapus '" + nama + "' dari database?")) {
        return;
    }
    katalog_.hapus(kode);
    simpanKatalogAtauPeringatkan();
    pesan("'" + nama + "' telah dihapus. Struk lama tetap menyimpan namanya.");
}

// ================================ Laporan =================================

Aplikasi::Lanjut Aplikasi::menuLaporan() {
    for (;;) {
        bersihkanLayar();
        std::cout << "=== Laporan Penjualan ===\n"
                     "1. Hari ini\n"
                     "2. Tanggal tertentu\n"
                     "3. Seluruh riwayat\n"
                     "4. Transaksi terakhir\n"
                     "5. Kembali ke menu utama\n";

        switch (bacaBilangan("Pilihan : ", 1, 5)) {
            case 1:
                bersihkanLayar();
                tampilkanLaporan(buatLaporan(riwayat_, tanggalHariIni()));
                tekanEnterUntukLanjut();
                break;
            case 2: {
                const std::string tanggal = bacaKata("Tanggal (YYYY-MM-DD) = ");
                if (!tanggalValid(tanggal)) {
                    pesan("Format tanggal harus YYYY-MM-DD, contoh " + tanggalHariIni() + ".");
                    break;
                }
                bersihkanLayar();
                tampilkanLaporan(buatLaporan(riwayat_, tanggal));
                tekanEnterUntukLanjut();
                break;
            }
            case 3:
                bersihkanLayar();
                tampilkanLaporan(buatLaporan(riwayat_, ""));
                tekanEnterUntukLanjut();
                break;
            case 4:
                bersihkanLayar();
                tampilkanRiwayat(riwayat_, RIWAYAT_TERAKHIR);
                tekanEnterUntukLanjut();
                break;
            default:
                return Lanjut::KeMenuUtama;
        }
    }
}

// ============================== Ganti akun ================================

Aplikasi::Lanjut Aplikasi::menuGantiKredensial() {
    bersihkanLayar();
    const Akun* akun = akun_.cari(usernameAktif_);
    if (akun == nullptr) {
        pesan("Akun yang sedang dipakai tidak ditemukan di database akun.");
        return Lanjut::KeMenuUtama;
    }

    std::cout << "=== Verifikasi ===\n";
    std::cout << "Hai " << usernameAktif_ << "!\n";
    bool terverifikasi = false;
    for (int percobaan = 1; percobaan <= MAKS_PERCOBAAN; ++percobaan) {
        if (bacaPassword("Password sekarang = ") == akun->password) {
            terverifikasi = true;
            break;
        }
        std::cout << "Password salah (percobaan " << percobaan << " dari " << MAKS_PERCOBAAN
                  << ").\n";
    }
    if (!terverifikasi) {
        pesan("Verifikasi gagal, kembali ke menu utama.");
        return Lanjut::KeMenuUtama;
    }

    std::cout << "\n=== Mengganti Username/Password ===\n";
    const std::string usernameBaru =
        bacaNilaiBaru("Username baru = ", "Ulangi username baru = ", ModeInput::Tampak);
    const std::string passwordBaru =
        bacaNilaiBaru("Password baru = ", "Ulangi password baru = ", ModeInput::Tersembunyi);

    if (!akun_.ubahKredensial(usernameAktif_, usernameBaru, passwordBaru)) {
        pesan("Gagal: username '" + usernameBaru + "' sudah dipakai akun lain.");
        return Lanjut::KeMenuUtama;
    }
    usernameAktif_ = usernameBaru;
    simpanAkunAtauPeringatkan();
    pesan("Selamat " + usernameAktif_ + ", username/password berhasil diubah.");
    return Lanjut::KeMenuUtama;
}

// ============================== Penyimpanan ===============================

void Aplikasi::simpanKatalogAtauPeringatkan() {
    if (!simpanKatalog(berkas_.barang, katalog_)) {
        std::cout << "PERINGATAN: database barang gagal disimpan ke " << berkas_.barang << ".\n";
    }
}

void Aplikasi::simpanAkunAtauPeringatkan() {
    if (!simpanAkun(berkas_.akun, akun_)) {
        std::cout << "PERINGATAN: data akun gagal disimpan ke " << berkas_.akun << ".\n";
    }
}

}  // namespace kasir
