#include "io/csv.h"

#include <cstddef>
#include <exception>
#include <string>

namespace kasir {
namespace {

bool perluDibungkus(const std::string& kolom) {
    return kolom.find_first_of(",\"\n\r") != std::string::npos;
}

}  // namespace

std::string gabungBarisCsv(const std::vector<std::string>& kolom) {
    std::string hasil;
    for (std::size_t indeks = 0; indeks < kolom.size(); ++indeks) {
        if (indeks > 0) {
            hasil.push_back(',');
        }
        if (!perluDibungkus(kolom[indeks])) {
            hasil += kolom[indeks];
            continue;
        }
        hasil.push_back('"');
        for (const char karakter : kolom[indeks]) {
            if (karakter == '"') {
                hasil.push_back('"');  // tanda kutip digandakan
            }
            hasil.push_back(karakter);
        }
        hasil.push_back('"');
    }
    return hasil;
}

std::vector<std::string> pecahBarisCsv(const std::string& baris) {
    std::vector<std::string> kolom;
    std::string sekarang;
    bool dalamKutip = false;

    for (std::size_t posisi = 0; posisi < baris.size(); ++posisi) {
        const char karakter = baris[posisi];
        if (dalamKutip) {
            if (karakter != '"') {
                sekarang.push_back(karakter);
                continue;
            }
            const bool kutipGanda = posisi + 1 < baris.size() && baris[posisi + 1] == '"';
            if (kutipGanda) {
                sekarang.push_back('"');
                ++posisi;
            } else {
                dalamKutip = false;
            }
            continue;
        }
        if (karakter == '"' && sekarang.empty()) {
            dalamKutip = true;
        } else if (karakter == ',') {
            kolom.push_back(sekarang);
            sekarang.clear();
        } else if (karakter != '\r') {
            sekarang.push_back(karakter);
        }
    }
    kolom.push_back(sekarang);
    return kolom;
}

bool keBilangan(const std::string& teks, long long& hasil) {
    if (teks.empty()) {
        return false;
    }
    try {
        std::size_t akhir = 0;
        const long long nilai = std::stoll(teks, &akhir);
        if (akhir != teks.size()) {
            return false;
        }
        hasil = nilai;
        return true;
    } catch (const std::exception&) {
        return false;  // bukan angka, atau di luar jangkauan long long
    }
}

}  // namespace kasir
