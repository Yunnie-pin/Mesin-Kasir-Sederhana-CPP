#include "io/input.h"

#include <cctype>
#include <cstddef>
#include <cstdio>
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace kasir {
namespace {

void abaikanSisaBaris() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Membuang input yang salah agar bisa dibaca ulang. Kegagalan karena aliran
// input tertutup atau rusak tidak bisa dipulihkan, jadi program dihentikan.
void pulihkanInputAtauBerhenti() {
    if (std::cin.eof() || std::cin.bad()) {
        throw InputBerakhir{};
    }
    std::cin.clear();
    abaikanSisaBaris();
}

std::string rapikanSpasi(const std::string& teks) {
    std::size_t awal = 0;
    while (awal < teks.size() && std::isspace(static_cast<unsigned char>(teks[awal])) != 0) {
        ++awal;
    }
    std::size_t akhir = teks.size();
    while (akhir > awal && std::isspace(static_cast<unsigned char>(teks[akhir - 1])) != 0) {
        --akhir;
    }
    return teks.substr(awal, akhir - awal);
}

}  // namespace

Uang bacaBilangan(const std::string& prompt, Uang minimum, Uang maksimum) {
    for (;;) {
        std::cout << prompt;
        Uang nilai = 0;
        if (!(std::cin >> nilai)) {
            pulihkanInputAtauBerhenti();
            std::cout << "Input harus berupa angka.\n";
            continue;
        }
        abaikanSisaBaris();
        if (minimum <= nilai && nilai <= maksimum) {
            return nilai;
        }
        std::cout << "Masukkan angka antara " << minimum << " sampai " << maksimum << ".\n";
    }
}

std::string bacaKata(const std::string& prompt) {
    for (;;) {
        std::cout << prompt;
        std::string kata;
        if (std::cin >> kata) {
            abaikanSisaBaris();
            return kata;
        }
        pulihkanInputAtauBerhenti();
    }
}

std::string bacaBaris(const std::string& prompt) {
    for (;;) {
        std::cout << prompt;
        std::string baris;
        if (!std::getline(std::cin, baris)) {
            pulihkanInputAtauBerhenti();
            continue;
        }
        const std::string hasil = rapikanSpasi(baris);
        if (!hasil.empty()) {
            return hasil;
        }
        std::cout << "Isian tidak boleh kosong.\n";
    }
}

#ifdef _WIN32

std::string bacaPassword(const std::string& prompt) {
    std::cout << prompt << std::flush;
    std::string hasil;
    for (;;) {
        const int karakter = _getch();
        if (karakter == '\r' || karakter == '\n') {
            break;
        }
        if (karakter == 3 || karakter == 26 || karakter == EOF) {  // Ctrl+C, Ctrl+Z
            throw InputBerakhir{};
        }
        if (karakter == 0 || karakter == 0xE0) {
            _getch();  // tombol panah/fungsi mengirim dua kode, kode kedua dibuang
            continue;
        }
        if (karakter == '\b' || karakter == 127) {
            if (!hasil.empty()) {
                hasil.pop_back();
                std::cout << "\b \b";
            }
            continue;
        }
        hasil.push_back(static_cast<char>(karakter));
        std::cout << '*';
    }
    std::cout << '\n';
    return hasil;
}

#else

std::string bacaPassword(const std::string& prompt) {
    std::cout << prompt << std::flush;

    termios pengaturanLama{};
    const bool bisaDisembunyikan = tcgetattr(STDIN_FILENO, &pengaturanLama) == 0;
    if (bisaDisembunyikan) {
        termios pengaturanBaru = pengaturanLama;
        pengaturanBaru.c_lflag &= ~static_cast<tcflag_t>(ECHO);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &pengaturanBaru);
    }

    std::string hasil;
    const bool berhasil = static_cast<bool>(std::getline(std::cin, hasil));

    if (bisaDisembunyikan) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &pengaturanLama);
    }
    std::cout << '\n';

    if (!berhasil) {
        pulihkanInputAtauBerhenti();
        return std::string{};
    }
    return hasil;
}

#endif

bool bacaKonfirmasi(const std::string& pertanyaan) {
    std::cout << pertanyaan << '\n';
    return bacaBilangan("Ketik 1 untuk ya, 0 untuk tidak = ", 0, 1) == 1;
}

std::string bacaNilaiBaru(const std::string& prompt, const std::string& promptUlang,
                          ModeInput mode) {
    for (;;) {
        // Username dibaca sebagai satu kata karena form login juga membaca satu
        // kata; username berspasi tidak akan pernah bisa diketik saat login.
        const std::string nilai =
            mode == ModeInput::Tersembunyi ? bacaPassword(prompt) : bacaKata(prompt);
        if (nilai.empty()) {
            std::cout << "Isian tidak boleh kosong.\n";
            continue;
        }
        const std::string ulangi =
            mode == ModeInput::Tersembunyi ? bacaPassword(promptUlang) : bacaKata(promptUlang);
        if (nilai == ulangi) {
            return nilai;
        }
        std::cout << "Isian tidak sama, silakan ulangi.\n";
    }
}

}  // namespace kasir
