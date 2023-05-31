#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100


struct node {
    char hastaNo[5];
    char hastaAdi[30];
    char hastaYasi[3];
    char cinsiyet[6];
    int mahkumlukDurumu;
    char engellilikOrani[4];
    char kanamaliHastaDurumu[11];
    char hastaKayitSaati[6];
    char muayeneSaati[6];
    int muayeneSuresi;
    int oncelikPuani;
    struct node* next;
};

struct node* Hasta = NULL;
struct node *Kayitlilar;
float MuayeneSaati=9.0;
float bSaat=8.0;
int muayenesirasi=1;

float Donustur(char str[]) {
    float sayi = atof(str);
    return sayi;
}
int DSayi(char str[]){
	int sayi = atoi(str);
	return sayi;
}



struct node* SiraliEkle(struct node* r, char a[5], char b[30], char c[3], char d[6], char e[5], char f[4], char g[11], char h[6], char k[6]) {
    struct node* eklenecek = (struct node*)malloc(sizeof(struct node));
    strcpy(eklenecek->hastaNo, a);
    strcpy(eklenecek->hastaAdi, b);
    strcpy(eklenecek->hastaYasi, c);
    strcpy(eklenecek->cinsiyet, d);
    eklenecek->mahkumlukDurumu=(strcmp(e,"true ")==0) ? 1 : 0 ;
    strcpy(eklenecek->engellilikOrani, f);
    strcpy(eklenecek->kanamaliHastaDurumu, g);
    strcpy(eklenecek->hastaKayitSaati, h);
    strcpy(eklenecek->muayeneSaati, k);
    eklenecek->oncelikPuani=OncelikPuaniHesapla(eklenecek);
    eklenecek->muayeneSuresi=MuayeneSuresiHesapla(eklenecek);
    eklenecek->next = NULL;

    if (r == NULL) {
        return eklenecek;
    }

    if (Donustur(r->hastaKayitSaati) > Donustur(h)) {
        eklenecek->next = r;
        return eklenecek;
    }

    struct node* iter = r;
    while (iter->next != NULL && Donustur(iter->next->hastaKayitSaati) < Donustur(h)) {
        iter = iter->next;
    }

    eklenecek->next = iter->next;
    iter->next = eklenecek;
    return r;
}

void HastaDosyaOku() {
    FILE* dosya;
    char satir[MAX_LINE_LENGTH];

    if ((dosya = fopen("Hasta.txt", "r")) == NULL) {
        printf("Dosya acilamadi!\n");
        exit(1);
    }
    else {
        printf("HastaNo\tHastaAdi\tHastaY.\tC  MahkumD.  EngellilikO.  KanamaD.\tKayitS.\tM.Saati M.Sure OncelikP.\n");
        while (fgets(satir, MAX_LINE_LENGTH, dosya)) {
            char* token;
            token = strtok(satir, ",");
            char a[5];
            strcpy(a, token);
            token = strtok(NULL, ",");
            char b[30];
            strcpy(b, token);
            token = strtok(NULL, ",");
            char c[3];
            strcpy(c, token);
            token = strtok(NULL, ",");
            char d[6];
            strcpy(d, token);
            token = strtok(NULL, ",");
            char e[5];
            strcpy(e,token);
            token = strtok(NULL, ",");
            char f[4];
            strcpy(f, token);
            token = strtok(NULL, ",");
            char g[11];
            strcpy(g, token);
            token = strtok(NULL, ",");
            char h[6];
            strcpy(h, token);
            token = strtok(NULL, ",");
            char k[6];
            strcpy(k, token);
            token = strtok(NULL, ",");

            if (strcmp(a, "*") == 0) strcpy(a, "");

            if (strcmp(k, "*") == 0) strcpy(k, "");
           

            Hasta = SiraliEkle(Hasta, a, b, c, d, e, f, g, h, k);
        }
        fclose(dosya);
    }
}

int OncelikPuaniHesapla(struct node* r){
	struct node* iter1 = (struct node*)malloc(sizeof(struct node));
	iter1 = r;
    	int toplam=0;
    	
    	if(DSayi(iter1->hastaYasi)<5){
    		toplam=toplam+20;
		}
		else if(DSayi(iter1->hastaYasi)<45){
			toplam=toplam+0;
		}
		else if(DSayi(iter1->hastaYasi)<65){
			toplam=toplam+15;
		}
        else{
        	toplam=toplam+25;
		}
		if(iter1->mahkumlukDurumu==1){
			toplam=toplam+50;
		}
		if(iter1->kanamaliHastaDurumu=="agirKanama"){
			toplam=toplam+50;
		}
		else if(iter1->kanamaliHastaDurumu=="kanama    "){
			toplam=toplam+20;
		}
		toplam=toplam+atof(iter1->engellilikOrani)/4;
		
		return toplam;
		
    
    
}

void saatEkle(float saat, int eklenecekDakika) {
    int saatKisim = (int)saat;  // Saat k�sm�n� al
    int dakikaKisim = (int)((saat - saatKisim) * 100);  // Dakika k�sm�n� al

    dakikaKisim += eklenecekDakika;  // Dakikay� ekleyerek g�ncelle

    // Saati ve dakikay� yeniden hesapla
    saatKisim += dakikaKisim / 60;
    dakikaKisim %= 60;

    MuayeneSaati = saatKisim + ((float)dakikaKisim / 100);  // Yeni saat de�erini olu�tur

}

void HastaMuayeneYap(struct node *MuayeneHastasi) {
	//sprintf(MuayeneHastasi->muayeneSaati, "%.2f", MuayeneSaati);

    printf("\n%d. sirada %s numarali %s adli hasta muayene olmustur.\n", muayenesirasi, MuayeneHastasi->hastaNo, MuayeneHastasi->hastaAdi);
    printf("Oncelik Puani: %d   Muayene suresi: %d  Muayene Giris Saati:%.2f", MuayeneHastasi->oncelikPuani, MuayeneHastasi->muayeneSuresi, MuayeneSaati);
    muayenesirasi++;
}



void Sirala() {
    struct node* gecici = NULL; // Ge�ici bir d���m i�aret�isi olu�turulur
    struct node* suanki = NULL; // �u anki d���m i�aret�isi olu�turulur
    int degisiklik;

    degisiklik = 1; // De�i�iklik olup olmad���n� kontrol etmek i�in bir de�i�ken
    while (degisiklik) {
        degisiklik = 0; // Her d�ng� ba��nda de�i�iklik yok varsay�l�r
        suanki = Kayitlilar; // �u anki d���m, Kayitlilar i�aret�isiyle ba�lat�l�r

        while (suanki->next != gecici) {
            if (suanki->oncelikPuani < suanki->next->oncelikPuani) {
                // E�er �u anki d���m�n �ncelik puan� bir sonraki d���m�n �ncelik puan�ndan k���kse
                // Hasta d���mleri yer de�i�tirir
                struct node* geciciDugum = suanki->next;
                suanki->next = suanki->next->next;
                geciciDugum->next = suanki;

                if (suanki == Kayitlilar) {
                    Kayitlilar = geciciDugum; // Yeni ba�lang�� d���m� ayarlan�r
                }
                else {
                    struct node* onceki = Kayitlilar;
                    while (onceki->next != suanki) {
                        onceki = onceki->next;
                    }
                    onceki->next = geciciDugum; // �nceki d���m�n ba�lant�s� g�ncellenir
                }

                degisiklik = 1; // Yap�lan bir de�i�iklik oldu�unu belirtir
            }
            else {
                suanki = suanki->next; // S�radaki d���me ilerler
            }
        }
        gecici = suanki; // En son i�lem g�ren d���m� ge�ici olarak kaydediyoruz
    }
}




void HastaKayitEkle(struct node* r) {
    struct node *yedek3; // Kay�t edilen hastalar aras�ndaki �nceli�i en d���k olan hasta
    int hastasayisi = 0; // Kay�t edilen hasta say�s�n� tutan de�i�ken
    

    while (r->next != NULL) {
        if (Kayitlilar == NULL) {
            hastasayisi++;
            Kayitlilar = r; // �lk kay�t, Kayitlilar olarak atan�r
            yedek3 = r; // Son kay�t olarak atan�r
            bSaat = Donustur(r->hastaKayitSaati); // Ba�lang�� saatini ayarla
            r = r->next; // Bir sonraki kay�ta ilerle
            // ��lem tamamland�ktan sonra d�ng�den ��k
        } else if (Donustur(r->hastaKayitSaati) >= MuayeneSaati) {
           
            HastaMuayeneYap(Kayitlilar); // Hastay� muayene et
            saatEkle(MuayeneSaati, Kayitlilar->muayeneSuresi); // Muayene s�resini ekle
            Kayitlilar = Kayitlilar->next; // Kay�t listesinde bir sonraki d���me ilerle
        } else {
            hastasayisi++;
            yedek3 = Kayitlilar;
            while (yedek3->next != NULL)
                yedek3 = yedek3->next;
            yedek3->next = r; // Yeni kayd� sona ekle
            bSaat = Donustur(r->hastaKayitSaati); // Ba�lang�� saatini ayarla
            yedek3 = yedek3->next; // Son hasta olarak de�i�tir
            r = r->next; // S�radaki hastay� s�raya ekle
            yedek3->next = NULL; // Son hastan�n bir sonraki d���me i�aret etmesini engelle
            Sirala(); // �ncelik s�ralamas�n� yap
            if (hastasayisi == 40)
                break; // Kaydedilen hasta say�s� 40'a ula�t���nda d�ng�den ��k
        }
    }

    while (Kayitlilar != NULL) {
        HastaMuayeneYap(Kayitlilar); // Hastay� muayene et
        saatEkle(MuayeneSaati, Kayitlilar->muayeneSuresi); // Muayene s�resini ekle
        Kayitlilar = Kayitlilar->next; // Kay�t listesinde bir sonraki d���me ilerle
    }

    printf("\n\n \t\tMUAYENE SONA ERMISTIR");
}




int MuayeneSuresiHesapla(struct node* r){
	struct node* iter1 = (struct node*)malloc(sizeof(struct node));
	iter1 = r;
    	int toplam=0;
    	
    	if(DSayi(iter1->hastaYasi)>=65){
    		toplam=toplam+15;
		}
		if(iter1->kanamaliHastaDurumu=="agirKanama"){
			toplam=toplam+20;
		}
		else if(iter1->kanamaliHastaDurumu=="kanama    "){
			toplam=toplam+10;
		}
		toplam=toplam+atof(iter1->engellilikOrani)/5;
		
		return toplam+10;
}



void yazdir() {
	
    struct node* iter = Hasta;
	int no=1;
    while (iter != NULL) {
    	sprintf(iter->hastaNo,"%d",no);
    	no++;
        printf("%s\t%s\t%s\t%s\t%d\t%s\t   %s\t%s\t%s\t%d\t%d\n", iter->hastaNo, iter->hastaAdi, iter->hastaYasi, iter->cinsiyet, iter->mahkumlukDurumu, iter->engellilikOrani, iter->kanamaliHastaDurumu, iter->hastaKayitSaati, iter->muayeneSaati, iter->muayeneSuresi, iter->oncelikPuani);
        iter = iter->next;
    }
    
}

int main(int argc, char* argv[]) {
    HastaDosyaOku();
    
    yazdir();
    

    HastaKayitEkle(Hasta);
    
    return 0;
}













