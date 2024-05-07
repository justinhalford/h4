# script: calculate_ret_offset.py
import struct

# Address of the 'temp1' buffer from GDB
temp1_buffer_addr = 0xffffd364

# Address of the saved EIP from GDB
eip_addr = 0xffffd360

# Calculate the offset to the return address
ret_offset = eip_addr - temp1_buffer_addr

print(f"Return address offset: {ret_offset}")