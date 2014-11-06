/*
 *============================================================================
 *Name        : ppqs.c
 *Author      : Uroš Golob
 *Version     : 1.3.11
 *Copyright   : Copyright 2010 Uroš Golob
 *Description : PARALLEL-PRESORT-QUICK-SORT in C MPI implementation
 *============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_ELEMENTS 10000
double startT, stopT;

//funkcija primerjanja za qsort
int comp(const int * a, const int * b)
{
	if (*a == *b)
		return 0;
	else if (*a < *b)
		return -1;
	else
		return 1;
}
//funkcija čakanja za debuggiranje
static void DebugWait(void)
{
	int i = 0;
	char hostname[256];
	gethostname(hostname, sizeof(hostname));
	printf("PID %d on %s ready for attach \n", getpid(), hostname);
	fflush(stdout);
	while (0 == i)
		sleep(5);
	printf("%d: on %s Starting now \n", getpid(), hostname);
}


//funkcija ki pruredi podatke (za presort načina)
int presort(int a[], int l, int r, int p)
{
	int i = l, j = r, t;
	while (i <= j) {
		while (a[i] < p)
			i++;
		while (a[j] > p)
			j--;
		if (i <= j) {
			t = a[i];
			a[i] = a[j];
			a[j] = t;
			i++;
			j--;
		}
	}
	return j;
}
//funkcija preveri če so polja urejena
int test(int *p,int size)
{
	int i;
	for (i=0;i<size-1;i++) {
		if (p[i]>p[i+1]) {
			printf("NAPAKA POLJE NI SORTIRANO na i: %d ",i);
			return -1;
		}
	}
	return 1;
}

//main
int main(int argc, char **argv)
{

	int myrank, i, j, count, np, min = 2147483647, max = -2147483647, t, c;
	MPI_Status status;

	MPI_Init(&argc, &argv); /* Initialize MPI */
	//DebugWait();
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	if (myrank == 0) { /* MAIN PROCCES */


		int *array;
		array = malloc(MAX_ELEMENTS * sizeof(int));
		srand(666);
		for (i = 0; i < MAX_ELEMENTS; i++)
			array[i] = rand() % 1000;
		startT=clock();
		if (np==1) {
			qsort((int*) array, MAX_ELEMENTS, sizeof(int), (void*) comp);

			stopT = clock ();
			printf ("\nVelikost polja: %d \nŠtevilo procesov(s): %d \nSortirali smo z rekurzivnim paralelnim quick sort algoritmom: %f s\n", MAX_ELEMENTS, np, (stopT - startT) / CLOCKS_PER_SEC);

			MPI_Finalize(); /* Terminate MPI */
			return 0;
		}

		int pmin,pmax;
		for (i = 0; i < MAX_ELEMENTS; i++) {
			t = array[i];
			if (t > max) {
				max = t;
				pmax=i;
			}
			if (t < min) {
				min = t;
				pmax=i;
			}
		}

		//rezerviramo prostor za podatke statistike
		int **pivoti;
		pivoti = malloc((np - 1) * sizeof(int*));
		if (!pivoti) {
			printf("nekaj je slo hudo narobe z pomnilnikom \n");
			MPI_Finalize();
			return 0;
		}

		for (i=0; i < np - 1; i++) {
			pivoti[i] = malloc(4 * sizeof(int));
			if (!pivoti[i]) {
				printf("nekaj je slo hudo narobe z gnezdenim pomnilnikom \n");
				MPI_Finalize();
				return 0;
			}
		}
		i = 0;
		int stkosov, n;
		float kvoc, povp, temp;
		stkosov = MAX_ELEMENTS / c;
		j = min;
		povp=min+max;
		for (n = 0 ; n < np-1 ; n++) {
			kvoc= ((float)n+1.0)/(float) np;
			temp= povp * kvoc;
			pivoti[n][0] = (int)temp+1;// na [0] damo željeni pivot
		}
		pivoti[0][1] = presort(array, 0, MAX_ELEMENTS, pivoti[0][0]);


		for (i = 1; i < np - 1; i++) {
			pivoti[i][1] = presort(array, pivoti[i-1][1]+1, MAX_ELEMENTS,pivoti[i][0]);

		}


		//printf("P:%d za presort funkcijami \n", myrank);
		//for (i = 0; i < MAX_ELEMENTS; i++)
		//printf("P:%d_%d \n", myrank, array[i]);

		// na pivoti[x][2] shranimi velikost trenutnega kosa

		for (i = 1; i < np; i++) {//prvi delavec je št 1 in ne 0!!!!!


			if (i==1) {
				pivoti[i-1][2]=pivoti[i-1][1]+1;
				MPI_Send((int *)array, pivoti[i-1][2], MPI_INT,i, 0, MPI_COMM_WORLD);
			} else {
				pivoti[i-1][2]=pivoti[i-1][1]-pivoti[i-2][1];
				MPI_Send((int *)(array+pivoti[i-2][1]+1), pivoti[i-1][2], MPI_INT,i, 0, MPI_COMM_WORLD);
			}
		}
		//int f,h;
		//for (f=0;f<np-1;f++){
		//for(h=0;h<3;h++)
		//{
		//printf("%d  ",pivoti[f][h]);
		//}
		//printf("\n");
		//}
		//ostala implementacija sortiranja (sortiramo od tam naprej od koder se konča j -> [1]+1 zadnega delavca)
		qsort((int*)(array+pivoti[np-2][1]+1), MAX_ELEMENTS-pivoti[np-2][1]-1, sizeof(int), (void*) comp);

		for (i = 1; i < np; i++) {//prvi delavec je št 1 in ne 0 pivot pa ima na [0]
			if (i==1) {
				MPI_Recv((int *)array, pivoti[i-1][2], MPI_INT, i,MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			} else {
				MPI_Recv((int *)(array+pivoti[i-2][1]+1), pivoti[i-1][2], MPI_INT, i,MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			}
		}
		if (test(array,MAX_ELEMENTS))
			printf("izvedli smo test urejenosti vse je ok");


	} else { /* WORKER */
		int x;
		for (x = 1; x < np; x++)
			if (myrank == x) {

				MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

				MPI_Get_count(&status, MPI_INT, &count);

				int* rec = (int*) malloc(count * sizeof(int));

				MPI_Recv(rec, MAX_ELEMENTS, MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD, &status);

				qsort(rec, count, sizeof(int), (void*) comp);

				MPI_Send(rec, count, MPI_INT,0,1, MPI_COMM_WORLD);

				free(rec);
			}
	}



	if (myrank == 0) {
		stopT = clock ();

		printf ("\nVelikost polja: %d \nŠtevilo procesov(s): %d \nSortirali smo z presort paralel quick sort algoritmom: %f s\n", MAX_ELEMENTS, np, (stopT - startT) / CLOCKS_PER_SEC);
	}




	MPI_Finalize(); /* Terminate MPI */
	return 0;
}
