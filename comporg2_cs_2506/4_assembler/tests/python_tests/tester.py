#!/usr/bin/python
import diff
import disassembler
import os
import optparse
import glob
import sys

assembler_name = 'assembler.py'
make_cmd = 'make'

directory = '.'
print_diff = False

only_data = False
only_instr = False

tmp_name = '__tmp.o'

def closest_to(f, ls):
    min = ''
    minDist = 10000
    for l in ls:
        dist = diff.levenshtein(f, l)
        if dist < minDist:
            minDist = dist
            min = l
    return min

def get_max_score(f):
    o = read_file(f)
    return len(filter(lambda x: x != '',  clean(o).split('\n')))
def test_whole_dir(infile, outfile):
    ins = glob.glob(infile)
    outs = glob.glob(outfile)
    
    results = []
    score = 0
    max_score = 0
    for i in ins:
        remove_tmp()
        o = closest_to(i, outs)
        if o == '':
            print '*** Could not find a valid output file to match: ' + outfile
            print ''
            continue
        print 'Testing your assembler on input file %s against output file %s' % (i, o)
        print '----------------------------------------------------------------------'
        print 'Running your assembler...'
        run_assembler(i, tmp_name)
        if not os.path.isfile(tmp_name):
            print 'It appears your assembler didn\'t output anything... (maybe it crashed?)'
            m = get_max_score(o)
            max_score += m
            results.append(((0, m), []))
            print 'score:', 0, '/', m
        else:
            if not only_data:
                print 'Checking your instruction output =>'
                s, r = test_outputs(o, tmp_name)
                a, b = s
                score += a
                max_score += b
                results.append((s, r))
                print ''

            if not only_instr:
                print 'Checking your data segment output =>'
                s, r = test_data_outputs(o, tmp_name)
                a, b = s
                score += a
                max_score += b
                results.append((s, r))
            print '-----------------------------------------------------------------------'
            print ''

    print 'Finished testing your assembler on %d file(s).' % len(ins)
    print 'Your total score is %d / %d' % (score, max_score)
def make_assembler():
    if not os.path.isfile(assembler_name):
        print 'Assembler file not found, running make command:', make_cmd
        os.system(make_cmd)
        print ''
def remove_tmp():
    try:
        os.remove(tmp_name)
    except:
        pass
def run_assembler(i, o):
    os.system('%s %s %s' % (assembler_name, i, o))
def clean(s):
    return s.replace('\r', '')
def get_instr_lines(s):
    s = clean(s)
    lines = s.split('\n')
    lines = map(lambda x: x.strip(), lines)
    if '' in lines:
        lines = lines[:get_whitespace(lines)]
    return lines
def get_whitespace(ls):
    for i in range(len(ls)):
        if ls[i] == '':
            return i
    return -1
def get_data_lines(s):
    s = clean(s)
    lines = s.split('\n')
    lines = map(lambda x: x.strip(), lines)

    if '' in lines:
        lines = lines[get_whitespace(lines):]
        return lines
    return []
def read_file(f):
    f = open(f, 'r')
    r = f.read() 
    f.close()
    return r
def test_data_outputs(realfile, genfile):
    real = read_file(realfile)
    gen = read_file(genfile)


    real_lines = get_data_lines(real)
    gen_lines = get_data_lines(gen)

    if print_diff:
        print 'Outputting diff between data segment =>'
        print '---------------------------------------'
        diff.sdiff(real_lines, gen_lines)
        print '---------------------------------------'

    n, e, a, r = diff.line_diff(real_lines, gen_lines)

    score = (max(0, len(real_lines) - n), len(real_lines))
    errors = []
    for k in e:
        i, j = k
        real_data = real_lines[i - 1]
        your_data = gen_lines[j - 1]
        
        msg = 'difference: real output => %s\n                    your output => %s' % (real_data, your_data)
        errors.append('(%d, %d) => \n\t%s' % (i, j, msg))
    for k in a:
        i, j = k
        your_data = gen_lines[j - 1]
        msg = 'random added line => ' + your_data + '\n\t maybe you padded too much, or added an extra array element?'
        errors.append('(%d, %d) => \n\t%s' % (i, j, msg))
    for k in r:
        i, j = k
        real_data = real_lines[i - 1]
        
        msg = 'missed line => ' + real_data + '\n\t maybe you forgot to pad your string, or missed an array element?'
        errors.append('(%d, %d) => \n\t%s' % (i, j, msg))
    errors.sort()

    if n == 0:
        errors.append('perfect match!')
    print '\n'.join(errors)
    c, m = score
    print 'score:', c, '/', m

    return (score, errors)
def test_outputs(realfile, genfile):
    real = read_file(realfile)
    gen = read_file(genfile)

    real_lines = get_instr_lines(real)
    gen_lines = get_instr_lines(gen)

    if print_diff:
        print 'Outputting diff between instructions =>'
        print '---------------------------------------'
        diff.sdiff(real_lines, gen_lines)
        print '---------------------------------------'

    n, e, a, r = diff.line_diff(real_lines, gen_lines)

    score = (max(0, len(real_lines) - n), len(real_lines))
    errors = []
    for k in e:
        i, j = k
        real_asm = disassembler.dasm_err(real_lines[i - 1])
        your_asm = disassembler.dasm_err(gen_lines[j - 1])
        
        msg = 'difference: real output => %s\n                    your output => %s' % (real_asm, your_asm)
        errors.append('(%d, %d) => \n\t%s' % (i, j, msg))
    for k in a:
        i, j = k
        your_asm = disassembler.dasm_err(gen_lines[j - 1])
        msg = 'random added line => ' + your_asm
        errors.append('(%d, %d) => \n\t%s' % (i, j, msg))
    for k in r:
        i, j = k
        real_asm = disassembler.dasm_err(real_lines[i - 1])
        
        msg = 'missed line => ' + real_asm
        errors.append('(%d, %d) => \n\t%s' % (i, j, msg))
    errors.sort()

    if n == 0:
        errors.append('perfect match!')
    print '\n'.join(errors)
    c, m = score
    print 'score:', c, '/', m

    return (score, errors)

if __name__ == '__main__':

    parser = optparse.OptionParser(description='Test suite for CS2506 assembler. Arguments are given as UNIX file patterns, ex: tester.py \'*.asm\' \'*.o\' tests your assembler on all the .asm files against their respective .o files. Note that if you are using a wildcard (*, ?) on UNIX you need to quote the argument as \'*.asm\' or the shell will send the file names instead of the pattern.')
    parser.add_option('-d', '--dir', default='.', action='store', help='working directory (for tests, your assembler, etc)')
    parser.add_option('-D', '--diff', action='store_true', help='display side by side diff on files')
    parser.add_option('-e', '--executable', default='./assembler', help='set executable of your assembler')
    parser.add_option('-m', '--make', default='make', help='set make command to build your assembler')
    parser.add_option('-c', '--clean', action='store_true', help='run make clean before running tests')

    parser.add_option('--data', action='store_true', help='only grade data segments')
    parser.add_option('--instr', action='store_true', help='only grade instructions')
    
    (opts, args) = parser.parse_args()

    if len(args) != 2:
        print 'Invalid command line usage.'
        print 'Expected only two arguments but got', len(args)
        if len(args) > 2:
            print 'Make sure you use quotes (eg: \'*.asm\') if you use a wildcard.'
        else:
            print 'Usage is tester.py infile.asm outfile.o'
    else:
        assembler_name = opts.executable
        make_cmd = opts.make
        directory = opts.dir
        print_diff = opts.diff
        only_data = opts.data
        only_instr = opts.instr

        if directory != '.':
            os.chdir(directory)
        if opts.clean:
            print 'Running a make clean =>'
            os.system('make clean')
        make_assembler()
        test_whole_dir(args[0], args[1])
        remove_tmp()
                
