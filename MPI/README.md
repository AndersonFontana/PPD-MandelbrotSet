# Mandelbrot em MPI

## Programação Paralela e Distribuída

Por: Anderson Antonio Fontana


### Compilação

```bash
	mpic++ mandelbrotMPI-ME.cpp -std=c++11 -o meste_escravo
```


### Execução Manual

```bash
	mpirun -np  2 --hostfile hosts meste_escravo 1024 768 18000   # Para execução com  2 processos
	mpirun -np  4 --hostfile hosts meste_escravo 1024 768 18000   # Para execução com  4 processos
	mpirun -np  8 --hostfile hosts meste_escravo 1024 768 18000   # Para execução com  8 processos
	mpirun -np 16 --hostfile hosts meste_escravo 1024 768 18000   # Para execução com 16 processos
	mpirun -np 32 --hostfile hosts meste_escravo 1024 768 18000   # Para execução com 32 processos
	mpirun -np 64 --hostfile hosts meste_escravo 1024 768 18000   # Para execução com 64 processos
```


### Execução Automática

```bash
	python3 autoMPI.py
```


Porém é preciso abrir o script e configurar algumas variáveis, como mostrado abaixo:

```python
	##  Variáveis  ##############################################

	PROCESSOS = [2, 4, 8, 16, 32, 64]  # Número de processos a ser executado
	MAX_ROWs  = [1024]                 # Números de Linhas
	MAX_COLs  = [768]                  # Números de Colunas
	MAX_Ns    = [18000]                # Tamanhos do N
	NRO_EXEC  = 5                      # Número de vezes para executar cada comando

	runFolder = '/mpi/152058/'         # Pasta para execução do programa paralelo
```
O resultado em tempo de execução do script *autoMPI.py*, será mostrado na saída padrão
