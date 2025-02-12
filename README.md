# What is this program?

This program is designed for translating morse code where the spaces between the letters have been lost.
There are two different ways to input morse code. A file can be input with a morse sentance or the user can input morse code while the program is running.

The program works with morse code sentances with less than 128 words, with each morse word being less than 23 characters in length.
Morse inputs, whether in files or user input, should be in the form "......-...-..---/.-----.-..-..-.." or ".... .. / .... --- .-- / .- .-. . / -.-- --- ..-" So, words should be seperated with '/', dots are represented with '.' and dashes with '-'.

# How it works

1. The program takes morse code (with words separated by '/') or a file name with the same input format
2. Words in the sentence are separated with a delimiter and each individual word is passed onto a thread
3. Each thread splits its assigned word into all possible morse combinations
4. All the splittings are decoded into English words
5. We wrote our own hash table to search through a collection of english words and all the matches are returned to the user in a readable format

# How to run this program

To run the program use the Makefile:

One thing to note is that the Makefile has some configurations that give us access to MD5 hashing through MathLAN which is our CS department's network of computers. As a result, unless you're in the Grinnell College science building using the CS department's computers, you won't be able to run the program

```
make
```

## Terminal Commands

These commands are meant to run for the computers in our classroom which have access to the CS faculty folders. We needed this access because we utilize MD5 hashing for our hash table.

```
LD_LIBRARY_PATH=/home/curtsinger/.local/lib ./morse simple
```

or

```
LD_LIBRARY_PATH=/home/curtsinger/.local/lib ./morse text
```

or

```
LD_LIBRARY_PATH=/home/curtsinger/.local/lib ./morse file inputs/input1.txt
```

or

```
LD_LIBRARY_PATH=/home/curtsinger/.local/lib ./morse file *FILENAME*
```

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
