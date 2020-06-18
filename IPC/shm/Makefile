all: server client

CC := gcc
CXX := g++
LD := g++
AR := ar cr
RANLIB := ranlib

DEBUG_CFLAGS     := -Wall -Wno-format -Wno-write-strings -Wno-unused-variable -Wno-unused-but-set-variable -g
RELEASE_CFLAGS   := -Wall -Wno-unknown-pragmas -Wno-format -Wno-write-strings -Wno-unused-variable -Wno-unused-but-set-variable

BASEWRKDIR := ./
INCS := -I${BASEWRKDIR}

CLIENT_SRCS = ${BASEWRKDIR}command.c
CLIENT_SRCS += ${BASEWRKDIR}client.c

SERVER_SRCS = ${BASEWRKDIR}command.c
SERVER_SRCS += ${BASEWRKDIR}server.c

server: ${SERVER_SRCS}
	${CXX} ${SERVER_SRCS} ${INCS} -o server

client: ${CLIENT_SRCS}
	${CXX} ${CLIENT_SRCS} ${INCS} -o client
