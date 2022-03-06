// Reads text file (*.2di, *.3di) for IQpuzzler and fills vector array with data.
// Expected format is a list of parts (separated by ','),
// where each part is a list of orientations ( start of list with '[', end of list with ']'),
// where each orientation is a list of dots (logically same number of dots within same part, usually 3 to 5, depending on part size),
// where each dot is a list of coordinates (single digit, non-negative integers, x,y, optional z for 3D puzzle),
// defining the possible geometric orientations of each dot of the parts.
// Thus, the text in the file can also be used directly in Python or C++ (replace [] by {}) to initialize lists / vector arrays.
//
// No checks are performed to verify correctness of the data (except counting the number of parts).
// Wrong data or format will lead to undefined behaviour of the read functions or the solution algorithm.
// Characters other than begin of list '[', end of list ']', separator ',' or single digits will be ignored and can be used for comments.
//
// Input files can be created automatically with the provided MATLAB / Octave tool
// for different game configurations (e.g. original IQpuzzlerPRO game).
//
// Written by Stefan Abendroth (sab@ab-solut.com)
// Last update: 03/06/2022

#include "IQpuzzler_read_input.hpp"

uint8_t read_number(ifstream& partfile)
{
    char c = 0;
    while ( (c < '0' || c > '9') && !partfile.eof() )  // read in single digit number
        partfile >> c;
    return c - '0';     // convert to number by subtracting ASCII codes
}

vector <uint8_t> read_dot(ifstream& partfile)
{
    char c = 0;
    vector <uint8_t> dot;

    while (c!='[' && !partfile.eof())
        partfile >> c;      // read in open bracket

    while (c!=']' && !partfile.eof())
    {
        dot.push_back(read_number(partfile));   // get dot position
        do
            partfile >> c;      // read in next delimiter
        while (c!=',' && c!=']' && !partfile.eof());
    }
    return dot;
}

vector <vector <uint8_t> > read_orientation(ifstream& partfile)
{
    char c = 0;
    vector <vector <uint8_t> > orientation;

    while (c!='[' && !partfile.eof())
        partfile >> c;      // read in open bracket

    while (c!=']' && !partfile.eof())
    {
        orientation.push_back(read_dot(partfile));   // get dot position
        do
            partfile >> c;      // read in next delimiter
        while (c!=',' && c!=']' && !partfile.eof());
    }
    return orientation;
}

vector <vector <vector <uint8_t> > > read_part(ifstream& partfile)
{
    char c = 0;
    vector <vector <vector <uint8_t> > > p;

    while (c!='[' && !partfile.eof())
        partfile >> c;      // read in open bracket

    while (c!=']' && !partfile.eof())
    {
        p.push_back(read_orientation(partfile));   // get dot position
        do
            partfile >> c;      // read in next delimiter
        while (c!=',' && c!=']' && !partfile.eof());
    }
    return p;
}

uint8_t read_input(string inputfile, vector <vector <vector <vector <uint8_t> > > > & part)
{
    ifstream partfile;   // input: file with part data
    vector <vector <vector <uint8_t> > > p;

    partfile.open (inputfile);

    while (!partfile.eof())
    {
        p=read_part(partfile);
        if (p.size())
            part.push_back(p);
    }
    
    partfile.close();
    return part.size()-1;                // don't count empty part[0]
}