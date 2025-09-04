# C++ Http Server

## Purpose

...

## Requirements

- Cmake
- Ninja
- C++ compiler
- vcpkg

Or just run with docker :smiley:

## How to run

### With docker

```sh
docker build . -t <name>
```

```sh
docker run -p 8443:8443 <your image name>
```

### With cmake

Create `CMakeUserPresets.json` and adjust vcpkg path.

```json
{
  "version": 2,
  "configurePresets": [
    {
      "name": "default",
      "inherits": "vcpkg",
      "environment": {
        "VCPKG_ROOT": "<path to vcpkg>"
      }
    }
  ]
}
```

```sh
cmake --preset=default
```

```sh
cmake --build build
```

```sh
./build/HTTP_SERVER
```
