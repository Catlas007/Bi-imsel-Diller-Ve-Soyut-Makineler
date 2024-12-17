#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
using namespace std;

// Bir durumun epsilon kapanımını hesaplayan fonksiyon
set<int> epsilonKapanimi(int durum, map<int, vector<pair<string, int>>> &gecisler) {
    set<int> kapanim;
    queue<int> kuyruk;
    kuyruk.push(durum);
    kapanim.insert(durum);

    while (!kuyruk.empty()) {
        int mevcut = kuyruk.front(); kuyruk.pop();
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
void NFAyiDFACevir(map<int, vector<pair<string, int>>> &nfa, set<int> &nfaKabulDurumlari, int baslangicDurumu, vector<string> &gecisAdlari) {
    map<set<int>, map<string, set<int>>> dfaGecisler; // DFA gecisleri
    set<set<int>> dfaDurumlari;
    queue<set<int>> durumKuyrugu;
    set<int> dfaBaslangicDurumu = epsilonKapanimi(baslangicDurumu, nfa);
    durumKuyrugu.push(dfaBaslangicDurumu);
    dfaDurumlari.insert(dfaBaslangicDurumu);

    while (!durumKuyrugu.empty()) {
        set<int> mevcutDurum = durumKuyrugu.front(); durumKuyrugu.pop();
        for (string gecisAdi : gecisAdlari) {
            set<int> yeniDurum;
            for (int durum : mevcutDurum) {
                for (auto &gecis : nfa[durum]) {
                    if (gecis.first == gecisAdi) {
                        set<int> kapanim = epsilonKapanimi(gecis.second, nfa);
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
            cout << "{'";
            for (int d : durum.first) cout << d;
            cout << "'} ---'" << gecis.first << "'---> {'";
            for (int d : gecis.second) cout << d;
            cout << "'}" << endl;
        }
    }
}

int main() {
    map<int, vector<pair<string, int>>> nfa;
    set<int> nfaKabulDurumlari;
    vector<string> gecisAdlari;
    int baslangicDurumu;
    int durumSayisi, gecisSayisi, kabulDurumSayisi;

    cout << "NFA'daki durum sayisini girin: ";
    cin >> durumSayisi;

    cout << "NFA'daki gecis sayisini girin: ";
    cin >> gecisSayisi;

    cout << "Gecisleri girin (baslangic_durumu gecis_adi hedef_durum):\n";
    for (int i = 0; i < gecisSayisi; ++i) {
        int baslangic, hedef;
        string gecisAdi;
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
        int kabulDurumu;
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
