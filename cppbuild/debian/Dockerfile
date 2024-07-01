ARG VERSION="bookworm"
FROM debian:${VERSION} as builder

RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    make \
    tar \
    zlib1g-dev \
    libbsd-dev \
    uuid-dev \
    curl \
    gnupg \
    ca-certificates \
    git

RUN curl -s https://repos.azul.com/azul-repo.key | gpg --dearmor -o /usr/share/keyrings/azul.gpg && \
    echo "deb [signed-by=/usr/share/keyrings/azul.gpg] https://repos.azul.com/zulu/deb stable main" | tee /etc/apt/sources.list.d/zulu.list && \
    apt-get update && apt-get install -y zulu-8

ENV JAVA_HOME=/usr/lib/jvm/zulu8 \
    BUILD_JAVA_HOME=/usr/lib/jvm/zulu8 \
    BUILD_JAVA_VERSION=8 \
    GRADLE_OPTS="-Dorg.gradle.daemon=false -Dorg.gradle.java.installations.auto-detect=false -Dorg.gradle.warning.mode=fail"

WORKDIR /opt/aeron
ADD ../.. /opt/aeron

RUN git config --global user.name "github-actions"
RUN git config --global user.email "github-actions@aeron.io"
RUN git config --global core.autocrlf false
RUN git update-index -q --refresh

FROM builder as essentials-build
RUN cppbuild/cppbuild --c-warnings-as-errors --cxx-warnings-as-errors --package

FROM scratch AS essentials-artifacts
COPY --from=essentials-build /opt/aeron/cppbuild/Release/*.tar.gz /
