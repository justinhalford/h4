# script: calculate_buffer_size.py
import struct

# Address of the 'temp1' buffer from GDB
temp1_buffer_addr = 0xffffd364

# Address of the saved EBP from GDB
ebp_addr = 0xffffd36c

# Calculate the buffer size
buffer_size = ebp_addr - temp1_buffer_addr + 8

print(f"Buffer size: {buffer_size}")