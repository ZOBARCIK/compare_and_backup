# cab
**Requires C++17**
**Multiplatform, works both on Linux and Windows.**

I've written this for a design project so that me and my mates could use something like git, but not git, while working on the project.

Program basically compares a text based file*(.txt, .c, .cpp, .py, .m, etc...*), to a reference file(*an older version of the comparison file*), and if they're different, creates a copy of the file with timestamp appended to its name.

**cab** is ideal to be used by groups who dont want to haggle with **git** to manage the project. A shared Drive folder can be set up for the group project and **cab** can keep track of the updates.
You can create a shortcut for the executable and put it into startup folder so it will be executed everytime Windows starts, and you wont have to execute it by hand every time.
*C:\Users\ ~username~ \AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup *

![Ekran görüntüsü 2024-02-13 014705](https://github.com/ZOBARCIK/compare_and_backup/assets/119112572/4d72931f-c500-442b-a1f8-a52c92d8ef24)

------------

#### Running the Program

Initially, program asks you to input:
1) Path of the file you want to backup
2) Path of the ***folder*** where you want to store your backups.

Those paths are stored in *location.txt* file, next to your cab.cpp/cab.exe, so you wont have to input everytime.
Next to your backups, there will be a *reference_file.extension* , which is a duplicate of your last backup for future comparisons.
