%%cu
#include<bits/stdc++.h>
#include<stdio.h>
using namespace std;
#include<chrono>
#define threads 4
__managed__ int dp[500][500];
__managed__ char X[500]={"AGGTABAGGTABAGGTABAGGTABAGGTABAGGTABAGGTABAGGTABAGGTABAGGTABGXTXAYB"};
__managed__ char Y[500]={"GXTXAYBGXTXAYBGXTXAYBGXTXAYBGXTXAYBGXTXAYBGXTXAYBGXTXAYBGXTXAYBXXXX"};


__global__ void test(int *d_a,int *d_b,int *d_c,int *d_d,int *d_e,int *d_f){

    int ix=threadIdx.x+blockIdx.x*blockDim.x;
    int ii=*d_d+*d_f;
    int jj=*d_e-*d_f;

    int val=0;
    if (ii == 0 || jj == 0)
        val = 0;
    else if (X[ii - 1] == Y[jj - 1])
        val = dp[ii - 1][jj - 1] + 1;
    else
        val = max(dp[ii - 1][jj], dp[ii][jj - 1]);
        d_a[ix]=ii;
        d_b[ix]=jj;
        d_c[ix]=val;

}


int lcs( string X, string Y, int m, int n ){
    int i,j,ii,jj,k,g,h;
    memset(dp,0,sizeof(dp));
    cudaEvent_t start, stop;
		float elapsedTime;

    cudaEventCreate(&start);
    cudaEventRecord(start,0);

    for(int i=0,j = 0; j <= n && i <= m; j++){	
		int sz = min(j,(int)m-i);
    
    int size = (sz+1)*sizeof(int);
    int size1 = 1*sizeof(int);


    
    int *d_a,*d_b,*d_c,*d_d,*d_e,*d_f;

    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_b, size);
    cudaMalloc((void **)&d_c, size);

    cudaMalloc((void **)&d_d, size1);
    cudaMalloc((void **)&d_e, size1);
    cudaMalloc((void **)&d_f, size1);

    
    
            for(int k=0;k<=sz;++k){	
                	
                int i1[sz+1],i2[sz+1],i3[sz+1];

                cudaMemcpy(d_d, &i, size1, cudaMemcpyHostToDevice);
                cudaMemcpy(d_e, &j, size1, cudaMemcpyHostToDevice);
                cudaMemcpy(d_f, &k, size1, cudaMemcpyHostToDevice);

                test<<<(sz+1)/threads + 1,threads>>>(d_a,d_b,d_c,d_d,d_e,d_f);
                cudaDeviceSynchronize();

                cudaMemcpy(&i1, d_a, size, cudaMemcpyDeviceToHost);
                cudaMemcpy(&i2, d_b, size, cudaMemcpyDeviceToHost);
                cudaMemcpy(&i3, d_c, size, cudaMemcpyDeviceToHost);

                for(int i=0;i<=sz;i++)
                {
                    dp[i1[i]][i2[i]]=i3[i];
                }
            }					
		if(j>=n)								
			j=n-1,i++;
	}	

  cudaEventCreate(&stop);
  cudaEventRecord(stop,0);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&elapsedTime, start,stop);
  printf("No of Threads=%d\nElapsed time (in seconds): %f\n" ,threads,elapsedTime/1000);

	return dp[m][n];
}

/* Driver program to test above function */
int main()
{
int m=67;
int n=67;

printf("Length of LCS is %d", lcs( X, Y, m, n ) );
cudaDeviceSynchronize();
return 0;
}