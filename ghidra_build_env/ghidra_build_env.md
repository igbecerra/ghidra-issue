OS: ```Debian 12```

apt-get pkgs: ```bison build-essential curl flex g++ gcc git make software-properties-common unzip wget xvfb openjdk-17-jdk```

JDK version: ```17.0.11+9-Debian-1deb12u1```

Gradle version: ```7.3```

### Build commands:
    git bisect
    Pre-load dependencies/downloads content from previous download
    patch gradle/support/fetchDependencies.gradle to disable the sha256 check as
        the checksums for most of the bisected commits are incorrect for the FIDs
    gradle --parallel --init-script gradle/support/fetchDependencies.gradle init
    gradle --parallel buildGhidra -x ip

### Extra notes:
    Bad commit: 02248d2251ec4da62c1d8fbcda1503c89dd7af9e
    Confirmed working OK in previous commits
    Confirmed as failing in bad commit and in releases 10.3, 11.0.3, 11.1.2 and
        in latest master commit 0857d52681687835a20cf8a898e5c317ae5d632e

