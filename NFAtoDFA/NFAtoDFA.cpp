#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <string>
using namespace std;

map<string, vector<pair<string, string>>> nfa; // NFA yapisi
set<string> nfaKabulDurumlari;
vector<string> gecisAdlari;
string baslangicDurumu;

// Bir durumun epsilon kapanimını hesaplayan fonksiyon
set<string> epsilonKapanimi(string durum, map<string, vector<pair<string, string>>> &gecisler) {
    set<string> kapanim;
    queue<string> kuyruk;
    kuyruk.push(durum);
    kapanim.insert(durum);

    while (!kuyruk.empty()) {
        string mevcut = kuyruk.front(); kuyruk.pop();
        for (auto &gecis : gecisler[mevcut]) {
            if (gecis.first == "bosluk" && kapanim.find(gecis.second) == kapanim.end()) {
                kapanim.insert(gecis.second);
                kuyruk.push(gecis.second);
            }
        }
    }
    return kapanim;
}

// NFA'dan DFA'ya dönüştüren fonksiyon
void NFAyiDFACevir(map<string, vector<pair<string, string>>> &nfa, set<string> &nfaKabulDurumlari, string baslangicDurumu, vector<string> &gecisAdlari) {
    map<set<string>, map<string, set<string>>> dfaGecisler; // DFA gecisleri
    set<set<string>> dfaDurumlari;
    queue<set<string>> durumKuyrugu;
    set<string> dfaBaslangicDurumu = epsilonKapanimi(baslangicDurumu, nfa);
    durumKuyrugu.push(dfaBaslangicDurumu);
    dfaDurumlari.insert(dfaBaslangicDurumu);

    while (!durumKuyrugu.empty()) {
        set<string> mevcutDurum = durumKuyrugu.front(); durumKuyrugu.pop();
        for (string gecisAdi : gecisAdlari) {
            set<string> yeniDurum;
            for (string durum : mevcutDurum) {
                for (auto &gecis : nfa[durum]) {
                    if (gecis.first == gecisAdi) {
                        set<string> kapanim = epsilonKapanimi(gecis.second, nfa);
                        yeniDurum.insert(kapanim.begin(), kapanim.end());
                    }
                }
            }
            if (!yeniDurum.empty() && dfaDurumlari.find(yeniDurum) == dfaDurumlari.end()) {
                dfaDurumlari.insert(yeniDurum);
                durumKuyrugu.push(yeniDurum);
            }
            dfaGecisler[mevcutDurum][gecisAdi] = yeniDurum;
        }
    }

    // DFA geçişlerini ekrana yazdırma
    cout << "\nDFA Gecisleri:" << endl;
    for (auto &durum : dfaGecisler) {
        for (auto &gecis : durum.second) {
            cout << "{";
            for (auto it = durum.first.begin(); it != durum.first.end(); ++it) {
                if (it != durum.first.begin()) cout << ", ";
                cout << *it;
            }
            cout << "} ---'" << gecis.first << "'---> {";
            for (auto it = gecis.second.begin(); it != gecis.second.end(); ++it) {
                if (it != gecis.second.begin()) cout << ", ";
                cout << *it;
            }
            cout << "}" << endl;
        }
    }
}

int main() {
    int durumSayisi, gecisSayisi, kabulDurumSayisi;

    cout << "NFA'daki durum sayisini girin: ";
    cin >> durumSayisi;

    cout << "NFA'daki gecis sayisini girin: ";
    cin >> gecisSayisi;

    cout << "Gecisleri girin (baslangic_durumu gecis_adi hedef_durum):\n";
    for (int i = 0; i < gecisSayisi; ++i) {
        string baslangic, hedef, gecisAdi;
        cin >> baslangic >> gecisAdi >> hedef;
        nfa[baslangic].push_back({gecisAdi, hedef});
        if (gecisAdi != "bosluk") {
            gecisAdlari.push_back(gecisAdi);
        }
    }

    cout << "Baslangic durumunu girin: ";
    cin >> baslangicDurumu;

    cout << "Kabul durumlarinin sayisini girin: ";
    cin >> kabulDurumSayisi;

    cout << "Kabul durumlarini girin:\n";
    for (int i = 0; i < kabulDurumSayisi; ++i) {
        string kabulDurumu;
        cin >> kabulDurumu;
        nfaKabulDurumlari.insert(kabulDurumu);
    }

    // Benzersiz geçiş adlarını tutmak için set kullanıyoruz
    set<string> gecisAdiSet(gecisAdlari.begin(), gecisAdlari.end());
    gecisAdlari.assign(gecisAdiSet.begin(), gecisAdiSet.end());

    cout << "NFA'dan DFA'ya donusturuluyor...\n";
    NFAyiDFACevir(nfa, nfaKabulDurumlari, baslangicDurumu, gecisAdlari);

    return 0;
}
