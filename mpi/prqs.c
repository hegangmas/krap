/*
 *============================================================================
 *Name        : prqs.c
 *Author      : Uroš Golob
 *Version     : 1.1.30
 *Copyright   : Copyright 2010 Uroš Golob
 *Description : PARALLEL-RECURSIVE-QUICK-SORT in C MPI implementation
 *============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>
double startT, stopT; //za štoparico

#define MAX_ELEMENTS 10000

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

int test(int *p,int size)
{
	int i;
	for (i=0;i<size-1;i++) {
		if (p[i]>p[i+1]) {
			printf("NAPAKA POLJE NI SORTIRANO na i: %d \n",i);
			return -1;
		}
	}
	return 0;
}

int sort_recursive(int* arr, int size, int prank, int maxrank, int rindex)
{
	MPI_Status status;
	int share = prank + pow(2, rindex);
	rindex ++;
	if (share > maxrank) {
		qsort(arr, size, sizeof(int), (void*) comp);
		return 0;
	}
	//najdemo min in max v vsakem podsetu da "obtimalno" zadenemo pivot
	int pmin,pmax,min = 2147483647, max = -2147483647,i,t;
	for (i = 0; i < size; i++) {
		t = arr[i];
		if (t > max) {
			max = t;
			pmax=i;
		}
		if (t < min) {
			min = t;
			pmax=i;
		}
	}

	int ostanek = presort(arr, 0, size,(max+min)/2) + 1;
	//kater del pošiljamo levi al desni
	if (ostanek > size - ostanek) {
		MPI_Send((arr + ostanek), size - ostanek, MPI_INT, share, ostanek, MPI_COMM_WORLD);
		sort_recursive (arr, ostanek, prank, maxrank,	rindex);
		MPI_Recv((arr + ostanek), size - ostanek, MPI_INT, share,MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	} else {
		MPI_Send(arr, ostanek, MPI_INT, share , ostanek,	MPI_COMM_WORLD);
		sort_recursive ((arr + ostanek), size - ostanek, prank,	maxrank, rindex);
		MPI_Recv(arr, ostanek, MPI_INT, share, MPI_ANY_TAG,	MPI_COMM_WORLD, &status);
	}
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

		sort_recursive(array, MAX_ELEMENTS, myrank, np -1,0);

		if (test(array,MAX_ELEMENTS))
			printf("izvedli smo test urejenosti vse je ok");




	} else {
		MPI_Status status;
		
		int* rec = NULL;
		
		int size = 0;
		
		int index_count = 0;
		
		int pr_source = 0;
		
		while (pow(2, index_count) <= myrank)index_count ++; //izračunamo trenutno rekurzijo
		
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,&status);
		
		MPI_Get_count(&status, MPI_INT, &size);
		
		pr_source = status.MPI_SOURCE;
		
		rec = (int*)malloc(size * sizeof(int));
		
		MPI_Recv(rec, size, MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
		sort_recursive(rec, size, myrank, np -1,index_count);
		
		MPI_Send(rec, size, MPI_INT, pr_source,0, MPI_COMM_WORLD);
		
		free(rec);
	}

	if (myrank == 0) {
		stopT = clock ();
		printf ("\nVelikost polja: %d \nŠtevilo procesov(s): %d \nSortirali smo z rekurzivnim paralelnim quick sort algoritmom: %f s\n", MAX_ELEMENTS, np, (stopT - startT) / CLOCKS_PER_SEC);
	}
	MPI_Finalize();
	return 0;
}
