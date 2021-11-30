FROM gcc:4.9
COPY . /usr/src/cliente
WORKDIR /usr/src/cliente
RUN g++ servidor2.cpp -o servidor2 -lpthread
CMD ["./servidor2"]