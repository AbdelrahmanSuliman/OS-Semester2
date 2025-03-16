# Task 1 
- This shell executes user commands, supports batch file execution, I/O redirection, and
background processes. It provides commands like cd, ls, echo, and environ, with a prompt
displaying the current directory. Users can redirect input/output, run commands in the
background, and execute scripts from a file.

[Code for task 1](task1/main.c)

---
# Task 2
- This program assigns structs to each thread storing information like the frequency of words
in key pair values (which is an already defined struct) and the words it loops through and
using mutex I am able to prevent race conditions so no two threads can split up a word and
finally I merge the results of all the threads into one and print the result.
For some reason the pthread.h library doesn’t work on windows so had to do it again for
linux

[Code for task 2](task2/main.c)
---
# Task 3
- My program reads from a file a sequence of page references and uses an Aging algorithm
on a fixed number of frames in this case 5 and then outsputs the number of page faults
which occurs when a program requests a page which isn’t found in the memory so it must
be loaded from the virtual memory in the OS case its usually a hard disk.
The algorithm ensures that the highest priority pages stay in the allotted fixed number of
frames, so if a page was not referenced for a while it will be replaced

[Code for task 3](task3/main.c)
---
# Task 7
- This program uses Work-Finish method to mark processes as either finished if the
requested resources are available or deadlocked if the resources aren’t available and is
dissimilar to the bankers algorithm in the fact that its merely detecting a deadlock not
preventing it

[Code for task 7](task7/main.c)

---
