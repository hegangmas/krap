/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cc
 * Copyright (C) 2012 Uroš Golob <uros@uros-MM061>
 *
 * histogram-test-cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * histogram-test-cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

/*#include <iostream>
#include <random>
#include <thread>
using namespace std;

#define NUMBER_OF_NUMBERS 1000000
#define NUMBER_OF_THREADS 40
#define NUMBER_OF_DIFF 65535

int numbers_random[NUMBER_OF_NUMBERS];
int numbers_dual[NUMBER_OF_NUMBERS];

int histogram[NUMBER_OF_THREADS][NUMBER_OF_DIFF];

double time_start[NUMBER_OF_THREADS];
double time_end[NUMBER_OF_THREADS];

void fill_histogram(int where, int what) {
    histogram[where][what]++;
}

void randomize(){
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,NUMBER_OF_DIFF-1);
    for(int i = 0;i<NUMBER_OF_NUMBERS;i++)
        numbers_random[i]=distribution(generator);
}

void randomize_dual(){
    for(int i = 0;i<NUMBER_OF_NUMBERS;i++){
    short r = rand()%1;
    if(r==1){
        numbers_dual[i]=5000;
    }
    else {
        numbers_dual[i]=20000;
    }
    }
}

int main()
{
    //za vsak primer na 0
    for(int x=0;x<NUMBER_OF_THREADS;x++){
        for(int y=0; y<NUMBER_OF_NUMBERS;y++){
            histogram[x][y]=0;
        }
    }
    //random
    randomize();
fill_histogram(nt,numbers_random[i]);

    //for(char nt=0;nt<NUMBER_OF_THREADS;nt++)
    //for(int i=0;i<NUMBER_OF_NUMBERS;i++){

   // std::thread t1(fill_histogram,nt,numbers_random[i]);
}

//for(char nt=0;nt<NUMBER_OF_THREADS;nt++)t1.join();
    cout << "Hello world!" << endl;
    return 0;
}
*/
#include <thread>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

#define NUMBER_OF_NUMBERS 1000000
#define NUMBER_OF_THREADS 1
#define NUMBER_OF_DIFF 65535

int numbers_random[NUMBER_OF_NUMBERS];
int numbers_dual[NUMBER_OF_NUMBERS];

//int histogram/*[NUMBER_OF_THREADS]*/[NUMBER_OF_DIFF];
int histogram[NUMBER_OF_DIFF];

//double time_start[NUMBER_OF_THREADS];
//double time_end[NUMBER_OF_THREADS];

void add_to_histogram(/*int where,*/ int what) {
  //  histogram/*[where]*/[what]++;
	int tmp=histogram[what];
	tmp++;
	histogram[what]=tmp;
}

void randomize(){
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,NUMBER_OF_DIFF-1);
    for(int i = 0;i<NUMBER_OF_NUMBERS;i++)
        numbers_random[i]=distribution(generator);
}

void randomize_dual(){

	std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,1);
    for(int i = 0;i<NUMBER_OF_NUMBERS;i++){
		if(distribution(generator)==1){
			numbers_dual[i]=5000;
		}
		else {
			numbers_dual[i]=20000;
		}
    }
}

void hello(){
    std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}

int main(){

    std::vector<std::thread> threads;

	randomize();
	//randomize_dual();

	for(int a =0;a<NUMBER_OF_NUMBERS;a++){
	add_to_histogram(/*0,*/numbers_random[a]);
	//	add_to_histogram(/*0,*/numbers_dual[a]);
	}

//	for(int h =0;h<NUMBER_OF_DIFF;h++)
//		cout<<histogram[h]<<"\t";

    for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread(hello));
    }

    for(auto& thread : threads){
        thread.join();
    }

    return 0;
}
