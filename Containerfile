FROM alpine:3.17.2

WORKDIR /cerver


RUN apk update
RUN apk add --no-cache git cmake make gcc openssh
RUN git clone https://github.com/Aapeli123/cerver.git
WORKDIR /cerver/cerver

RUN git submodule update --init --recursive



RUN chmod +x ./scripts/configure
RUN ./scripts/configure
RUN make

