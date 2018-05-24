#!/usr/bin/env python3

'''
	Script para disciplina de Programação Paralela e Distribuída
	                                     by: Anderson A. Fontana
	Parar: Ctrl+C
'''

import os, re, subprocess

##  Variáveis  ##############################################

PROCESSOS = [2, 4, 8, 16, 32, 64]
MAX_ROWs  = [1024]
MAX_COLs  = [768]
MAX_Ns    = [18000]
NRO_EXEC  = 5

# runFolder = '/mpi/152058/'
runFolder = '152058/'

##############################################################

# subprocess.run('module load mpi/openmpi-x86_64', shell=True)
subprocess.run('mkdir -p '+runFolder, shell=True)
subprocess.run('mpic++ mandelbrot.cpp -std=c++11 -o sequencial', shell=True)
subprocess.run('mpic++ mandelbrotMPI-ME.cpp -std=c++11 -o '+runFolder+'meste_escravo', shell=True)

if not os.path.isfile(runFolder+'hosts'):
	print ('Gerando arquivo de hosts...')
	subprocess.run('python3 hosts.py > '+runFolder+'hosts', shell=True)

def execCommandNTimes(programa, np, row, col, n):
	if programa == 'sequencial':
		cmd = 'mpirun -np {1} {0} {2} {3} {4} > {0}.out'.format(programa, np, row, col, n)
	else:
		cmd = 'cd {5} && mpirun -np {1} --hostfile hosts {0} {2} {3} {4} > {0}.out'.format(programa, np, row, col, n, runFolder)

	print(cmd)

	soma = 0
	for i in range(0, NRO_EXEC):
		if programa == 'sequencial':
			subprocess.run(cmd, shell=True)
			tempo = subprocess.getoutput('head -n +1 sequencial.out | tr -d "Tempo[:space:]"')
		else:
			subprocess.run(cmd, shell=True)

			diff = subprocess.getoutput('bash -c \'diff <(tail -n +2 sequencial.out) <(tail -n +2 '+runFolder+'meste_escravo.out)\'')
			if diff == '':
				tempo = subprocess.getoutput('head -n +1 '+runFolder+'meste_escravo.out | tr -d "Tempo[:space:]"')
			else:
				print(diff)
				print('Saídas Diferentes')
				exit(1)

		soma += float(tempo)
		print( '##{}: {} ({} {} {}) {}: T={}'.format(i+1, programa, row, col, n, np, tempo) )
	print( 'avg: {} ({} {} {}) {}: T={}\n'.format(programa, row, col, n, np, soma/NRO_EXEC) )


print(__doc__)

for i in range(0, len(MAX_ROWs)):   # ou MAX_COLs ou MAX_Ns, pois len(MAX_ROWs) == len(MAX_COLs) == len(MAX_Ns)
	execCommandNTimes('sequencial', 1, MAX_ROWs[i], MAX_COLs[i], MAX_Ns[i])

	for p in PROCESSOS:
		execCommandNTimes('meste_escravo', p, MAX_ROWs[i], MAX_COLs[i], MAX_Ns[i])
