#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>

int D;
char ** buffer;

void pset(int x, int y, char c) {
    if (x >= 0 && x < D && y >= 0 && y < D) {
        buffer[x][y] = c;
    }
}

void circle(int x0, int y0, int r, char c) {
    int x = r;
    int y = 0;
    int radiusError = 1-x;

    while(x >= y) {
        pset( x + x0,  y + y0, c);
        pset( y + x0,  x + y0, c);
        pset(-x + x0,  y + y0, c);
        pset(-y + x0,  x + y0, c);
        pset(-x + x0, -y + y0, c);
        pset(-y + x0, -x + y0, c);
        pset( x + x0, -y + y0, c);
        pset( y + x0, -x + y0, c);
        y++;
        if (radiusError<0) {
            radiusError += 2 * y + 1;
        } else {
          x--;
            radiusError += 2 * (y - x) + 1;
        }
    }
}

void change_color(int color) {
    printf("\e[48;5;%dm", color);
}

void cursor_home() {
    printf("\e[H");
}

void draw() {
    cursor_home();

    int x, y;
    int cc = -1;
    for (y = 0; y < D; ++ y) {
        for (x = 0; x < D; ++ x) {
            char c = buffer[x][y];
            if ((int)c != cc) {
                cc = c;
                change_color(cc);
            }
            printf("  ");
        }
        printf("\n");
    }
}

int max_diameter() {
    struct winsize wsize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsize);

    int cols = wsize.ws_col / 2;
    
    return (wsize.ws_row < cols ? wsize.ws_row : cols) - 2;
}

void init_buffer() {
    int y;
    buffer = malloc(D * sizeof(char *));
    for (y = 0; y < D; ++ y) {
        buffer[y] = malloc(D * sizeof(char));
    }
}

int main() {
    D = max_diameter();
    init_buffer();

    int R = D / 2;
    unsigned int offset = 0;
    
    int r;
    while (1) {
        for (r = 0; r < R; ++ r) {
            circle(R, R, r, (offset + r) % 256);
        }
        draw();
        usleep(100000);

        ++ offset;
    }

    return 0;
}
