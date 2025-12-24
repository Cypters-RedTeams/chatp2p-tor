# ChatP2P-Tor

## EN — English

A minimal, bloat-free **peer-to-peer chat** over Tor using SOCKS5.
Supports **full P2P** communication between `.onion` addresses and can handle **incoming connections** and **outgoing connections simultaneously**.

* **Language:** C (100% bloat-free)
* **Features:**

  * Connect to a remote peer via Tor SOCKS5
  * Accept incoming connections from other peers
  * Full-duplex messaging with receive/send threads
  * Retry connecting to peer for up to 1 minute
* **Structure:**

  ```
  include/*.h
  core/*.c
  main.c
  Makefile
  ```
* **Compile:**

  ```bash
  make
  ./out/chat <LOCAL_PORT> <PEER_ONION> <PEER_PORT>
  ```
* **Example:**

  ```bash
  ./out/chat 12345 address.onion 12345
  ```

---

## RU — Russian

Минимальный, без "мусора", **peer-to-peer чат** через Tor с использованием SOCKS5.
Поддерживает **полноценное P2P** соединение между `.onion` адресами и может обрабатывать **входящие и исходящие подключения одновременно**.

* **Язык:** C (100% без лишних зависимостей)
* **Особенности:**

  * Подключение к удалённому пиру через Tor SOCKS5
  * Приём входящих соединений от других пиров
  * Двусторонняя отправка и приём сообщений через потоки
  * Повторная попытка подключения к пиру до 1 минуты
* **Структура проекта:**

  ```
  include/*.h
  core/*.c
  main.c
  Makefile
  ```
* **Компиляция:**

  ```bash
  make
  ./out/chat <LOCAL_PORT> <PEER_ONION> <PEER_PORT>
  ```
* **Пример:**

  ```bash
  ./out/chat 12345 address.onion 12345
  ```

---

## CH — 中文

一个极简、无冗余的 **P2P 聊天程序**，通过 Tor 使用 SOCKS5。
支持 `.onion` 地址之间的 **完整 P2P 通信**，可同时处理 **入站和出站连接**。

* **语言:** C (100% 精简)
* **功能:**

  * 通过 Tor SOCKS5 连接远程节点
  * 接收其他节点的入站连接
  * 发送/接收线程实现全双工消息传输
  * 尝试连接节点最长 1 分钟
* **项目结构:**

  ```
  include/*.h
  core/*.c
  main.c
  Makefile
  ```
* **编译:**

  ```bash
  make
  ./out/chat <LOCAL_PORT> <PEER_ONION> <PEER_PORT>
  ```
* **示例:**

  ```bash
  ./out/chat 12345 address.onion 12345
  ```