//
//  History.h
//  Project1
//
//  Created by Justin Yi on 1/9/20.
//  Copyright © 2020 Justin Yi. All rights reserved.
//

#ifndef History_h
#define History_h
#include "globals.h"

class History
{
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
  private:
    int m_nRows;
    int m_nCols;
    int m_grid[MAXROWS][MAXCOLS];
    char m_displayGrid[MAXROWS][MAXCOLS];
};

#endif /* History_h */
