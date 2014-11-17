#!/usr/bin/python
import sys

# taken from wikibooks.org
def levenshtein(s1, s2):
    if len(s1) < len(s2):
        return levenshtein(s2, s1)
 
    if len(s2) == 0:
        return len(s1)
 
    previous_row = xrange(len(s2) + 1)
    for i, c1 in enumerate(s1):
        current_row = [i + 1]
        for j, c2 in enumerate(s2):
            insertions = previous_row[j + 1] + 1 
            deletions = current_row[j] + 1       
            substitutions = previous_row[j] + (c1 != c2)
            current_row.append(min(insertions, deletions, substitutions))
        previous_row = current_row
 
    return previous_row[-1]

# DP line diff algorithm
# modification of WF algorithm
def line_diff(a, b):
    memo = {}
    # returns a tuple of (line errors, [(i, j) of line errors],
    #     [(i, j) of additions], [(i, j) of deletions])
    def recur(i, j):
        if i == j == 0:
            return (0, [], [], [])
        if j == 0:
            ls = []
            for k in range(i):
                ls.append(((k + 1), 1))
            return (i, [], [], ls)
        if i == 0:
            ls = []
            for k in range(j):
                ls.append((1, (k + 1)))
            return (j, [], ls, [])
        if (i,j) in memo:
            return memo[(i,j)]
        if a[i - 1] == b[j - 1]:
            v = recur(i - 1, j - 1)
            memo[(i,j)] = v
            return v
        else:
            d1, e1, a1, r1 = recur(i - 1, j) # deletion of a line
            d2, e2, a2, r2 = recur(i, j - 1) # insertion of a line
            d3, e3, a3, r3 = recur(i - 1, j - 1) # error in line

            v = None
            if d1 < d2 and d1 < d3:
                v = (d1 + 1, e1, a1, r1 + [(i, j+1)])
            elif d2 < d1 and d2 < d3:
                v = (d2 + 1, e2, a2 + [(i+1, j)], r2)
            else:
                v = (d3 + 1, e3 + [(i, j)], a3, r3)
            memo[(i,j)] = v
            return v
    return recur(len(a), len(b))
    
def sdiff(a, b):
    num, errors, additions, removals = line_diff(a, b)
    maxlena = max(map(len, a))
    maxlenb = max(map(len, b))
    maxlen = max(maxlena, maxlenb)

    i = 1
    j = 1
    while i <= len(a) and j <= len(b):
        m = a[i - 1]
        n = b[j - 1]
        s = ''
        if (i, j) in errors:
            fmt = '%-' + str(maxlen + 3) + 's######   %s'
            s = fmt % (m, n)
        elif (i, j) in additions:
            fmt = '%-' + str(maxlen + 3) + 's++++++   %s'
            s = fmt % ('*', n)
            i -= 1
        elif (i, j) in removals:
            fmt = '%-' + str(maxlen + 3) + 's------   *'
            s = fmt % m
            j -= 1
        else:
            fmt = '%-'+ str(maxlen + 3) + 's         %s'
            s = fmt % (m, n)
        print s
        i += 1
        j += 1

    if num == 0:
        print '=> Clean diff between files'
    else:
        print '=>', num, 'line differences'

if __name__ == '__main__':
    infile = sys.argv[1]
    infile2 = sys.argv[2]

    a = open(infile, 'r').read().replace('\r', '').split('\n')
    b = open(infile2, 'r').read().replace('\r', '').split('\n')

    sdiff(a, b)
