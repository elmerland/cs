int i;
for (i = N - 1; i > 0; i--) {
	z[i].next = &z[i-1];
}
z[0].next = &y;
sp->next = &z[9];
struct list temp = { .next = sp};
lp->next = &temp;
