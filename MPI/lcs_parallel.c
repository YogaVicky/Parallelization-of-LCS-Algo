#include <iostream>
#include <fstream>
#include <string.h>
#include "mpi.h"
#include <bits/stdc++.h>
using namespace std;
#define ll long long int

double start,e;

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

void lcs(string X, string Y, ll m, ll n ,int argc, char *argv[])
{
    int no_tasks, taskid, no_workers, source, workers, mtype, no_elements_iter,no_elements_it2, no_elements, no_elements_left, index,rc, ack;
    double s1,s2;
    ll i,j,ii,jj,k,g,h;
    ll dp[m+1][n+1];
    memset(dp,0,sizeof(dp));
    MPI_Status status;
	MPI_Request request;
    MPI_Init(&argc, &argv);
	
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &no_tasks);

    if (no_tasks < 2){
        printf("Available Processors for execution=%d\n",no_tasks);
        printf("Program is Terminated since there are less than 2 threads\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }

	char pro_name[MPI_MAX_PROCESSOR_NAME];
	int length;
	MPI_Get_processor_name(pro_name,&length);

    no_workers = no_tasks - 1;
	start=MPI_Wtime();
    if(taskid==0){

         for (workers=1; workers<=no_workers; workers++){

            MPI_Send(&m, 1, MPI_INT, workers, 1, MPI_COMM_WORLD);
            MPI_Send(&n, 1, MPI_INT, workers, 2, MPI_COMM_WORLD);
            MPI_Send(X.c_str(), m, MPI_CHAR, workers, 3, MPI_COMM_WORLD);
            MPI_Send(Y.c_str(), n, MPI_CHAR, workers, 4, MPI_COMM_WORLD);

        }
        
        for(ll i=0,j = 0; j <= n && i <= m; j++){	
            //cout<<n<<" "<<i<<" "<<j<<"\n";
            k=0;
		ll sz = min(j,(ll)m-i);

        
        no_elements = sz/no_workers;
        no_elements_left = sz%no_workers;
        index = 0;
        mtype = FROM_MASTER;
        ack=1;
         for (workers=1; workers<=no_workers; workers++){
            no_elements_iter = (workers == no_workers) ? no_elements + no_elements_left : no_elements;
            MPI_Send(&ack, 1, MPI_INT, workers, 100, MPI_COMM_WORLD);
            MPI_Send(&k, 1, MPI_INT, workers, 5, MPI_COMM_WORLD);
            //cout<<workers<<" "<<no_elements + no_elements_left<<endl;
            MPI_Send(&i, 1, MPI_INT, workers, 6, MPI_COMM_WORLD);
            MPI_Send(&j, 1, MPI_INT, workers, 7, MPI_COMM_WORLD);
            MPI_Send(&no_elements_iter, 1, MPI_INT, workers, 8, MPI_COMM_WORLD);
            MPI_Send(&dp, (m+1)*(n+1), MPI_LONG_LONG_INT, workers, 9, MPI_COMM_WORLD);
            k+=no_elements_iter;

        }
        
        for (int ik=1; ik<=no_workers; ik++){
            source = ik;
            MPI_Recv(&ack,  1, MPI_INT, source, 11,MPI_COMM_WORLD, &status); 
            if(ack!=1)
            exit(0);
            //MPI_Recv(&dp,  (m+1)*(n+1), MPI_LONG_LONG_INT, source, 10,MPI_COMM_WORLD, &status); 
            MPI_Recv(&no_elements_iter,  1, MPI_INT, source, 12,MPI_COMM_WORLD, &status); 
            vector<int> ind1;
            vector<int> ind2;
            vector<int> val;
            ind1.resize(no_elements_iter);
            ind2.resize(no_elements_iter);
            val.resize(no_elements_iter);
            MPI_Recv(&ind1[0],  no_elements_iter, MPI_INT, source, 13,MPI_COMM_WORLD, &status); 
            MPI_Recv(&ind2[0],  no_elements_iter, MPI_INT, source, 14,MPI_COMM_WORLD, &status); 
            MPI_Recv(&val[0],  no_elements_iter, MPI_INT, source, 15,MPI_COMM_WORLD, &status); 
            for(int i=0;i<no_elements_iter;i++)
            {
                dp[ind1[i]][ind2[i]]=val[i];
            }
        }

        if(j>=n)	
        {
            j=n-1,i++;
        }	

        
        }

        ack=0;
        for (workers=1; workers<=no_workers; workers++){
            
            MPI_Send(&ack, 1, MPI_INT, workers, 100, MPI_COMM_WORLD);

        }
	e=MPI_Wtime();
	printf("Time= %f\n",e-start);
	printf("Length of LCS is %lld\n",dp[m][n] );	
    	return;    

    }
    if(taskid>0){
        // mtype = FROM_MASTER;
        MPI_Recv(&m, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&n, 1, MPI_INT, MASTER, 2, MPI_COMM_WORLD, &status);
        char X[m];
        char Y[m];
        MPI_Recv(&X, m, MPI_CHAR, MASTER, 3, MPI_COMM_WORLD, &status);
        MPI_Recv(&Y, m, MPI_CHAR, MASTER, 4, MPI_COMM_WORLD, &status);
        while(1)
        {
            MPI_Recv(&ack, 1, MPI_INT, MASTER, 100, MPI_COMM_WORLD, &status);
            //cout<<ack<<endl;
            if(ack==0)
            break;
            int k2=1;
            MPI_Recv(&k2, 1, MPI_INT, MASTER, 5, MPI_COMM_WORLD, &status);
            MPI_Recv(&i, 1, MPI_INT, MASTER, 6, MPI_COMM_WORLD, &status);
            MPI_Recv(&j, 1, MPI_INT, MASTER, 7, MPI_COMM_WORLD, &status);
            MPI_Recv(&no_elements_iter, 1, MPI_INT, MASTER, 8, MPI_COMM_WORLD, &status);
            MPI_Recv(&dp,(m+1)*(n+1) ,  MPI_LONG_LONG_INT, MASTER, 9,MPI_COMM_WORLD, &status);  
            //cout<<i<<" "<<j<<" "<<k2<<endl;
            //cout<<k2<<" "<<no_elements_iter<<endl;
            int ind1[no_elements_iter+1];
            int ind2[no_elements_iter+1];
            int val[no_elements_iter+1];

            for(int kl=k2;kl<=k2+no_elements_iter;kl++)
            {
                ll ii=i+kl;
                ll jj=j-kl;
                if (ii == 0 || jj == 0)
                    dp[ii][jj] = 0;
                else if (X[ii - 1] == Y[jj - 1])
                    dp[ii][jj] = dp[ii - 1][jj - 1] + 1;
                else
                    dp[ii][jj] = max(dp[ii - 1][jj], dp[ii][jj - 1]);
                ind1[kl-k2]=ii;
                ind2[kl-k2]=jj;
		val[kl-k2]=dp[ii][jj];
            }

            int len=no_elements_iter+1;
            MPI_Send(&ack ,1, MPI_INT, MASTER, 11, MPI_COMM_WORLD);
            MPI_Send(&len ,1, MPI_INT, MASTER, 12, MPI_COMM_WORLD);
            MPI_Send(&ind1[0] ,no_elements_iter+1, MPI_INT, MASTER, 13, MPI_COMM_WORLD);
            MPI_Send(&ind2[0] ,no_elements_iter+1, MPI_INT, MASTER, 14, MPI_COMM_WORLD);
            MPI_Send(&val[0] ,no_elements_iter+1, MPI_INT, MASTER, 15, MPI_COMM_WORLD);
        }
		
    }
    MPI_Finalize();
}

int main(int argc, char *argv[]){
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

lcs( X, Y, m, n,argc, argv);

return 0;
}
