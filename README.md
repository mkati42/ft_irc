<h1 align="center">Ft_irc</h1>

<p align="center">
  <img alt="Github top language" src="https://img.shields.io/github/languages/top/mkati42/ft_irc?color=56BEB8">
  <img alt="Github language count" src="https://img.shields.io/github/languages/count/mkati42/ft_irc?color=56BEB8">
  <img alt="Repository size" src="https://img.shields.io/github/repo-size/mkati42/ft_irc?color=56BEB8">
  <img alt="License" src="https://img.shields.io/github/license/mkati42/ft_irc?color=56BEB8">
</p>

<p align="center">
  <a href="#dart-about">About</a> &#xa0; | &#xa0; 
  <a href="#sparkles-features">Features</a> &#xa0; | &#xa0;
  <a href="#rocket-technologies">Technologies</a> &#xa0; | &#xa0;
  <a href="#white_check_mark-requirements">Requirements</a> &#xa0; | &#xa0;
  <a href="#checkered_flag-starting">Starting</a> &#xa0; | &#xa0;
  <a href="#memo-license">License</a> &#xa0; | &#xa0;
  <a href="https://github.com/mkati42" target="_blank">Author</a>
</p>

<br>

## :dart: About ##

**ft_irc**, 42 Network tarafından verilen bir socket programlama projesidir. Bu projede, RFC 1459 standartlarına uygun şekilde çalışan basit bir IRC (Internet Relay Chat) sunucusu geliştirmeniz beklenir. Bu sunucu, istemcilerin bağlanmasına, kanallara katılmasına ve birbirleriyle mesajlaşmasına olanak tanır.

## :sparkles: Features ##

:heavy_check_mark: IPv4 üzerinden TCP bağlantı;\
:heavy_check_mark: RFC1459 uyumlu mesaj protokolü;\
:heavy_check_mark: Nick, User, Join, Part, Privmsg gibi temel IRC komutlarının desteği;\
:heavy_check_mark: Kanallarda kullanıcı yönetimi;\
:heavy_check_mark: Non-blocking socket işlemleri ve `select` kullanımı;\
:heavy_check_mark: Hatalı girişler için hata mesajları ve güvenli bağlantı yönetimi.

## :rocket: Technologies ##

Bu projede kullanılan teknolojiler ve konseptler:

- C++
- POSIX Sockets (IPv4, TCP)
- `select()` ile multiplexing
- `fcntl()` ile non-blocking sockets
- Signal handling (`SIGINT`)
- Temel IRC protokolü (RFC1459'e referansla)
- UNIX/Linux sistem programlama

## :white_check_mark: Requirements ##

Projeyi başlatmadan önce aşağıdakilerin sisteminizde yüklü olduğundan emin olun:

- Git
- C++ compiler (g++ veya clang++)
- Make

## :checkered_flag: Starting ##

```bash
# Bu projeyi klonlayın
$ git clone https://github.com/mkati42/ft_irc.git

# Proje dizinine girin
$ cd ft_irc

# Derleyin
$ make

# Sunucuyu başlatın (örneğin port 6667'de)
$ ./ircserv <port> <password>
# Örnek:
$ ./ircserv 6667 1234

# Ardından bir IRC istemcisi ile bağlanabilirsiniz:
# Örnek: irssi, weechat veya netcat
$ nc localhost 6667
