#include<stdio.h>
#include<stdlib.h>
double seriKazanma(int n,double p){
	int i,j;								//n+1 ,initial conditionlarla beraber
	double q = 1-p;							//sat�r s�tun say�s�
	double **P = (double**)malloc((n+1) * sizeof(double*));	
	for(i=0; i<n+1; i++)
		P[i]=(double *)malloc(sizeof(double) * (n+1));
	
	for(j=0; j<n+1; j++)
		P[0][j] = 1.0;						//A'n�n kazanmas� gereken ma�lar�n 
	for(i=0; i<n+1; i++)					//hepsini kazand��� durumlar
		P[i][0] = 0.0;						//B i�in ayn� i�lemler
	
	for(i=1; i<n+1; i++){
		for(j=1; j<n+1; j++){
			P[i][j] = p*P[i-1][j] + q*P[i][j-1];
			//printf("%.2lf    ",P[i][j]);
		}
		//printf("\n\n");
	}	
	return P[n][n];
}

int main(){
	printf("%lf",seriKazanma(4,0.6));
}
