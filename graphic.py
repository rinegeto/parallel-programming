import matplotlib.pyplot as plt

def create_mpi_graph():
    sizes = [200, 400, 800, 1200, 1600, 2000]
    
    t_1 = [0.0219, 0.1746, 1.4221, 4.8359, 13.2670, 25.5170]
    t_2 = [0.0118, 0.0944, 0.7687, 2.6140, 7.1714, 13.7930]
    t_4 = [0.0071, 0.0563, 0.4587, 1.5599, 4.2797, 8.2313]
    t_8 = [0.0046, 0.0364, 0.2963, 1.0075, 2.7640, 5.3160]
    
    plt.figure(figsize=(10, 6))
    
    plt.plot(sizes, t_1, 'o-', label='1 процесс', linewidth=2, markersize=5)
    plt.plot(sizes, t_2, 'o-', label='2 процесса', linewidth=2, markersize=5)
    plt.plot(sizes, t_4, 'o-', label='4 процесса', linewidth=2, markersize=5)
    plt.plot(sizes, t_8, 'o-', label='8 процессов', linewidth=2, markersize=5)
    
    plt.xlabel('Размер матрицы (N × N)')
    plt.ylabel('Время выполнения (секунды)')
    plt.title('MPI: Зависимость времени от размера матрицы')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.xticks(sizes)
    
    plt.savefig('graphic.png', dpi=150, bbox_inches='tight')

    plt.show()

if __name__ == "__main__":
    create_mpi_graph()