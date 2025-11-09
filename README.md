# 📝 Simple TODO List in C (File-Based Project)

## 📘 Overview
This project is a **menu-driven TODO List application** written in **C language**.  
It allows users to add, display, update, delete, and search tasks — all stored in a **text file (`tasks.txt`)**.

Each task includes:
- **Task Name**
- **Deadline Date** (DD/MM/YYYY)
- **Deadline Time** (HH:MM)
- **Status** → `0` = Not Done, `1` = Done

When a task is marked as *Done*, it is automatically removed from the list during file updates.

---

## ⚙️ Features
✅ Add new tasks with deadlines  
✅ Display all tasks (sorted by date and time)  
✅ Search tasks by name  
✅ Update task details (edit or mark as done)  
✅ Delete tasks manually  
✅ Data is saved persistently using a simple text file  
✅ Easy to understand and modify (beginner-friendly)

---

## 🧩 Technologies Used
- **Language:** C  
- **Concepts Used:**  
  - File Handling  
  - Structures  
  - Arrays  
  - Sorting (Bubble Sort)  
  - String Operations  
  - Menu-Driven Programming

---

## 📂 File Details
- `todo_list.c` → Main source code file  
- `tasks.txt` → Stores all task records  
  Each line in the file follows the format:

---

## ▶️ How to Run

### On Windows (MinGW)

gcc todo_list.c -o todo
todo

## TODO LIST (text file) 
1. Add Task
2. Display All Tasks
3. Search Task by Name
4. Update Task
5. Delete Task
0. Exit
Enter choice: 1
Enter Task Name: Complete Project Report
Enter Deadline Date (DD/MM/YYYY): 12/11/2025
Enter Deadline Time (HH:MM): 18:00
Task added.

## Output inside tasks.txt
Complete Project Report|12/11/2025|18:00|0
Prepare Presentation|14/11/2025|09:30|0

## MIT License

Copyright (c) 2025 Revan Kumar Goud Bommagoni

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
