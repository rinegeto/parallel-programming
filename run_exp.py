import subprocess
import os

sizes = [200, 400, 800, 1200, 1600, 2000]
threads = [1, 2, 4, 8]

print("Size,Threads,Time")

for size in sizes:
    print(f"Генерация матриц {size}x{size}.")
    subprocess.run(["python", "generator_m.py", "-s", str(size)], check=True)
    
    for t in threads:
        try:
            subprocess.run(["./lab", str(t)], check=True, capture_output=True)
            
            with open("result_c.txt", "r", encoding="utf-8") as f:
                for line in f:
                    if line.startswith("Time:"):
                        time_val = line.split(":")[1].strip()
                        print(f"{size},{t},{time_val}")
                        break
                        
        except Exception as e:
            print(f"ERROR size={size}, threads={t}: {e}")