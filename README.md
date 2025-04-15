# Task 1 
- This shell executes user commands, supports batch file execution, I/O redirection, and
background processes. It provides commands like cd, ls, echo, and environ, with a prompt
displaying the current directory. Users can redirect input/output, run commands in the
background, and execute scripts from a file.

[Code for task 1](task1/main.c)

## Output:
![image](https://github.com/user-attachments/assets/88282256-4396-4af0-b790-8ee1745db7cc)
![image](https://github.com/user-attachments/assets/5f217480-ae83-4be6-9dde-f5ee60bff3d8)
![image](https://github.com/user-attachments/assets/3e7e6f46-4ebe-4e63-b023-fbd6409c3c8d)
![image](https://github.com/user-attachments/assets/8e95bcb3-f83f-40db-b41a-2bfcb3c73487)
![image](https://github.com/user-attachments/assets/175988f9-d613-4d16-a1e6-cab136dec404)
![image](https://github.com/user-attachments/assets/228a9d0c-f48a-48e0-801b-775dd060f4c8)
![image](https://github.com/user-attachments/assets/4779ba30-5a8a-4877-96b3-4dee2473c51d)
![image](https://github.com/user-attachments/assets/834c7867-1d4d-49db-bd13-2a4731ceebad)
![image](https://github.com/user-attachments/assets/067abba4-274d-4c51-82ce-384ca6ad87b4)

---
# Task 2
- This program assigns structs to each thread storing information like the frequency of words
in key pair values (which is an already defined struct) and the words it loops through and
using mutex I am able to prevent race conditions so no two threads can split up a word and
finally I merge the results of all the threads into one and print the result.
For some reason the pthread.h library doesn’t work on windows so had to do it again for
linux

[Code for task 2](task2/main.c)

## Output:
![image](https://github.com/user-attachments/assets/dc0d401b-3849-4ea1-836f-87a541d1276d)
![image](https://github.com/user-attachments/assets/f2bd0d42-c5df-433e-a3a1-3ef99f4a3cd8)

---
# Task 3
- My program reads from a file a sequence of page references and uses an Aging algorithm
on a fixed number of frames in this case 5 and then outsputs the number of page faults
which occurs when a program requests a page which isn’t found in the memory so it must
be loaded from the virtual memory in the OS case its usually a hard disk.
The algorithm ensures that the highest priority pages stay in the allotted fixed number of
frames, so if a page was not referenced for a while it will be replaced

[Code for task 3](task3/main.c)

## Ouput:
![image](https://github.com/user-attachments/assets/09d31ef4-ba4b-4d21-81e7-ffbce7b30660)


---
# Task 5
 - My program first takes the command line arguments (file path and bin size) and recursively goes down the file path, checking first if it’s a file which it then accesses its metadata and adds the size to an array or if it’s a directory it skips and recurses the files inside it instead, the array is then passed into a histogram which is the final output

[Code for task 5](task5/main.c)

## Output:
![image](https://github.com/user-attachments/assets/dbbf5af8-d1d9-4a11-ab0a-5d753d901bae)

---
# Task 6
- My program reads the inputted file path in the command line argument and opens the directory and using a while loop prints its content, each on a new line

[Code for task 6](task6/main.c)

## Output:
![image](https://github.com/user-attachments/assets/3c171d61-04b7-45c8-81cc-2bfc1c20d3ba)

# Task 7
- This program uses Work-Finish method to mark processes as either finished if the
requested resources are available or deadlocked if the resources aren’t available and is
dissimilar to the bankers algorithm in the fact that its merely detecting a deadlock not
preventing it

[Code for task 7](task7/main.c)

## Ouput:
![image](https://github.com/user-attachments/assets/137d5657-ddb5-43f5-a474-dfb79f6f98b3)


---
# Task 10
- My program generates 10 password and applies DES algorithm to each password with a unique salt and prints the results, it also demonstrates the password verification by encrypting a password with a random salt and verifies it again both the correct and incorrect inputs to confirm the encyprytions robustness

[Code for task 10](task10/main.c)

## Ouput:
![image](https://github.com/user-attachments/assets/238c8b7b-86d8-4c9c-bda8-6e08a34304e3)

)
