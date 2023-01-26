// Finds all solutions for IQ Puzzler 5x11 board by solving the exact coverage problem with Knuth's algorithm X.
// Part definitions and orientations are read from text file "orig.2di"
// or from filename.2di, where filename is defined in command line argument.
//
// Written by Stefan Abendroth (sab@ab-solut.com)
// Last update: 01/26/2023

#include "../include/IQpuzzler_read_input.hpp"     // read part shapes and orientations from input file

constexpr auto BOARD_ROWS = 5;
constexpr auto BOARD_COLUMNS = 11;

vector <vector <vector <vector <uint8_t> > > > part;
uint8_t partcount;
vector <vector <uint8_t> > X;
vector <size_t> solution;
vector <vector <size_t> > all_solutions;

void show_solution()
{
    vector <uint8_t> board(X[0].size(),0);
    for (size_t i = 0; i < solution.size(); i++)
        for (size_t j=0; j < board.size(); j++)
            board[j] += X[solution[i]][j];
    cout << "Solution " << to_string(all_solutions.size()) << ":";
    for (size_t j = 0; j < partcount; j++)
        cout << " " << to_string(board[j]);
    cout << endl;
    for (size_t i = 0; i < BOARD_ROWS; i++)
    {
        for (size_t j = 0; j < BOARD_COLUMNS; j++)
            cout << to_string(board[partcount+j*BOARD_ROWS+i]) << " ";
        cout << endl;
    }
    cout << endl;
}

void prepareX(vector <vector <vector <vector <uint8_t> > > > part)
// Prepare matrix for Knuth's algorithm X.
// Each row of X represents a single part with a possible orientation and position on the board.
// Algorithm X will identify those sets of rows that can be combined to a completely filled column (=solution of exact coverage problem).
{
    size_t xpos, ypos;
    vector <uint8_t> Xrow(partcount + BOARD_ROWS * BOARD_COLUMNS, 0);
    bool fit;

    for (size_t part_number = 0; part_number < partcount; part_number++)
    {
        fill(Xrow.begin(), Xrow.end(), 0);
        Xrow[part_number] = 1;
        for (size_t orientation = 0; orientation < part[part_number].size(); orientation++)
            for (size_t row = 0; row < BOARD_ROWS; row++)
                for (size_t col = 0; col < BOARD_COLUMNS; col++)
                {
                    fit = true;
                    for (size_t dot = 0; dot < part[part_number][orientation].size(); dot++)
                    {
                        xpos = col + part[part_number][orientation][dot][0];
                        ypos = row + part[part_number][orientation][dot][1];
                        if (xpos < BOARD_COLUMNS && ypos < BOARD_ROWS)
                            Xrow[partcount + xpos*BOARD_ROWS + ypos] = part_number+1;
                        else
                            fit = false;
                    }
                    if (fit)
                        X.push_back(Xrow);
                    fill(Xrow.begin() + partcount, Xrow.end(), 0);
                }
    }
}

void knuth(vector < vector <uint8_t> > A , vector <size_t> p)
// Implementation of Knuth's algorithm X to solve exact coverage problem, as published in
// https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/0011047.pdf
// Arguments:
// Matrix 'A' stores remaining coverage matrix. Each row represents a way to place a part on the board.
// Vector 'p' stores row numbers of original coverage matrix X.
{
    if (A.size())
    {
        // choose column with minimum number of entries
        // (this will minimize the number of recursions)
        vector <size_t> entries(A[0].size(), 0);
        uint32_t min_entries = UINT32_MAX;
        size_t min_column = 0;
        for (size_t col = 0; col < A[0].size(); col++)
        {
            for (size_t row = 0; row < A.size(); row++)
                entries[col] += A[row][col] > 0;
            if (entries[col] < min_entries)
            {
                min_entries = entries[col];
                min_column = col;
            }
        }
        if (min_entries)
            // for each row such that A[row][min_column]>0
            for (size_t row=0; row<A.size(); row++)
                if (A[row][min_column])
                {
                    // include row in partial solution
                    solution.push_back(p[row]);
                    // show_solution();
                    vector < vector <uint8_t> > Ar = A;
                    vector <size_t> pr = p, del_rows(A.size(), 0);
                    // for each j such that A[row][j]>0
                    for (size_t j = A[row].size(); j > 0; j--)  // backwards to avoid indexing errors
                        if (A[row][j-1])
                        {
                            // delete column j from A
                            for (size_t r = Ar.size(); r > 0 ; r--)
                                Ar[r-1].erase(Ar[r-1].begin() + j-1);
                            // for each i such that A[i][j]>0
                            for (size_t i = A.size(); i > 0; i--)  // backwards to avoid indexing errors
                                if (A[i-1][j-1])
                                    // mark row for deletion
                                    del_rows[i - 1] = 1;
                        }
                    // delete rows from A (and p)
                    for (size_t r = Ar.size(); r > 0 ; r--)
                        if (del_rows[r-1])
                        {
                            Ar.erase(Ar.begin() + r-1);
                            pr.erase(pr.begin() + r-1);
                        }
                    // repeat algorithm recursively on the reduced matrix A (and p)
                    knuth(Ar, pr);
                    solution.pop_back();
                }      
    }
    // if A is empty, a solution is found
    else if (solution.size()==partcount)
    {
        all_solutions.push_back(solution);
        show_solution();
    }
}

int main(int argc, char* argv[])
{
    string inputfile, outputfile;

    if (argc > 1)
    {
        inputfile = argv[1];
        inputfile += ".2di";
        outputfile = argv[1];
        outputfile += "_rect.2do";
    }
    else
    {
        inputfile = "orig.2di";
        outputfile = "orig_rect.2do";
    }
    
    partcount=read_input(inputfile, part);
    prepareX(part);

    vector <size_t> p(X.size());
    for (size_t i = 0; i < p.size(); i++)
        p[i] = i;
    knuth(X, p);

    return(all_solutions.size());
}