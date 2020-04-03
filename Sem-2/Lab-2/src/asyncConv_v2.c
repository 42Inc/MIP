#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int N = 11;

int main(){
	srand(time(0));
	int t = 0, ar = 100000;
	int Conv[N];
	for (int i = 0; i < N; i++) {
		Conv[i] = 0;
	}
	int buf[N+1];
	for (int i = 0; i < N+1; i++) {
		buf[i] = 0;
	}
	int f = 0;
	while (f != 1) {
		if(buf[0] == 0 && ar > 0){
			buf[0] = rand()%8 +1;
			--ar;
		}
		for (int i = 0; i < N; ++i) {
			if(buf[i] != 0 && Conv[i] == 0){
				Conv[i] = buf[i];
				buf[i] = 0;
				--Conv[i];
			} else if(Conv[i] != 0){
				--Conv[i];
				if(Conv[i] == 0){
					buf[i+1] = rand()%8 + 1;
				}
			}
		}
		if(ar == 0){
			for(int i = 0; i < N; ++i){
				if(Conv[i] != 0 ){
					f = 0;
					break;
				} else {
					f = 1;
				}
			}
		}
		++t;
	}
	printf("%d\n", t); //количество тактов return 0; }
	return EXIT_SUCCESS;
}
