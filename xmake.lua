add_rules("mode.debug", "mode.release")
add_requires("cunit")

target("ptsd")
set_kind("static")
add_includedirs("include")
add_cflags("-fsanitize=address", { tools = "gcc" })
add_files("src/*.c")

for _, file in ipairs(os.files("test/test_*.c")) do
	local name = path.basename(file)
	target(name)
	set_default(false)
	add_packages("cunit")
	add_files("test/" .. name .. ".c")
	add_includedirs("include")
	add_files("src/*.c")
	add_tests(name)
end
