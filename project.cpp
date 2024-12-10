#include <iostream>
#include <string>

using namespace std;

#define MAX_MEETINGS 100
#define MAX_MEMBERS 50

struct Meeting {
    string title;
    int startHour;
    int endHour;
    string members[MAX_MEMBERS];
    int numMembers;
    int date;
};

struct MemberSchedule {
    string name;
    bool available[31][25]; // availability for each hour of the day (0-23)
};

void initializeAvailability(MemberSchedule members[], int numMembers) {
    for (int i = 0; i < numMembers; ++i) {
        for(int k=1;k<=30;k++){
            for (int j = 0; j < 24; ++j) {
                members[i].available[k][j] = (j >= 8 && j < 17); // Default available from 8 AM to 5 PM
            } 
        }

    }
}

void addMeeting(Meeting meetings[], int &numMeetings, MemberSchedule memberSchedules[], int numMembers) {
    if (numMeetings >= MAX_MEETINGS) {
        cout << "Tidak bisa menambahkan lebih banyak rapat, kapasitas penuh.\n";
        return;
    }

    Meeting newMeeting;
    cout << "Masukkan judul rapat: ";
    cin.ignore();
    getline(cin, newMeeting.title);
    cout << "Masukkan tanggal rapat: ";
    cin>>newMeeting.date;
    cout << "Masukkan jam mulai rapat (8-17): ";
    cin >> newMeeting.startHour;
    cout << "Masukkan jam selesai rapat (8-17): ";
    cin >> newMeeting.endHour;

    if (newMeeting.startHour < 8 || newMeeting.startHour > 17 ||
        newMeeting.endHour < 8 || newMeeting.endHour > 17 ||
        newMeeting.startHour >= newMeeting.endHour || newMeeting.date <= 0 || newMeeting.date > 30) {
        cout << "Jam rapat tidak valid. Silakan coba lagi.\n";
        return;
    }

    cout << "Masukkan jumlah anggota yang akan menghadiri rapat: ";
    cin >> newMeeting.numMembers;
    if (newMeeting.numMembers > numMembers) {
        cout << "Jumlah anggota melebihi kapasitas. Silakan coba lagi.\n";
        return;
    }

    cin.ignore();
    for (int i = 0; i < newMeeting.numMembers; ++i) {
        cout << "Masukkan nama anggota " << i + 1 << ": ";
        getline(cin, newMeeting.members[i]);
    }
    // for(int i=0;i<newMeeting.numMembers; ++i){
    //     cout<<newMeeting.members[i]<<endl;
    // }

    // Check availability of all members
    bool bisa=1;
    // cout<<"banyak anggota "<<numMembers<<endl;
    for (int i = 0; i < newMeeting.numMembers; ++i) {
        string memberName = newMeeting.members[i];
        // cout<<"i: "<<memberName<<endl;
        bool memberFound = false;
        for (int j = 0; j < numMembers; ++j) {

            // cout<<"debug i: "<<i<<" j: "<<j<<" "<<memberSchedules[j].name<<" "<<memberName<<endl;

            if (memberSchedules[j].name == memberName) {
                memberFound = true;
                for (int k = newMeeting.startHour; k < newMeeting.endHour; ++k) {
                    // cout<<"cek avail"<<memberName<<" "<<memberSchedules[j].available[newMeeting.date][k]<<endl;
                    if (!memberSchedules[j].available[newMeeting.date][k]) {
                        cout << "Anggota " << memberName << " tidak tersedia di tanggal " << newMeeting.date << " pada jam " << k << ":00.\n";
                        bisa = 0;
                        // return;
                    }
                }
                break;
            }
        }
        if (!memberFound) {
            cout << "Anggota " << memberName << " tidak ditemukan dalam jadwal anggota.\n";
            return;
        }
    }



    if(bisa){
        // Menandai ketersediaan anggota
        for (int i = 0; i < newMeeting.numMembers; ++i) {
            string memberName = newMeeting.members[i];
            for (int j = 0; j < numMembers; ++j) {
                if (memberSchedules[j].name == memberName) {
                    for (int k = newMeeting.startHour; k < newMeeting.endHour; ++k) {
                        memberSchedules[j].available[newMeeting.date][k] = false;
                    }
                }
            }
        }
        meetings[numMeetings] = newMeeting;
        ++numMeetings;
        cout << "Rapat berhasil ditambahkan!\n"; 
    }
    else{
        cout << "Rapat gagal ditambahkan!\n";
    }
    
}

void displayMeetings(const Meeting meetings[], int numMeetings) {
    if (numMeetings == 0) {
        cout << "Tidak ada rapat yang dijadwalkan.\n";
        return;
    }

    cout << "\nJadwal Rapat: \n";
    for (int i = 0; i < numMeetings; ++i) {
        cout << i + 1 << ". " << meetings[i].title << " - " << 
        " pada tanggal " << meetings[i].date << " pukul " <<meetings[i].startHour << ":00 sampai " << meetings[i].endHour << ":00\n";
        cout << "   Anggota yang hadir: ";
        for (int j = 0; j < meetings[i].numMembers; ++j) {
            cout << meetings[i].members[j];
            if (j < meetings[i].numMembers - 1) {
                cout << ", ";
            }
        }
        cout << "\n";
    }
}

int main() {
    Meeting meetings[MAX_MEETINGS];
    MemberSchedule memberSchedules[MAX_MEMBERS];
    int numMeetings = 0;
    int numMembers;

    cout << "Masukkan jumlah anggota organisai: ";
    cin >> numMembers;
    cin.ignore();
    for (int i = 0; i < numMembers; ++i) {
        cout << "Masukkan nama anggota " << i + 1 << ": ";
        getline(cin, memberSchedules[i].name);
    }
    // for(int i = 0; i < numMembers; i++) {
    //     cout<<memberSchedules[i].name<<endl;
    // }

    initializeAvailability(memberSchedules, numMembers);

    int choice;
    do {
        cout << "\nPenentu Jadwal Rapat\n";
        cout << "1. Tambah Rapat\n";
        cout << "2. Lihat Semua Rapat\n";
        cout << "3. Keluar\n";
        cout << "Pilihan: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addMeeting(meetings, numMeetings, memberSchedules, numMembers);
                break;
            case 2:
                displayMeetings(meetings, numMeetings);
                break;
            case 3:
                cout << "Keluar dari program.\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (choice != 3);

	return 0;
}