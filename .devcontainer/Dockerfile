FROM ubuntu:20.04

ENV TZ=Europe/Paris
RUN \   
  apt update && DEBIAN_FRONTEND=noninteractive apt install --assume-yes build-essential cmake valgrind python3 python3-pip graphviz && \
  pip install gprof2dot

WORKDIR /app
