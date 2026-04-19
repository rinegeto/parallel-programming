import matplotlib.pyplot as plt

def create_graph(sizes, times):
    plt.figure(figsize=(10, 6))
    plt.plot(sizes, times, marker='o', color='green', linewidth=2, markersize=6)
    plt.title('График зависимости времени выполнения от размера матрицы')
    plt.xlabel('Размер матрицы (N x N)')
    plt.ylabel('Время выполнения (секунды)')
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.savefig('execution_time_graph.png', dpi=150)
    plt.show()

def main():
    sizes = [0, 200, 400, 800, 1200, 1600, 2000]
    times = [0, 0.0054683, 0.0726069, 0.95123, 2.90343, 8.3992, 36.7616]
    create_graph(sizes, times)

if __name__ == "__main__":
    main()
