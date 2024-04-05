#!/bin/bash

# Run a QEMU machine and restart it when ISO changes
# Courtesy of ChatGPT

# Path to the ISO file
ISO_FILE="build/myos.iso"

# Initial checksum of the ISO file
checksum=$(md5sum "$ISO_FILE")

# Start qemu initially
qemu-system-i386 -cdrom "$ISO_FILE" -monitor unix:/tmp/qemu-monitor.sock,server,nowait &

# Function to send a command to QEMU monitor
send_command_to_monitor() {
    echo "$1" | socat - UNIX-CONNECT:/tmp/qemu-monitor.sock
}

# Reset the QEMU virtual machine
reset_qemu() {
    send_command_to_monitor "system_reset"
}

# Monitor for changes in the ISO file
while true; do
    # Calculate the checksum of the ISO file
    new_checksum=$(md5sum "$ISO_FILE")

    # Compare checksums to detect changes
    if [ "$checksum" != "$new_checksum" ]; then
        # Changes detected, send reset command to qemu monitor interface
        reset_qemu
        checksum="$new_checksum" # Update checksum
    fi

    # Sleep for a while before checking again
    sleep 1
done
