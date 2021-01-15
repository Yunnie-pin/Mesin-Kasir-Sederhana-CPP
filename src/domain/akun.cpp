#include "domain/akun.h"

#include <algorithm>

namespace kasir {

std::string namaPeran(Peran peran) { return peran == Peran::Admin ? "admin" : "kasir"; }

bool peranDariTeks(const std::string& teks, Peran& hasil) {
    if (teks == "admin") {
        hasil = Peran::Admin;
        return true;
    }
    if (teks == "kasir") {
        hasil = Peran::Kasir;
        return true;
    }
    return false;
}

DaftarAkun DaftarAkun::bawaan() {
    DaftarAkun daftar;
    daftar.tambah({"admin", "admin", Peran::Admin});
    daftar.tambah({"kasir", "kasir", Peran::Kasir});
    return daftar;
}

const Akun* DaftarAkun::cari(const std::string& username) const {
    const auto posisi = std::find_if(daftar_.begin(), daftar_.end(), [&username](const Akun& akun) {
        return akun.username == username;
    });
    return posisi == daftar_.end() ? nullptr : &*posisi;
}

bool DaftarAkun::tambah(const Akun& akun) {
    if (akun.username.empty() || akun.password.empty() || cari(akun.username) != nullptr) {
        return false;
    }
    daftar_.push_back(akun);
    return true;
}

bool DaftarAkun::ubahKredensial(const std::string& usernameLama, const std::string& usernameBaru,
                                const std::string& passwordBaru) {
    if (usernameBaru.empty() || passwordBaru.empty()) {
        return false;
    }
    if (usernameBaru != usernameLama && cari(usernameBaru) != nullptr) {
        return false;
    }

    const auto posisi =
        std::find_if(daftar_.begin(), daftar_.end(), [&usernameLama](const Akun& akun) {
            return akun.username == usernameLama;
        });
    if (posisi == daftar_.end()) {
        return false;
    }
    posisi->username = usernameBaru;
    posisi->password = passwordBaru;
    return true;
}

}  // namespace kasir
