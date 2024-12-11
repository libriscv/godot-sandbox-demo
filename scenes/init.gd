extends Node

func _init() -> void:
	Sandbox.load_binary_translation("res://bintr_test.so")
	var api = Sandbox.generate_api()
	var fa = FileAccess.open("generated_api.hpp", FileAccess.WRITE)
	fa.store_string(api)
	fa.close()

func _ready():
	if !FileAccess.file_exists("res://hello_world.elf"):
		var buffer = Sandbox.download_program("hello_world")
		var fa = FileAccess.open("res://hello_world.elf", FileAccess.WRITE)
		fa.store_buffer(buffer)
		fa.close()

	var n = Node.new()
	n.set_script(load("res://hello_world.elf"))
	n.use_precise_simulation = true
	n.execution_timeout = 0
	print(n.hello_world())
	n.print_string("Hello World!")
	print("fibonacci(256) == ", n.fibonacci(256))
	print(n.meaning_of_life)

	if !FileAccess.file_exists("res://luajit.elf"):
		var buffer = Sandbox.download_program("luajit")
		var fa = FileAccess.open("res://luajit.elf", FileAccess.WRITE)
		fa.store_buffer(buffer)
		fa.close()

	print("Loading LuaJIT!")
	var luajit = Node.new()
	luajit.set_script(load("res://luajit.elf"))
	luajit.execution_timeout = 0
	luajit.add_function("test", func(name): return "Test " + str(name) + " called!")
	luajit.add_function("add", func(a, b): return a + b)
	luajit.run("""
	--print(test(1))
	print(add(333, 666))
	function fib(n, acc, prev)
		if (n < 1) then
			return acc
		else
			return fib(n - 1, prev + acc, acc)
		end
	end
	print("The 500th fibonacci number is " .. fib(500, 0, 1))
	""")
