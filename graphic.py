import matplotlib.pyplot as plt

def create_graph():
    sizes = [200, 400, 800, 1200, 1600, 2000]
    
    time_1 = [0.0285, 0.2234, 1.8125, 6.1234, 14.4523, 29.5123]
    time_2 = [0.0158, 0.1187, 0.9234, 3.1045, 7.4891, 15.2134]
    time_4 = [0.0091, 0.0635, 0.5103, 1.7712, 4.4812, 9.0876]
    time_8 = [0.0064, 0.0451, 0.3612, 1.2589, 3.1745, 6.3521]
    
    plt.figure(figsize=(10, 6))
    
    plt.plot(sizes, time_1, 'o-', label='1 поток', linewidth=2, markersize=5)
    plt.plot(sizes, time_2, 'o-', label='2 потока', linewidth=2, markersize=5)
    plt.plot(sizes, time_4, 'o-', label='4 потока', linewidth=2, markersize=5)
    plt.plot(sizes, time_8, 'o-', label='8 потоков', linewidth=2, markersize=5)
    
    plt.xlabel('Размер матрицы (N × N)')
    plt.ylabel('Время выполнения (секунды)')
    plt.title('Зависимость времени выполнения от размера матрицы')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.xticks(sizes)
    
    plt.savefig('graphic.png', dpi=150, bbox_inches='tight')
    plt.show()

if __name__ == "__main__":
    create_graph()