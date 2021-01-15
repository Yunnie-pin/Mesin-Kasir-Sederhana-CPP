#pragma once

#include <cstddef>
#include <string>

namespace kasir {

void bersihkanLayar();
void tekanEnterUntukLanjut();

// Garis pemisah, misalnya garis('=', 40).
std::string garis(char karakter, std::size_t panjang);

// Memendekkan teks yang lebih panjang dari lebar kolom agar tabel tetap rata.
std::string ringkasTeks(const std::string& teks, std::size_t lebarMaksimal);

}  // namespace kasir
