FROM debian:stable

RUN apt-get update && apt-get install -y build-essential libcurl4-openssl-dev git libssl-dev
RUN git clone https://github.com/Cogmasters/concord.git --depth=1 -b dev
RUN cd concord && make -j4 && make install

RUN mkdir /bot
WORKDIR /bot
ADD . /bot

RUN make -j4
CMD ["/bot/bot"]
