import sys

v = []
for line in sys.stdin:
    a = line.split()
    v.append((a[0], sum(map(float, a[1:]))/4.0))

for p, a in v:
    print p, float(v[0][1])/a
