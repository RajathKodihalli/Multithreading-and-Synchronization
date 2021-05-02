#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <error.h>
//#include <pthread.h>

#define N_SLAVES	5

pthread_t slaves[N_SLAVES];


void *
write_into_file(void *arg)
{
	char file_name[64];
	char string_to_write[64];
	int len;
	int count;

	int *thread_id = (int *)arg;

	sprintf(file_name, "thread_%d.txt", *thread_id);

	FILE *fptr = fopen(file_name, 'w');

	if(!fptr){
		printf("Error: Could not open log file %s, errno = %d\n", file_name, errno);
		return 0;
	}

	while(1) {
		len = sprintf(string_to_write, "%d: I am thread %d\n", count++, *thread_id);
		/*
		 * size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
				ptr - This is pointer to array of elements to be written
					size -  This is the size in bytes of each element to be written
						nmemb - This is the number of elements, each one with a size of size bytes
							stream - This is the pointer to a FILE object that specifies an output stream
		 * */

		fwrite(string_to_write, sizeof(char), len, fptr);

		fflush(fptr);

		sleep(1);
	}
	return 0;
}

int
main(int argc, char **argv){

	int i;
	int *thread_id = 0;

	for(i=0; i < N_SLAVES; i++) {
		thread_id = calloc(1, sizeof(*thread_id));
		*thread_id = i;

		/*****************************************************************
		 int pthread_create(pthread_t * thread,
                   const pthread_attr_t * attr,
                   void * (*start_routine)(void *),
                   void *arg);

		 *thread: pointer to an unsigned integer value that returns the thread id of the thread created.
		 *attr: pointer to a structure that is used to define thread attributes like detached state, scheduling policy, stack address, etc.
		 *attr: Set to NULL for default thread attributes.
		 *start_routine: pointer to a subroutine that is executed by the thread. The return type and parameter type of the subroutine must be of type void *.
		 *start_routine: The function has a single attribute but if multiple values need to be passed to the function, a struct must be used.
		 *arg: pointer to void that contains the arguments to the function defined in the earlier argument
         */
		pthread_create(&slaves[i], 0, write_into_file, thread_id);
	}

	/*Main menu*/
	int choice;
	int thread_num;

	while(1) {

		printf("1. Cancel the thread\n");
		scanf("%d", &choice);

		printf("Enter slave thread id [0-%d] :", N_SLAVES -1);
		scanf("%d", &thread_num);

		if(thread_num < 0 || thread_num >= N_SLAVES) {
			printf("Invalid Thread id\n");
			exit(0);
		}
		printf("\n");

		switch(choice) {

		case 1:
			break;
		default:
			continue;
		}

	}
	return 0;
}
