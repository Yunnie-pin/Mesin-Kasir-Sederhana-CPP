#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

void login();
void menu();
void programKasir();
void listData();
void pembelian();
void footer();
void struk();
void konfirmasiPembayaran();
void footer();
void tambahDB();
void footerMain();
int rumusRekursif(int *a,int *b, int i);
string username = "admin";
string password = "admin";
string inputUsername,inputPassword,strukBarang[] = {" "," "," "," "," "," "," "," "};
int inputMenu,kodeBarang,jumlahBeli,inputFooter,nomorStruk,i;
long perkalianBarang,totalBelanjaan,uangPengunjung,uangKembalian;
int jumlahBarang = 3;
string namaBarang[] = {"kosong", "aqua", "botol", "curut"," "," "," "," "};
int hargaBarang[] = {0, 2000, 3000, 4000};
int nomorBarang[] = {0,0,0,0,0,0,0,0,0};
int hargaStruk[] = {0,0,0,0,0,0,0,0,0};
int arrayJumlahBeli[] = {0,0,0,0,0,0,0,0,0};
bool confirm;

int main(){
	system("clear");
    cout << "PROGRAM MESIN KASIR"<<endl;
    cout << "Kelompok 10" <<endl;
	login();
	cin.get();
	return 0;
}

void login(){
    cout << "Masukan Username dan Password untuk melanjutkan"<<endl;
	cout << "Username : " ;
    cin >> inputUsername;
    if (username == inputUsername){
		cout << "Password : ";
		cin >> inputPassword; 
        if (password == inputPassword){
			menu();
		}else{
			cout << "Password anda salah "<<endl;
			login();
		}
    } else {
        cout << "Username anda salah" <<endl;
		login();
    }
}

void menu(){
	system("clear");
	totalBelanjaan = 0;
	nomorStruk = 0;
	cout << "Selamat Datang " << username <<endl;
	cout << "==================== Menu ====================" <<endl;
	cout << "1. Program Kasir" <<endl;
	cout << "2. Tambahkan Database Barang" <<endl;
	cout << "3. Ganti Username/Password" <<endl;
	cout << "4. Exit Program" <<endl;
	cout << "Masukkan kode angka yang sesuai lalu tekan enter"<<endl;
	cout << "Kode Angka : ";
	cin >> inputMenu;
	switch (inputMenu){
	case 1:
		system("clear");
		cout << "== Program Kasir ==" <<endl;
		programKasir();

		break;
	case 2:
		system("clear");
		cout << "2. Tambahkan Database Barang" <<endl;
		tambahDB();
		break;
	case 3:
		system("clear");
		cout << "3. Ganti Username/Password" <<endl;
		break;
	case 4:
		system("clear");
		cout << "4. Exit Program" <<endl;
		break;
	
	default:
		system("clear");
		menu();
		break;
	}
}

void programKasir(){
	system("clear");
	listData();
	pembelian();
}

void listData(){
	cout << "==================== Database Barang ===================" << endl;
	cout << "Kode Barang\tNama Barang\t\tHarga Barang"<<endl;
	for(i=1; i <= jumlahBarang; i++){
		cout << i<<".\t\t"<<namaBarang[i]<<"\t\t\t"<<hargaBarang[i] <<endl; 
	}
}	

void pembelian(){
	cout << "====================================================" << endl;
	do{
	cout << "Masukkan kode barang = ";										// Output teks
	cin >> kodeBarang;														//memasukkan nilai int ke variabel kodeBarang
	} while (kodeBarang==0);
													//memasukkan nilai int ke variabel kodeBarang
	if (kodeBarang <= jumlahBarang){				//  Penggunaan IF untk melanjutkan jika kondisi kodeBarang <= (kurang dari samadengan) jumlahBarang
			do{
			cout << "Masukkan jumlah barang '"<<namaBarang[kodeBarang]<< "' = ";	//Output namaBarang yang akan dibeli menggunakan logika database array
			cin >> jumlahBeli;												// memasukkan nilai int pada variabel jumlahBeli (Jumlah Barang yang dibeli)
			} while (jumlahBeli==0);

			nomorStruk = nomorStruk + 1;									// menambah nilai +1 pada variabel nomorStruk setiap pemanggilan fungsi Pembelian 
			arrayJumlahBeli[nomorStruk] = jumlahBeli;						//untuk menampilkan jumlah barang yang dibeli pada struk
			strukBarang[nomorStruk] = namaBarang[kodeBarang];				// memasukkan nilai string dari variabel namaBarang ke namaBarang
			hargaStruk[nomorStruk] = hargaBarang[kodeBarang];

			totalBelanjaan = rumusRekursif(hargaStruk,arrayJumlahBeli,nomorStruk);
		
			cout << "Harga Total = Rp. "<< totalBelanjaan << endl <<endl;	// menampilkan output Total Belanjaan
		}
	else{								// penggunaan else jika kondisi kodeBarang <= (kurang dari samadengan) jumlahBarang
			cout << " Tolong masukkan Kode barang yang tertera"<<endl;		//output teks
			pembelian();				//memanggil pungsi pembelian() kembali
	}
	footer();
}

int rumusRekursif(int *a,int *b, int i){
	if(i > 0 ){
		return (a[i]*b[i]) + rumusRekursif(a,b,i-1);
	}
	else{
		return 0;
	}
	

}

void konfirmasiPembayaran(){
	do{
		uangKembalian = 0;
		cout << "================== Konfirmasi Pembayaran ==================" << endl;
		system ("clear");
	
		cout << "Total Harga = Rp. "<< totalBelanjaan << endl;	// menampilkan output Total Belanjaan
		do{
			cout << "Masukkan uang = Rp. ";
			cin >> uangPengunjung;
		} while (totalBelanjaan >= uangPengunjung);

		uangKembalian = uangPengunjung - totalBelanjaan; 		//rumus uang kembalian

	cout << "Ketik 1 / true untuk melanjutkan, dan 0 / false untuk konfirmasi ulang"<<endl;
		cin >> confirm;
		} while (confirm != true);
	struk();
}

void struk(){
	system("clear");
	cout << "====================STRUK BARANG====================" << endl;
	cout << "Nomor\tNama Barang\tJumlah Barang\tHarga perpcs"<<endl;
	for (int i = 1; i <= nomorStruk; i++){
		cout << i <<"\t"<< strukBarang[i]<< "\t\t\t" << arrayJumlahBeli[i] <<"\t Rp. "<< hargaStruk[i] <<endl;
	}
	cout << "====================================================" << endl;
		cout << "Kasir = " << username <<endl;
		cout << "Harga Total = Rp. "<< totalBelanjaan << endl;	// menampilkan output Total Belanjaan
		cout << "Uang Pembeli = Rp. " << uangPengunjung <<endl;
		cout << "Uang Kembalian = Rp. "<< uangKembalian <<endl<<endl;

		footerMain();
}

void footer(){
	cout << "Masukkan 0 untuk menambah barang"<<endl;
	cout << "Masukkan 1 untuk konfirmasi pembayaran"<<endl;
	cout << "Masukkan 99 untuk kembali ke menu utama"<<endl;
	cout << "Masukkan 98 untuk keluar dari program"<<endl;
	cout << "Masukkan nomor = " ;
	cin >> inputFooter;
	if (inputFooter == 0){
		programKasir();
	}else if (inputFooter == 99){
		menu();
	}else if(inputFooter == 98){
		menu();
	}else if(inputFooter == 1){
		konfirmasiPembayaran();
	}

}

void footerMain(){
	cout << "Masukkan 99 untuk kembali ke menu utama"<<endl;
	cout << "Masukkan 98 untuk keluar dari program"<<endl;
	cout << "Masukkan nomor = " ;
	cin >> inputFooter;
	if (inputFooter == 99){
		menu();
	}else if (inputFooter == 98){
		menu();
	}
}

void tambahDB(){
	listData();
	jumlahBarang = jumlahBarang + 1;
	cout << "=== Menambah Database ===" << endl;
	cout << "Menambah Kode Barang ke " << jumlahBarang <<endl;
	cout << "Masukkan Nama Barang = ";
	cin >> namaBarang[jumlahBarang];
	cout << " Masukkan Harga Barang " << namaBarang[jumlahBarang] << " = ";
	cin >> hargaBarang[jumlahBarang];
	cout << "Selamat! "<< namaBarang[jumlahBarang] << " Telah terdaftar di Database!"<<endl;
	footerMain ();
}

