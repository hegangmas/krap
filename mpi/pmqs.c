/*
 *============================================================================
 *Name        : pmqs.c
 *Author      : Uroš Golob
 *Version     : 1.0.10
 *Copyright   : Copyright 2010 Uroš Golob
 *Description : PARALLEL-MERGE-QUICK-SORT in C MPI implementation
 *============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
double startT, stopT;

#define MAX_ELEMENTS 10000

showElapsed (int id, char* m)
{
	printf ("%d: %s %f secs\n", id, m, (clock () - startT) / CLOCKS_PER_SEC);
}

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



void quick_sort(int* arr, int left, int right)
{
	int p = (left + right) / 2;
	int part_index = presort(arr, left, right, arr[p]);
	if (left < part_index)
		quick_sort(arr, left, part_index);
	if (part_index + 1 < right)
		quick_sort(arr, part_index + 1, right);
}

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

int* postsort (int* v1, int n1, int* v2, int n2)
{
	int i, j, k;
	int* result;

	result = (int *)malloc ((n1 + n2) * sizeof (int));

	i = 0;
	j = 0;
	k = 0;

	while (i < n1 && j < n2)
		if (v1[i] < v2[j]) {
			result[k] = v1[i];
			i++;
			k++;
		} else {
			result[k] = v2[j];
			j++;
			k++;
		}

	if (i == n1)
		while (j < n2) {
			result[k] = v2[j];
			j++;
			k++;
		}
	else
		while (i < n1) {
			result[k] = v1[i];
			i++;
			k++;
		}

	return result;
}

int test(int *p,int size)
{
	int i;
	for (i=0;i<size-1;i++) {
		if (p[i]>p[i+1]) {
			printf("NAPAKA POLJE NI SORTIRANO na i: %d ",i);
			return i;
		}
	}
	return -1;
}

//main
int main(int argc, char **argv)
{

	int myrank, i, j, count, np, min = 2147483647, max = -2147483647, t, c,s;
	MPI_Status status;

	MPI_Init(&argc, &argv); /* Initialize MPI */
	//DebugWait();
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	if (myrank == 0) { /* MAIN PROCCES */

		int *array, * res, *rec;

		array = malloc(MAX_ELEMENTS * sizeof(int));
		srand(666);
		for (i = 0; i < MAX_ELEMENTS; i++)
			array[i] = rand() % 1000;


		startT=clock();
		if (np==1) {
			qsort((int*) array, MAX_ELEMENTS, sizeof(int), (void*) comp);
			MPI_Finalize(); /* Terminate MPI */
			return 0;
		}


		int c= MAX_ELEMENTS/np;


		for (i = 1; i < np; i++) {//prvi delavec je št 1 in ne 0!!!!!
			MPI_Send((int *)(array+((i-1)*c)), c, MPI_INT,i, 0, MPI_COMM_WORLD);
		}


		//ostala implementacija sortiranja (sortiramo od tam naprej od koder se konča j -> [1]+1 zadnega delavca)
		s=MAX_ELEMENTS-(c*(np-1));

		res = malloc(MAX_ELEMENTS * sizeof(int));
		rec = malloc(c * sizeof(int));
		//

		qsort((int*) array+(c*(np-1)) , s, sizeof(int), (void*) comp);

		res=memcpy(res, array+(c*(np-1)), s*sizeof(int));
		for (i = 1; i < np; i++) {//prvi delavec je št 1 in ne 0 pivot pa ima na [0]

			MPI_Recv((int *)rec, c, MPI_INT, i,1, MPI_COMM_WORLD, &status);
			res=postsort(res,s,rec,c);
			s+=c;
		}
		free(rec);

				if (test(res,MAX_ELEMENTS)==-1)
			printf("izvedli smo test urejenosti vse je ok");
			else printf("izvedli smo test urejenosti pojavila se je napaka");


	} else { /* WORKER */
		int x;
		for (x = 1; x < np; x++)
			if (myrank == x) {

				MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

				MPI_Get_count(&status, MPI_INT, &count);

				int* rec = (int*) malloc(count * sizeof(int));

				MPI_Recv(rec, MAX_ELEMENTS, MPI_INT, MPI_ANY_SOURCE,0, MPI_COMM_WORLD, &status);

				qsort(rec, count, sizeof(int), (void*) comp);

				MPI_Send(rec, count, MPI_INT,0,1, MPI_COMM_WORLD);

				free(rec);
			}
	}

	if (myrank == 0) {

		stopT = clock ();

		printf ("\nVelikost polja: %d \nŠtevilo procesov(s): %d \nSortirali smo z parallel merge quicksort algoritmom: %f s\n", MAX_ELEMENTS, np, (stopT - startT) / CLOCKS_PER_SEC);
	}

	MPI_Finalize(); /* Terminate MPI */
	return 0;
}
