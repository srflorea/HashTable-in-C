all: build

build: tema0.exe

tema0.exe: tema0.obj hash.obj
	cl /Fetema0.exe /nologo /Wall tema0.obj hash.obj
	
tema0.obj: tema0.c
	cl /Fotema0.obj /c tema0.c
	
hash.obj: hash.c
	cl /Fohash.obj /c hash.c
	
clean:
	del *.obj tema0.exe