#include <iostream>					//memanggil library

using namespace std;				// untuk mempersingkat pengkodingan

void login();										//membuat fungsi
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
int rumusRekursif(int *a,int *b, int i);			//membuat fungsi rekursif
string username = "admin";																	//membuat variabel bertipe data string
string password = "admin";
string reUser,rePass,verifikasi;
string inputUsername,inputPassword;
int inputMenu,kodeBarang,jumlahBeli,inputFooter,nomorStruk,i;								//membuat variabel bertipe data integer
int jumlahBarang = 4;
long perkalianBarang,totalBelanjaan,uangPengunjung,uangKembalian;							//membuat variabel bertipe data long
string strukBarang[] = {" "," "," "," "," "," "," "," "," "," "}; 							//membuat variabel bertipe data string dengan array
string namaBarang[] = {"kosong", "Buku", "Bolpoin", "Tipe-X","Pensil"," "," "," "," "," "};
int hargaBarang[] = {0, 5000, 2000, 1000,1000,0,0,0,0,0};									//membuat variabel bertipe data integer dengan array
int nomorBarang[] = {0,0,0,0,0,0,0,0,0,0};
int hargaStruk[] = {0,0,0,0,0,0,0,0,0,0};
int arrayJumlahBeli[] = {0,0,0,0,0,0,0,0,0,0};

bool confirm;																				//membuat variabel bertipe data boolean

int main(){																					
	system("cls");											//fungsi untuk membersihkan data di command tanpa menutup program
	cout << "==================="<<endl;						//output
    cout << "PROGRAM MESIN KASIR"<<endl;
	cout << "==================="<<endl;
	login();													//memanggil fungsi login
	cin.get();
	return 0;
}

void login(){															//memberikan logic pada function login
    cout << "Masukan Username dan Password untuk melanjutkan"<<endl;	
	cout << "Username : " ;												//membuat form input username
    cin >> inputUsername;												
    if (username == inputUsername){										//membuat pengkondisian dengan if else
		cout << "Password : ";											
		cin >> inputPassword; 
        if (password == inputPassword){									//membuat pengkondisian didalam pengkondisian dengan if else
			menu();														//jika pengkondisian sesuai maka akan memanggil fungsi menu()
		}else{															//else jika pengkondisian tidak sesuai
			cout << "Password anda salah "<<endl;
			login();													//jika pengkondisian tidak sesuai maka akan memanggil fungsi login()
		}	
    } else {															//else jika pengkondisian tidak sesuai
        cout << "Username anda salah" <<endl;
		login();														//jika pengkondisian tidak sesuai maka akan memanggil fungsi login()
    }
}

void menu(){															//memberikan logic pada function menu
	system("cls");													//fungsi untuk membersihkan
	totalBelanjaan = 0;													//pendeklarasian variabel
	nomorStruk = 0;
	cout << "Selamat Datang " << username <<endl;						//ouput menampilkan variabel username
	cout << "==================== Menu ====================" <<endl;	//output
	cout << "1. Program Kasir" <<endl;
	cout << "2. Tambahkan Database Barang" <<endl;
	cout << "3. Ganti Username/Password" <<endl;
	cout << "4. Exit Program" <<endl;
	cout << "Masukkan kode angka yang sesuai lalu tekan enter"<<endl;
	cout << "Kode Angka : ";											//membuat form input menu
	cin >> inputMenu;	
	switch (inputMenu){													//membuat percabangan dengan menggunakan switch case sebagai fitur pemilihan inputMenu
	case 1:												//jika inputMenu = 1 maka akan mengeluarkan output berikut
		system("cls");
		cout << "== Program Kasir ==" <<endl;
		programKasir();									//memanggil fungsi programKasir()

		break;											
	case 2:												//jika inputMenu = 2 maka akan mengeluarkan output berikut
		system("cls");	
		cout << "2. Tambahkan Database Barang" <<endl;
		tambahDB();										//memanggil fungsi tambahDB();
		break;
	case 3:												//jika inputMenu = 3 maka akan mengeluarkan output berikut
		system("cls");								
		cout << "3. Ganti Username/Password" <<endl;
		verifikasiUser();								//memanggil fungsi verifikasiUser()
		break;
	case 4:												//jika inputMenu = 3 maka akan mengeluarkan output berikut
		system("cls");
		cout << "4. Exit Program" <<endl;		
		cout << "Selamat Tinggal" << username;		
		break;
	
	default:											//jika inputMenu tidak sama dengan case diatas maka akan mengeluarkan output sebagai berikut
		system("cls");
		cout << "Selamat Tinggal" << username;
		break;
	}
}

void programKasir(){									//memberikan logic pada fungsi programKasir()
	system("cls");
	listData();											//memanggil fungsi listData()
	pembelian();										//memanggil fungsi pembelian()
}

void listData(){																		//memberikan logic pada fungsi listData()
	cout << "==================== Database Barang ===================" << endl;			
	cout << "Kode Barang\tNama Barang\t\tHarga Barang"<<endl;					
	for(i=1; i <= jumlahBarang; i++){													//menggunakan fungsi loop for untuk menampilkan database data Barang dengan logika Array
		cout << i<<".\t\t"<<namaBarang[i]<<"\t\t\t"<<hargaBarang[i] <<endl; 
	}
}	

void pembelian(){																//memberikan logic pada fungsi pembelian
	cout << "====================================================" << endl;
	do{																		//menggunakan fungsi loop do while
	cout << "Masukkan kode barang = ";										// Output teks
	cin >> kodeBarang;														//memasukkan nilai int ke variabel kodeBarang
	} while (kodeBarang==0);
													//memasukkan nilai int ke variabel kodeBarang
	if (kodeBarang <= jumlahBarang){				//  Penggunaan IF untk melanjutkan jika kondisi kodeBarang <= (kurang dari samadengan) jumlahBarang
			do{																//menggunakan fungsi loop do while 
			cout << "Masukkan jumlah barang '"<<namaBarang[kodeBarang]<< "' = ";	//Output namaBarang yang akan dibeli menggunakan logika database array
			cin >> jumlahBeli;												// memasukkan nilai int pada variabel jumlahBeli (Jumlah Barang yang dibeli)
			} while (jumlahBeli==0);										// jika hasil inputan sesuai maka akan kembali ke loop do

			nomorStruk = nomorStruk + 1;									// menambah nilai +1 pada variabel nomorStruk setiap pemanggilan fungsi Pembelian 
			arrayJumlahBeli[nomorStruk] = jumlahBeli;						//untuk menampilkan jumlah barang yang dibeli pada struk
			strukBarang[nomorStruk] = namaBarang[kodeBarang];				// memasukkan nilai string dari variabel namaBarang ke namaBarang
			hargaStruk[nomorStruk] = hargaBarang[kodeBarang];

			totalBelanjaan = rumusRekursif(hargaStruk,arrayJumlahBeli,nomorStruk);	// penggunaan fungsi rekursif dengan function rumusRekursif
		
			cout << "Harga Total = Rp. "<< totalBelanjaan << endl <<endl;	// menampilkan output Total Belanjaan
		}
	else{								// penggunaan else jika kondisi kodeBarang <= (kurang dari samadengan) jumlahBarang
			cout << " Tolong masukkan Kode barang yang tertera"<<endl;		//output teks
			pembelian();				//memanggil pungsi pembelian() kembali
	}
	footer();							//memanggil fungsi footer()
}

int rumusRekursif(int *a,int *b, int i){		//membuat rumus fungsi rekursif dengan kembalian
	if(i > 0 ){									//menggunakan logika if else agar dapat membuat batas jika  i = 0
		return (a[i]*b[i]) + rumusRekursif(a,b,i-1);	//penghitungan dengan rumus rekursif pada database array
	}
	else{										//jika i == 0 maka akan mengembalikan nilai 0
		return 0;
	}
	

}

void konfirmasiPembayaran(){					//memberikan logic pada fungsi konfirmasiPembayaran()
	do{											//menggunakan looping/ perulangan do while
		uangKembalian = 0;
		cout << "================== Konfirmasi Pembayaran ==================" << endl;
		system ("cls");
	
		cout << "Total Harga = Rp. "<< totalBelanjaan << endl;	// menampilkan output Total Belanjaan
		do{														// menggunakan looping do while
			cout << "Masukkan uang = Rp. ";						// membuat form uang pengunjung
			cin >> uangPengunjung;
		} while (totalBelanjaan >= uangPengunjung);				// jika input sesuai dengan kondisi maka akan kembali memanggil fungsi do while

		uangKembalian = uangPengunjung - totalBelanjaan; 		//rumus uang kembalian

		cout << "Ketik 1 / true untuk melanjutkan, dan 0 / false untuk konfirmasi ulang"<<endl;		//membuat form confirm
		cin >> confirm;											
	} while (confirm != true);									//jika input sesuai dengan kondisi maka akan kembali memanggil fungsi do
	struk();													//memanggil fungsi struk()
}

void struk(){													//memberikan logic pada fungsi struk
	system("cls");											//fungsi untuk membersihkan layar
	cout << "====================STRUK BARANG====================" << endl;
	cout << "Nomor\tNama Barang\tJumlah Barang\tHarga perpcs"<<endl;
	for (int i = 1; i <= nomorStruk; i++){								// menggunakan fungsi loop for untuk menampilkan data nilai array struk
		cout << i <<"\t"<< strukBarang[i]<< "\t\t\t" << arrayJumlahBeli[i] <<"\t Rp. "<< hargaStruk[i] <<endl;
	}
	cout << "====================================================" << endl;
		cout << "Kasir = " << username <<endl;
		cout << "Harga Total = Rp. "<< totalBelanjaan << endl;	// menampilkan output Total Belanjaan
		cout << "Uang Pembeli = Rp. " << uangPengunjung <<endl;	// menampilkan output uang pengunjung
		cout << "Uang Kembalian = Rp. "<< uangKembalian <<endl<<endl;	//menampilan output uang kembalian

		footerMain();											//memanggil fungsi footerMain()
}

void footer(){													//memberikan logic pada fungsi footer
	do{															//menggunakan loop do while
		cout << "Masukkan 0 untuk menambah barang"<<endl;
		cout << "Masukkan 1 untuk konfirmasi pembayaran"<<endl;
		cout << "Masukkan 99 untuk kembali ke menu utama"<<endl;
		cout << "Masukkan 98 untuk keluar dari program"<<endl;
		cout << "Masukkan nomor = " ;							//membuat form inputFooter
		cin >> inputFooter;
		if (inputFooter == 0){									//fungsi if else if untuk pengkondisian
			programKasir();										// jika variabel input footer = 0 maka akan memanggil fungsi programKasir()
		}else if (inputFooter == 99){
			menu();												// jika variabel input footer = 99 maka akan memanggil fungsi menu()
		}else if(inputFooter == 98){							
																// jika variabel input footer = 98 maka akan membuat program berhenti
		}else if(inputFooter == 1){
			konfirmasiPembayaran();								// jika variabel input footer = 1 maka akan memanggil fungsi konfirmasiPembayaran()
		}
	} while (inputFooter<0,inputFooter>99,inputFooter<98,inputFooter>1);

}

void footerMain(){												//memberikan logic pada fungsi footerMain
	do{															//menggunakan loop do while
		cout << "Masukkan 99 untuk kembali ke menu utama"<<endl;
		cout << "Masukkan 98 untuk keluar dari program"<<endl;
		cout << "Masukkan nomor = " ;
		cin >> inputFooter;
		if (inputFooter == 99){
			menu();												// jika variabel input footer = 99 maka akan memanggil fungsi menu()
		}else if (inputFooter == 98){
																// jika variabel input footer = 98 maka akan keluar dari program
		}
	} while (inputFooter>99,inputFooter<98);					//jika kondisi inputFooter sesuai maka akan kembali ke awal do
}

void tambahDB(){												//memberikan logic pada fungsi tambahDB
	do{															//menggunakan looping do while
	listData();													//memanggil listData;
		jumlahBarang = jumlahBarang + 1;						//rumus setiap perulangan maka variable jumlahBarang akan ditambah 1
		cout << "=== Menambah Database ===" << endl;
		cout << "Menambah Kode Barang ke " << jumlahBarang <<endl;
		cout << "Masukkan Nama Barang(Tanpa Spasi) = ";			//form nama barang
		cin >> namaBarang[jumlahBarang];						
		cout << " Masukkan Harga Barang " << namaBarang[jumlahBarang] << " = ";		//form penambahan harga barang
		cin >> hargaBarang[jumlahBarang];
		cout << "Selamat! "<< namaBarang[jumlahBarang] << " Telah terdaftar di Database!"<<endl;
		cout << "Ketik 1 / true untuk menambah lagi"<<endl;
		cout << "Ketik 0 / false untuk selesai"<<endl;
		cin >> confirm;											//form confirm
	} while (confirm==1);										//jika kondisi sesuai dengan confirm maka akan kembali ke do
	
	footerMain ();												//memanggil fungsi footerMain
}

void verifikasiUser(){											//memberikan logic pada fungsi verifikasiUser
	cout << "===========Verifikasi============"<<endl;
	cout << "Hai "<< username << "!"<<endl;
	cout << "Masukkan Password untuk verifikasi"<<endl;
	cout << "Password = ";										//membuat form password
	cin >> inputPassword;										
	if (inputPassword == password){								//pengkondisian if
		gantiUserPass();										//jika sesuai maka akan memanggil fungsi gantiUserPass()
	}
	else{														//jika pengkonfisian if tidak sesuai
		cout << "Password Salah! harap ulangi"<<endl;
		verifikasiUser();										//memanggil fungsi verifikasiUser Kembali
	}
	
}

void gantiUserPass(){											//memberikan logic pada fungsi gantiUserPass
	system("cls");
	cout << "======Mengganti Username/Password======"<<endl;	
	do{															//looping do while
		cout << "Masukkan Username Baru = ";					//membuat form reUser
		cin >> reUser;
		cout << "Masukkan ulang username baru = ";				//membuat form verifikasi
		cin >> verifikasi;
	} while (reUser != verifikasi);								//jika kondisi sesuai maka akan kembali ke do

	username = reUser;											// isi variabel reUser akan di copy ke variabel username

	do{															//looping do while
		cout << "Masukkan Password Baru = ";					//membuat form rePass
		cin >> rePass;
		cout << "Masukkan ulang password baru = ";				//membuat form verifikasi
		cin >> verifikasi;
	} while (rePass != verifikasi);								//jika kondisi sesuai maka akan kembali ke do

	password = rePass;											// isi variabel rePass akan di copy ke variabel password
	cout << "Selamat! " << username << ", Username/Password telah berhasil dirubah"<<endl;

	cout << "Ketik 1 / true untuk selesai"<<endl;
	cout << "Ketik 0 / false untuk memasukkan kembali"<<endl;
	cin >> confirm;												//membuat form confirm
	if (confirm = true){										//pengkondisian if else dengan variabel bool confirm
		main();													//memanggil fungsi main()
	}else{	
		gantiUserPass();										//memanggil fungsiGantiUserPass
	}
}
