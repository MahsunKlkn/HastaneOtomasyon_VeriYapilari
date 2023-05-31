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
    int saatKisim = (int)saat;  // Saat kýsmýný al
    int dakikaKisim = (int)((saat - saatKisim) * 100);  // Dakika kýsmýný al

    dakikaKisim += eklenecekDakika;  // Dakikayý ekleyerek güncelle

    // Saati ve dakikayý yeniden hesapla
    saatKisim += dakikaKisim / 60;
    dakikaKisim %= 60;

    MuayeneSaati = saatKisim + ((float)dakikaKisim / 100);  // Yeni saat deðerini oluþtur

}

void HastaMuayeneYap(struct node *MuayeneHastasi) {
	//sprintf(MuayeneHastasi->muayeneSaati, "%.2f", MuayeneSaati);

    printf("\n%d. sirada %s numarali %s adli hasta muayene olmustur.\n", muayenesirasi, MuayeneHastasi->hastaNo, MuayeneHastasi->hastaAdi);
    printf("Oncelik Puani: %d   Muayene suresi: %d  Muayene Giris Saati:%.2f", MuayeneHastasi->oncelikPuani, MuayeneHastasi->muayeneSuresi, MuayeneSaati);
    muayenesirasi++;
}



void Sirala() {
    struct node* gecici = NULL; // Geçici bir düðüm iþaretçisi oluþturulur
    struct node* suanki = NULL; // Þu anki düðüm iþaretçisi oluþturulur
    int degisiklik;

    degisiklik = 1; // Deðiþiklik olup olmadýðýný kontrol etmek için bir deðiþken
    while (degisiklik) {
        degisiklik = 0; // Her döngü baþýnda deðiþiklik yok varsayýlýr
        suanki = Kayitlilar; // Þu anki düðüm, Kayitlilar iþaretçisiyle baþlatýlýr

        while (suanki->next != gecici) {
            if (suanki->oncelikPuani < suanki->next->oncelikPuani) {
                // Eðer þu anki düðümün öncelik puaný bir sonraki düðümün öncelik puanýndan küçükse
                // Hasta düðümleri yer deðiþtirir
                struct node* geciciDugum = suanki->next;
                suanki->next = suanki->next->next;
                geciciDugum->next = suanki;

                if (suanki == Kayitlilar) {
                    Kayitlilar = geciciDugum; // Yeni baþlangýç düðümü ayarlanýr
                }
                else {
                    struct node* onceki = Kayitlilar;
                    while (onceki->next != suanki) {
                        onceki = onceki->next;
                    }
                    onceki->next = geciciDugum; // Önceki düðümün baðlantýsý güncellenir
                }

                degisiklik = 1; // Yapýlan bir deðiþiklik olduðunu belirtir
            }
            else {
                suanki = suanki->next; // Sýradaki düðüme ilerler
            }
        }
        gecici = suanki; // En son iþlem gören düðümü geçici olarak kaydediyoruz
    }
}




void HastaKayitEkle(struct node* r) {
    struct node *yedek3; // Kayýt edilen hastalar arasýndaki önceliði en düþük olan hasta
    int hastasayisi = 0; // Kayýt edilen hasta sayýsýný tutan deðiþken
    

    while (r->next != NULL) {
        if (Kayitlilar == NULL) {
            hastasayisi++;
            Kayitlilar = r; // Ýlk kayýt, Kayitlilar olarak atanýr
            yedek3 = r; // Son kayýt olarak atanýr
            bSaat = Donustur(r->hastaKayitSaati); // Baþlangýç saatini ayarla
            r = r->next; // Bir sonraki kayýta ilerle
            // Ýþlem tamamlandýktan sonra döngüden çýk
        } else if (Donustur(r->hastaKayitSaati) >= MuayeneSaati) {
           
            HastaMuayeneYap(Kayitlilar); // Hastayý muayene et
            saatEkle(MuayeneSaati, Kayitlilar->muayeneSuresi); // Muayene süresini ekle
            Kayitlilar = Kayitlilar->next; // Kayýt listesinde bir sonraki düðüme ilerle
        } else {
            hastasayisi++;
            yedek3 = Kayitlilar;
            while (yedek3->next != NULL)
                yedek3 = yedek3->next;
            yedek3->next = r; // Yeni kaydý sona ekle
            bSaat = Donustur(r->hastaKayitSaati); // Baþlangýç saatini ayarla
            yedek3 = yedek3->next; // Son hasta olarak deðiþtir
            r = r->next; // Sýradaki hastayý sýraya ekle
            yedek3->next = NULL; // Son hastanýn bir sonraki düðüme iþaret etmesini engelle
            Sirala(); // Öncelik sýralamasýný yap
            if (hastasayisi == 40)
                break; // Kaydedilen hasta sayýsý 40'a ulaþtýðýnda döngüden çýk
        }
    }

    while (Kayitlilar != NULL) {
        HastaMuayeneYap(Kayitlilar); // Hastayý muayene et
        saatEkle(MuayeneSaati, Kayitlilar->muayeneSuresi); // Muayene süresini ekle
        Kayitlilar = Kayitlilar->next; // Kayýt listesinde bir sonraki düðüme ilerle
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













