Jot
===

###Overview

Jot is a simple notepad type application that I made to test out the QT framework. 

I built it specifically for my MacBook to take quick notes in a dark UI and archive them based on date time.

While I made every attempt to use cross-platform techniques in the development of this application it is currently untested on Linux and Windows.


###Jot Usage

Jot saves all of your notes in plain text as follows: **~/JotNotes/{date-time-stamp}.note**
This allows command line users to process the note data as they see fit.
Jot can therefore simply remain a simple text input program and leave the heavy lifting to grep and sed.

#####New Note
**(Cmd+n)** - Clears all text in JotPad, ready for user to type whatever note they wish.

#####Display All
**(Cmd+d + Cmd+a)** - Show all notes saved with Jot in ascending date-time. If you click the timestamp, it will display the note.

#####Find Note
**(Cmd+f + Cmd+n)** - Case insensitive search in all notes for a keyword via a text input box.

#####Font Color
**(Cmd+f + Cmd+c)** - Set the primary font color for the application via a color picker.

#####Save Note
**(Cmd+s)** - Save the text in JotPad in the JotNotes directory with the current date and time as the file name.

###Notes

! Jot will remember window size and font color settings between sessions. !

! Jot does not currently allow you to edit previously saved notes via it's interface. This is by design as a way to encourage transparent edits of ideas and notes. If you wish to edit a note, that is an activity best acheived by using a standard text editor or the command line. This way Jot's purpose is strictly to add to the collection of notes and let the history of the notes speak for the growth and change of the idea. !

