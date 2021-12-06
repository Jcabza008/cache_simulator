#!/usr/bin/env bash

SUDO=sudo
if [ "$GITHUB_ACTOR" == "nektos/act" ]; then
    echo "Running in 'act' container, not using sudo"
    SUDO=""
fi

echo "Updating packages"
$SUDO apt-get update

echo "Install gtests and gmock"
$SUDO apt-get install -y \
    libgtest-dev \
    libgmock-dev