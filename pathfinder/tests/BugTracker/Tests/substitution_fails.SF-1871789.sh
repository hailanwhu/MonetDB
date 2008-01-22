#!/usr/bin/env bash

q=substitution_fails.SF-1871789.ok.xq
echo "\"pf -M $q >/dev/null\""
echo "\"pf -M $q >/dev/null\"" >&2
pf -M $q >/dev/null
echo "\"pf -A $q >/dev/null\""
echo "\"pf -A $q >/dev/null\"" >&2
pf -A $q >/dev/null

q=substitution_fails.SF-1871789.ko.xq
echo "\"pf -M $q >/dev/null\""
echo "\"pf -M $q >/dev/null\"" >&2
pf -M $q >/dev/null
echo "\"pf -A $q >/dev/null\""
echo "\"pf -A $q >/dev/null\"" >&2
pf -A $q >/dev/null
