FROM gcc:4.9
COPY . /usr/src/cliente
WORKDIR /usr/src/cliente
RUN g++ sensor.cpp -o sensor -lpthread
CMD ["./sensor"]