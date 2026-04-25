import matplotlib.pyplot as plt

def create_cuda_graph():
    sizes = [200, 400, 800, 1200, 1600, 2000]
    
    t_8 = [0.000812, 0.001583, 0.005124, 0.016247, 0.037156, 0.071892]
    t_16 = [0.000741, 0.001389, 0.004102, 0.011538, 0.025841, 0.048216]
    t_32 = [0.000798, 0.001412, 0.004089, 0.011421, 0.025193, 0.047865]
    
    plt.figure(figsize=(10, 6))
    
    plt.plot(sizes, t_8, 'o-', label='Блок 8×8', linewidth=2, markersize=5)
    plt.plot(sizes, t_16, 'o-', label='Блок 16×16', linewidth=2, markersize=5)
    plt.plot(sizes, t_32, 'o-', label='Блок 32×32', linewidth=2, markersize=5)
    
    plt.xlabel('Размер матрицы (N × N)')
    plt.ylabel('Время выполнения (секунды)')
    plt.title('CUDA: Зависимость времени от размера матрицы и блока')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.xticks(sizes)
    
    plt.savefig('graphic.png', dpi=150, bbox_inches='tight')
    plt.show()

if __name__ == "__main__":
    create_cuda_graph()