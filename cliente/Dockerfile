FROM gcc:4.9
COPY . /usr/src/cliente
WORKDIR /usr/src/cliente
RUN g++ cliente.cpp -o cliente -lpthread
CMD ["./cliente"]