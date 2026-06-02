void swap (int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int min (int a, int b, int c) {
    int m;
    if (a <= b) {
        if (a <= c)  m = a;
        else         m = c;
    }
    else m = (b <= c) ? b : c;
    return m;
}

int max (int a, int b, int c) {
    if (a <= b)  swap (&a, &b);
    if (a <= c)  swap (&a, &c);
    return a;
}
