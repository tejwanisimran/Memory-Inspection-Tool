# Memory_Inspection_Tool

A Linux-based system programming project that analyzes process information and memory layout using the `/proc` filesystem. This tool helps in understanding how memory is allocated and managed for a running process.

---

## Features

- Inspect process details using PID
- Display memory layout of a process
- Classify memory segments:
  - Heap
  - Stack
  - Text (Code)
  - Data
  - VDSO / VVAR
  - Anonymous memory
- Lightweight and efficient
- Works directly with Linux `/proc` filesystem

---

## Technologies Used

- C Programming
- Linux System Programming
- `/proc` Filesystem
- File Handling 
- String Parsing 

---

##  Project Structure

```
|----Memory_Inspection_Tool
|    |---src
|    |     |----Memory_Inspection_Tool.c
|    |----README.md

```

---

## How It Works

1. User enters a **Process ID (PID)**
2. Program reads:
   - `/proc/[pid]/status` → for process information
   - `/proc/[pid]/maps` → for memory layout
3. Parses and displays:
   - Process Name, PID, State, Threads
   - Memory regions with size and permissions
4. Classifies memory into meaningful sections

---
