#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
	int MAX_ROW, MAX_COL, MAX_NUM;

	if (argc == 4) {
		MAX_ROW = atoi(argv[1]);
		MAX_COL = atoi(argv[2]);
		MAX_NUM = atoi(argv[3]);
	}
	else
		exit(1);

	char **mat = (char**)malloc(sizeof(char*)*MAX_ROW);

	for (int i=0; i<MAX_ROW;i++)
		mat[i]=(char*)malloc(sizeof(char)*MAX_COL);

	for(int r = 0; r < MAX_ROW; ++r){
		for(int c = 0; c < MAX_COL; ++c){
			int n = 0;

			float x=0,y=0, tmp;
			while ((x*x) + (y*y) < 4 && ++n < MAX_NUM) {
				tmp = x*x - y*y + ((float)c * 2 / MAX_COL - 1.5);
				y   = x * y * 2 + ((float)r * 2 / MAX_ROW - 1);
				x   = tmp;
			}


			mat[r][c]=(n == MAX_NUM ? '#' : '.');
		}
	}

	for(int r = 0; r < MAX_ROW; ++r){
		for(int c = 0; c < MAX_COL; ++c)
			cout << mat[r][c];
		cout << '\n';
	}	
}


