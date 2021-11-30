FROM gcc:4.9
COPY . /usr/src/cliente
WORKDIR /usr/src/cliente
RUN g++ servidor.cpp -o servidor -lpthread
CMD ["./servidor"]