#include <complex>
#include <iostream>
#include <cstdlib>
#include <mpi.h>

using namespace std;


int main(int argc, char **argv) {
	int  MAX_ROW, MAX_COL, MAX_N, meurank;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
	
	if (argc == 4) {
		MAX_ROW = atoi(argv[1]);
		MAX_COL = atoi(argv[2]);
		MAX_N   = atoi(argv[3]);
	}
	else {
		cout << "São esperados 4 parametros, recebido " << argc - 1 << endl;
		exit(0);
	}

	if (meurank == 0) {
		char **mat = (char**)malloc(sizeof(char*)*MAX_ROW);

		for (int i=0; i<MAX_ROW;i++)
			mat[i]=(char*)malloc(sizeof(char)*MAX_COL);

		double inicio = MPI_Wtime();

		for(int r = 0; r < MAX_ROW; ++r){
			for(int c = 0; c < MAX_COL; ++c){
				complex<float> z;
				int n = 0;
				while(abs(z) < 2 && ++n < MAX_N)
					z = pow(z, 2) + decltype(z)(
						(float)c * 2 / MAX_COL - 1.5,
						(float)r * 2 / MAX_ROW - 1
					);
				mat[r][c]=(n == MAX_N ? '#' : '.');
			}
		}

		cout << "Tempo\t" << MPI_Wtime() - inicio << endl;

		for(int r = 0; r < MAX_ROW; ++r){
			for(int c = 0; c < MAX_COL; ++c)
				std::cout << mat[r][c];
			cout << endl;
		}
	}

	MPI_Finalize();

	return 0;
}


