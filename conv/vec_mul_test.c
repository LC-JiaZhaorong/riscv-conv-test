#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define X_OR_SHIFT_Y(X,Y) ((X)|((X)>>(Y)))
#define NEXT_POWER_OF_2(X) (X_OR_SHIFT_Y(X_OR_SHIFT_Y(X_OR_SHIFT_Y(X_OR_SHIFT_Y(X_OR_SHIFT_Y(X-1,1),2),4),8),16)+1)
#define NP2(X) NEXT_POWER_OF_2(X)
#define ATM_C 64
#define ATM_K 32
#define CHANNEL 64
#define WIDTH 56
#define HIGHT 56
#define R 1
#define S 1
#define KERNEL 256
#define BASE_LEN 512
#define STRIPE_LEN 64
#define STRIDE 1

void *vec_mul_64e(size_t n,  char *dat, char *wt, short *vec_tmp, size_t *param);


char dat[NP2(CHANNEL/ATM_C)][NP2(WIDTH*HIGHT)][NP2(ATM_C)] = {0};
char wt[NP2(KERNEL/ATM_K)][NP2(CHANNEL/ATM_C)][NP2(R*S)][NP2(ATM_K)][NP2(ATM_C)] = {0};
char base[NP2(BASE_LEN/ATM_C)][NP2(ATM_C)] = {0};
int output[NP2(KERNEL)][NP2(WIDTH*HIGHT)] = {0};
size_t param[9]={STRIPE_LEN*64, //vector number
		CHANNEL,WIDTH,HIGHT,KERNEL,R,S,STRIDE,ATM_C};

void read_data() {
	FILE *fp_wt = fopen("weight.coe","r");
  	FILE *fp_dat = fopen("feature.coe","r");
  	FILE *fp_bias = fopen("bias.coe","r");
	int dat_tmp;// read ",\n" in every coe line
	//read coe file into buffer, just like moving datas from DDR to SRAM
	//read feature
	for(int i=0;i<CHANNEL/ATM_C;i++){
		for(int j=0;j<WIDTH*HIGHT;j++){
			for(int k=0;k<ATM_C;k++){
				fscanf(fp_dat, "%2x", &dat[i][j][k]);
				//printf("dat[%d][%d][%d]=%#x\n",i,j,k,dat[i][j][k]);
			}
			fscanf(fp_dat, ",\n", &dat_tmp);
			//printf("------------------%d------------------------\n",dat_tmp);
		}
	}
	//read wt
	for(int i=0;i<KERNEL/ATM_K;i++){
		for(int j=0;j<CHANNEL/ATM_C;j++){
			for(int k=0;k<R*S;k++){
				for(int l=0;l<ATM_K;l++){
					for(int m=0;m<ATM_C;m++){
						fscanf(fp_wt, "%2x", &wt[i][j][k][l][m]);
					    if((m==31)||(m==63)) fscanf(fp_wt, ",\n", &dat_tmp);
						//if(m==63) printf("wt[%d][%d][%d][%d][%d]=%#x\n",i,j,k,l,m,wt[i][j][k][l][m]);
					}
				}
			}
		}
	}
	
	//read base
	
	for(int i=0;i<BASE_LEN/ATM_C;i++){
		for(int j=0;j<ATM_C;j++){
			fscanf(fp_bias, "%2x", &base[i][j]);
			//printf("base[%d][%d]=%#x\n",i,j,base[i][j]);
		}
		fscanf(fp_wt, ",\n", &dat_tmp);
	}
	fclose(fp_wt);
	fclose(fp_dat);
	printf("-------------data read done--------------------\n");
}

void read_data_test(){
	printf("feature test:");
	for(int i=0;i<ATM_C;i++){
		printf(" 0x%02x",*(dat[0][0]+i));
	}
	printf("\n");

	printf("wight test:");
	for(int i=0;i<ATM_C;i++){
		printf(" 0x%02x",*(wt[0][0][0][0]+i));
	}
	printf("\n");
}

int main(void)
{
	printf("\nRunning rvv-benchmark...\n");
	FILE *fp_out = fopen("RISCV-OUTPUT.TXT","w");
	read_data();
	read_data_test();
	char *dat_addr;
	char *wt_addr;
	short vec_tmp[CHANNEL] = {0};
	int sum = 0;
	for(int knl=0; knl<KERNEL; knl++){
		for(int rs=0; rs< R*S; rs++){
			for(int hw=0; hw< HIGHT*WIDTH; hw++){
				for(int c=0; c<CHANNEL/ATM_C; c++){
					dat_addr = &dat[0][0][0] + ATM_C * hw;
					wt_addr = &wt[0][0][0][0][0] + ATM_C*knl;
					vec_mul_64e(ATM_C,dat_addr,wt_addr,vec_tmp,param);
					for(int i=0;i<ATM_C;i++){
						sum = sum + vec_tmp[i]; 
					}
					//printf("knl=%d, hw=%d\n",knl,hw);
				}
				output[knl][hw]= sum;
				//printf("sum = %8#x",sum);
				sum = 0;
			}
		}
	}
	printf("output is :-------------------------------------------\n");
	for(int i=0;i<KERNEL/ATM_K;i++){
		for(int j=0;j<HIGHT*WIDTH;j++){
		//	printf("0x%08x,",output[32*i][j]);
			for(int k=0;k<ATM_K;k++){
				fprintf(fp_out,"%08x,",output[k+32*i][j]);
			}
				fprintf(fp_out,"\n");
			//	printf("\n");
		}
	}
	printf("-------------------------------------------------------\n");
	fclose(fp_out);
}
