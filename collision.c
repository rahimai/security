
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "openssl/sha.h"
#include <time.h>

int USERS=10;

void read_hashed_passwd(char *filename, char **hashes){
    FILE *fptr;
    fptr = fopen(filename,"r");
    if (fptr == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }
    int buffer_size = 50;
    
    char ch[buffer_size];
    if (fgets(ch, buffer_size, fptr)!=NULL) {
        int i=0;        
        while (fgets(ch, buffer_size, fptr)!=NULL && i<USERS) {
            hashes[i] = (char*) malloc((SHA_DIGEST_LENGTH*2+1)*sizeof(char));
            memset(hashes[i], 0, (SHA_DIGEST_LENGTH*2+1)*sizeof(char));
            memcpy(hashes[i], &ch[strlen(ch)-SHA_DIGEST_LENGTH*2-1],\
                    SHA_DIGEST_LENGTH*2*sizeof(char));                
            memset(ch, 0, buffer_size*sizeof(char));
            i++;
        }
    }
    fclose(fptr);
    
    return;
}

char test_collision(char **hashes, char *test, char *oritxt){
	for (int i=0;i<USERS;i++){
                if (strcmp(hashes[i], test) == 0){
                    int uid = i + 1;
                    printf("A%i: ", uid);

                    // print salt hash pair
                    if (strlen(oritxt) == 2)
                        printf("%c,%c\n", oritxt[0], oritxt[1]);
                    else
                        printf("%c%c,%c\n", oritxt[0], oritxt[1], oritxt[2]);
                }

	}
        return 0;
}

void collision_attack(char *filename){
	char **hashes = (char**) malloc(USERS*SHA_DIGEST_LENGTH*2*sizeof(char));
	

	read_hashed_passwd(filename, hashes);
	
	for (int i=0;i<USERS;i++){
		printf("%s", hashes[i]);
		printf("\n");

	}

        
        char test1 = 0;
        for (int i=0; i < 128; i++){
                char test2 = 0;            
                for (int j= 0; j < 128; j++){
                    char test3 = 0;
                    for (int k = 0; k < 128; k++){
                        char data[] = {test1, test2, test3, '\0'};
        
                         // generate hash
                         size_t length = strlen(data);
                         unsigned char hash[SHA_DIGEST_LENGTH];
                         memset(hash, 0x0, SHA_DIGEST_LENGTH);
                         SHA1((unsigned char *)data, length, hash);
                         char buf[SHA_DIGEST_LENGTH*2];
                         memset(buf, 0x0, SHA_DIGEST_LENGTH*2);
                         for (int k=0; k < SHA_DIGEST_LENGTH; k++) {
	               	     sprintf((char*)&(buf[k*2]), "%02x", hash[k]);
                         }
                        // test collision
                        test_collision(hashes, buf, data);
                        test3++;
                    }
                    test2++;
                    
                }
                test1++;
        }

}


int main(int argc, char const *argv[])
{
	char data[] = "hi";
	size_t length = strlen(data);

	unsigned char hash[SHA_DIGEST_LENGTH];
	memset(hash, 0x0, SHA_DIGEST_LENGTH);

	clock_t start, end;
	double cpu_time_used;
	 
	start = clock();

	SHA1((unsigned char *)data, length, hash);
	char buf[SHA_DIGEST_LENGTH*2];
	memset(buf, 0x0, SHA_DIGEST_LENGTH*2);
	for (int k=0; k < SHA_DIGEST_LENGTH; k++) {
		sprintf((char*)&(buf[k*2]), "%02x", hash[k]);
	}
	printf("SHA1(%s) is %s\n", data, buf);

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("SHA1 takes %f seconds\n", cpu_time_used);

	char *filename = "salting_passwd";
	start = clock();        
	collision_attack(filename);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("SHA1 takes %f seconds\n", cpu_time_used);


	return 0;
}

