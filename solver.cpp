#include <stdio.h>

int status[82][9][9];
int possible_fill[82][9][9];
int priority_lvl[82][9][9];

int solve(int seq);
int fill(int seq, int x, int y, int num);
int update(int seq, int &next_x, int &next_y);

int solve_sodoku()
{
  for (int i=0; i<9; ++i) {
    for (int j=0; j<9; ++j) {
      possible_fill[0][i][j]  = 0x01FF;
      priority_lvl[0][i][j]   = 9;
    }
  }
  printf("please input sodoku puzzle:\n");
  int t;
  for (int i=0; i<9; ++i) {
    for (int j=0; j<9; ++j) {
      scanf("%d", &t);
      if (t < 1 || t > 9) {
        t = 0;
      }
      fill(0, i, j, t);
    }
  }
  printf("try to solve sodoku ...\n");
  return solve(0);
}

int solve(int seq)
{
  int ret, nx, ny;
  if (update(seq, nx, ny) == 1) {
    for (int i=0; i<9; ++i) {
      for (int j=0; j<9; ++j) {
        if (j > 0) {
          printf("\t");
        }
        printf("%d", status[seq][i][j]);
      }
      printf("\n");
    }
    return 0;
  }
  for (int k=0; k<9; ++k) {
    if (possible_fill[seq][nx][ny] & (1<<k)) {
      for (int i=0; i<9; ++i) {
        for (int j=0; j<9; ++j) {
          status[seq+1][i][j]         = status[seq][i][j];
          possible_fill[seq+1][i][j]  = possible_fill[seq][i][j];
          priority_lvl[seq+1][i][j]   = priority_lvl[seq][i][j];
        }
      }
      if (fill(seq+1, nx, ny, k+1) == 0) {
        ret = solve(seq+1);
        if (ret == 0) return 0;
      } else {
        return -1;
      }
    }
  }
  return -1;
}

int fill(int seq, int x, int y, int num)
{
  if ((possible_fill[seq][x][y] & (1<<(num-1))) == 0) {
    return -1;
  }
  if (num == 0) {
    status[seq][x][y] = 0;
    return 0;
  }
  status[seq][x][y]         = num;
  possible_fill[seq][x][y]  = 1<<(num-1);
  for (int i=0; i<9; ++i) {
    if (i != x) {
      possible_fill[seq][i][y]  &= ~(1<<(num-1));
      if (possible_fill[seq][i][y] == 0) {
        return -1;
      }
      int n = 0;
      for(int k=0; k<9; ++k) {
        if (possible_fill[seq][i][y] & (1<<k)) {
          ++n;
        }
      }
      priority_lvl[seq][i][y] = n;
    }
    if (i != y) {
      possible_fill[seq][x][i]  &= ~(1<<(num-1));
      if (possible_fill[seq][x][i] == 0) {
        return -1;
      }
      int n = 0;
      for(int k=0; k<9; ++k) {
        if (possible_fill[seq][x][i] & (1<<k)) {
          ++n;
        }
      }
      priority_lvl[seq][x][i] = n;
    }
  }
  int box_x = x / 3;
  int box_y = y / 3;
  for (int i=0; i<3; ++i) {
    for (int j=0; j<3; ++j) {
      if (x == 3*box_x+i && y == 3*box_y+j) {
        continue;
      }
      possible_fill[seq][3*box_x+i][3*box_y+j]  &= ~(1<<(num-1));
      if (possible_fill[seq][3*box_x+i][3*box_y+j] == 0) {
        return -1;
      }
      int n = 0;
      for(int k=0; k<9; ++k) {
        if (possible_fill[seq][3*box_x+i][3*box_y+j] & (1<<k)) {
          ++n;
        }
      }
      priority_lvl[seq][3*box_x+i][3*box_y+j] = n;
    }
  }
  return 0;
}

int update(int seq, int &next_x, int &next_y)
{
  int min_priority  = 10;
  for (int i=0; i<9; ++i) {
    for (int j=0; j<9; ++j) {
      if (priority_lvl[seq][i][j] < min_priority && status[seq][i][j] == 0) {
        min_priority  = priority_lvl[seq][i][j];
        next_x        = i;
        next_y        = j;
      }
    }
  }
  if (min_priority == 10) {
    return 1;
  }
  return 0;
}

int main()
{
  solve_sodoku();
  return 0;
}

