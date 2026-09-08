FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    gdb \
    valgrind \
    make \
    strace \
    net-tools \
    netcat \
    tcpdump \
    man-db \
    manpages-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

CMD ["/bin/bash"]