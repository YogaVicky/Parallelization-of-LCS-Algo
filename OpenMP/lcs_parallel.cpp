#include <time.h>
#include<bits/stdc++.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
#define ll long long int

ll lcs( string X, string Y, ll m, ll n ,ll t){
    sleep(1);
    float st = omp_get_wtime();
    omp_set_num_threads(t);
    ll i,j,ii,jj,k,g,h;
    ll dp[m+1][n+1];
    memset(dp,0,sizeof(dp));
    for(ll i=0,j = 0; j <= n && i <= m; j++){	
		ll sz = min(j,(ll)m-i);
        #pragma omp parallel shared(i,j,dp,X,Y,sz)
        {
            #pragma omp for
            for(ll k=0;k<=sz;++k){									
                ll ii=i+k,jj=j-k;									
                if (ii == 0 || jj == 0)
                    dp[ii][jj] = 0;
                else if (X[ii - 1] == Y[jj - 1])
                    dp[ii][jj] = dp[ii - 1][jj - 1] + 1;
                else
                    dp[ii][jj] = max(dp[ii - 1][jj], dp[ii][jj - 1]);
            }
            #pragma omp barrier
        }						
		if(j>=n)								
			j=n-1,i++;
	}
    float et = omp_get_wtime();
    float ext = et - st;
    printf("Thread=%lld , Execution Time=%f  \n",t, ext);
	return dp[m][n];
}

/* Driver program to test above function */
int main()
{
string X,Y;
ifstream f1("a.txt"); //taking file as inputstream
if(f1) {
    ostringstream ss;
    ss << f1.rdbuf(); // reading data
    X = ss.str();
}
ifstream f2("b.txt"); //taking file as inputstream
if(f2) {
    ostringstream ss;
    ss << f2.rdbuf(); // reading data
    Y = ss.str();
}
srand(time(0));
ll m = X.length();
ll n = Y.length();
ll thread[] = {1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
ll thread_arr_size = 13;
for (ll k = 0; k < thread_arr_size; k++){
    printf("Length of LCS is %lld\n", lcs( X, Y, m, n ,k) );
}
cout<<endl;
return 0;
}
