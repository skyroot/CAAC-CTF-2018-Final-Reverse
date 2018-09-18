#include <stdio.h>
#include <string.h>

#define SWAP(n,s,i,j) tmp=s[(i)%n];s[(i)%n]=s[(j)%n];s[(j)%n]=tmp;
#define A(n,s,i) SWAP(n,s,i*7, i*13)
#define B(n,s,i) SWAP(n,s,i|12,i<<3)
#define C(n,s,i) SWAP(n,s,i*3, i*17)
#define D(n,s,i) SWAP(n,s,i^3, i*i)

#define E(n,s,i) A(n,s,i);B(n,s,i);C(n,s,i);D(n,s,i)
#define F(n,s,i) B(n,s,i);C(n,s,i);D(n,s,i);A(n,s,i)
#define G(n,s,i) C(n,s,i);D(n,s,i);A(n,s,i);B(n,s,i)
#define H(n,s,i) D(n,s,i);A(n,s,i);B(n,s,i);C(n,s,i)

#define I(n,s,i) E(n,s,i);F(n,s,i);G(n,s,i);H(n,s,i)
#define J(n,s,i) H(n,s,i);G(n,s,i);F(n,s,i);E(n,s,i)
#define K(n,s,i) F(n,s,i);F(n,s,i);G(n,s,i);G(n,s,i)
#define L(n,s,i) E(n,s,i);E(n,s,i);H(n,s,i);H(n,s,i)

//flag{0llyDbg0rIDAPr0_To_DebUg_A5_weLL}
void f() {
	char v1[] = "0123456789abcdeffedcba9876543210";
	int v2[] = {82,95,94,74,125,87,81,7,74,40,33,35,100,69,5,57,101,12,111,117,6,3,108,3,103,119,83,59,67,87,41,124,0};
	char flag[33];
	int i;
	int tmp;
	int n = sizeof(v1)-1;
	for(i = 0; i < n*100; i++)
	{
		I(n, v1, i);
		J(n, v1, i);
		K(n, v1, i);
		L(n, v1, i);
	}
	for(i = 0; i < sizeof(v2); i++) {
		flag[i] = v2[i] ^ v1[i];
	}
	printf("flag{%s}\n", flag);
}

int main(int argc, char *argv[]) {
	f();
}