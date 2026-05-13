# Manuale VR

## Utilizzo, esempi e test

Nella cartella `test/` si trovano dei programmi di test per la libreria mentre nella cartella `example/` si trovano degli esempi di uso. Per compilare tutto in Windows è sufficiente chiamare lo script `build.bat` nella root del progetto, per compilare in Linux il processo è simile con `build.sh` ma potrebbe essere necessario rendere eseguibili tutti gli script prima di chiamarli.

Il codice è stato testato su [Windows 10/11](https://it.wikipedia.org/wiki/Microsoft_Windows), [Linux Mint DE 7](https://it.wikipedia.org/wiki/Linux_Mint_Debian_Edition) e [Fedora Linux 43](https://en.wikipedia.org/wiki/Fedora_Linux) nativi e su [Ubuntu 24.04](https://it.wikipedia.org/wiki/Ubuntu) tramite WSL2. Al momento la libreria non supporta MacOS e potrebbe dare problemi su sistemi in cui non è mai stata testata.

# VR system memory

Modulo che permette di lavorare con il sistema operativo per ottenere e liberare memoria.

## Tabella dei contenuti

1. [Funzioni libere](#funzioni-libere)
	- [vr_memory_page_size](#vr_memory_page_size)
	- [vr_memory_reserve](#vr_memory_reserve)
	- [vr_memory_release](#vr_memory_release)

## Funzioni libere

### vr_memory_page_size

Restituisce la dimensione (in byte) delle pagine usate dal sistema operativo.

```c
intptr vr_memory_page_size()
```

### vr_memory_reserve

Riserva un'arena di memoria delle dimensioni richieste. La quantità riservata è sempre arrotondata per eccesso alla grandezza di una pagina.

```c
VR_ArenaAlloc vr_memory_reserve(intptr elem_count, intptr elem_size)
```

Parametri:

- `intptr elem_count` - La quantità di elementi da riservare, **deve essere positiva**.
- `intptr elem_size` - La dimensione (in byte) degli elementi da riservare, **deve essere positiva**.

Risultato:

- Un'arena di memoria delle dimensioni richieste se l'operazione ha successo, un'arena di memoria vuota altrimenti.

### vr_memory_release

Rilascia un'arena di memoria precedentemente riservata.

```c
void vr_memory_release(VR_ArenaAlloc* arena)
```

Parametri:

- `VR_ArenaAlloc* arena` - L'arena da rilasciare, **non può essere NULL**.

# VR system socket

Modulo che permette di lavorare con il sistema operativo per comunicare tramite socket TCP e UDP.

## Tabella dei contenuti

1. [Network IP Address](#network-ip-address)
    - [VR_NetworkIpAddr_Kind](#vr_networkipaddr_kind)
    - [VR_NetworkIpAddr_Ver4](#vr_networkipaddr_ver4)
    - [VR_NetworkIpAddr_Ver6](#vr_networkipaddr_ver6)
    - [vr_network_ip_addr_none](#vr_network_ip_addr_none)
    - [vr_network_ip_addr_empty](#vr_network_ip_addr_empty)
    - [vr_network_ip_addr_local](#vr_network_ip_addr_local)
    - [vr_network_ip_addr_ver4](#vr_network_ip_addr_ver4)
    - [vr_network_ip_addr_ver6](#vr_network_ip_addr_ver6)
    - [vr_network_ip_addr_is_equal](#vr_network_ip_addr_is_equal)
2. [Socket TCP](#socket_tcp)
    - [vr_socket_tcp_reserve](#vr_socket_tcp_reserve)
    - [vr_socket_tcp_init](#vr_socket_tcp_init)
    - [vr_socket_tcp_init_bound](#vr_socket_tcp_init_bound)
    - [vr_socket_tcp_uninit](#vr_socket_tcp_uninit)
    - [vr_socket_tcp_listen](#vr_socket_tcp_listen)
    - [vr_socket_tcp_accept](#vr_socket_tcp_accept)
    - [vr_socket_tcp_connect](#vr_socket_tcp_connect)
    - [vr_socket_tcp_write](#vr_socket_tcp_write)
    - [vr_socket_tcp_write](#vr_socket_tcp_write_all)
    - [vr_socket_tcp_read](#vr_socket_tcp_read)
3. [Socket UDP](#socket-udp)
    - [vr_socket_udp_reserve](#vr_socket_udp_reserve)
    - [vr_socket_udp_init](#vr_socket_udp_init)
    - [vr_socket_udp_init_bound](#vr_socket_udp_init_bound)
    - [vr_socket_udp_uninit](#vr_socket_udp_uninit)
    - [vr_socket_udp_write](#vr_socket_udp_write)
    - [vr_socket_udp_write](#vr_socket_udp_write_all)
    - [vr_socket_udp_read](#vr_socket_udp_read)

## Network IP Address

Classe per lavorare con indirizzi IP. Un indirizzo è costituito da un indirizzo IPv4 o IPv6 e da una porta.

```c
struct VR_NetworkIpAddr
{
    VR_NetworkIpAddr_Kind kind;

    union {
        VR_NetworkIpAddr_Ver4 ip_ver4;
        VR_NetworkIpAddr_Ver6 ip_ver6;
    };

    uint16 port;
};
```

Membri:

- `VR_NetworkIpAddr_Kind kind` - Il ipo di indirizzo IP.
- `VR_NetworkIpAddr_Ver4 ip_ver4` - L'indirizzo IPv4, è sempre disposto in **big endian**.
- `VR_NetworkIpAddr_Ver6 ip_ver6` - L'indirizzo IPv6, è sempre disposto in **big endian**.
- `uint16 port` - La porta, è sempre disposta come **l'endian nativo della macchina**.

### VR_NetworkIpAddr_Kind

Elenca i vari tipi di indirizzi IP che un indirizzo IP può rappresentare.

```c
enum VR_NetworkIpAddr_Kind
{
    VR_NetworkIpAddr_Kind_None,
    VR_NetworkIpAddr_Kind_Ver4,
    VR_NetworkIpAddr_Kind_Ver6,
};
```

Membri:

- `VR_NetworkIpAddr_Kind_None` - Indirizzo IP invalido.
- `VR_NetworkIpAddr_Kind_Ver4` - Indirizzo IPv4.
- `VR_NetworkIpAddr_Kind_Ver6` - Indirizzo IPv6.

### VR_NetworkIpAddr_Ver4

Rappresenta un indirizzo IPv4. Gli elementi sono sempre disposti in **big endian**.

```c
union VR_NetworkIpAddr_Ver4
{
    uint8[4] elements;

    struct {
        uint8 elem_0, elem_1, elem_2, elem_3;
    };
};
```

### VR_NetworkIpAddr_Ver6

Rappresenta un indirizzo IPv6. Gli elementi sono sempre disposti in **big endian**.

```c
union VR_NetworkIpAddr_Ver6
{
    uint8[16] elements;

    struct {
        uint8 elem_0,  elem_1,  elem_2,  elem_3;
        uint8 elem_4,  elem_5,  elem_6,  elem_7;
        uint8 elem_8,  elem_9,  elem_10, elem_11;
        uint8 elem_12, elem_13, elem_14, elem_15;
    };
};
```

### vr_network_ip_addr_none

Produce un indirizzo IP invalido.

```c
VR_NetworkIpAddr vr_network_ip_addr_none()
```

### vr_network_ip_addr_empty

Produce un indirizzo IP vuoto a partire da un tipo. Se il tipo specificato è `VR_NetworkIpAddr_Kind_None` diventa equivalente a [vr_network_ip_addr_none()](#vr_network_ip_addr_none).

```c
VR_NetworkIpAddr vr_network_ip_addr_empty(VR_NetworkIpAddr_Kind kind)
```

### vr_network_ip_addr_local

Produce un indirizzo IP localhost a partire da un tipo e una porta. Se il tipo specificato è `VR_NetworkIpAddr_Kind_None` diventa equivalente a [vr_network_ip_addr_none()](#vr_network_ip_addr_none).

```c
VR_NetworkIpAddr vr_network_ip_addr_local(VR_NetworkIpAddr_Kind kind, uint16 port)
```

### vr_network_ip_addr_ver4

Produce un indirizzo IPv4 a partire da un indirizzo e una porta.

```c
VR_NetworkIpAddr vr_network_ip_addr_ver4(VR_NetworkIpAddr_Ver4 ipv4, uint16 port)
```

### vr_network_ip_addr_ver6

Produce un indirizzo IPv6 a partire da un indirizzo e una porta.

```c
VR_NetworkIpAddr vr_network_ip_addr_ver6(VR_NetworkIpAddr_Ver6 ipv6, uint16 port)
```

### vr_network_ip_addr_is_equal

Determina se due indirizzi IP contengono lo stesso indirizzo e la stessa porta.

```c
bool32 vr_network_ip_addr_is_equal(VR_NetworkIpAddr self, VR_NetworkIpAddr other)
```

Risultato:

- Zero se i due indirizzi sono diversi, un valore diverso da zero altrimenti.

## Socket TCP

Classe per lavorare con socket TCP.

Per utilizzare questa classe lato client è sufficiente preparare un socket e connettersi ad un certo indirizzo IP. Ad esempio:

```c
VR_SocketTcp socket = vr_socket_tcp_reserve(&arena);

 // localhost:34137
VR_NetworkIpAddr server_addr = vr_network_ip_addr_ver4(
    VR_NETWORK_IP_ADDR_VER4_LOCAL, 34137);

vr_socket_tcp_init(socket, server_addr.kind);
vr_socket_tcp_connect(socket, server_addr);

uint8 buffer[256] = {0};

intptr count = snprintf((char8*) buffer, sizeof buffer - 1, "%s", "Ciao!");

vr_socket_tcp_write(socket, buffer, count);

count = vr_socket_tcp_read(socket, buffer, sizeof buffer);
```

Per utilizzare questa classe lato server invece è necessario preparare un socket **bound** che deve essere proposso a **listener** e successivamente riservare un socket per le connessioni che si intende accettare. Ad esempio:

```c
VR_SocketTcp listener = vr_socket_tcp_reserve(&arena);

// localhost:34137
vr_socket_tcp_init_bound(listener, VR_NetworkIpAddr_Kind_Ver4, 34137);
vr_socket_tcp_listen(listener);

VR_SocketTcp socket = vr_socket_tcp_reserve(&arena);

uint8 buffer[256] = {0};

vr_socket_tcp_accept(socket, listener);

intptr count = vr_socket_tcp_read(socket, buffer, sizeof buffer);

vr_socket_tcp_write(socket, buffer, count);
```

**Nota**: È sempre possibile creare un socket bound a patto che la porta sia disponibile, nel caso di un server è fondamentale ma nel caso di un client è consigliato lasciare che sia il sistema operativo a selezionarne una libera in automatico.

```c
struct VR_SocketTcp { void* impl; };
```

### vr_socket_tcp_reserve

Riserva la memoria per un socket TCP.

```c
VR_SocketTcp vr_socket_tcp_reserve(VR_Alloc* alloc)
```

Restituisce:

- Un socket invalido se l'operazione fallisce, uno valido altrimenti.

### vr_socket_tcp_init

Inizializza un socket a partire da un tipo di indirizzo IP.

```c
bool32 vr_socket_tcp_init(VR_SocketTcp self, VR_NetworkIpAddr_Kind kind)
```

Parametri:

- `VR_SocketTcp self` - Il socket da inizializzare.
- `VR_NetworkIpAddr_Kind kind` - Il tipo di indirizzo IP, **non può essere `VR_NetworkIpAddr_Kind_None`**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_tcp_init_bound

Inizializza un socket **bound** a partire da un tipo di indirizzo IP e una porta.

```c
bool32 vr_socket_tcp_init_bound(VR_SocketTcp self, VR_NetworkIpAddr_Kind kind, uint16 port)
```

Parametri:

- `VR_SocketTcp self` - Il socket da inizializzare.
- `VR_NetworkIpAddr_Kind kind` - Il tipo di indirizzo IP, **non può essere `VR_NetworkIpAddr_Kind_None`**.
- `uint16 port` - La porta, **non può essere zero**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_tcp_uninit

Distrugge un socket. Questa operazione chiude la connessione e dall'altro capo nessun socket riuscirà più a scrivere o leggere.

```c
void vr_socket_tcp_uninit(VR_SocketTcp self)
```

### vr_socket_tcp_listen

Promuove un socket a listener.

```c
bool32 vr_socket_tcp_listen(VR_SocketTcp listener)
```

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_tcp_accept

Accetta una nuova connessione su un socket a partire da un socket listener.

```c
bool32 vr_socket_tcp_accept(VR_SocketTcp self, VR_SocketTcp listener)
```

Parametri:

- `VR_SocketTcp self` - Il socket su cui accettare la nuova connessione, **deve essere riservato ma non inizializzato**.
- `VR_SocketTcp listener` - Il socket listener.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_tcp_connect

Inizia una nuova connessione su un socket verso un altro host a partire da un indirizzo IP.

```c
bool32 vr_socket_tcp_connect(VR_SocketTcp self, VR_NetworkIpAddr addr)
```

Parametri:

- `VR_SocketTcp self` - Il socket con cui iniziare la nuova connessione.
- `VR_NetworkIpAddr addr` - L'addr a cui connettersi.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_tcp_write

Scrive su un socket **non listener** il contenuto di un blocco di memoria.

L'operazione potrebbe non inviare tutto il blocco, in generale è consigliato controllare quanti byte sono stati effettivamente inviati e scrivere i rimanenti con altre chiamate. L'operazione inoltre è bloccante, cioè attende che sia possibile scrivere almeno un byte prima di ritornare al chiamante.

**Nota**: Se il socket è in uno stato valido e i parametri sono corretti ma l'operazione comunque termina con zero byte scritti, può significare che la connessione dall'altro lato è stata interrotta.

```c
intptr vr_socket_tcp_write(VR_SocketTcp self, uint8* pntr, intptr size)
```

Parametri:

- `VR_SocketTcp self` - Il socket su cui scrivere.
- `uint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `intptr size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte scritti compreso in `[1, size]` altrimenti.

### vr_socket_tcp_write_all

Scrive su un socket **non listener** il contenuto di un blocco di memoria.

L'operazione è bloccante, cioè attende che sia possibile scrivere almeno un byte prima di ritornare al chiamante.

**Nota**: Se il socket è in uno stato valido e i parametri sono corretti ma l'operazione comunque termina con zero byte scritti, può significare che la connessione dall'altro lato è stata interrotta.

```c
intptr vr_socket_tcp_write_all(VR_SocketTcp self, uint8* pntr, intptr size)
```

Parametri:

- `VR_SocketTcp self` - Il socket su cui scrivere.
- `uint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `intptr size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte scritti compreso in `size` altrimenti.

### vr_socket_tcp_read

Legge da un socket **non listener** e copia il contenuto in un blocco di memoria.

L'operazione è bloccante, cioè attende che sia possibile leggere almeno un byte prima di ritornare al chiamante.

**Nota**: Se il socket è in uno stato valido e i parametri sono corretti ma l'operazione comunque termina con zero byte letti, può significare che la connessione dall'altro lato è stata interrotta.

```c
intptr vr_socket_tcp_read(VR_SocketTcp self, uint8* pntr, intptr size)
```

Parametri:

- `VR_SocketTcp self` - Il socket da cui leggere.
- `uint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `intptr size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte letti compreso in `[1, size]` altrimenti.

## Socket UDP

Classe per lavorare con socket UDP.

Per utilizzare questa classe lato client è sufficiente preparare un socket. Ad esempio:

```c
VR_SocketUdp socket = vr_socket_udp_reserve(alloc);

// localhost:34137
VR_NetworkIpAddr server_addr = vr_network_ip_addr_ver4(
    VR_NETWORK_IP_ADDR_VER4_LOCAL, 34137);

vr_socket_udp_init(socket, server_addr.kind);

uint8 buffer[256] = {0};

intptr count = snprintf((char8*) buffer, sizeof buffer - 1, "%s", "Ciao!");

vr_socket_udp_write(socket, buffer, count, addr);

count = vr_socket_udp_read(socket, buffer, sizeof buffer, &addr);
```

Per utilizzare questa classe lato server invece è necessario preparare un socket **bound**. Ad esempio:

```c
VR_SocketUdp socket = vr_socket_udp_reserve(alloc);

VR_NetworkIpAddr addr = {0};

vr_socket_udp_init_bound(listener, VR_NetworkIpAddr_Kind_Ver4, 34137);

uint8 buffer[256] = {0};

intptr count = vr_socket_udp_read(socket, buffer, sizeof buffer, &addr);

vr_socket_udp_write(socket, buffer, count, addr);
```

**Nota**: È sempre possibile creare un socket bound a patto che la porta sia disponibile, nel caso di un server è fondamentale ma nel caso di un client è consigliato lasciare che sia il sistema operativo a selezionarne una libera in automatico.

```c
struct VR_SocketUdp { void* impl; };
```

### vr_socket_udp_reserve

Riserva la memoria per un socket UDP.

```c
VR_SocketUdp vr_socket_udp_reserve(VR_Alloc* alloc)
```

Restituisce:

- Un socket invalido se l'operazione fallisce, uno valido altrimenti.

### vr_socket_upd_init

Inizializza un socket a partire da un tipo di indirizzo IP.

```c
bool32 vr_socket_udp_init(VR_SocketUdp self, VR_NetworkIpAddr_Kind kind)
```

Parametri:

- `VR_SocketUdp self` - Il socket da inizializzare.
- `VR_NetworkIpAddr_Kind kind` - Il tipo di indirizzo IP, **non può essere `VR_NetworkIpAddr_Kind_None`**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_upd_init_bound

Inizializza un socket **bound** a partire da un tipo di indirizzo IP e una porta.

```c
bool32 vr_socket_udp_init_bound(VR_SocketUdp self, VR_NetworkIpAddr_Kind kind, uint16 port)
```

Parametri:

- `VR_SocketUdp self` - Il socket da inizializzare.
- `VR_NetworkIpAddr_Kind kind` - Il tipo di indirizzo specificato, **non può essere `VR_NetworkIpAddr_Kind_None`**.
- `uint16 port` - La porta specificata, **non può essere zero**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_upd_uninit

Distrugge un socket.

```c
void vr_socket_udp_uninit(VR_SocketUdp self)
```

### vr_socket_udp_write

Scrive su un socket il contenuto di un blocco di memoria ad un certo addr.

L'operazione potrebbe non inviare tutto il blocco, in generale è consigliato controllare quanti byte sono stati effettivamente inviati e scrivere i rimanenti con altre chiamate. L'operazione inoltre è bloccante, cioè attende che sia possibile scrivere almeno un byte prima di tornare al chiamante.

**Nota**: L'operazione può terminare con successo ma non è garantito che dall'altro lato il contenuto venga ricevuto. Nel caso in cui vengano svolte più chiamate non è nemmeno garantito l'ordine di arrivo delle varie parti.

```c
intptr vr_socket_udp_write(VR_SocketTcp self, uint8* pntr, intptr size, VR_NetworkIpAddr addr)
```

Parametri:

- `VR_SocketUdp self` - Il socket su cui scrivere.
- `uint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `intptr size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.
- `VR_NetworkIpAddr addr` - Endpoint del destinatario.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte scritti compreso in `[1, size]` altrimenti.

### vr_socket_udp_write_all

Scrive su un socket il contenuto di un blocco di memoria ad un certo addr.

L'operazione è bloccante, cioè attende che sia possibile scrivere almeno un byte prima di tornare al chiamante.

**Nota**: L'operazione può terminare con successo ma non è garantito che dall'altro lato il contenuto venga ricevuto. Nel caso in cui vengano svolte più chiamate non è nemmeno garantito l'ordine di arrivo delle varie parti.

```c
intptr vr_socket_udp_write_all(VR_SocketTcp self, uint8* pntr, intptr size, VR_NetworkIpAddr addr)
```

Parametri:

- `VR_SocketUdp self` - Il socket su cui scrivere.
- `uint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `intptr size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.
- `VR_NetworkIpAddr addr` - Endpoint del destinatario.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte scritti compreso in `[1, size]` altrimenti.

### vr_socket_udp_read

Legge da un socket e copia il contenuto in un blocco di memoria.

L'operazione è bloccante, cioè attende che sia possibile leggere almeno un byte prima di ritornare al chiamante.

**Nota**: L'operazione può terminare con successo ma non è garantito che il contenuto sia quello inviato originariamente dall'altro lato. In più se dall'altro lato viene interrotta la comunicazione l'operazione potrebbe attendere all'infinito.

```c
intptr vr_socket_udp_read(VR_SocketUdp self, uint8* pntr, intptr size, VR_NetworkIpAddr* addr)
```

Parametri:

- `VR_SocketUdp self` - Il socket da cui leggere.
- `uint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `intptr size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.
- `VR_NetworkIpAddr* addr` - Endpoint del mittente, può essere NULL.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte letti compreso in `[1, size]` altrimenti.
