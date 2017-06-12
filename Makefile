on_air: on_air.c
	clang -lobjc -framework CoreAudio -framework CoreMediaIO on_air.c -o on_air
