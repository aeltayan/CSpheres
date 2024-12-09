# Variables
CC = gcc
CFLAGS = -Wall -Werror -std=c99

# Executables
all: MS1_assg MS2_assg FS_assg

MS1_assg: src/assg_ms1.o src/vector.o src/spheres.o src/color.o
	$(CC) $(CFLAGS) src/assg_ms1.o src/vector.o src/spheres.o src/color.o -o MS1_assg -lm

MS2_assg: src/assg_ms2.o src/vector.o src/spheres.o src/color.o
	@echo "Linking MS2_assg"
	$(CC) $(CFLAGS) src/assg_ms2.o src/vector.o src/spheres.o src/color.o -o MS2_assg -lm

FS_assg: src/assg_fs.o src/vector.o src/spheres.o src/color.o
	@echo "Linking FS_assg"
	$(CC) $(CFLAGS) src/assg_fs.o src/vector.o src/spheres.o src/color.o -o FS_assg -lm

# Object files with specific macros
src/assg_ms1.o: src/assg.c src/vector.h src/spheres.h
	@echo "Compiling assg.c with MS1"
	$(CC) $(CFLAGS) -DMS1 -c src/assg.c -o src/assg_ms1.o

src/assg_ms2.o: src/assg.c src/vector.h src/spheres.h
	@echo "Compiling assg.c with MS2"
	$(CC) $(CFLAGS) -DMS2 -c src/assg.c -o src/assg_ms2.o

src/assg_fs.o: src/assg.c src/vector.h src/spheres.h
	@echo "Compiling assg.c with FS"
	$(CC) $(CFLAGS) -DFS -c src/assg.c -o src/assg_fs.o

# Other object files
src/vector.o: src/vector.c src/vector.h
	@echo "Compiling vector.c"
	$(CC) $(CFLAGS) -c src/vector.c -o src/vector.o

src/spheres.o: src/spheres.c src/spheres.h
	@echo "Compiling spheres.c"
	$(CC) $(CFLAGS) -c src/spheres.c -o src/spheres.o

src/color.o: src/color.c src/color.h
	@echo "Compiling color.c"
	$(CC) $(CFLAGS) -c src/color.c -o src/color.o

# Clean
clean:
	@echo "Deleting intermediate files and executables"
	rm -f src/*.o MS1_assg MS2_assg FS_assg

