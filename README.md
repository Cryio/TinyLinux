# TinyLinux Distro

A minimal Linux distribution focusing on a compact kernel, custom shell, and efficient system call integration. Designed for educational purposes and lightweight applications.

![TinyLinux Demo](https://via.placeholder.com/800x400.png?text=TinyLinux+Demo+in+QEMU)

## Features

- **Ultra-Compact Kernel**: Configured with `tinyconfig` (size ~781 KB).
- **Custom Shell**: Built in C with assembly-optimized system calls.
- **Minimal Footprint**: Final ISO size as low as **2.0 MB** (3.1 MB with Lua).
- **QEMU-Compatible**: Test the distro effortlessly in an emulated environment.

## Project Structure

~~~
TINYLINUX/
├── shell/                   # Shell implementation and dependencies
│   ├── asm_shell/          # Assembly-based shell components
│   │   └── shell.asm
│   └── lua_build/          # Lua integration build files
│       ├── lua-5.4.7/
│       └── lua-5.4.7.tar.gz
├── kernel_without_shell/   # Minimal kernel configuration without shell
├── build.sh                # Main build script
├── init*                   # Initialization scripts (.cpio variants)
├── shell.c                 # C-based shell implementation
├── sys.S                   # Assembly system call wrappers
├── syscalls_64.h           # System call definitions
├── tinylinux_shell.iso     # Base ISO image
├── tinyLinux_shell.ua.iso  # ISO with Lua integration
├── LICENSE
└── README.md
~~~

# Build Artifacts (generated during compilation)

- *.o :                # Object files
- entry.id :          # Build identifier
- files.cpio :        # Packaged filesystem
- lua :               # Compiled Lua interpreter


**Key Files**:
- `build.sh`: Automated build script for the distro
- `init*.cpio`: Different initramfs configurations
- `sys.S`: Assembly system call implementations
- `syscalls_64.h`: Kernel system call headers

**Notable Directories**:
- `asm_shell/`: Contains low-level assembly shell components
- `lua_build/`: Lua source and build artifacts for extended functionality
- `kernel_without_shell/`: Minimal kernel build without shell dependencies

## Prerequisites

- Linux-based host system (Ubuntu/Debian recommended)
- Build tools: `gcc`, `make`, `binutils`, `libc-dev`
- QEMU for emulation: `sudo apt install qemu-system-x86`

## Building the Kernel

1. **Clone and Configure the Kernel**:
   ~~~bash
   git clone https://github.com/torvalds/linux.git
   cd linux
   make tinyconfig
   ~~~
   - Enable **64-bit support**, **initramfs**, and **C standard libraries** in `make menuconfig`.

2. **Compile**:
   ~~~bash
   make -j4
   ~~~
   Validate the kernel image size at `arch/x86/boot/bzImage`.

## User Space Setup

1. **Build the Shell**:
   ~~~bash
   gcc -static -o shell shell.c sys.S
   strip shell   # Reduce binary size
   ~~~

2. **Create initramfs**:
   ~~~bash
   mkdir initramfs
   echo '#!/bin/sh' > initramfs/init
   echo '/shell' >> initramfs/init
   chmod +x initramfs/init
   cp shell initramfs/
   ~~~

## Testing with QEMU

1. **Package the ISO**:
   ~~~bash
   cd initramfs
   find . | cpio -o -H newc | gzip > ../initramfs.cpio.gz
   cd ..
   qemu-system-x86_64 -kernel linux/arch/x86/boot/bzImage -initrd initramfs.cpio.gz
   ~~~

2. **Verify Functionality**:
   - Test commands like `/bin/ls` in the custom shell.

## Assembly System Calls

The `sys.S` file provides low-level system call wrappers:
~~~nasm
.global asm_waitid
asm_waitid:
    mov rax, 247
    syscall
    ret
~~~

## Final ISO Creation

1. **Build Bootable Image**:
   ~~~bash
   grub-mkrescue -o TinyLinux.iso kernel/ initramfs/
   # or use xorriso for finer control
   ~~~

## Conclusion

This project demonstrates:
- Configuring a minimal Linux kernel.
- Integrating custom user-space components.
- Achieving a functional distro under **3.1 MB**.

## License

MIT License. See [LICENSE](LICENSE) for details.
