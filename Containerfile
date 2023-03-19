FROM alpine:3.17.2


WORKDIR /cerver

# Update the package manager
RUN apk update

# Clone the repository and submodules
RUN apk add --no-cache git
RUN git clone https://github.com/Aapeli123/cerver.git
WORKDIR /cerver/cerver
RUN git submodule update --init --recursive

# Configure the package and build dependencies (openssl)
RUN apk add --no-cache perl libc-dev zlib-dev linux-headers cmake make gcc g++ bash
RUN chmod +x /cerver/cerver/scripts/configure
RUN /cerver/cerver/scripts/configure
RUN make

COPY testdir ./testdir
COPY ./scripts/docker.config ./

ENTRYPOINT [ "./cerver", "./docker.config" ]