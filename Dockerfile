FROM ubuntu:22.04

ENV CFLAGS="-std=c99 -Wall -Wconversion -Wtype-limits -Werror -pedantic -ffunction-sections -fdata-sections -O2 -no-pie -lm -g"
ENV LDFLAGS="-Wl,--gc-sections"
ENV VFLAGS="--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes"

RUN apt-get update --fix-missing && apt-get install -y --fix-missing bash build-essential curl micro valgrind
RUN echo "alias gcc2='gcc \$CFLAGS \$LDFLAGS /usr/local/src/*.c'" >> /etc/bash.bashrc
RUN echo "alias valgrind2='valgrind \$VFLAGS'" >> /etc/bash.bashrc

WORKDIR /home

CMD ["/bin/bash"]
