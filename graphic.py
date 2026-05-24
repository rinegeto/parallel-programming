import matplotlib.pyplot as plt

sizes = [100, 200, 400, 800, 1200, 1600, 2000]

threads_1 = [1, 13, 81, 618, 2187, 5412, 10234]
threads_2 = [0, 7, 42, 315, 1098, 2789, 5287]
threads_4 = [0, 3, 22, 189, 641, 1456, 2834]
threads_8 = [0, 1, 13, 162, 478, 1087, 2103]

plt.figure(figsize=(12, 7))

plt.plot(sizes, threads_1, 'o-', label='1 поток', linewidth=2, markersize=8)
plt.plot(sizes, threads_2, 'o-', label='2 потока', linewidth=2, markersize=8)
plt.plot(sizes, threads_4, 'o-', label='4 потока', linewidth=2, markersize=8)
plt.plot(sizes, threads_8, 'o-', label='8 потоков', linewidth=2, markersize=8)

plt.xlabel('Размер матрицы (N × N)', fontsize=12)
plt.ylabel('Время выполнения (секунды)', fontsize=12)
plt.title('Зависимость времени выполнения от размера матрицы и количества потоков', fontsize=14, fontweight='bold')
plt.legend(fontsize=11)
plt.grid(True, linestyle='--', alpha=0.7)
plt.xticks(sizes)
plt.tight_layout()

plt.savefig('graphic.png', dpi=300, bbox_inches='tight')

plt.show()