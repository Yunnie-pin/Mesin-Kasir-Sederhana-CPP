#pragma once

// Kerangka uji minimal, tanpa pustaka luar, supaya proyek ini tetap bisa
// dikompilasi hanya dengan compiler C++17. Pemakaian:
//
//   CEK(hitungan == 5);
//   return uji::ringkasan();

#include <iostream>
#include <string>

namespace uji {

inline int jumlahPemeriksaan = 0;
inline int jumlahGagal = 0;

inline void periksa(bool lulus, const std::string& ungkapan, const std::string& berkas, int baris) {
    ++jumlahPemeriksaan;
    if (!lulus) {
        ++jumlahGagal;
        std::cout << "GAGAL  " << berkas << ":" << baris << "  " << ungkapan << '\n';
    }
}

inline int ringkasan() {
    std::cout << jumlahPemeriksaan - jumlahGagal << " dari " << jumlahPemeriksaan
              << " pemeriksaan lulus\n";
    return jumlahGagal == 0 ? 0 : 1;
}

}  // namespace uji

#define CEK(ungkapan) uji::periksa((ungkapan), #ungkapan, __FILE__, __LINE__)
