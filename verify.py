import numpy as np
import sys

def main(): 
    matrixA = np.loadtxt("matrix_a.txt", skiprows=1)
    matrixB = np.loadtxt("matrix_b.txt", skiprows=1)
    matrixAB = np.loadtxt("result_c.txt", skiprows=1)  
    
    matrixRes = np.dot(matrixA, matrixB)
    
    if np.allclose(matrixRes, matrixAB, rtol=1e-9, atol=1e-9):
        print("Проверка прошла успешно")
        return 0
    else:
        print("Ошибка: результаты не совпадают")
        print(f"Макс. разница: {np.max(np.abs(matrixRes - matrixAB)):.2e}")
        return 1
    
if __name__ == "__main__":
    result = main()
    sys.exit(result)
