PATH := $(PWD)/node_modules/.bin:$(PATH)

.SILENT:
.ONESHELL:
.PHONY: all clean cubeb binding

all: clean cubeb binding

clean:
	rm -rf cubeb/build build dist

cubeb:
	mkdir cubeb/build && \
	cd cubeb/build && \
	cmake .. && \
	cmake --build .

binding:
	mkdir -p build && \
	cd build && \
	cmake-js build -d ..
