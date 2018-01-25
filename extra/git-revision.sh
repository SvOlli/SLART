#!/bin/sh

echo "$(git rev-list --count HEAD).$(git rev-parse --verify --short HEAD)"

