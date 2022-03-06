# IQpuzzler
Finds all solutions for IQ Puzzler game by brute force algorithm.
There are three versions of the game:
IQpuzzler_rect solves the 2D 11x5 rectangular board.
IQpuzzler_diag solves the 2D diagonal arrangement.
IQpuzzler_pyra solves the 3D pyramid.
Part definitions and orientations are read from text file "orig.3di" or from filename.3di, where filename is defined in command line argument.
Besides the original 12 parts defined in "orig.3di", there are other configurations with 11 parts ("cross", "long") available.
Creates Logfile "original.3do"  or filename.3do with solutions and timestamps (parts are represented as A,B,C,...).
Visualization of current search and solutions are printed to terminal window.
The search is accelerated (factor ~10) by identification of isolated cavities before going into new recursions.

Expected input format is a list of parts (separated by ','), where each part is a list of orientations ( start of list with '[', end of list with ']'), where each orientation is a list of dots (logically same number of dots within same part, usually 3 to 5, depending on part size), where each dot is a list of coordinates (single digit, non-negative integers, x,y, optional z for 3D puzzle), defining the possible geometric orientations of each dot of the parts.
Thus, the text in the file can also be used directly in Python or C++ (replace [] by {}) to initialize lists / vector arrays.

No checks are performed to verify correctness of the data (except counting the number of parts).
Wrong data or format will lead to undefined behaviour of the read functions or the solution algorithm.
Characters other than begin of list '[', end of list ']', separator ',' or single digits will be ignored and can be used for comments.
Input files can be created automatically with the provided MATLAB / Octave tool for different game configurations (e.g. original IQpuzzlerPRO game).
