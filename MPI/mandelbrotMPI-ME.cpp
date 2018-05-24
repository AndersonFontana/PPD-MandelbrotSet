#include <iostream>
#include <complex>
#include <stdlib.h>
#include <mpi.h>

using namespace std;

int is_power_of_2(int num) {
	return (num & (num - 1)) == 0;
}

int main(int argc, char **argv){
	char *vet;
	int i, ind, tempInd, nElements;
	int n, r, c, meurank, procs, parte;
	int MAX_ROW, MAX_COL, MAX_N;
	int tag{10}, fim{101};
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);

	if (argc == 4) {
		MAX_ROW = atoi(argv[1]);
		MAX_COL = atoi(argv[2]);
		MAX_N   = atoi(argv[3]);
	}
	else {
		if (meurank == 0)
			cout << "São esperados 4 parametros, recebido " << argc - 1 << endl;
		MPI_Finalize();
		exit(0);
	}

	if (!is_power_of_2(procs) || procs > 64) {
		if (meurank == 0)
			cout << "Número de processos (" << procs << ") não é potência de 2" << endl;
		MPI_Finalize();
		exit(0);
	}

	int indices[procs];

	parte = 128;	// MAX_ROW*MAX_COL / procs;

	int MAX = MAX_ROW*MAX_COL;

	double inicio;
	if (meurank == 0) {
		vet = (char *) malloc(sizeof(char) * MAX);
		char * tempVet = (char *) malloc(sizeof(char) * parte);

		inicio = MPI_Wtime();

		ind = 0;
		for (i=1; i < procs; ++i) {
			indices[i] = ind;
			MPI_Send(&ind, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
			ind += parte;
		}

		// Conforme recebe manda uma nova fatia
		while ( ind < MAX ) {
			MPI_Recv(tempVet, parte, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			tempInd = indices[status.MPI_SOURCE];
			MPI_Get_count(&status, MPI_BYTE, &nElements);

			indices[status.MPI_SOURCE] = ind;
			MPI_Send(&ind, 1, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD);
			ind += parte;

			memcpy(&vet[tempInd], tempVet, sizeof(char) * nElements);
		}
		// Recebe resultado dos ultimos procs processos 
		for (i=1; i < procs; ++i) {
			MPI_Recv(tempVet, parte, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			tempInd = indices[status.MPI_SOURCE];
			MPI_Get_count(&status, MPI_BYTE, &nElements);
			memcpy(&vet[tempInd], tempVet, sizeof(char) * nElements);
		}

		// Manda finalizar todos
		for (i=1; i < procs; ++i)
			MPI_Send(&i, 1, MPI_INT, i, fim, MPI_COMM_WORLD);

		cout << "Tempo\t" << MPI_Wtime() - inicio << endl;

		for(i = 0; i < MAX; ++i) {
			cout << vet[i];
			if (i % MAX_COL == MAX_COL-1)
				cout << endl;
		}
	}
	else {
		vet = (char *) malloc(sizeof(char) * parte);
		while (1) {
			MPI_Recv(&ind, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			if (status.MPI_TAG != tag)
				break;

			for (i = 0; i < parte; ++i) {
				
				if (ind + i >= MAX)
					break;

				c = (ind + i) % MAX_COL;
				r = (ind + i) / MAX_COL;

				n = 0;
				complex<float> z;
				while (abs(z) < 2 && ++n < MAX_N)
					z = pow(z, 2) + decltype(z)(
						(float)c * 2 / MAX_COL - 1.5,
						(float)r * 2 / MAX_ROW - 1
					);
				vet[i] = (n == MAX_N ? '#' : '.');
			}

			MPI_Send(vet, i, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
		}
	}

	free(vet);

	MPI_Finalize();

	return(0);
}
