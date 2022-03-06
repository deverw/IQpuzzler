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

#ifndef _IQPUZZLER_READ_INPUT_
#define _IQPUZZLER_READ_INPUT_

#include <cstdint>      // uint8_t
#include <vector>       // vector
#include <string>       // string
#include <iostream>     // cout, endl
#include <fstream>      // fopen, fclose
#include <chrono>       // create timestamps
#include <sys/ioctl.h>  // get size of terminal window

#define BEGIN_OF_LIST '['
#define END_OF_LIST ']'
#define LIST_SEPARATOR ','

using namespace std;

uint8_t read_number(ifstream& partfile);
vector <uint8_t> read_dot(ifstream& partfile);
vector <vector <uint8_t> > read_orientation(ifstream& partfile);
vector <vector <vector <uint8_t> > > read_part(ifstream& partfile);
uint8_t read_input(string inputfile, vector <vector <vector <vector <uint8_t> > > > & part);

#endif