#include <stdio.h>
int main() {
    int b,c,sink, source, N;
    // read source from input
    source = getchar();
    int i = 0;
    while (i < N) {
        if (i % 2 == 0)
            b = source;
        else
            c = b;
        i++;
    }
    sink = c;
}
