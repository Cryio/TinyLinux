# TinyLinux Distro

A minimal Linux distribution focusing on a compact kernel, custom shell, and efficient system call integration. Designed for educational purposes and lightweight applications.

*Example shell interaction (see [screenshots](#screenshots) below).*

---

## Features

- **Ultra-Compact Kernel**: Configured with `tinyconfig` (size ~781 KB).
- **Custom Shell**: Built in C with assembly-optimized system calls ([ABI reference](https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI)).
- **Minimal Footprint**: Final ISO size as low as **2.0 MB** (3.1 MB with Lua).
- **QEMU-Compatible**: Test the distro effortlessly in an emulated environment.

---

## Project Structure

~~~
TINYLINUX/
├── shell/                   # Shell implementation
│   ├── asm_shell/          # Assembly-based components
│   │   └── shell.asm
│   └── lua_build/          # Lua integration
│       ├── lua-5.4.7/
│       └── lua-5.4.7.tar.gz
├── kernel_without_shell/   # Minimal kernel (no shell)
├── build.sh                # Build automation script
├── init*                   # Initramfs configurations
├── shell.c                 # C shell source
├── sys.S                   # Assembly syscall wrappers
├── tinylinux_shell.iso     # Base ISO
├── tinyLinux_shell.ua.iso  # ISO with Lua
└── ...                     # [See full structure](#screenshots)
~~~

---

## Screenshots

### 1. Shell Implementation (Code Snippet)
![Shell Code](ss/(6).jpeg)  
*Core loop for command execution (full code in `shell.c`).*

### 2. Kernel Configuration Interface
![Kernel Config](ss/(2).jpeg)  
*Example of kernel module selection and error handling.*

---

## Key Technical Components

### Custom Shell Design
~~~c
#include <unistd.h>
#include <sys/wait.h>

// Simplified main loop (full code in shell.c)
int main() {
    char command[255];
    for (;;) {
        write(1, "# ", 2);
        int count = read(0, command, 255);
        command[count-1] = 0; // Null-terminate
        
        pid_t pid = fork();
        if (pid == 0) {
            execve(command, NULL, NULL);
            _exit(1); // Fail if execve returns
        } else {
            siginfo_t info;
            waitid(P_ALL, 0, &info, WEXITED);
        }
    }
}
~~~
*Follows [System V AMD64 ABI](https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI) for system calls.*

### Assembly System Calls
~~~nasm
; sys.S - Waitid implementation (Intel SDM Vol. 2 reference)
asm_waitid:
    mov rax, 247     ; SYS_waitid
    syscall
    ret
~~~
*See [Intel® 64 and IA-32 Architectures Software Developer Manuals](https://cdrdv2-public.intel.com/782156/325383-sdm-vol-2abcd.pdf) for syscall details.*

---

## Building & Testing

1. **Compile the Shell**:
   ~~~bash
   gcc -static -o shell shell.c sys.S
   strip shell
   ~~~

2. **Create Initramfs**:
   ~~~bash
   echo '#!/bin/sh' > init
   echo '/shell' >> init
   chmod +x init
   ~~~

3. **Test in QEMU**:
   ~~~bash
   qemu-system-x86_64 -kernel bzImage -initrd initramfs.cpio.gz
   ~~~

---

## References

1. [System V AMD64 ABI](https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI) - Calling convention for syscalls
2. [Intel® 64 Architecture Manual](https://cdrdv2-public.intel.com/782156/325383-sdm-vol-2abcd.pdf) - Low-level syscall details
3. [Linux Kernel Documentation](https://www.kernel.org/doc/html/latest/) - Kernel configuration

---

## License

MIT License. See [LICENSE](LICENSE) for details.
