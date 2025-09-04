FROM ubuntu:latest

RUN apt-get update && apt-get -y install cmake \ 
    build-essential\
    pkg-config \
    git \
    ninja-build \
    curl \
    zip \
    unzip \
    tar 

RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg \
    && /opt/vcpkg/bootstrap-vcpkg.sh

WORKDIR /build/

COPY DockerUserPresets.json CMakeUserPresets.json
COPY . .

RUN cmake --preset=default
RUN cmake --build build  

EXPOSE 8443

CMD ["./build/HTTP_SERVER"]



