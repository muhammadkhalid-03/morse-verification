# What is this program?
This program is designed for translating morse code where the spaces between the letters have been lost.
There are two different ways to input morse code. A file can be input with a morse sentance or the user can input morse code while the program is running.

The program works with morse code sentances with less than 128 words, with each morse word being less than 23 characters in length.
Morse inputs, whether in files or user input, should be in the form "......-...-..---/.-----.-..-..-.." or ".... .. / .... --- .-- / .- .-. . / -.-- --- ..-" So, words should be seperated with '/', dots are represented with '.' and dashes with '-'. 



# How to run this program
To run the program:
make
LD_LIBRARY_PATH=/home/curtsinger/.local/lib ./morse simple
               or
LD_LIBRARY_PATH=/home/curtsinger/.local/lib ./morse text
               or
LD_LIBRARY_PATH=/home/curtsinger/.local/lib ./morse file inputs/input1.txt
               or
LD_LIBRARY_PATH=/home/curtsinger/.local/lib ./morse file *FILENAME*



Choosing simple will prompt you to enter morse input to the program. It will then decode what you wrote, and replace any invalid morse code letters with '?'.
Choosing text will prompt you to enter morse input to the program. It will then decode every possible splitting, assuming that you enter the spaces incorrectly.
Choosing file with a valid filename will read in the file as its input. It will then decode every possible splitting, assuming that the spaces were entered incorrectly.
Choosing file with input/input1.txt, input/input2.txt, input/input3.txt, or input/input4.txt will give you some examples of our program that we have created for you. 

If you want test inputs, the website:
https://morsecode.world/international/translator.html
is good to come up with properly spaced inputs that our program can take. Simple will decode them to what you entered, where text will decode every possible splitting of the input regardless of whether or not you delete the spaces that the website gives you.

# Notes about runtime and characters
Words of lengths over 23 characters are able to be decoded 'easily'. 23 characters will take around 10 seconds to decode on the computers in our classroom, every added character will double the length of time required. Special characters that can be written in Morse code, ?, #, etc. do not appear in the dictionary, so will not translate. This almost always allows for better outputs to the user.

# Credit
The authors were Muhammad Khalid and Maya Flynn.
Help was obtained from the mentors of 213.
This is a final project for CSC213-01 in Fall 2024


