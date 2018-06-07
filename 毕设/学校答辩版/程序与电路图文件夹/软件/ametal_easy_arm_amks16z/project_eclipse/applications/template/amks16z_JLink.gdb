# connect to the J-Link gdb server
target remote localhost:2331

# Select flash device
monitor flash device = MKL16Z128xxx4

# Set gdb server to little endian
monitor endian little

monitor if SWD

# Set JTAG speed to 1000 kHz
monitor speed 1000

# Reset the target
monitor reset
#monitor sleep 100

# Set JTAG speed to 2000 khz
monitor speed 2000

# Vector table placed in Flash
monitor writeu32 0xE000ED08 = 0x00000000

load

monitor reg r13 = (0x00000000)
monitor reg pc  = (0x00000004)

break main
continue
