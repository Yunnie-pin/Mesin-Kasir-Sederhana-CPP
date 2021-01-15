// Uji untuk logika yang tidak butuh konsol: format uang, keranjang, katalog,
// transaksi, laporan, CSV, dan penyimpanan.

#include <cstdio>
#include <string>
#include <vector>

#include "domain/akun.h"
#include "domain/barang.h"
#include "domain/keranjang.h"
#include "domain/laporan.h"
#include "domain/transaksi.h"
#include "domain/uang.h"
#include "io/csv.h"
#include "io/penyimpanan.h"
#include "uji.h"

namespace {

using namespace kasir;

Barang buatBarang(int kode, const std::string& nama, Uang harga, int stok) {
    Barang barang;
    barang.kode = kode;
    barang.nama = nama;
    barang.harga = harga;
    barang.stok = stok;
    return barang;
}

BarisKeranjang buatBaris(int kode, const std::string& nama, Uang harga, int jumlah) {
    BarisKeranjang baris;
    baris.kodeBarang = kode;
    baris.nama = nama;
    baris.hargaSatuan = harga;
    baris.jumlah = jumlah;
    return baris;
}

void ujiFormatUang() {
    CEK(formatAngkaRibuan(0) == "0");
    CEK(formatAngkaRibuan(999) == "999");
    CEK(formatAngkaRibuan(1000) == "1.000");
    CEK(formatAngkaRibuan(5000) == "5.000");
    CEK(formatAngkaRibuan(1000000) == "1.000.000");
    CEK(formatAngkaRibuan(-2500) == "-2.500");
    CEK(formatRupiah(15000) == "Rp. 15.000");
}

void ujiKeranjang() {
    const Barang buku = buatBarang(1, "Buku", 5000, 10);
    const Barang pensil = buatBarang(2, "Pensil", 1000, 10);

    Keranjang keranjang;
    CEK(keranjang.kosong());
    CEK(keranjang.total() == 0);

    // Barang yang sama harus digabung menjadi satu baris, bukan dua.
    keranjang.tambah(buku, 2);
    keranjang.tambah(buku, 3);
    CEK(keranjang.jumlahBaris() == 1);
    CEK(keranjang.jumlahDibeli(1) == 5);
    CEK(keranjang.total() == 25000);

    keranjang.tambah(pensil, 4);
    CEK(keranjang.jumlahBaris() == 2);
    CEK(keranjang.total() == 29000);

    // Jumlah tidak masuk akal diabaikan.
    keranjang.tambah(pensil, 0);
    keranjang.tambah(pensil, -5);
    CEK(keranjang.jumlahDibeli(2) == 4);

    // Nomor baris mengikuti tampilan, dimulai dari 1.
    CEK(!keranjang.hapusBaris(0));
    CEK(!keranjang.hapusBaris(3));
    CEK(keranjang.hapusBaris(1));
    CEK(keranjang.jumlahBaris() == 1);
    CEK(keranjang.total() == 4000);
    CEK(keranjang.jumlahDibeli(1) == 0);
}

void ujiKatalog() {
    Katalog katalog;
    const int kodeBuku = katalog.tambah("Buku", 5000, 10);
    const int kodePensil = katalog.tambah("Pensil", 1000, 3);
    CEK(kodeBuku == 1);
    CEK(kodePensil == 2);
    CEK(katalog.jumlah() == 2);

    CEK(katalog.namaDipakai("buku"));  // tanpa membedakan huruf besar/kecil
    CEK(!katalog.namaDipakai("Buku", kodeBuku));
    CEK(!katalog.namaDipakai("Tipe-X"));

    CEK(katalog.kurangiStok(kodePensil, 3));
    CEK(katalog.cari(kodePensil)->stok == 0);
    CEK(!katalog.kurangiStok(kodePensil, 1));  // stok tidak boleh minus
    CEK(!katalog.kurangiStok(99, 1));          // kode tidak ada

    // Kode tidak dipakai ulang walau barang di tengah dihapus.
    CEK(katalog.hapus(kodeBuku));
    CEK(!katalog.hapus(kodeBuku));
    CEK(katalog.cari(kodeBuku) == nullptr);
    CEK(katalog.kodeBerikutnya() == 3);
    CEK(katalog.tambah("Tipe-X", 2000, 5) == 3);

    CEK(katalog.ubah(kodePensil, "Pensil 2B", 1500, 7));
    CEK(katalog.cari(kodePensil)->nama == "Pensil 2B");
    CEK(katalog.cari(kodePensil)->harga == 1500);
    CEK(!katalog.ubah(99, "Apa Saja", 1000, 1));
    CEK(!katalog.ubah(kodePensil, "Pensil 2B", -1, 1));  // harga minus ditolak

    CEK(!katalog.tambahDenganKode(buatBarang(3, "Kode Kembar", 1000, 1)));
    CEK(!katalog.tambahDenganKode(buatBarang(0, "Kode Nol", 1000, 1)));
    CEK(katalog.tambahDenganKode(buatBarang(10, "Penghapus", 500, 4)));
    CEK(katalog.kodeBerikutnya() == 11);
}

void ujiTransaksi() {
    Transaksi transaksi;
    transaksi.waktu = "2026-08-17 10:00:00";
    transaksi.baris.push_back(buatBaris(1, "Buku", 5000, 2));
    transaksi.baris.push_back(buatBaris(2, "Pensil", 1000, 3));
    transaksi.uangPembeli = 20000;

    CEK(transaksi.total() == 13000);
    CEK(transaksi.kembalian() == 7000);
    CEK(transaksi.tanggal() == "2026-08-17");

    std::vector<Transaksi> riwayat;
    CEK(buatNomorTransaksi(riwayat, "2026-08-17 10:00:00") == "TRX-20260817-0001");

    transaksi.nomor = "TRX-20260817-0001";
    riwayat.push_back(transaksi);
    CEK(buatNomorTransaksi(riwayat, "2026-08-17 11:00:00") == "TRX-20260817-0002");
    // Nomor mulai dari satu lagi pada hari berikutnya.
    CEK(buatNomorTransaksi(riwayat, "2026-08-18 09:00:00") == "TRX-20260818-0001");
}

void ujiLaporan() {
    Transaksi pertama;
    pertama.nomor = "TRX-20260817-0001";
    pertama.waktu = "2026-08-17 10:00:00";
    pertama.baris.push_back(buatBaris(1, "Buku", 5000, 2));
    pertama.baris.push_back(buatBaris(2, "Pensil", 1000, 1));

    Transaksi kedua;
    kedua.nomor = "TRX-20260817-0002";
    kedua.waktu = "2026-08-17 12:00:00";
    kedua.baris.push_back(buatBaris(2, "Pensil", 1000, 5));

    Transaksi hariLain;
    hariLain.nomor = "TRX-20260818-0001";
    hariLain.waktu = "2026-08-18 08:00:00";
    hariLain.baris.push_back(buatBaris(1, "Buku", 5000, 10));

    const std::vector<Transaksi> riwayat{pertama, kedua, hariLain};

    const Laporan harian = buatLaporan(riwayat, "2026-08-17");
    CEK(harian.jumlahTransaksi == 2);
    CEK(harian.omzet == 16000);
    CEK(harian.perBarang.size() == 2);
    CEK(harian.perBarang[0].nama == "Pensil");  // 6 pcs, terlaris
    CEK(harian.perBarang[0].jumlahTerjual == 6);
    CEK(harian.perBarang[0].omzet == 6000);
    CEK(harian.perBarang[1].nama == "Buku");

    const Laporan seluruh = buatLaporan(riwayat, "");
    CEK(seluruh.jumlahTransaksi == 3);
    CEK(seluruh.omzet == 66000);
    CEK(seluruh.perBarang[0].nama == "Buku");  // 12 pcs

    const Laporan kosong = buatLaporan(riwayat, "2026-01-01");
    CEK(kosong.jumlahTransaksi == 0);
    CEK(kosong.omzet == 0);
    CEK(kosong.perBarang.empty());
}

void ujiCsv() {
    CEK(gabungBarisCsv({"1", "Buku", "5000"}) == "1,Buku,5000");
    CEK(gabungBarisCsv({"Buku Tulis Sinar Dunia"}) == "Buku Tulis Sinar Dunia");
    CEK(gabungBarisCsv({"Pensil, 2B"}) == "\"Pensil, 2B\"");
    CEK(gabungBarisCsv({"Buku 10\""}) == "\"Buku 10\"\"\"");

    const std::vector<std::string> kolom = pecahBarisCsv("1,Buku Tulis,5000,10");
    CEK(kolom.size() == 4);
    CEK(kolom[1] == "Buku Tulis");
    CEK(kolom[3] == "10");

    // Bolak-balik: hasil gabung harus bisa dipecah kembali menjadi asalnya.
    const std::vector<std::string> asal{"Pensil, 2B", "Buku 10\" \"besar\"", "biasa"};
    const std::vector<std::string> hasil = pecahBarisCsv(gabungBarisCsv(asal));
    CEK(hasil == asal);

    long long angka = 0;
    CEK(keBilangan("1234", angka) && angka == 1234);
    CEK(keBilangan("-50", angka) && angka == -50);
    CEK(!keBilangan("kode", angka));    // baris judul CSV ditolak
    CEK(!keBilangan("12abc", angka));
    CEK(!keBilangan("", angka));
    CEK(!keBilangan("99999999999999999999999", angka));
}

void ujiPenyimpanan() {
    const std::string berkasBarang = "uji_barang.csv";
    const std::string berkasAkunUji = "uji_akun.csv";
    const std::string berkasTransaksi = "uji_transaksi.csv";
    std::remove(berkasBarang.c_str());
    std::remove(berkasAkunUji.c_str());
    std::remove(berkasTransaksi.c_str());

    // Berkas yang belum ada menghasilkan data bawaan, bukan data kosong.
    CEK(!berkasAda(berkasBarang));
    CEK(muatKatalog(berkasBarang).jumlah() == 4);
    CEK(muatAkun(berkasAkunUji).jumlah() == 2);
    CEK(muatRiwayat(berkasTransaksi).empty());

    Katalog katalog;
    katalog.tambah("Buku Tulis, 38 lembar", 5500, 12);
    katalog.tambah("Pensil 2B", 1500, 30);
    CEK(simpanKatalog(berkasBarang, katalog));

    const Katalog dimuat = muatKatalog(berkasBarang);
    CEK(dimuat.jumlah() == 2);
    CEK(dimuat.cari(1)->nama == "Buku Tulis, 38 lembar");  // koma tetap utuh
    CEK(dimuat.cari(1)->harga == 5500);
    CEK(dimuat.cari(2)->stok == 30);
    CEK(dimuat.kodeBerikutnya() == 3);

    DaftarAkun daftar;
    daftar.tambah({"budi", "rahasia", Peran::Admin});
    daftar.tambah({"sari", "kunci", Peran::Kasir});
    CEK(!daftar.tambah({"budi", "lain", Peran::Kasir}));  // username kembar
    CEK(simpanAkun(berkasAkunUji, daftar));

    const DaftarAkun akunDimuat = muatAkun(berkasAkunUji);
    CEK(akunDimuat.jumlah() == 2);
    CEK(akunDimuat.cari("budi")->peran == Peran::Admin);
    CEK(akunDimuat.cari("sari")->password == "kunci");
    CEK(akunDimuat.cari("tidak-ada") == nullptr);

    Transaksi transaksi;
    transaksi.nomor = "TRX-20260817-0001";
    transaksi.waktu = "2026-08-17 10:00:00";
    transaksi.kasir = "budi";
    transaksi.uangPembeli = 20000;
    transaksi.baris.push_back(buatBaris(1, "Buku Tulis, 38 lembar", 5500, 2));
    transaksi.baris.push_back(buatBaris(2, "Pensil 2B", 1500, 1));

    Transaksi berikutnya = transaksi;
    berikutnya.nomor = "TRX-20260817-0002";
    berikutnya.waktu = "2026-08-17 11:30:00";
    berikutnya.baris = {buatBaris(2, "Pensil 2B", 1500, 4)};
    berikutnya.uangPembeli = 10000;

    CEK(tambahkanTransaksi(berkasTransaksi, transaksi));
    CEK(tambahkanTransaksi(berkasTransaksi, berikutnya));

    const std::vector<Transaksi> riwayat = muatRiwayat(berkasTransaksi);
    CEK(riwayat.size() == 2);
    CEK(riwayat[0].baris.size() == 2);  // dua baris digabung jadi satu transaksi
    CEK(riwayat[0].total() == 12500);
    CEK(riwayat[0].kembalian() == 7500);
    CEK(riwayat[0].baris[0].nama == "Buku Tulis, 38 lembar");
    CEK(riwayat[1].nomor == "TRX-20260817-0002");
    CEK(riwayat[1].total() == 6000);

    std::remove(berkasBarang.c_str());
    std::remove(berkasAkunUji.c_str());
    std::remove(berkasTransaksi.c_str());
}

}  // namespace

int main() {
    ujiFormatUang();
    ujiKeranjang();
    ujiKatalog();
    ujiTransaksi();
    ujiLaporan();
    ujiCsv();
    ujiPenyimpanan();
    return uji::ringkasan();
}
