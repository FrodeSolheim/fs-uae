#include <stdio.h>
#include <math.h>

void test(int scale_y) {
    int scale_x = round(scale_y / 1.2);
    int rev_scale_y = round(scale_x * 1.2);

    printf(" %2d : %2d  (%0.2f)  REV %d OK? %d\n",
        scale_y, scale_x, 1.0 * scale_x / scale_y, rev_scale_y, rev_scale_y == scale_y);
}

int main(int argc, char argv[]) {
    for (int i = 1; i <= 20; i++) {
        test(i);
    }
}
