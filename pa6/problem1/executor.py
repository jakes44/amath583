import subprocess

if __name__ == "__main__":
    # Run the command to get the list of files
    program = "./thread_quad"

    print("Running to capture scaling efficiency")
    for i in range(5):
        threads = 2 ** i
        num_points = 100000000
        result = subprocess.run([program, str(num_points), str(threads)], capture_output=True, text=True)
        # Save output to a file
        with open("scaling_efficiency.csv", "a") as f:
            f.write(result.stderr)
    
    print("Running to capture numerical error")
    for i in range(1,7):
        threads = 12
        num_points = 10 ** i
        result = subprocess.run([program, str(num_points), str(threads)], capture_output=True, text=True)
        # Save output to a file
        with open("numerical_error.csv", "a") as f:
            f.write(result.stderr)