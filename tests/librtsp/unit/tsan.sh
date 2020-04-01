#!/bin/bash

TSAN_OPTIONS="suppressions=tsan-suppression.txt" ./librtspunit
