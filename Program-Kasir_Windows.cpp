#include <iostream>					//memanggil library

using namespace std;				// untuk mempersingkat pengkodingan

void login();						//memanggil fungsi
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
void verifikasiUser();
void gantiUserPass();
int rumusRekursif(int *a,int *b, int i);	//memanggil fungsi rekursif
string username = "admin";					
string password = "admin";
string reUser,rePass,verifikasi;
string inputUsername,inputPassword,strukBarang[] = {" "," "," "," "," "," "," "," "," "," "};
int inputMenu,kodeBarang,jumlahBeli,inputFooter,nomorStruk,i;
long perkalianBarang,totalBelanjaan,uangPengunjung,uangKembalian;
int jumlahBarang = 4;
string namaBarang[] = {"kosong", "Buku", "Bolpoin", "Tipe-X","Pensil"," "," "," "," "," "};
int hargaBarang[] = {0, 5000, 2000, 1000,1000,0,0,0,0,0};
int nomorBarang[] = {0,0,0,0,0,0,0,0,0,0};
int hargaStruk[] = {0,0,0,0,0,0,0,0,0,0};
int arrayJumlahBeli[] = {0,0,0,0,0,0,0,0,0,0};

bool confirm;

int main(){
	system("cls");
	cout << "==================="<<endl;
    cout << "PROGRAM MESIN KASIR"<<endl;
	cout << "==================="<<endl;
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
	system("cls");
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
		system("cls");
		cout << "== Program Kasir ==" <<endl;
		programKasir();

		break;
	case 2:
		system("cls");	
		cout << "2. Tambahkan Database Barang" <<endl;
		tambahDB();
		break;
	case 3:
		system("cls");
		cout << "3. Ganti Username/Password" <<endl;
		verifikasiUser();
		break;
	case 4:
		system("cls");
		cout << "4. Exit Program" <<endl;
		break;
	
	default:
		system("cls");
		cout << "Selamat Tinggal" << username;
		menu();
		break;
	}
}

void programKasir(){
	system("cls");
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
		system ("cls");
	
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
	system("cls");
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
	do{
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
	} while (inputFooter<0,inputFooter>99,inputFooter<98,inputFooter>1);

}

void footerMain(){
	do{
		cout << "Masukkan 99 untuk kembali ke menu utama"<<endl;
		cout << "Masukkan 98 untuk keluar dari program"<<endl;
		cout << "Masukkan nomor = " ;
		cin >> inputFooter;
		if (inputFooter == 99){
			menu();
		}else if (inputFooter == 98){
			menu();
		}
	} while (inputFooter>99,inputFooter<98);
}

void tambahDB(){
	do{
	listData();
		jumlahBarang = jumlahBarang + 1;
		cout << "=== Menambah Database ===" << endl;
		cout << "Menambah Kode Barang ke " << jumlahBarang <<endl;
		cout << "Masukkan Nama Barang(Tanpa Spasi) = ";
		cin >> namaBarang[jumlahBarang];
		cout << " Masukkan Harga Barang " << namaBarang[jumlahBarang] << " = ";
		cin >> hargaBarang[jumlahBarang];
		cout << "Selamat! "<< namaBarang[jumlahBarang] << " Telah terdaftar di Database!"<<endl;
		cout << "Ketik 1 / true untuk menambah lagi"<<endl;
		cout << "Ketik 0 / false untuk selesai"<<endl;
		cin >> confirm;
	} while (confirm==1);
	
	footerMain ();
}

void verifikasiUser(){
	cout << "===========Verifikasi============"<<endl;
	cout << "Hai "<< username << "!"<<endl;
	cout << "Masukkan Password untuk verifikasi"<<endl;
	cout << "Password = ";
	cin >> inputPassword;
	if (inputPassword == password){
		gantiUserPass();
	}
	else{
		cout << "Password Salah! harap ulangi"<<endl;
		verifikasiUser();
	}
	
}

void gantiUserPass(){
	system("cls");
	cout << "======Mengganti Username/Password======"<<endl;
	do{
		cout << "Masukkan Username Baru = ";
		cin >> reUser;
		cout << "Masukkan ulang username baru = ";
		cin >> verifikasi;
	} while (reUser != verifikasi);

	username = reUser;

	do{
		cout << "Masukkan Password Baru = ";
		cin >> rePass;
		cout << "Masukkan ulang password baru = ";
		cin >> verifikasi;
	} while (rePass != verifikasi);

	password = rePass;
	cout << "Selamat! " << username << ", Username/Password telah berhasil dirubah"<<endl;

	cout << "Ketik 1 / true untuk selesai"<<endl;
	cout << "Ketik 0 / false untuk memasukkan kembali"<<endl;
	cin >> confirm;
	if (confirm = true){
		main();
	}else{
		gantiUserPass();
	}
}
