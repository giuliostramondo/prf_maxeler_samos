/*
    Copyright 2016 Giulio Stramondo

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "prf.h"
#include "utility.h"
#include "testbench_utils.h"

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

int N;
int M;


int main(int argc, char* argv[])
{
    int i,j,k,w;
    Options parameters = parseArguments(argc,argv);
    if(parameters.error)
	return 0;
    int p = parameters.p;
    int q = parameters.q;
    N = parameters.N;
    M = parameters.M;
    scheme s = parameters.s;
    //int A_test[N][M];

    int **A_test = (int**)malloc(sizeof(int*)*N);

    for(i=0;i<N;i++){
        A_test[i]= (int*)malloc(sizeof(int)*M);
    }
    for(i=0;i<N;i++){
        for(j=0;j<M;j++){
            A_test[i][j]= i*M+j;
        }
    }
    data_type* input_data;
    data_type* output_data;
    struct timeval after,before;
    int counter=0;
	char* scheme_name = schemeStringFromMappingScheme(s);
        printf("Testing %s scheme ...\n",scheme_name);
        printf("Filling PRF...\n");

        input_data = fillPRF( p, q, RECT_ROW,A_test,  &counter);


    	output_data = (data_type*) malloc(sizeof(data_type)*p*q*counter);
        //Add here first call to the PRF on maxeler board
    	gettimeofday(&before, NULL);
    	ExampleProject( counter, input_data,output_data);
    	gettimeofday(&after, NULL);
    	double time =(double)(after.tv_sec - before.tv_sec) +
    	        		(double)(after.tv_usec - before.tv_usec) / 1e6;
    	printf("Read %d data from memory in %lf seconds\n",p*q*counter,time);
    	printf("The average time to read one element is %lf seconds\n",time/(p*q*counter));
    	printf("Assuming that each element is 64bit the bandwidth is %lf bit/s, %lf byte/s, %lf KByte/s, %lf MByte/s\n",(p*q*counter*64)/time,(p*q*counter*64)/(8*time),(p*q*counter*64)/(1024*8*time),(p*q*counter*64)/(1024*1024*8*time));
        //PRF_Complete( counter, input_data,output_data);
        t_list* available_acc_type = getAvaliableAccessType(s);
        while(available_acc_type!=NULL){
            acc_type* curr_shape = (acc_type*) available_acc_type->data;
            char* shape_name = accessStringFromAccessType(*curr_shape);
            printf("Testing %s shape...\n",shape_name);
            input_data = all_accesses(p,q,*curr_shape,N,M,&counter);
            data_type* expected_output = generate_expected_output(p,q,A_test,input_data,counter);
            data_type* output_data = (data_type*) malloc(sizeof(data_type)*p*q*counter);
	    //Send the input_data to the PRF and collect outputs
           // PRF_Complete( counter, input_data,output_data);
        	gettimeofday(&before, NULL);
        	ExampleProject( counter, input_data,output_data);
        	gettimeofday(&after, NULL);
        	double time =(double)(after.tv_sec - before.tv_sec) +
        	        		(double)(after.tv_usec - before.tv_usec) / 1e6;
        	printf("Read %d data from memory in %lf seconds\n",p*q*counter,time);
        	printf("The average time to read one element is %lf seconds\n",time/(p*q*counter));
        	printf("Assuming that each element is 64bit the bandwidth is %lf bit/s, %lf byte/s, %lf KByte/s, %lf MByte/s\n",(p*q*counter*64)/time,(p*q*counter*64)/(8*time),(p*q*counter*64)/(1024*8*time),(p*q*counter*64)/(1024*1024*8*time));


            int error = 0;
            //Check equivalence
            for (j=0;j<counter;j++){
            	if (s == ROW_COL && !((input_data[j*((p*q)+4) + (p*q)] % p==0) || (input_data[j*((p*q)+4) + (p*q)+1] % q==0)) ){
                                continue;
                }
                for(k=0;k<p*q;k++){
                    if(output_data[j*(p*q)+k] != expected_output[j*(p*q)+k]){
                        error=1;
                    }
                }
                if(error){
                    //Print error
                    printf("\033[31m[Error]\033[0m\t output %d\n",j);
                    printf("Input: ");
                     for(w=0;w<(p*q)+4;w++){
                        printf("%d ",input_data[j*((p*q)+4)+w]);
                    }
                    printf("\n");
                    printf("Expected Output: ");
		    for(w=0;w<p*q;w++){
                        printf("%d ",expected_output[j*(p*q)+w]);
                    }
                    printf("\n");
                    printf("Output: ");
                    for(w=0;w<p*q;w++){
                        printf("%d ",output_data[j*(p*q)+w]);
                    }
                    printf("\n");
                    error=0;
                }else{
			printf("\033[32m[Correct]\033[0m\t output %d\n",j);
			printf("Input: ");
                     	for(w=0;w<(p*q)+4;w++){
                        	printf("%d ",input_data[j*((p*q)+4)+w]);
                    	}
                    	printf("\n");
                    	printf("Output: ");
                    	for(w=0;w<p*q;w++){
                        	printf("%d ",output_data[j*(p*q)+w]);
                    	}
                    	printf("\n");

		}
            }
            available_acc_type = available_acc_type->next;

        }



    return 0;
}


