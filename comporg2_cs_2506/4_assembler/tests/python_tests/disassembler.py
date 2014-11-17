import sys

register_nums = {
    30 : 'fp',
    16 : 's0',
    28 : 'gp',
    23 : 's7',
    25 : 't9',
    11 : 't3',
    2 : 'v0',
    3 : 'v1',
    0 : 'zero',
    6 : 'a2',
    1 : 'at',
    21 : 's5',
    22 : 's6',
    20 : 's4',
    29 : 'sp',
    5 : 'a1',
    24 : 't8',
    4 : 'a0',
    14 : 't6',
    15 : 't7',
    12 : 't4',
    13 : 't5',
    10 : 't2',
    7 : 'a3',
    8 : 't0',
    9 : 't1',
    19 : 's3',
    27 : 'k1',
    26 : 'k0',
    31 : 'ra',
    18 : 's2',
    17 : 's1'
}

instructions = {
    '100100' : ('and', 'r'),
    '000001' : ('bltz', 'b1'),
    '001100' : ('syscall', 'sys'),
    '101011' : ('sw', 'offs'),
    '000110' : ('blez', 'b1'),
    '001100' : ('andi', 'i'),
    '100010' : ('sub', 'r'),
    '100000' : ('add', 'r'),
    '100011' : ('lw', 'offs'),
    '000011' : ('jal', 'j'),
    '000101' : ('bne', 'b2'),
    '001000' : ('jr', 'jr'),
    '001101' : ('ori', 'i'),

    '001001' : ('addiu', 'i'),
    '000011' : ('sra', 's'),
    '101010' : ('slt', 'r'),
    '000010' : ('j', 'j'),
    '001010' : ('slti', 'i'),
    '000010' : ('srl', 's'),
    '000000' : ('sll', 's'),
    '001000' : ('addi', 'i'),
    '000100' : ('beq', 'b2'),
    '100101' : ('or', 'r')
}

def get_reg(s):
    r = int(s, 2)
    return '$' + register_nums[r]

def dasm_s(s, op):
    blank = s[6:11]
    r1 = get_reg(s[11:16])
    r2 = get_reg(s[16:21])
    sa = int(s[21:26], 2)
    error = ''
    if blank != '00000':
        error = ' (non-zero blank space)'
    return ('%s %s, %s, %s' % (op, r2, r1, sa)) + error

def dasm_b1(s, op):
    reg = get_reg(s[6:11])
    blank = s[11:16]
    offs = s[16:]
    imm = int(offs, 2)

    if offs[0] == '1':
        imm = imm - 0x10000

    error = ''
    if blank != '00000':
        error = ' (non-zero blank space)'

    imm <<= 2
    return ('%s %s, %d' % (op, reg, imm)) + error

def dasm_j(s, op):
    targ = s[6:]
    targ = int(targ, 2)
    targ <<= 2

    return '%s %s' % (op, targ)
def dasm_jr(s, op):
    reg = get_reg(s[6:11])
    blank = s[11:26]
    error = ''
    if blank != '000000000000000':
        error = ' (non-zero 15bit blank)'
    return ('%s %s' % (op, reg)) + error
def dasm_b2(s, op):
    reg = get_reg(s[6:11])
    reg2 = get_reg(s[11:16])
    offs = s[16:]
    imm = int(offs, 2)

    if offs[0] == '1':
        imm = imm - 0x10000
    imm <<= 2
    return '%s %s, %s, %d' % (op, reg, reg2, imm)
def dasm_r(s, op):
    r1 = get_reg(s[6:11])
    r2 = get_reg(s[11:16])
    r3 = get_reg(s[16:21])

    return '%s %s, %s, %s' % (op, r3, r1, r2)
def dasm_i(s, op):
    r1 = get_reg(s[6:11])
    r2 = get_reg(s[11:16])
    i =  s[16:]

    imm = int(i, 2)
    if i[0] == '1':
        imm = imm - 0x10000
    return '%s %s, %s, %d' % (op, r2, r1, imm)
def dasm_offs(s, op):
    r1 = get_reg(s[6:11])
    r2 = get_reg(s[11:16])

    i = s[16:]
    imm = int(i, 2)
    if i[0] == '1':
        imm = imm - 0x10000
    return '%s %s, %d(%s)' % (op, r2, imm, r1)
def dasm(s):
    op = s[0:6]
    if op == '000000': # r type
        op = s[26:32]
        if op not in instructions:
            return None
        instr, t = instructions[op]
        if instr == 'andi':
            return 'syscall'
        elif t == 's':
            return dasm_s(s, instr)
        return dasm_r(s, instr)
    else:
        if op not in instructions:
            return None
            
        instr, t = instructions[op]
        if t == 'i':
            return dasm_i(s, instr)
        elif t == 'offs':
            return dasm_offs(s, instr)
        elif t == 'j':
            return dasm_j(s, instr)
        elif t == 'jr':
            return dasm_jr(s, instr)
        elif t == 'b1':
            return dasm_b1(s, instr)
        elif t == 'b2':
            return dasm_b2(s, instr)
def dasm_err(s):
    d = dasm(s)
    if d == None:
        return error_msg(s)
    return d
def get_opcode(s):
    
    op = s[0:6]
    if op == '000000':
        op =  s[26:32]
    return op
def error_msg(s):
    if len(s) != 32:
        return 'instruction is not 32 bits'
    return 'unknown instruction with opcode ' + get_opcode(s)
def disassemble_lines(lines):
    return map(lambda x: x if x != None else error_msg(x), map(dasm, lines))

def disassemble_str(s):
    return '\n'.join(disassemble_lines(s.split('\n')))

if __name__ == '__main__':
    infile = sys.argv[1]
    outfile = sys.argv[2]

    inb = open(infile, 'r').read()
    inb = inb.replace('\r', '')
    lines = inb.split('\n')
    if lines.index('') > 0:
        lines = lines[:lines.index('')]
    o = '\n'.join(disassemble_lines(lines)) + '\n'
    open(outfile, 'w').write(o)
