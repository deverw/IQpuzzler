# IQpuzzler
Finds all solutions for IQ Puzzler game (https://www.smartgames.eu/uk/one-player-games/iq-puzzler-pro) by brute force algorithm. There are three versions of the game:
<ul>
  <li>IQpuzzler_rect solves the 2D 11x5 rectangular board.</li>
  <li>IQpuzzler_diag solves the 2D diagonal arrangement.</li>
  <li>IQpuzzler_pyra solves the 3D pyramid.</li>
</ul>
Part definitions and orientations are read from a text file *.2di (for the 2D games) or *.3di (for the 3D game).

The input filename (without extension) is provided in the command line argument. If no argument is provided, "orig" is used as default input file, using the 12 parts from the original game. Besides "orig", some other configurations with 11 parts ("cross", "long") are provided.

Expected input format is a list of parts (separated by ','), where each part is a list of orientations ( start of list with '[', end of list with ']'), where each orientation is a list of dots (logically same number of dots within same part, usually 3 to 5, depending on part size), where each dot is a list of coordinates (single digit, non-negative integers, x,y, optional z for 3D puzzle), defining the possible geometric orientations of each dot of the parts.
Thus, the text in the file can also be used directly in Python or C++ (replace [] by {}) to initialize lists / vector arrays.

No checks are performed to verify correctness of the data (except counting the number of parts).
Wrong data or format will lead to undefined behaviour of the read functions or the solution algorithm.
Characters other than begin of list '[', end of list ']', separator ',' or single digits will be ignored and can be used for comments.
Input files can be created automatically with the provided MATLAB / Octave tool for different game configurations (e.g. original IQpuzzlerPRO game).

Symmetric or rotated solutions can be avoided by reducing the orientations of a particular part.

As output, the program creates a text file *.2do (for the 2D games) or *.3do (for the 3D game)  with solutions and timestamps (parts are represented as A,B,C,...).
A visualization of the current search and identified solutions is printed to the terminal window.
The search algorithm is accelerated by identification of isolated cavities before going into new recursions.

For the original game, 1.082.785 solutions have been found for the 2D rectangular version, and 4.360 solutions for the 3D pyramid.
So far, no solutions have been found for the 3D pyramid using configurations with 11 parts (only "cross" and "long" have been tested).
