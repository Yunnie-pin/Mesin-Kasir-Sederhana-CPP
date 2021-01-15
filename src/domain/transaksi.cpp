#include "domain/transaksi.h"

#include <algorithm>
#include <cstddef>
#include <numeric>

namespace kasir {

Uang Transaksi::total() const {
    return std::accumulate(baris.begin(), baris.end(), Uang{0},
                           [](Uang jumlah, const BarisKeranjang& satuBaris) {
                               return jumlah + satuBaris.subtotal();
                           });
}

Uang Transaksi::kembalian() const { return uangPembeli - total(); }

std::string Transaksi::tanggal() const {
    return waktu.substr(0, std::min<std::size_t>(10, waktu.size()));
}

std::string buatNomorTransaksi(const std::vector<Transaksi>& riwayat, const std::string& waktu) {
    const std::string tanggal = waktu.substr(0, std::min<std::size_t>(10, waktu.size()));

    const std::ptrdiff_t transaksiHariIni =
        std::count_if(riwayat.begin(), riwayat.end(), [&tanggal](const Transaksi& transaksi) {
            return transaksi.tanggal() == tanggal;
        });

    std::string tanggalRapat;  // "2026-08-17" -> "20260817"
    for (const char karakter : tanggal) {
        if (karakter != '-') {
            tanggalRapat.push_back(karakter);
        }
    }

    std::string urutan = std::to_string(transaksiHariIni + 1);
    while (urutan.size() < 4) {
        urutan.insert(urutan.begin(), '0');
    }
    return "TRX-" + tanggalRapat + "-" + urutan;
}

}  // namespace kasir
