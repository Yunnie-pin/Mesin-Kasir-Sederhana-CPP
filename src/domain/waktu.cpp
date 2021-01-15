#include "domain/waktu.h"

#include <cctype>
#include <cstddef>
#include <ctime>

namespace kasir {
namespace {

std::tm waktuLokal() {
    const std::time_t sekarang = std::time(nullptr);
    std::tm hasil{};
#ifdef _WIN32
    localtime_s(&hasil, &sekarang);
#else
    // std::localtime memakai penyangga bersama, jadi hasilnya langsung disalin.
    // Aman di sini karena program berjalan satu utas.
    hasil = *std::localtime(&sekarang);
#endif
    return hasil;
}

std::string formatWaktu(const char* pola, std::size_t panjangMaksimal) {
    const std::tm waktu = waktuLokal();
    std::string hasil(panjangMaksimal, '\0');
    const std::size_t terpakai = std::strftime(&hasil[0], hasil.size(), pola, &waktu);
    hasil.resize(terpakai);
    return hasil;
}

}  // namespace

std::string waktuSekarang() { return formatWaktu("%Y-%m-%d %H:%M:%S", 32); }

std::string tanggalHariIni() { return formatWaktu("%Y-%m-%d", 16); }

bool tanggalValid(const std::string& tanggal) {
    if (tanggal.size() != 10) {
        return false;
    }
    for (std::size_t posisi = 0; posisi < tanggal.size(); ++posisi) {
        const bool harusTandaHubung = posisi == 4 || posisi == 7;
        const unsigned char karakter = static_cast<unsigned char>(tanggal[posisi]);
        if (harusTandaHubung ? tanggal[posisi] != '-' : std::isdigit(karakter) == 0) {
            return false;
        }
    }
    return true;
}

}  // namespace kasir
