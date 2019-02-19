# node-cubeb

Cubeb bindings for Node.js.

## Install

```sh
npm i @audio/node-cubeb
```

## Usage

### Building

Building the project manually involves a couple steps. You must have CMake installed, and Linux may also need `libasound2-dev` installed.

Get the Cubeb dependency using Git first:

```
git submodule update --init --recursive
git submodule sync --recursive
```

(Note: Cubeb includes its own submodules making the recursive necessary)

Then execute Make to build the entire project, just the binding, or just Cubeb:

```
make
make binding
make cubeb
```