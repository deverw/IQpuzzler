// Finds all solutions for IQ Puzzler 5x11 board by brute force algorithm (backtracking).
// Part definitions and orientations are read from text file "orig.2di"
// or from filename.3di, where filename is defined in command line argument.
// Creates Logfile "orig.2do"  or filename.2do with solutions and timestamps (parts are represented as A,B,C,...).
// Acceleration by identification of small isolated fields (1x1,2x1,3x1,2x2).
// Prints solutions to terminal window and visualizes current search.
//
// Written by Stefan Abendroth (sab@ab-solut.com)
// Last update: 03/07/2022

#include "IQpuzzler_read_input.hpp"     // read part shapes and orientations from input file

// Part shapes incl. all possible orientations (up to 8)
vector <vector <vector <vector <uint8_t> > > > part = {{{{}}}};

uint8_t partcount;

// board with margin, 7 rows, 13 columns
vector<vector<uint8_t>> board =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// ANSI escape color codes (4-bit):
vector <string> color =
{        "\033[1;0m",   // black
         "\033[1;46m",  // mid blue
         "\033[1;101m", // red
         "\033[1;43m",  // orange
         "\033[1;42m",  // dark green
         "\033[1;45m",  // magenta
         "\033[1;102m", // light green
         "\033[1;103m", // yellow
         "\033[1;105m", // pink
         "\033[1;106m", // cyan
         "\033[1;100m", // gray (no dark red in 4-bit color scheme)
         "\033[1;44m",  // dark blue
         "\033[1;104m"  // light blue
};

// reset solution counter
int solutions = 0;

// terminal size
struct winsize w;

// files
ifstream partfile;   // input: file with part data
ofstream logfile;   // output: log file with solutions and time stamps

// timer
chrono::steady_clock::time_point start;
chrono::steady_clock::time_point elapsed;

// recursive function to perform backtracking algorithm
bool find_position(uint8_t part_number)
{
    bool fit;
    uint8_t dotx,doty;
    for (uint8_t orientation=0; orientation<part[part_number].size(); orientation++)
    {
        for (uint8_t row=1; row<6; row++)
        {
            for (uint8_t col=1; col<12; col++)
            {
                fit=true;
                // check if part at layer,row,col with orientation fits in board
                for (uint8_t dot=0; dot<part[part_number][orientation].size(); dot++)
                {
                    dotx=part[part_number][orientation][dot][0];
                    doty=part[part_number][orientation][dot][1];
                    if ((row+doty>5) || (col+dotx>11))           // dot out of board?
                    {
                        fit=false;
                    }
                    else if (board[row+doty][col+dotx] != 0)     // field occupied?
                    {
                        fit=false;
                    }
                }
                // check if part creates isolated fields (this accelerates the search significantly)
                if (fit)
                {
                    // put part on board
                    for (uint8_t dot=0; dot<part[part_number][orientation].size(); dot++)
                    {
                        dotx=part[part_number][orientation][dot][0];
                        doty=part[part_number][orientation][dot][1];
                        board[row+doty][col+dotx]=part_number;
                    }
                    // check 1x1 holes
                    for (uint8_t y=1; y<6; y++)
                        for (uint8_t x=1; x<12; x++)
                            if (board[y][x]==0 && board[y-1][x] && board[y+1][x] && board[y][x-1] && board[y][x+1])
                                fit=false;
                    // check 1x2 holes
                    for (uint8_t y=1; y<6; y++)
                        for (uint8_t x=1; x<11; x++)
                            if (board[y][x]==0 && board[y][x+1]==0 &&
                                board[y-1][x] && board[y+1][x] && board[y-1][x+1] && board[y+1][x+1] &&
                                board[y][x-1] && board[y][x+2])
                                fit=false;
                    // check 2x1 holes
                    for (uint8_t y=1; y<5; y++)
                        for (uint8_t x=1; x<12; x++)
                            if (board[y][x]==0 && board[y+1][x]==0 &&
                                board[y][x-1] && board[y][x+1] && board[y+1][x-1] && board[y+1][x+1] &&
                                board[y-1][x] && board[y+2][x])
                                fit=false;
                    // check 2x2 holes (light blue part would fit in here, but would create a 1x1 hole)
                    for (uint8_t y=1; y<5; y++)
                        for (uint8_t x=1; x<11; x++)
                            if (board[y][x]==0 && board[y+1][x]==0 && board[y][x+1]==0 && board[y+1][x+1]==0 &&
                                board[y-1][x] && board[y-1][x+1] &&
                                board[y+2][x] && board[y+2][x+1] &&
                                board[y][x-1] && board[y+1][x-1] &&
                                board[y][x+2] && board[y+1][x+2])
                                fit=false;
                    // check 1x3 holes
                    for (uint8_t y=1; y<6; y++)
                        for (uint8_t x=1; x<10; x++)
                            if (board[y][x]==0 && board[y][x+1]==0 && board[y][x+2]==0 &&
                                board[y-1][x] && board[y-1][x+1] && board[y-1][x+2] &&
                                board[y+1][x] && board[y+1][x+1] && board[y+1][x+2] &&
                                board[y][x-1] && board[y][x+3])
                                fit=false;
                    // check 3x1 holes
                    for (uint8_t y=1; y<4; y++)
                        for (uint8_t x=1; x<12; x++)
                            if (board[y][x]==0 && board[y+1][x]==0 && board[y+2][x]==0 &&
                                board[y][x-1] && board[y+1][x-1] && board[y+2][x-1] &&
                                board[y][x+1] && board[y+1][x+1] && board[y+2][x+1] &&
                                board[y-1][x] && board[y+3][x])
                                fit=false;
                    if (!fit)
                    {
                        // remove part if isolated fields were found
                        for (uint8_t dot=0; dot<part[part_number][orientation].size(); dot++)
                        {
                            dotx=part[part_number][orientation][dot][0];
                            doty=part[part_number][orientation][dot][1];
                            board[row+doty][col+dotx]=0;
                        }
                    }
                }
                if (fit)    // all checks passed
                {                    
                    // show board in terminal
                    for (uint8_t y=1; y<6; y++)
                    {
                        for (uint8_t x=1; x<12; x++)
                        {
                            // use ANSI escape codes to access terminal
                            cout<<color[board[y][x]];    // set color
                            // print square (two blanks with background color) at y-x-location in terminal
                            cout<<"\33[" << y+w.ws_row-6 << ";" << 2*x-1 << "H  ";
                        }
                    }
                    cout<<flush;
                    
                    if (part_number==partcount)    // all parts on board? -> Heureka!
                    {
                        elapsed=chrono::steady_clock::now();    // measure time
                        solutions++;
                        // print solution in terminal
                        cout<<"\033[0;0m";         // reset colors
                        cout<<endl<<"Solution "<<solutions<<endl;
                        for (uint8_t y=0; y<5; y++)
                            cout<<endl;    // print empty lines to scroll up solution
                        // write solution and time stamp to log file
                        logfile<<"Solution "<<to_string(solutions)<<" ("<<chrono::duration_cast<chrono::seconds>(elapsed - start).count()<<"s)"<<endl;
                        for (uint8_t y=1; y<6; y++)
                        {
                            for (uint8_t x=1; x<12; x++)
                                logfile<<(char)(board[y][x]+64);     // convert 1 to A, 2 to B, ... , 12 to L
                            logfile<<endl;
                        }
                        logfile<<endl;
                    }                          
                    else                   // try to find position for next part -> this creates many recursions
                    {
                        if (!find_position(part_number+1))
                        {
                            fit=false;
                        } 
                    }
                    // remove part from board
                    for (uint8_t dot=0; dot<part[part_number][orientation].size(); dot++)
                    {
                        dotx=part[part_number][orientation][dot][0];
                        doty=part[part_number][orientation][dot][1];
                        board[row+doty][col+dotx]=0;
                    }
                }   // end if fit
            }   // next col
        }   // next row
    }   // next orientation
    return fit;
}

int main(int argc, char *argv[])
{
    string inputfile, outputfile;

    if (argc>1)
    {
        inputfile=argv[1];
        inputfile+=".2di";
        outputfile=argv[1];
        outputfile+="_rect.2do";
    }
    else
    {
        inputfile="orig.2di";
        outputfile="orig_rect.2do";
    }

    // read parts from input file
    partcount=read_input(inputfile, part);
    cout<<to_string(partcount)<<" Parts loaded."<<endl;
    
    // get columns and lines of terminal
    ioctl(0, TIOCGWINSZ, &w);

    // scroll up screen by printing empty lines
    for (uint8_t i=0; i<6; i++)
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