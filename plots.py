import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data_LZ = pd.read_csv('Resultados_LZ.csv', delimiter=';', header=None)
data_HF = pd.read_csv('Resultados_Huffman.csv', delimiter=';', header=None)

meantotaltimeHF = np.mean(data_HF.iloc[:,0])
meancodtimeHF = np.mean(data_HF.iloc[:,1])
meandecodtimeHF = np.mean(data_HF.iloc[:,2])

print(100*'-')
print('Tiempos promedio experimentos Huffman')
print(100*'-')
print(f'Tiempo promedio ambas operaciones: {meantotaltimeHF}. Desviación estándar: {np.std(data_HF.iloc[:,0])}')
print(f'Tiempo promedio codificacion: {meancodtimeHF}. Desviación estándar: {np.std(data_HF.iloc[:,1])}')
print(f'Tiempo promedio decodificacion: {meandecodtimeHF}. Desviación estándar: {np.std(data_HF.iloc[:,2])}')
print(100*'-')

meantotaltimeLZ = np.mean(data_LZ.iloc[:,0])
meancomptimeLZ = np.mean(data_LZ.iloc[:,1])
meandecomptimeLZ = np.mean(data_LZ.iloc[:,2])

print(100*'-')
print('Tiempos promedio experimentos Lempel-Ziv')
print(100*'-')
print(f'Tiempo promedio ambas operaciones: {meantotaltimeLZ}. Desviación estándar: {np.std(data_LZ.iloc[:,0])}')
print(f'Tiempo promedio compresion: {meancomptimeLZ}. Desviación estándar: {np.std(data_LZ.iloc[:,1])}')
print(f'Tiempo promedio descompresion: {meandecomptimeLZ}. Desviación estándar: {np.std(data_LZ.iloc[:,2])}')
print(100*'-')

#Plots para comparar tiempos
x = np.linspace(1, 20, num=20) #Linspace que va de 1 en 1 de 1 hasta 20, para igualar el numero de iteraciones

#LempelZiv
#plt.plot(x, data_LZ.iloc[:,0], label='Tiempo total')
#plt.plot(x, data_LZ.iloc[:,1], label='Tiempo Compresión')
#plt.plot(x, data_LZ.iloc[:,2], label='Tiempo Descompresión')
#plt.xlabel('Número de iteración.')
#plt.ylabel('Tiempo $[s]$')
#plt.title('Tiempo experimentos, compresión Lempel-Ziv')

#Huffman
plt.plot(x, data_HF.iloc[:,0], label='Tiempo total')
plt.plot(x, data_HF.iloc[:,1], label='Tiempo Codificación')
plt.plot(x, data_HF.iloc[:,2], label='Tiempo Decodificación')
plt.xlabel('Número de iteración.')
plt.ylabel('Tiempo $[s]$')
plt.title('Tiempo experimentos, Codificación Huffman')

plt.legend()
plt.grid(True, alpha=0.5)
plt.xlim(1,20)
plt.show()
