#!/usr/bin/env bash
exec 1>&2

if test -z "$LABWC_NEXT_PID"; then
	echo "LABWC_NEXT_PID not set" >&2
	exit 1
fi

echo "Running with pid $LABWC_NEXT_PID"

# Runtime tests
echo "Executing foot"
foot sh -c 'sleep 1; exit'
echo "Foot exited with $?"

echo "Killing labwc"
kill -s TERM $LABWC_NEXT_PID
