#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define M 997	//hashtable size icin 1000e en yakin asal sayi secildi
#define MM 996
#define MAX_WORD_LENGTH 25
#define BUFFER_SIZE 150
#define MAX_DIST 2			//max k degerimiz 2 olacak

typedef struct oneri {
    char oneriAdi[MAX_WORD_LENGTH];
    struct oneri *next;
} oneri;

typedef struct Kelime{
	char word[MAX_WORD_LENGTH];		//key olarak
	oneri *oneriRoot; //ilgili kelimenin icinde oldugu onerilari tutan linked list yapisi
} Kelime;

FILE *openFile(char *fileName);
//Basarili olunduysa file pointeri return et, yoksa null dondur
int searchHash(char *word,Kelime *hashTable[]);
//Kelime hashtable'da varsa indexini yoksa 0 dondur
int hornerMethod(char *in);
//Verilen bir string degerinin hashing icin key degerini cikartir
int loadFile(Kelime *hashTable[]);
//Istenilen txt'nin icindeki kelimeleri hashtable'a ekle
int hashCode(int key, int i);
//Verilen kelimenin key degerini alip indexini double-hashing ile return eder
int hash1(int key);
int hash2(int key);
int min(int a, int b, int c);
//Double hashing icin fonksiyonlar
void listDocuments(char *word,Kelime *hashTable[]);
//Hata tablosundaki kelime icin onceki onerilen kelimeyi basar
void listLinkedList(oneri *root);
void pushEnd(oneri *ekle,oneri **root);
int insertKelime(char *word,Kelime *hashTable[]);
//Bir kelimeyi ilgili hashtablea eklemek icin kullanilir
void oneriEkle(char *oneriAdi, Kelime* kelime);
//Kelimenin onerilar LL'sinde ilgili oneri yoksa LL'ye oneri ismini ekleme islemi, varsa bir sey yapmaz
char *convertToLower(char *word);
//Collision'u engellemek icin okunulan kelimeyi tamamen lowercase hale getirip oyle hashcode'unu cikaracagiz
int levenDist(char *word1, char *word2);
//Verilen iki kelime arasýndaki Edit Distance'ý 2'den büyük deðilse return eder, büyükse 999 return eder

int main(){
	int i,choice=1,var = 0,index;	
	FILE *fp;
	Kelime *sozlukTablo[M],*hataliTablo[M];
	char *kelimeBol,cumle[100],temp[MAX_WORD_LENGTH],inputKel[MAX_WORD_LENGTH];
	
	for(i=0; i<M; i++){
		sozlukTablo[i] = (Kelime*)malloc(sizeof(Kelime));
		strcpy(sozlukTablo[i]->word,"bosbos");		//hashTable initializing
		sozlukTablo[i]->oneriRoot = NULL; 		//bosbos stringi ilgili kýsma herhangi bir kelime eklenmedigini
	} 											//belirtiyor
	
	for(i=0; i<M; i++){
		hataliTablo[i] = (Kelime*)malloc(sizeof(Kelime));
		strcpy(hataliTablo[i]->word,"bosbos");	
		hataliTablo[i]->oneriRoot = NULL;
	}
		
	if(loadFile(sozlukTablo) == 1){	//smallDictionary.txt'nin hashe alýnmasý	
		system("CLS");			//program ile ayný dosyada olmalý
		printf("Sozluk yuklendi\n");
		while(choice==1){
			printf("Cumlenizi giriniz:\n-----------------\n");
			fgets (cumle, 100, stdin);
			printf("\n");
			printf("\n");
			printf("\n");
			kelimeBol = strtok(cumle," \n");
			while(kelimeBol != NULL){
				strcpy(temp,kelimeBol);
				if(searchHash(temp,sozlukTablo) == -1){
					if(searchHash(temp,hataliTablo) == -1){	//iki tabloda da yoksa
						printf("%s sozlukte yok. Bunu mu demek istediniz:\n-----------------\n",temp);
						for(i=0; i<M; i++){
							if(strcmp(sozlukTablo[i]->word,"bosbos") != 0){
								if(levenDist(temp, sozlukTablo[i]->word) < MAX_DIST){
									printf("%s\n",sozlukTablo[i]->word);
									var = 1;					//eger dist 1 olan varsa 2yi gostermeye gerek yok
								}
							}
						}
						if(var != 1){							//1 yoksa 2leri arayip goster
							for(i=0; i<M; i++){
								if(strcmp(sozlukTablo[i]->word,"bosbos") != 0){
									if(levenDist(temp, sozlukTablo[i]->word) == MAX_DIST){
										printf("%s\n", sozlukTablo[i]->word);
										var=2;
									}
								}
							}	
						}
						printf("\n");
						if(var == 0){		//eger 1 ve 2 dist'a sahip kelime bulunamadiysa belirt
							printf("Ýlgili kelime icin onerilecek kelime bulunamadi.\n-----------------\n");
						}
						else{
							printf("Sectiginiz kelimeyi yaziniz:\n-----------------\n");
							scanf("%s",inputKel);
							index = insertKelime(temp,hataliTablo);	//verilen onerilerden kullanicinin
							oneriEkle(inputKel,hataliTablo[index]);	//sectigi kelimeyi oneri olarak ekle
							printf("Secilen oneri kelimeye eklendi\n-----------------\n");
						}											//kelimeyi hataliTabloya ekle
					}			
					else{		//hataliTabloda var
						printf("%s icin HataliTabloda daha once onerilen:\n",temp);	
						listDocuments(temp,hataliTablo);	////daha once onerilmis kelimeleri kullanýcýya goster
						printf("-----------------\n\n");
					}
				}
				else{
					//printf("sozlukTabloda var\n");	//bir sey yapýlmayacak
				}
				kelimeBol = strtok(NULL," \n");	
				var = 0;
			}
			printf("Cikmak icin 0'i devam icin 1i tuslayiniz:\n");
			scanf("%d",&choice);
			fseek(stdin,0,SEEK_END); //stdin bufferýný temizleme
		}    
	}
	
			
	return 0;
}

FILE *openFile(char *fileName){
	FILE* fp;
	fp = fopen(fileName,"r");
	if(fp != NULL){
		return fp;
	}
	else{
		//printf("Dosya acilamadi\n");
		return NULL;
	}
}
char *convertToLower(char *word){
	int i,n;
	char *ret;
	n =strlen(word);
	ret = (char*)malloc(n*sizeof(char));	
	for(i=0; i<n+1; i++){
		ret[i] = tolower(word[i]);
	}	
	return ret;
}

int hornerMethod(char *in){
	int i,hash=0;
	int n = strlen(in);
	char *word = convertToLower(in);
	for(i=0; i<n; i++){
		hash = (2*hash + (word[i]-'A'+1));
	}
	return hash % M;
}
int hash1(int key){
	return key % M;
}
int hash2(int key){
	return 1 + (key % MM);
}
int hashCode(int key, int i){
	int ret;
	ret=(hash1(key) + i*hash2(key))%M;
	return ret;
} 

int min(int a, int b, int c){
	if(a<b && a<c){
		return a;
	}
	else if(b<a && b<c){
		return b;
	}
	else{
		return c;
	}	
}

void matrixBas(int len1, int len2, int **arr){
	int i,j;
	for(i=0; i<len1; i++){
		for(j=0; j<len2; j++){
			printf("%d	",arr[i][j]);
		}
		printf("\n\n\n");
	}
}

int levenDist(char *word1, char *word2){
	int len1,len2,i,j,dist,mini,insert,del,chg,thre,kontrol;
	char c1,c2;
	len1 = strlen(word1)+1;		//0 de
	len2 = strlen(word2)+1;
	if(abs(len1-len2) > MAX_DIST){
		//printf("Oneri yok\n");
		return 999;
	}	
	int **LED=(int**)malloc(len1*sizeof(int*));	//satirlar
	for(i=0;i<len1;i++){ 
		LED[i]=(int*)malloc(len2*sizeof(int));  //sutunlar
	}
	if(LED == NULL){
		printf("\nLED icin yer ayrilamadi, cikiliyor..");
		exit(0);
	}
	for(i=0; i<len1; i++){
		if(i<MAX_DIST+1)
			LED[i][0] = i;
		else
			LED[i][0] = 999;
	}	
	for(j=0; j<len2; j++){	//ön koþullar
		if(j<MAX_DIST+1)
			LED[0][j] = j;
		else
			LED[0][j] = 999;
	}
	for(i=1; i<len1; i++){
		kontrol = 1;
		for(j=1; j<len2; j++){
			if(!(abs(i-j) > MAX_DIST)){	
				if(word1[i-1] == word2[j-1]){	//esitlerse distance artmayacak
					LED[i][j]= LED[i-1][j-1];	//diagonal'dan direkt gelecek deger
				}
				else{
					insert = LED[i-1][j] + 1;
					del = LED[i][j-1] + 1;		//esit olmama costu her durum için 1 seçildi
					chg = LED[i-1][j-1] + 1;
					mini = min(insert,del,chg);
					LED[i][j] = mini;
				}
			}
			else{
				LED[i][j] = 999;	//999 olarak isaretlenen indexler max distance'tan büyük
			}						//deðerleri her zaman içerecekleri için önemsizler
		}							//sonuca etki etmiyorlar
		while(j-1 >= 0){
			if(LED[i][j-1] != 999 && LED[i][j-1] <= MAX_DIST){
				kontrol = 0;		//yani bulundugumuz son satirda max_distten kucuk/esit bi deger var
			}
			j--;
		}
		if(kontrol == 1){	//bulundugumuz son satirdaki önemli bütün degerler max_distten büyük
			//printf("%d .satirda cikis yapildi.Kriter disi kelime cifti\n",i);	
			return 999;
		}
	}
	//matrixBas(len1,len2,LED);
	dist = LED[len1-1][len2-1];		
	return dist;
}
void pushEnd(oneri *ekle,oneri **root){
	oneri *cur = *root;	
	while(cur->next != NULL){
		cur = cur->next;
	}
	cur->next = ekle;
	cur->next->next = NULL;
	
}
void oneriEkle(char *oneriAdi, Kelime* kelime){
	oneri *ekle;
	oneri *curr = kelime->oneriRoot;
	if(curr == NULL){
		kelime->oneriRoot = (oneri*)malloc(sizeof(oneri));
		if(kelime->oneriRoot == NULL){
			printf("Couldnt allocate memory");
			return;
		}
		strcpy(kelime->oneriRoot->oneriAdi,oneriAdi);
		kelime->oneriRoot->next = NULL;
	}
	else
	{	
		ekle = (oneri*)malloc(sizeof(oneri));
		strcpy(ekle->oneriAdi,oneriAdi);
		pushEnd(ekle,&(kelime->oneriRoot));
	}
}
int searchHash(char *word,Kelime *hashTable[]){
	int key = hornerMethod(word);
	char *ara;
	ara = convertToLower(word);
	int i = 0;
	while(i<M && strcmp(hashTable[hashCode(key,i)]->word,"bosbos") != 0){
		if(strcmp(hashTable[hashCode(key,i)]->word,ara) == 0){
			//printf("%s bulundu\n",ara);
			return hashCode(key,i);
		}					
		i++;
	} 
	//printf("Hashte bulunamadi \n");
	return -1;
}
int insertKelime(char *word,Kelime *hashTable[]){
	int key,adr,i=0;
	char *eklencek;
	eklencek = convertToLower(word);
	key = hornerMethod(word);
	adr = hashCode(key,i);
	while(i<M && (strcmp(hashTable[adr]->word,"bosbos") != 0)){
		++i;
		adr = hashCode(key,i);
	}
	strcpy(hashTable[adr]->word,eklencek);
	return adr;
}
void listLinkedList(oneri *root){
	oneri *cur = root;
	while(cur != NULL){
		printf("%s ",cur->oneriAdi);
		cur = cur->next;
	}
}

void listDocuments(char *word, Kelime *hashTable[]){
	int i = searchHash(word,hashTable);
	int key = hornerMethod(word);
	if(i != -1){
		listLinkedList(hashTable[i]->oneriRoot);
		printf("\n");
	}
	else{
		printf("Boyle bir kelime mevcut degil \n");
	}
}

int loadFile(Kelime *hashTable[]){
	FILE *fp1;
	fp1 = openFile("smallDictionary.txt");
	if(fp1 != NULL){
		char word[MAX_WORD_LENGTH];
		while(fscanf(fp1,"%s ",word) == 1){		//dosyadaki her kelimeyi ht'ye at
			if(searchHash(word, hashTable) == -1){
			insertKelime(word,hashTable);
			printf("%s sozluge yuklendi\n",word);	
			}			
		}
		fclose(fp1);			
		return 1;
	}	
	else{
		printf("Sozluk yuklenemedi,tekrar deneyiniz\n");
		return 0;
	}	
}





