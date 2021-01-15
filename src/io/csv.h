#pragma once

#include <string>
#include <vector>

namespace kasir {

// CSV sederhana bergaya RFC 4180: kolom yang memuat koma atau tanda kutip
// dibungkus tanda kutip, dan tanda kutip di dalam kolom digandakan. Ini yang
// membuat nama barang berspasi maupun berkoma aman disimpan.
std::string gabungBarisCsv(const std::vector<std::string>& kolom);
std::vector<std::string> pecahBarisCsv(const std::string& baris);

// Mengubah teks menjadi bilangan. Gagal (false) bila ada karakter yang bukan
// bagian dari angka, jadi baris judul pada berkas CSV otomatis ditolak.
bool keBilangan(const std::string& teks, long long& hasil);

}  // namespace kasir
