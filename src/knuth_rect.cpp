// Finds all solutions for IQ Puzzler 5x11 board by solving the exact coverage problem with Knuth's algorithm X.
// Part definitions and orientations are read from text file "orig.2di"
// or from filename.3di, where filename is defined in command line argument.
//
// Written by Stefan Abendroth (sab@ab-solut.com)
// Last update: 01/21/2023

#include "../include/IQpuzzler_read_input.hpp"     // read part shapes and orientations from input file

#define BOARD_ROWS 5
#define BOARD_COLUMNS 11

// Part shapes incl. all possible orientations
vector <vector <vector <vector <uint8_t> > > > part = {{{{}}}};
uint8_t partcount;

vector <vector <uint8_t> > X = {{}};

using namespace std;

void prepareX()
{
    // Prepare matrix for algorithm X.
    // Each column represents a single part with a possible orientation and position on the board.
    // Algorithm X will identify those sets of columns that can be combined to a completely filled column (=solution of exact coverage problem).

    vector <uint8_t> Xcol(partcount+BOARD_ROWS*BOARD_COLUMNS);
    uint8_t pos;
    bool fit;

    for (uint8_t part_number=1; part_number<=partcount; part_number++)
    {
        fill(Xcol.begin(),Xcol.end(),0);
        Xcol[part_number-1]=1;
        for (uint8_t orientation=0; orientation<part[part_number].size(); orientation++)
            for (uint8_t row=0; row<BOARD_ROWS; row++)
                for (uint8_t col=0; col<BOARD_COLUMNS; col++)
                {
                    fit=true;
                    fill(Xcol.begin()+partcount,Xcol.end(),0);
                    for (uint8_t dot=0; dot<part[part_number][orientation].size(); dot++)
                    {
                        pos = row+part[part_number][orientation][dot][1] + (col+part[part_number][orientation][dot][0])*BOARD_ROWS;
                        if (pos < BOARD_ROWS*BOARD_COLUMNS)
                            Xcol[partcount+pos] = part_number;
                        else
                            fit=false;
                    }
                    if (fit)
                        X.push_back(Xcol);
                }
    }
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

    partcount=read_input(inputfile, part);
    prepareX();
}