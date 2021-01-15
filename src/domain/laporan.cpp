#include "domain/laporan.h"

#include <algorithm>

namespace kasir {

Laporan buatLaporan(const std::vector<Transaksi>& riwayat, const std::string& tanggal) {
    Laporan laporan;
    laporan.tanggal = tanggal;

    for (const Transaksi& transaksi : riwayat) {
        if (!tanggal.empty() && transaksi.tanggal() != tanggal) {
            continue;
        }
        ++laporan.jumlahTransaksi;
        laporan.omzet += transaksi.total();

        for (const BarisKeranjang& baris : transaksi.baris) {
            const auto posisi = std::find_if(
                laporan.perBarang.begin(), laporan.perBarang.end(),
                [&baris](const RingkasanBarang& ringkasan) { return ringkasan.nama == baris.nama; });
            if (posisi == laporan.perBarang.end()) {
                laporan.perBarang.push_back({baris.nama, baris.jumlah, baris.subtotal()});
            } else {
                posisi->jumlahTerjual += baris.jumlah;
                posisi->omzet += baris.subtotal();
            }
        }
    }

    // Terlaris di atas; nama dipakai sebagai pemecah seri supaya urutannya tetap
    // sama setiap kali laporan dibuat.
    std::sort(laporan.perBarang.begin(), laporan.perBarang.end(),
              [](const RingkasanBarang& kiri, const RingkasanBarang& kanan) {
                  if (kiri.jumlahTerjual != kanan.jumlahTerjual) {
                      return kiri.jumlahTerjual > kanan.jumlahTerjual;
                  }
                  return kiri.nama < kanan.nama;
              });
    return laporan;
}

}  // namespace kasir
