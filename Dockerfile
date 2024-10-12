# Используем официальный образ Ubuntu
FROM ubuntu:20.04

# Рабочая директория
WORKDIR /app

# Установка зависимостей
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make \
    libssl-dev \
    libpthread-stubs0-dev \
    clang-format \
    && rm -rf /var/lib/apt/lists/*

# Копируем исходные файлы в контейнер
COPY . /app

# Создаем директорию для сборки
RUN mkdir -p build

# Переходим в директорию сборки
WORKDIR /app/build

# Выполняем сборку
RUN cmake .. && make

# Команда для запуска приложения
CMD ["./synledger"]
