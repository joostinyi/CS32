//
//  History.cpp
//  Project1
//
//  Created by Justin Yi on 1/9/20.
//  Copyright © 2020 Justin Yi. All rights reserved.
//

#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_nRows = nRows;
    m_nCols = nCols;
    for(int r = 1; r <= nRows; r++)
        for(int c = 1; c <= nCols; c++)
        {
            m_grid[r-1][c-1] = 0;
            m_displayGrid[r-1][c-1] = '.';
        }
}

bool History::record(int r, int c)
{
    if (r < 1  ||  r > m_nRows  ||  c < 1  ||  c > m_nCols)
        return false;
    // record value to grid
    m_grid[r-1][c-1]++;
    switch (m_grid[r-1][c-1])
    {
        case 1:  m_displayGrid[r-1][c-1] = 'A'; break;
        default: if(m_grid[r-1][c-1] <= 26) m_displayGrid[r-1][c-1]++; break;
    }
            return true;
}

void History::display() const
{
    clearScreen();
    int r,c;
    for (r = 1; r <= m_nRows; r++)
    {
        for (c = 1; c <= m_nCols; c++)
            cout << m_displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
}


