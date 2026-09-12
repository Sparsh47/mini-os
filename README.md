# Mini-OS

A hands-on, modular implementation of fundamental operating system concepts in C (C11), developed as a low-level systems lab. Each module explores a core OS component—from process management and virtual address translation to custom heap allocation, paging, and file systems.

---

## 📂 Project Architecture

```
mini-os/
├── allocator/        # Custom heap memory allocator (search policies, splitting, coalescing, benchmarking)
├── shell/            # Interactive Unix shell (fork-exec, command parsing, REPL)
├── translate/        # Virtual memory inspector (Linux /proc/[pid]/maps parser)
├── common/           # Shared utilities and helper headers
├── paging/           # Page tables & address translation (in development)
├── tlb/              # Translation Lookaside Buffer simulation (in development)
├── replace/          # Page replacement policies (FIFO, LRU, Clock) (in development)
├── fs/               # File system abstractions (in development)
├── net/              # Networking & socket programming (in development)
├── Dockerfile        # Ubuntu 22.04 container with GDB, Valgrind, strace
└── Makefile          # Top-level build orchestration
```

---

## 🛠️ Modules Overview

### 1. Custom Memory Allocator (`allocator/`)
A custom userspace memory allocator implementing core principles of dynamic heap management (`malloc` / `free`):
* **In-Band Metadata (`struct mem_header`)**: Tracks block size, allocation status (`FREE` vs `ALLOCATED`), and a magic watermark (`0xDEADBEEF`) to detect heap corruption.
* **Pluggable Placement Policies (`policies.c`)**:
  * **First-Fit**: Scans sequentially to return the first large enough block.
  * **Best-Fit**: Minimizes leftover block fragments by picking the smallest sufficient block.
  * **Worst-Fit**: Selects the largest available block to leave larger, more usable split remainders.
  * **Next-Fit**: Continues search from the previous allocation pointer to distribute allocations.
  * Switch policies at compile-time via `#define ALLOC_POLICY` in `mymalloc.c`.
* **Block Splitting**: Carves out requested payloads from larger free blocks and dynamically inserts a new free block header for the remaining space.
* **Block Coalescing**: Scans and merges contiguous adjacent free blocks on `my_free()` to mitigate external fragmentation.
* **Fragmentation Benchmark Suite (`benchmark.c`)**:
  * Simulates realistic allocation/deallocation workloads with deterministic seeded random traces.
  * Quantifies **External Fragmentation** using:
    $$\text{Fragmentation} = 1.0 - \left(\frac{\text{Largest Free Block}}{\text{Total Free Memory}}\right)$$
  * Reports allocation success/failure counts, heap block counts (total, free, allocated), and free memory distributions.
* **Integrity Validation**: Verifies headers and magic signatures before freeing memory.

```bash
cd allocator
make
./mymalloc
```

### 2. Address Space Inspector (`translate/`)
A virtual memory analyzer that reads and visualizes Linux process memory layouts from `/proc/<pid>/maps`:
* Decodes memory ranges, permissions (`rwxp`), file offsets, device/inode identifiers, and pathnames.
* Categorizes mapped segments into logical memory regions: `CODE`, `HEAP`, `STACK`, `VDSO`, `VVAR`, `VSYS`, and `ANONYMOUS` mappings.

```bash
cd translate
make
./translate <PID>
```

### 3. Interactive Shell (`shell/`)
A Unix command-line interpreter:
* **Interactive REPL**: Continuously prompts, accepts user commands, and handles clean exit.
* **Process Lifecycle**: Implements `fork()` and `execvp()` to execute system programs concurrently, with parent process synchronization using `waitpid()`.
* **Argument Tokenizer**: Parses input commands into null-terminated argument vectors (`argv`).

```bash
cd shell
make
./shell
```

---

## 🐳 Development Environment (Docker)

To ensure a consistent Linux environment (crucial for `/proc` filesystem access and POSIX APIs on macOS/Windows), a development Docker container is provided.

### Build the Image
```bash
docker build -t mini-os-dev .
```

### Run the Container
Mount your local directory into the container to compile and debug with `valgrind`, `gdb`, and `strace`:
```bash
docker run -it --rm -v "$(pwd)":/workspace mini-os-dev
```

---

## ⚙️ Compilation

Each module contains its own dedicated `Makefile`. The project targets the `C11` standard with strict compiler warnings:

```bash
gcc -Wall -Wextra -std=c11 -I../common <source_files> -o <binary>
```

To build a specific module, navigate into its folder and run:
```bash
make
```

---

## 🤝 Contributing

Contributions, experiments, and new educational modules are welcome! Here’s how to get involved:

### 1. Workflow
1. **Fork the repository** and clone your fork locally.
2. **Create a feature branch** from `main`:
   ```bash
   git checkout -b feat/your-feature-name
   ```
3. **Develop within the Docker container** or a compatible POSIX/Linux environment:
   ```bash
   docker run -it --rm -v "$(pwd)":/workspace mini-os-dev
   ```
4. **Commit with descriptive messages**:
   ```bash
   git commit -m "feat(allocator): implement coalescing for adjacent free blocks"
   ```
5. **Push and open a Pull Request**.

### 2. Code Quality & Standards
* **C Standard**: Stick strictly to `C11`.
* **Zero Warnings**: All code must compile cleanly with `-Wall -Wextra -std=c11`.
* **Memory Safety**: Check your code with `valgrind` to verify zero memory leaks or invalid pointer accesses.
* **Modular Structure**: Place shared helper functions in `common/` and keep domain-specific logic self-contained within its respective module directory.

### 3. Ideas for Contributions
* **`allocator/`**: Add boundary tags for $O(1)$ bidirectional coalescing, segregated free lists, or a buddy allocator.
* **`paging/`**: Multi-level page table walk simulator with physical address calculation.
* **`replace/`**: Simulation of LRU, FIFO, and Clock page replacement algorithms.
* **`shell/`**: Support for I/O redirection (`>`, `<`) and pipes (`|`).

