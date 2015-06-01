/* For illustrative purposes, a wrong implementation is shown. */
/* BEGIN of the section you must change */
    int i;
    lp->next = sp;
    sp->next = &x;
    x.next = z;

    for (i = 0; i < N - 1; i++)
        z[i].next = z + i + 1;

    z[N - 1].next = &y;
/* END of the section you must change */
