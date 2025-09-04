# C++ Http Server

## :globe_with_meridians: About This Project

I started this project to better understand how HTTP works at a low level.
My initial goal was simple — to build a server that could send an HTML page to a client.

As I progressed, I expanded the project to include:

- HTTPS support – learning how to implement secure communication

- Security features – for example, path sanitization to prevent path traversal attacks

Coding this project, I learned:

- Socket programming in C++

- How HTTP/1.0 and HTTP/1.1 work

- The basics of HTTPS and TLS

- How to integrate security features into a custom server

## :clipboard: Requirements

- Cmake
- Ninja
- C++ compiler
- vcpkg

Or just run with docker :smiley:

## :floppy_disk: Setup & Run

### Docker

```sh
docker build . -t <name>
```

```sh
docker run -p 8443:8443 <your image name>
```

### Cmake

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
