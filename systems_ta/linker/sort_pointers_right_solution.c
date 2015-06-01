
    int i;

    lp->next = &x;
    x.next = z + N - 1;
    for (i = N - 1; i > 0; i--)
        z[i].next = z + i - 1;

    z[0].next = sp;
    sp->next = &y;
