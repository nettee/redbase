.PHONY: all clean testers
all:
	cd src/ && make all
clean:
	cd src/ && make clean
tester:
	cd src/ && make testers
