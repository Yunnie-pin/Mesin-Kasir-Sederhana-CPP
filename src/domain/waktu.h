#pragma once

#include <string>

namespace kasir {

// Waktu lokal sekarang dengan format "YYYY-MM-DD HH:MM:SS".
std::string waktuSekarang();

// Tanggal lokal hari ini dengan format "YYYY-MM-DD".
std::string tanggalHariIni();

// Memeriksa bentuk "YYYY-MM-DD" (hanya polanya, bukan keberadaan tanggalnya).
bool tanggalValid(const std::string& tanggal);

}  // namespace kasir
