import argparse
import numpy as np


def generate_matrix(size):
    """
    Генерирует случайную матрицу
    """
    return np.random.randint(-500, 500, (size, size))


def save_matrix(filepath, matrix, size):
    """
    Сохраняет матрицу в текстовый файл
    """
    lines = []
    for row in matrix:
        lines.append(" ".join(map(str, row)))
    
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"{size}\n")
        f.write("\n".join(lines))


def main():
    parser = argparse.ArgumentParser(description="Генератор тестовых матриц")
    parser.add_argument("-s", "--size", type=int, default=2000, 
                       help="Размер матрицы")
    parser.add_argument("-a", "--matrix-a", type=str, default="matrix_a.txt",
                       help="Имя файла для первой матрицы")
    parser.add_argument("-b", "--matrix-b", type=str, default="matrix_b.txt",
                       help="Имя файла для второй матрицы")
    
    args = parser.parse_args()
    
    try:
        mat_a = generate_matrix(args.size)
        mat_b = generate_matrix(args.size)

        save_matrix(args.matrix_a, mat_a, args.size)
        save_matrix(args.matrix_b, mat_b, args.size)
        
        print(f"Созданы файлы: {args.matrix_a}, {args.matrix_b}")

    except Exception as e:
        print(f"ERROR {e}")


if __name__ == "__main__":
    main()