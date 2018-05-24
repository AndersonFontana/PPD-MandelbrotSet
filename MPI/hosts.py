#!/usr/bin/env python3

'''
	Imprime todas as máquinas na sala especificada que estão ligadas e com linux executando
										by: Anderson A. Fontana
	Parar: Ctrl+C
'''

import subprocess
import re


def canUseIp(ip):
	ping = subprocess.getoutput('ping ' + ip + ' -c 1 -W 1')
	# print(ping)
	ping = ping.split('\n')[1]
	if ping != "":
		ttl = re.findall(r'ttl=(\d+)', ping)[0]
		return ttl == '64'
	else:
		return False


print(__doc__)

rede = '192.168.14.'

ifconfig = subprocess.getoutput('ifconfig | grep "inet "')
meuIP = re.findall(r'inet(?: end\.:)? (' + rede + r'\d+)', ifconfig)[0]

print(meuIP + ' slots=4')
for i in range(1, 30):
	ip = rede + str(i)
	if meuIP != ip and canUseIp(ip):
		print(ip + ' slots=4')
