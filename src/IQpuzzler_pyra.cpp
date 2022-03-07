// Finds all solutions for IQ Puzzler 3D pyramid by brute force algorithm (backtracking).
// Part definitions and orientations are read from text file "orig.3di"
// or from filename.3di, where filename is defined in command line argument.
// Creates Logfile "orig.3do"  or filename.3do with solutions and timestamps (parts are represented as A,B,C,...).
// Visualization of current search and solutions are printed to terminal window.
// The search is accelerated (factor ~10) by identification of isolated cavities before going into new recursions.
//
// Written by Stefan Abendroth (sab@ab-solut.com)
// Last update: 03/07/2022

#include "IQpuzzler_read_input.hpp"     // read part shapes and orientations from input file

// Part shapes incl. all possible orientations
// part{number}{orientation}{dot}{x,y,z}
// format is {x,y,z}-coordinates for each element, one line for each possible orientation, one block per part
// Horizontal orientations use double steps, vertical layers are shifted by 1 step
vector <vector <vector <vector <uint8_t> > > > part = {{{{}}}}; // part[0] will not be used (0 is used for empty cells in board)

uint8_t partcount;

// ANSI escape color codes
vector <string> color =
{   "\033[1;0m",   // black
    "\033[1;101m", // red
    "\033[1;43m",  // orange
    "\033[1;42m",  // dark green
    "\033[1;102m", // light green
    "\033[1;103m", // yellow
    "\033[1;105m", // pink
    "\033[1;46m",  // mid blue
    "\033[1;45m",  // magenta
    "\033[1;44m",  // dark blue
    "\033[1;106m", // cyan
    "\033[1;100m", // gray (no dark red in 4bit color scheme)
    "\033[1;104m"  // light blue
};

// 3D board (pyramid with 5 layers) with margin and intermediate spaces in x-y-direction, pre-filled with 1
// 7 layers, 13 rows, 13 columns (margin is needed for cavity checks and avoiding index range violations)
// actual pyramid is in layers 1-5, rows/columns 2,4,6,8,10 (in layer 1), rows/columns 3,5,7,9 (in layer 2), ... , row=column=6 in layer 5
vector<vector<vector<uint8_t>>> board (7,vector<vector<uint8_t>>(13,vector<uint8_t>(13,1)));

// search vectors for neighbouring spaces {z,y,x}
// opposite directions: 2 steps within the same plane
// plane [0,1,2] is defined by bits 2 and 3 of index
vector<vector<int8_t>> search = {
    { 1, 1, 1},{-1, 1, 1},{ -1,-1,-1},{ 1,-1,-1},   // 4 directions in plane parallel to x=y
    { 0, 2, 0},{ 0, 0,-2},{ 0,-2, 0},{ 0, 0, 2},    // 4 directions in plane parallel to z=0
    { 1,-1, 1},{-1,-1, 1},{-1, 1,-1},{ 1, 1,-1}     // 4 directions in plane parallel to x=-y
};

int solutions = 0;  // solution counter
struct winsize w;   // terminal size

// files
ifstream partfile;   // input: file with part data
ofstream logfile;   // output: log file with solutions and time stamps

// timer
chrono::steady_clock::time_point start;
chrono::steady_clock::time_point elapsed;

// recursive function to perform backtracking algorithm
bool find_position(uint8_t part_number)
{
    bool fit, cavity;
    int8_t dx, dy, dz;
    uint8_t dotx, doty, dotz;
    uint8_t n, i, si, ii;

    for (uint8_t orientation=0; orientation<part[part_number].size(); orientation++)
    {
        for (uint8_t layer=1; layer<6; layer++)
        {
            for (uint8_t row=layer+1; row<12-layer; row+=2)
            {
                for (uint8_t col=layer+1; col<12-layer; col+=2)
                {
                    fit=true;
                    // check if part at layer,row,col with orientation fits in board
                    for (uint8_t dot=0; dot<part[part_number][orientation].size(); dot++)
                    {
                        dotx=part[part_number][orientation][dot][0];
                        doty=part[part_number][orientation][dot][1];
                        dotz=part[part_number][orientation][dot][2];
                        if ((layer+dotz > 5) || (row+doty > 11-layer) || (col+dotx > 11-layer))
                        {    // dot out of board?
                            fit=false;
                        }
                        else if (board[layer+dotz][row+doty][col+dotx] != 0)     // field occupied?
                        {
                            fit=false;
                        }
                    }

                    // check for cavities (single or double spaces)
                    if (fit)
                    {
                        // put part on board (free space has already been checked)
                        for (uint8_t dot=0; dot<part[part_number][orientation].size(); dot++)
                        {
                            dotx=part[part_number][orientation][dot][0];
                            doty=part[part_number][orientation][dot][1];
                            dotz=part[part_number][orientation][dot][2];
                            board[layer+dotz][row+doty][col+dotx]=part_number;
                        }

                        for (uint8_t z=1; z<6; z++)
                        {
                            for (uint8_t y=z+1; y<(12-z); y+=2)
                            {
                                for (uint8_t x=z+1; x<(12-z); x+=2)
                                {
                                    if (board[z][y][x]==0)
                                    {   // free space detected
                                        cavity=true;
                                        for (uint8_t plane=0; plane<3; plane++) // check neighbour spaces in all three planes
                                        {
                                            n=0;
                                            for (uint8_t dir=0; dir<4; dir++)       // check every direction within plane
                                            {
                                                // create index for search vector from plane (bits 3,2) and direction (bits 1,0)
                                                i = (plane<<2) | dir;
                                                if (!board[z+search[i][0]] [y+search[i][1]] [x+search[i][2]])
                                                {
                                                    n++;                    // count neighbours
                                                    si=i;                   // store index (in case we need to check further neighbours)
                                                }
                                            }
                                            if (n==1)   // only one free neighbour?
                                                for (uint8_t d=3; d<6; d++)    // check further neighbour spaces in all directions on same plane, except backwards
                                                {   // instead of -1,0,1 use 3,4,5 to stay unsigned
                                                    ii =  (si&12) | ((si+d)&3);    // this yields search index ii which is on the same plane as si
                                                    if (!board[z+search[si][0]+search[ii][0]] [y+search[si][1]+search[ii][1]] [x+search[si][2]+search[ii][2]])
                                                        cavity=false;    // further neighbour space on same plane allows more parts to be placed
                                                }
                                            if (n>1)    // several free neighbours?
                                                cavity=false;
                                        }
                                        if (cavity)
                                            fit=false;
                                    }
                                    if (!fit)
                                        break;      // no need to search further
                                }   // next x
                                if (!fit)
                                    break;
                            }   // next y
                            if (!fit)
                                break;
                        }   // next z

                        if (!fit)
                        {
                            for (uint8_t dot=0; dot<part[part_number][orientation].size(); dot++)
                            {
                                dotx=part[part_number][orientation][dot][0];
                                doty=part[part_number][orientation][dot][1];
                                dotz=part[part_number][orientation][dot][2];
                                board[layer+dotz][row+doty][col+dotx]=0;
                            }
                        }
                    }

                    // all checks passed
                    if (fit)    // part already on board
                    {
                        // show board in terminal
                        for (uint8_t z=1; z<6; z++)
                            for (uint8_t y=z+1; y<(12-z); y+=2)
                                for (uint8_t x=z+1; x<(12-z); x+=2)
                                {
                                    // use ANSI escape codes to access terminal
                                    cout<<color[board[z][y][x]];    // set color
                                    // print square (two blanks with background color) at y-x-location in terminal
                                    cout<<"\033[" << y/2+w.ws_row-5 << ";" << 12*z+x-13 << "H  ";
                                }
                        // try to find position for next part -> this creates many recursions
                        if (part_number<12)
                        {
                            if (!find_position(part_number+1))
                            {
                                fit=false;
                            }    
                        }                          
                        else // all parts on board? -> Heureka!
                        {
                            elapsed=chrono::steady_clock::now();    // measure time
                            solutions++;                            // count solutions
                            // print solution in terminal
                            cout<<"\033[0;0m";         // reset colors
                            cout<<endl<<endl<<endl<<"Solution "<<solutions<<endl;
                            for (uint8_t y=0; y<5; y++)
                                cout<<endl;    // print empty lines to scroll up solution
                            // write solution and time stamp to log file
                            logfile<<"Solution "<<to_string(solutions)<<" ("<<chrono::duration_cast<chrono::seconds>(elapsed - start).count()<<"s)"<<endl;
                            for (uint8_t z=1; z<6; z++)
                                for (uint8_t y=z+1; y<(12-z); y+=2)
                                {
                                    for (uint8_t x=z+1; x<(12-z); x+=2)
                                        logfile<<(char)(board[z][y][x]+64);     // convert 1 to A, 2 to B, ... , 12 to L
                                    logfile<<endl;
                                }
                            logfile<<endl;
                        }
                        // remove part from board board
                        for (uint8_t dot=0; dot<part[part_number][orientation].size(); dot++)
                        {
                            dotx=part[part_number][orientation][dot][0];
                            doty=part[part_number][orientation][dot][1];
                            dotz=part[part_number][orientation][dot][2];
                            board[layer+dotz][row+doty][col+dotx]=0;
                        }
                    }   // if fit
                }   // next col
            }   // next row
        }   // next layer
    }   // next orientation
    return fit;
}

int main(int argc, char *argv[])
{
    string inputfile, outputfile;

    if (argc>1)
    {
        inputfile=argv[1];
        inputfile+=".3di";
        outputfile=argv[1];
        outputfile+="_pyra.3do";
    }
    else
    {
        inputfile="orig.3di";
        outputfile="orig_pyra.3do";
    }

    // read parts from input file
    partcount=read_input(inputfile, part);
    cout<<to_string(partcount)<<" Parts loaded."<<endl;

    // Clear pyramid with 5 layers (5x5,4x4,3x3,2x2,1x1 elements) inside board cube.
    // All fields outside pyramide and outside grid remain occupied (=1).
    for (uint8_t z=1; z<6; z++)
        for (uint8_t y=z+1; y<(12-z); y+=2)
            for (uint8_t x=z+1; x<(12-z); x+=2)
                board[z][y][x]=0;
    
    // get columns and lines of terminal
    ioctl(0, TIOCGWINSZ, &w);
    // scroll up screen by printing empty lines
    for (uint8_t i=0; i<5; i++)
        cout<<endl;

    // open log file
    logfile.open(outputfile);

    // start timer
    start = chrono::steady_clock::now();

    // start backtracking                
    find_position(1);

    // close log file
    logfile.close();

    // scroll up solutions by printing empty lines
    for (uint8_t i=0; i<5; i++)
        cout<<endl;    // print empty lines to scroll up solution

    return solutions;
}