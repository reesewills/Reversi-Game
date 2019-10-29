Lab: 4
Author: Reese Wills
ID:448128
email: reesewills@wustl.edu

Design Options:
1) I chose to read in the board from a file as I already had the means to do so. The file is "Nine_Almonds.txt",
"Magic_Square.txt", or "Reversi.txt"

2) I chose to save -1 in the save file if the file should not used the saved game, and 1 as a marker to show
that the code should continue to read in from the saved file.

3) This saved file is called the same thing as the regular file with "_Game" added at the end.

4) I chose to use the game piece code that I wrote before, using different colors to represent the two
Reversi pieces.


Errors:
1) When creating the checkValid method, I was getting a vector out of bounds error becuase I was not
checking to see if the vector was in size range

2) I was not able to place a piece unless there was only one of the opposite color surrounded by the current color
at first, but I instead used a while loop as opposed to an if statement to fix this.

3) I was not linking header files well at first, then added too many links which resulted in a circle. Finally 
corrected this to be the right amount with everything working


Trials:
1) Tried many solutions, they worked.

2) Tried putting pieces in assorted invalid places, it did not work

3) Tried a method that lead to a stalemate, it worked

3) Tried lab4.exe in commang line, got usage message

4) Tried lab4.exe Reversiiii, got usage message

5) Tried solution to NineAlmonds, it worked

6) Tried the solution to MagicSquare, it worked

7) Tried putting two pieces on top of each other for Reversi, it worked

8) Tried to put a piece out of the range of the DX and DY, it wouldn't let me


Constructors:

1) Copy Constructor - Used when passing things by value mainly. This creates a copy so that it can use what is copied
in the method without manipulating the value of the object because it was not passed by reference. This was allowed
in the lab because things needed to be passed by value so as to not manipulate them.


2) Move Constructor - Does not allocate new resources when used so it is more efficient than the copy constructor.
I did not allow this to be used because it was not necessary. For this to happen, I simply did not add in a move
constructor to be used.


3) Copy-assignment Constructor - This was not necessary to be used either, so I did not allow it by not adding
in the constructor.


4) Move-assignment Constructor - This was not necessary for the lab either, and only could add potential harmful
things to the code by a toxic coder. For this reason, I did not allow it and did not add the constructor in.


5) Destructor - Did not have to use manual destructors because I used the shared_ptr. This automatically uses
the destructor when it can. This was allowed as well, but not manually used. This is because I do not think it would be
harmful to let someone use.