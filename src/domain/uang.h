#pragma once

#include <string>

namespace kasir {

// Uang disimpan sebagai bilangan bulat rupiah, bukan pecahan, supaya tidak ada
// galat pembulatan. long long dipakai karena long hanya 32 bit di Windows.
using Uang = long long;

// 5000 -> "5.000"
std::string formatAngkaRibuan(Uang nilai);

// 5000 -> "Rp. 5.000"
std::string formatRupiah(Uang nilai);

}  // namespace kasir
