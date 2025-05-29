#include <iostream>
using namespace std;

const int maxSize = 999;

struct Buku {
    string judul, pengarang;
    int tahun;
    Buku* kiri;
    Buku* kanan;
};

struct Stack {
    Buku data[maxSize];
    int top = -1;

    bool kosong() { return top == -1; }
    bool penuh() { return top == maxSize - 1; }

    void push(Buku b) {
        if (!penuh()) data[++top] = b;
    }

    Buku pop() {
        if (!kosong()) return data[top--];
        return {};
    }
};

Buku* akar = nullptr;
Stack undoStack;
string aksiTerakhir = "";

string tempJudul, tempPengarang, tempHapusJudul;
bool tempSimpanUndo = true;
int tempTahun;

Buku* buatBuku() {
    Buku* baru = new Buku();
    baru->judul = tempJudul;
    baru->pengarang = tempPengarang;
    baru->tahun = tempTahun;
    baru->kiri = baru->kanan = nullptr;
    return baru;
}

void sisipkanBuku() {
    Buku* bukuBaru = buatBuku();
    if (!akar) {
        akar = bukuBaru;
    } else {
        Buku* sekarang = akar;
        while (true) {
            if (tempJudul < sekarang->judul) {
                if (!sekarang->kiri) {
                    sekarang->kiri = bukuBaru;
                    break;
                }
                sekarang = sekarang->kiri;
            } else if (tempJudul > sekarang->judul) {
                if (!sekarang->kanan) {
                    sekarang->kanan = bukuBaru;
                    break;
                }
                sekarang = sekarang->kanan;
            } else {
                cout << "Judul \"" << tempJudul << "\" sudah ada!\n";
                delete bukuBaru;
                return;
            }
        }
    }

    if (!undoStack.penuh()) {
        undoStack.push(*bukuBaru); //salin datanya saja, bukan pointer
        aksiTerakhir = "tambah";
    }

    cout << "Buku berhasil ditambahkan.\n";
}

void tampilkan(Buku* node) {
    if (node) {
        tampilkan(node->kanan);
        cout << "Judul: " << node->judul << endl;
        cout << "Pengarang: " << node->pengarang << endl;
        cout << "Tahun: " << node->tahun << endl;
        cout << "--------------------------\n";
        tampilkan(node->kiri);
    }
}

void tampilkanDescending() {
    tampilkan(akar);
}

void tampilkanBuku() {
    if (!akar) {
        cout << "Belum ada buku yang disimpan.\n";
        return;
    }
    cout << "Daftar Buku (Z-A):\n";
    tampilkanDescending();
}

Buku* cariMinimum(Buku* node) {
    while (node && node->kiri)
        node = node->kiri;
    return node;
}

Buku* hapusBuku(Buku* node) {
    if (!node) return nullptr;
    if (tempHapusJudul < node->judul)
        node->kiri = hapusBuku(node->kiri);
    else if (tempHapusJudul > node->judul)
        node->kanan = hapusBuku(node->kanan);
    else {
        if (tempSimpanUndo && !undoStack.penuh()) {
            undoStack.push(*node); //simpan salinan
            aksiTerakhir = "hapus";
        }

        if (!node->kiri) {
            Buku* temp = node->kanan;
            delete node;
            return temp;
        } else if (!node->kanan) {
            Buku* temp = node->kiri;
            delete node;
            return temp;
        } else {
            Buku* pengganti = cariMinimum(node->kanan);
            node->judul = pengganti->judul;
            node->pengarang = pengganti->pengarang;
            node->tahun = pengganti->tahun;
            tempHapusJudul = pengganti->judul;
            tempSimpanUndo = false;
            node->kanan = hapusBuku(node->kanan);
        }
    }
    return node;
}

void hapus() {
    cout << "Judul yang dihapus: ";
    cin >> tempHapusJudul;

    //cek apakah buku ada
    Buku* cek = akar;
    bool ditemukan = false;
    while (cek) {
        if (tempHapusJudul == cek->judul) {
            ditemukan = true;
            break;
        } else if (tempHapusJudul < cek->judul) {
            cek = cek->kiri;
        } else {
            cek = cek->kanan;
        }
    }

    if (!ditemukan) {
        cout << "Buku tidak ditemukan dalam daftar!\n";
        return;
    }

    tempSimpanUndo = true;
    akar = hapusBuku(akar);
    cout << "Buku berhasil dihapus.\n";
}

void undoAksi() {
    if (undoStack.kosong()) {
        cout << "Tidak ada aksi untuk di-undo.\n";
        return;
    }

    Buku dataUndo = undoStack.pop();

    if (aksiTerakhir == "tambah") {
        cout << "Undo tambah: menghapus buku \"" << dataUndo.judul << "\".\n";
        tempHapusJudul = dataUndo.judul;
        tempSimpanUndo = false;
        akar = hapusBuku(akar);
    } else if (aksiTerakhir == "hapus") {
        cout << "Undo hapus: menambahkan kembali buku \"" << dataUndo.judul << "\".\n";
        tempJudul = dataUndo.judul;
        tempPengarang = dataUndo.pengarang;
        tempTahun = dataUndo.tahun;
        sisipkanBuku();
    }
}

int main() {
    int pilihan;
    do {
        system("cls");
        cout << "\nMenu:\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Buku (Z-A)\n";
        cout << "3. Hapus Buku\n";
        cout << "4. Undo Aksi Terakhir\n";
        cout << "5. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;
        system("cls");
        switch (pilihan) {
            case 1:
                cout << "Tambah Buku\n";
                cout << "Judul: "; cin >> tempJudul;
                cout << "Pengarang: "; cin >> tempPengarang;
                cout << "Tahun Terbit: "; cin >> tempTahun;
                sisipkanBuku();
                break;
            case 2:
                cout << "Tampilkan Buku\n";
                tampilkanBuku();
                break;
            case 3:
                cout << "Hapus Buku\n";
                hapus();
                break;
            case 4:
                cout << "Undo Aksi Terakhir\n";
                undoAksi();
                break;
            case 5:
                cout << "Program selesai.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
        if (pilihan != 5) {
            cout << endl;
            system("pause");
        }
    } while (pilihan != 5);
    return 0;
}