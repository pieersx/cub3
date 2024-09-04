#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

int width = 160;
int height = 44;
int idx;
int xp, yp;

float zbuffer[160*44];
float k1 = 40;
float A, B, C;
float x, y, z;
float ooz;

char buffer[160*44];

int background_ASCII = ' ';
int distance_from_cam = 100;

float cube_width = 20;
float increment_speed = 0.4;

float cal_x(int i, int j, int k)
{
    return j*sin(A)*sin(B)*cos(C) -
           k*cos(A)*sin(B)*cos(C) +
           j*cos(A)*sin(C) +
           k*sin(A)*sin(C) +
           i*cos(B)*cos(C);
}

float cal_y(int i, int j, int k)
{
    return j*cos(A)*cos(C) +
           k*sin(A)*cos(C) -
           j*sin(A)*sin(B)*sin(C) +
           k*cos(A)*sin(B)*sin(C) -
           i*cos(B)*sin(C);
}

float cal_z(int i, int j, int k)
{
    return k*cos(A)*cos(B) -
           j*sin(A)*cos(B) +
           i*sin(B);
}

void calculate_for_surface(float cube_x, float cube_y, float cube_z, int ch)
{
    x = cal_x(cube_x, cube_y, cube_z);
    y = cal_y(cube_x, cube_y, cube_z);
    z = cal_z(cube_x, cube_y, cube_z) + distance_from_cam;

    ooz = 1/z;
    xp = (int)(width/2 - 2*cube_width + (k1*ooz) * (x*2));
    yp = (int)(height/2 + (k1*ooz) * y);

    idx = xp + yp*width;
    if (idx >= 0 && idx < (width*height)) {
        if (ooz > zbuffer[idx]) {
            zbuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main()
{

    printf("\x1b[2J]");

    while (1) {
        memset(buffer, background_ASCII, width*height);
        memset(zbuffer, 0, width*height*4);

        for (float cube_x = -cube_width; cube_x < cube_width; cube_x += increment_speed) {
            for (float cube_y = -cube_width; cube_y < cube_width; cube_y += increment_speed) {
                calculate_for_surface(cube_x, cube_y, -cube_width, '@');
                calculate_for_surface(cube_width, cube_y, cube_x, '$');
                calculate_for_surface(-cube_width, cube_y, -cube_x, '~');
                calculate_for_surface(-cube_x, cube_y, cube_width, '#');
                calculate_for_surface(cube_x, -cube_width, -cube_y, ';');
                calculate_for_surface(cube_x, cube_width, cube_y, '+');
            }
        }

        printf("\x1b[H");
        for (int k = 0; k < width*height; ++k) {
            putchar(k % width ? buffer[k] : 10);
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;

        usleep(800*2);
    }

    return 0;
}
