cmake_minimum_required(VERSION 3.16)
project(MesinKasirSederhana VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opsi peringatan dikumpulkan di satu tempat lalu dipakai semua target.
add_library(opsi_peringatan INTERFACE)
if(MSVC)
    target_compile_options(opsi_peringatan INTERFACE /W4 /permissive-)
else()
    target_compile_options(opsi_peringatan INTERFACE
        -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

# Logika program tanpa fungsi main, supaya bisa dipakai program utama maupun
# program uji.
add_library(kasir_inti
    src/domain/akun.cpp
    src/domain/barang.cpp
    src/domain/keranjang.cpp
    src/domain/laporan.cpp
    src/domain/transaksi.cpp
    src/domain/uang.cpp
    src/domain/waktu.cpp
    src/io/csv.cpp
    src/io/input.cpp
    src/io/konsol.cpp
    src/io/penyimpanan.cpp
    src/io/tampilan.cpp
)
target_include_directories(kasir_inti PUBLIC src)
target_link_libraries(kasir_inti PRIVATE opsi_peringatan)

add_executable(kasir
    src/main.cpp
    src/app/aplikasi.cpp
)
target_link_libraries(kasir PRIVATE kasir_inti opsi_peringatan)

enable_testing()

add_executable(uji_kasir tests/uji_kasir.cpp)
target_include_directories(uji_kasir PRIVATE tests)
target_link_libraries(uji_kasir PRIVATE kasir_inti opsi_peringatan)
add_test(NAME uji_kasir COMMAND uji_kasir)
