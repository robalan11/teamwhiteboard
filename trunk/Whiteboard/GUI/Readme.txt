This is the main directory for the Whiteboard program.

admin.cpp/h:
    -This is the window that holds the list of connected people.

canvas.cpp/h:
    -This is the part of the whiteboard window that holds drawn stuff

main.cpp/h:
    -This runs first and launches the text window, which launches everything else

prompt.cpp/h:
    -This is the window that asks you for your name and an IP

Readme.txt:
    -This document

statusWindow.cpp/h:
    -This window shows status text

textWindow.cpp/h:
    -This is the chat window, which also handles all the launching of other windows,
     networking, and quitting.

whiteboard.cpp/h:
    -This is the whiteboard window, which contains tool buttons and a canvas to draw on.

This program requires only a properly linked installation of wxWidgets to compile.