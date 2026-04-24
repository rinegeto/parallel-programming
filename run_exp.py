import subprocess
import os
import time

sizes = [200, 400, 800, 1200, 1600, 2000]
procs_list = [1, 2, 4, 8]

print("Size,Processes,Time")

for size in sizes:
    print(f"Generating {size}x{size} matrices")
    subprocess.run(["python", "generator_m.py", "-s", str(size)], check=True)

    for p in procs_list:
        try:
            print(f"Size={size}, Processes={p}")
            
            subprocess.run(["mpiexec", "-n", str(p), "mpi_lab.exe"], 
                           check=True, capture_output=True)
            
            time.sleep(0.2)

            if os.path.exists("result_c.txt"):
                with open("result_c.txt", "r", encoding="utf-8") as f:
                    for line in f:
                        if line.startswith("Time:"):
                            t_val = line.split(":")[1].strip()
                            print(f"{size},{p},{t_val}")
                            break
                    else:
                        print(f"Time not found in result_c.txt for size={size}, p={p}")
            else:
                print(f"ERROR result_c.txt not created for size={size}, p={p}")

        except subprocess.CalledProcessError as e:
            err_msg = e.stderr.decode('utf-8', errors='ignore') if e.stderr else "Unknown error"
            print(f"ERROR MPI failed for size={size}, p={p}: {err_msg}")
        except Exception as e:
            print(f"ERROR {e}")