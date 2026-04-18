import numpy as np
import sys

def read_mat(file):
    with open(file) as f:
        n = int(f.readline())
        return np.array([list(map(float, f.readline().split())) for _ in range(n)])

try:
    A = read_mat("matrix_a.txt")
    B = read_mat("matrix_b.txt")
    C = read_mat("result_c.txt")
    
    if np.allclose(np.dot(A, B), C):
        print("Верификация успешна")
        sys.exit(0)
    else:
        print("Ошибка вычислений")
        sys.exit(1)
except Exception as e:
    print(f"ERROR {e}")
    sys.exit(1)
