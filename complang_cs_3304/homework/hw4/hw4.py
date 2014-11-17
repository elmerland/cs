def x(): 
	# Dynamic or Static scoping?
	stack=[1]*5
	def z():
		print(stack[1])
	def y():
		stack=[2]*5
		z()
	y()
	# Scoping: Static

	# Dynamic or Static type binding?
	a = 5
	print(a)
	a = 'abc'
	print(a)
	# Binding: Dynamic

	# Dynamic or Static type checking?
	a = 5
	b = 'abc'
	print (a+b)
	# Checking: Dynamic

x()