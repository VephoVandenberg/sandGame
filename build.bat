cl  main.c src/*.c^
	-I dependencies/GLEW/include -I dependencies/GLFW/include ^
	dependencies/GLEW/lib/glew32.lib dependencies/GLFW/lib/glfw3dll.lib ^
	opengl32.lib user32.lib gdi32.lib