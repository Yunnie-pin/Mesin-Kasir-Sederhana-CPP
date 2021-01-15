#include "domain/uang.h"

#include <cstddef>

namespace kasir {
namespace {

// Ditulis lewat unsigned agar nilai terkecil long long tidak overflow saat
// tandanya dibalik.
unsigned long long nilaiMutlak(Uang nilai) {
    return nilai < 0 ? 0ULL - static_cast<unsigned long long>(nilai)
                     : static_cast<unsigned long long>(nilai);
}

}  // namespace

std::string formatAngkaRibuan(Uang nilai) {
    const std::string digit = std::to_string(nilaiMutlak(nilai));

    std::string hasil;
    hasil.reserve(digit.size() + digit.size() / 3 + 1);
    if (nilai < 0) {
        hasil.push_back('-');
    }
    for (std::size_t posisi = 0; posisi < digit.size(); ++posisi) {
        const std::size_t sisaDigit = digit.size() - posisi;
        if (posisi > 0 && sisaDigit % 3 == 0) {
            hasil.push_back('.');
        }
        hasil.push_back(digit[posisi]);
    }
    return hasil;
}

std::string formatRupiah(Uang nilai) { return "Rp. " + formatAngkaRibuan(nilai); }

}  // namespace kasir
