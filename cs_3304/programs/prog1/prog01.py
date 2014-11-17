PF1 = "f>f<1&|"
PF2 = "1<f>&>8|9|3~7&^"
tos = -1
mask = 0x0f


def Push(stack, element):
    global tos
    tos += 1
    stack[tos] = element

def Pop(stack):
    global tos
    tos -= 1
    return stack[tos + 1]

def BitTwiddle(expression):
    # Create stack
    global tos 
    tos = -1
    stack = [0] * len(expression)

    # Define operations
    def Shift(direction, value):
        if direction is 'left':
            return (value << 1) & mask
        elif direction is 'right':
            return (value >> 1) & mask
        else:
            print("Oops error here brah!")

    def Not(value):
        return (~value) & mask

    def And(value1, value2):
        return value1 & value2 & mask

    def Xor(value1, value2):
        return (value1 ^ value2) & mask

    def Or(value1, value2):
        return (value1 | value2) & mask

    # Process expresion
    print(expression, end="")

    # Loop through every character in expression
    for c in expression:
        if c.isalnum():
            result = int(c, 16)
        else:
            num1 = Pop(stack)
            if c is '<':
                result = Shift('left', num1)
            elif c is '>':
                result = Shift('right', num1)
            elif c is '~':
                result = Not(num1)
            else:
                num2 = Pop(stack)
                if c is '&':
                    result = And(num1, num2)
                elif c is '^':
                    result = Xor(num1, num2)
                elif c is '|':
                    result = Or(num1, num2)
                else:
                    print("Oops error here brosef! Character: " + c)
        Push(stack, result)

    result = Pop(stack)
    print(" -> " + str(hex(result)[2]))


BitTwiddle(PF1)
BitTwiddle(PF2)
BitTwiddle("f9>&3~8^|c~b||")
