#!/bin/bash
NICK=pedro
SERVER=localhost
PORT=4342
CHAN="#truc"
PASS=toto

nc -C $SERVER $CHAN
PASS $PASS
NICK $NICK
USER pedro tech localhost :OUI
JOIN $CHAN

while read input; do
  echo "$input" | nc -C $SERVER $PORT
done