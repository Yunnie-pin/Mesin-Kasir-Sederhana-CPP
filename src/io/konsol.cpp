#include "io/konsol.h"

#include <cstdlib>
#include <iostream>
#include <limits>

namespace kasir {

void bersihkanLayar() {
#ifdef _WIN32
    static_cast<void>(std::system("cls"));
#else
    static_cast<void>(std::system("clear"));
#endif
}

void tekanEnterUntukLanjut() {
    std::cout << "\nTekan Enter untuk melanjutkan...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string garis(char karakter, std::size_t panjang) { return std::string(panjang, karakter); }

std::string ringkasTeks(const std::string& teks, std::size_t lebarMaksimal) {
    if (teks.size() <= lebarMaksimal) {
        return teks;
    }
    if (lebarMaksimal <= 2) {
        return teks.substr(0, lebarMaksimal);
    }
    return teks.substr(0, lebarMaksimal - 2) + "..";
}

}  // namespace kasir
