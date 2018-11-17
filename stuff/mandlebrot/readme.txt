Controls
--------
w - zoom in
s - zoom out
a - precision zoom in
d - precision zoom out
arrow keys - move
shift + arrow keys - precision move


If you want to try compiling this yourself, add the included SFML folder to your IDE's list of include directories, and then just add all this shit to the linker: -lsfml-graphics -lsfml-main -lsfml-window -lsfml-system

Ive included the sfml help file to assist in any attempts to edit or extend this program

As a bonus, ive included my integer-to-string conversion program, it allocates a char array in the heap so be sure to to free up all string references when you are done before your computer screams at you for a segfault

This program is not even close to done, im just releasing the source code so people can laugh at my undocumented spaghetti code




