FROM php:8.2-fpm-alpine
RUN apk add --no-cache openssl && docker-php-ext-install opcache
WORKDIR /var/www/html
RUN chown -R www-data:www-data /var/www/html
CMD ["php-fpm"]
