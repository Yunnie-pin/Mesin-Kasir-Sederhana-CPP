#pragma once

#include <limits>
#include <string>

#include "domain/uang.h"

namespace kasir {

// Dilempar saat aliran input tertutup (Ctrl+Z / Ctrl+D) atau rusak, supaya
// program berhenti rapi alih-alih terjebak dalam perulangan pembacaan input.
struct InputBerakhir {};

constexpr Uang TANPA_BATAS_ATAS = std::numeric_limits<Uang>::max();

enum class ModeInput { Tampak, Tersembunyi };

// Semua pembaca di bawah ini menolak input yang tidak sesuai dan bertanya
// kembali, jadi pemanggil selalu menerima nilai yang sudah valid.
Uang bacaBilangan(const std::string& prompt, Uang minimum, Uang maksimum = TANPA_BATAS_ATAS);
std::string bacaKata(const std::string& prompt);      // satu kata, tanpa spasi
std::string bacaBaris(const std::string& prompt);     // boleh berspasi, tidak boleh kosong
std::string bacaPassword(const std::string& prompt);  // ketikan tidak tampil di layar
bool bacaKonfirmasi(const std::string& pertanyaan);

// Meminta nilai dua kali sampai kedua isian sama; dipakai untuk username dan
// password baru.
std::string bacaNilaiBaru(const std::string& prompt, const std::string& promptUlang,
                          ModeInput mode);

}  // namespace kasir
