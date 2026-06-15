FROM php:8.2-fpm-alpine

RUN apk add --no-cache \
    libzip-dev \
    gmp-dev \
    icu-dev \
    oniguruma-dev \
    libsodium-dev \
    zip unzip curl git \
    $PHPIZE_DEPS

RUN docker-php-ext-install \
    pdo pdo_mysql zip opcache gmp mbstring intl bcmath sodium

RUN curl -sS https://getcomposer.org/installer \
    | php -- --install-dir=/usr/local/bin --filename=composer

WORKDIR /var/www/html

COPY src/composer.json /var/www/html/composer.json

RUN composer install \
    --no-dev --optimize-autoloader \
    --no-interaction --no-scripts --prefer-dist

EXPOSE 9000
CMD ["php-fpm"]
