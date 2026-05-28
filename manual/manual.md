# Manuale VR

## Utilizzo, esempi e test

Nella cartella `test/` si trovano dei programmi di test per la libreria mentre nella cartella `example/` si trovano degli esempi di uso. Per compilare sia test che esempi in Windows è sufficiente chiamare lo script `build.bat` presente nella root del progetto, analogamente in Linux è sufficiente chiamare il corrispettivo `build.sh`.

**Nota**: Dato che gli script di compilazione generali si basano sugli script dei test e degli esempi, su linux potrebbe essere necessario rendere eseguibili tutti questi script prima di chiamarli.

Il codice è stato testato su [Windows 10/11](https://it.wikipedia.org/wiki/Microsoft_Windows), [Linux Mint DE 7](https://it.wikipedia.org/wiki/Linux_Mint_Debian_Edition) e [Fedora Linux 43](https://en.wikipedia.org/wiki/Fedora_Linux) nativi e su [Ubuntu 24.04](https://it.wikipedia.org/wiki/Ubuntu) tramite WSL2. Al momento la libreria non supporta MacOS e potrebbe dare problemi su sistemi in cui non è mai stata testata.

# VR platform memory

Modulo che permette di lavorare con il sistema operativo per ottenere e liberare memoria.

## Tabella dei contenuti

1. Procedure:
	- [vr_memory_page_size](#vr_memory_page_size)
	- [vr_memory_reserve](#vr_memory_reserve)
	- [vr_memory_release](#vr_memory_release)

### Procedure

#### `vr_memory_page_size`

Restituisce la dimensione (in byte) delle pagine usate dal sistema operativo.

```c
VrSint vr_memory_page_size(void)
```

#### `vr_memory_reserve`

Riserva un'arena di memoria delle dimensioni richieste. La quantità riservata è sempre arrotondata per eccesso alla grandezza di una pagina.

```c
VrArenaAlloc vr_memory_reserve(VrSint elem_count, VrSint elem_size)
```

Parametri:

- `VrSint elem_count` - La quantità di elementi da riservare, **deve essere positiva**.
- `VrSint elem_size` - La dimensione (in byte) degli elementi da riservare, **deve essere positiva**.

Risultato:

- Un'arena di memoria delle dimensioni richieste se l'operazione ha successo, un'arena di memoria vuota altrimenti.

#### `vr_memory_release`

Rilascia un'arena di memoria precedentemente riservata.

```c
void vr_memory_release(VrArenaAlloc* arena)
```

Parametri:

- `VrArenaAlloc* arena` - L'arena da rilasciare, **non può essere NULL**.

# VR platform socket

Modulo che permette di lavorare con il sistema operativo per comunicare tramite socket TCP e UDP.

## Tabella dei contenuti

1. [Address IP](#address-ip)
    - Tipi:
        - [VrAddressIp](#vraddressip)
        - [VrAddressIpKind](#vraddressipkind)
        - [VrAddressIpV4](#vraddressipv4)
        - [VrAddressIpV6](#vraddressipv6)
    - Procedure:
        - [vr_address_ip_none](#vr_address_ip_none)
        - [vr_address_ip_empty](#vr_address_ip_empty)
        - [vr_address_ip_local](#vr_address_ip_local)
        - [vr_address_ipv4](#vr_address_ipv4)
        - [vr_address_ipv6](#vr_address_ipv6)
        - [vr_address_ip_is_equal](#vr_address_ip_is_equal)
2. [Socket TCP](#socket-tcp)
    - Tipi:
        - [VrTcpListener](#vrtcplistener)
        - [VrTcpSocket](#vrtcpsocket)
    - Procedure:
        - [vr_tcp_listener_reserve](#vr_tcp_listener_reserve)
        - [vr_tcp_listener_init](#vr_tcp_listener_init)
        - [vr_tcp_listener_uninit](#vr_tcp_listener_uninit)
        - [vr_tcp_listener_bind_and_listen](#vr_tcp_listener_bind_and_listen)
        - [vr_tcp_socket_reserve](#vr_tcp_socket_reserve)
        - [vr_tcp_socket_init](#vr_tcp_socket_init)
        - [vr_tcp_socket_uninit](#vr_tcp_socket_uninit)
        - [vr_tcp_socket_bind](#vr_tcp_socket_bind)
        - [vr_tcp_socket_accept](#vr_tcp_socket_accept)
        - [vr_tcp_socket_connect](#vr_tcp_socket_connect)
        - [vr_tcp_socket_write](#vr_tcp_socket_write)
        - [vr_tcp_socket_write_all](#vr_tcp_socket_write_all)
        - [vr_tcp_socket_read](#vr_tcp_socket_read)
3. [Socket UDP](#socket-udp)
    - Tipi:
        - [VrUdpSocket](#vrudpsocket)
    - Procedure:
        - [vr_udp_socket_reserve](#vr_udp_socket_reserve)
        - [vr_udp_socket_init](#vr_udp_socket_init)
        - [vr_udp_socket_uninit](#vr_udp_socket_uninit)
        - [vr_udp_socket_bind](#vr_udp_socket_bind)
        - [vr_udp_socket_write](#vr_udp_socket_write)
        - [vr_udp_socket_write_all](#vr_udp_socket_write_all)
        - [vr_udp_socket_read](#vr_udp_socket_read)

## Address IP

## Tipi

#### `VrAddressIp`

Classe per lavorare con indirizzi IP. Un indirizzo è costituito da un indirizzo IPv4 o IPv6 e da una porta.

```c
struct VrAddressIp
{
    VrAddressIpKind kind;

    union
    {
        VrAddressIpV4 v4;
        VrAddressIpV6 v6;
    }
    ip;

    VrUint16 port;
};
```

Membri:

- [`VrAddressIpKind kind`](#vraddressipkind) - Il tipo di indirizzo IP.
- [`VrAddressIpV4 ip.v4`](#vraddressipv4) - L'indirizzo IPv4, è sempre disposto in **big endian**.
- [`VrAddressIpV6 ip.v6`](#vraddressipv6) - L'indirizzo IPv6, è sempre disposto in **big endian**.
- `VrUint16 port` - La porta, è sempre disposta come **l'endian nativo della macchina**.

#### `VrAddressIpKind`

Elenca i vari tipi di indirizzi IP che un indirizzo IP può rappresentare.

```c
enum VrAddressIpKind
{
    VrAddressIpKind_None,
    VrAddressIpKind_V4,
    VrAddressIpKind_V6,
};
```

Membri:

- `VrAddressIpKind_None` - Indirizzo IP invalido.
- `VrAddressIpKind_V4` - Indirizzo IPv4.
- `VrAddressIpKind_V6` - Indirizzo IPv6.

#### `VrAddressIpV4`

Rappresenta un indirizzo IPv4. Gli elementi sono sempre disposti in **big endian**.

```c
struct VrAddressIpV4
{
    union
    {
        VrUint8[4] array;

        struct
        {
            VrUint8 elem_0, elem_1, elem_2, elem_3;
        }
        members;
    }
    elements;
};
```

#### `VrAddressIpV6`

Rappresenta un indirizzo IPv6. Gli elementi sono sempre disposti in **big endian**.

```c
struct VrAddressIpV6
{
    union
    {
        VrUint8[16] array;

        struct
        {
            VrUint8 elem_0,  elem_1,  elem_2,  elem_3;
            VrUint8 elem_4,  elem_5,  elem_6,  elem_7;
            VrUint8 elem_8,  elem_9,  elem_10, elem_11;
            VrUint8 elem_12, elem_13, elem_14, elem_15;
        }
        members;
    }
    elements;
};
```

### Procedure

#### `vr_address_ip_none`

Produce un indirizzo IP invalido.

```c
VrAddressIp vr_address_ip_none(void)
```

#### `vr_address_ip_empty`

Produce un indirizzo IP vuoto a partire da un tipo. Se il tipo specificato è [`VrAddressIpKind_None`](#vraddressipkind) diventa equivalente a [`vr_address_ip_none`](#vr_address_ip_none).

```c
VrAddressIp vr_address_ip_empty(VrAddressIpKind kind)

#define vr_address_ipv4_empty() \
    vr_address_ip_empty(VrAddressIpKind_V4)

#define vr_address_ipv6_empty() \
    vr_address_ip_empty(VrAddressIpKind_V6)
```

#### `vr_address_ip_local`

Produce un indirizzo IP localhost a partire da un tipo e una porta. Se il tipo specificato è [`VrAddressIpKind_None`](#vraddressipkind) diventa equivalente a [`vr_address_ip_none`](#vr_address_ip_none).

```c
VrAddressIp vr_address_ip_local(VrAddressIpKind kind, VrUint16 port)

#define vr_address_ipv4_local(port) \
    vr_address_ip_local(VrAddressIpKind_V4, port)

#define vr_address_ipv6_local(port) \
    vr_address_ip_local(VrAddressIpKind_V6, port)
```

#### `vr_address_ipv4`

Produce un indirizzo IPv4 a partire da un indirizzo e una porta.

```c
VrAddressIp vr_address_ipv4(VrAddressIpV4 ipv4, VrUint16 port)
```

#### `vr_address_ipv6`

Produce un indirizzo IPv6 a partire da un indirizzo e una porta.

```c
VrAddressIp vr_address_ipv6(VrAddressIpV6 ipv6, VrUint16 port)
```

#### `vr_address_ip_is_equal`

Determina se due indirizzi IP contengono lo stesso indirizzo e la stessa porta.

```c
VrBool32 vr_address_ip_is_equal(VrAddressIp self, VrAddressIp other)
```

Risultato:

- Zero se i due indirizzi sono diversi, un valore diverso da zero altrimenti.

## Socket TCP

## Tipi

#### `VrTcpListener`

Classe per lavorare con listener TCP.

```c
struct VrTcpListener { void* impl; };
```

Per utilizzare questa classe è necessario inizializzare un listener, svolgere la [`bind_and_listen`](#vr_tcp_listener_bind_and_listen) e successivamente riservare un socket per le connessioni che si intende accettare. Ad esempio:

```c
VrArenaAlloc arena = /* ... */;

VrTcpListener listener = vr_tcp_listener_reserve((VrAlloc*) &arena);
VrTcpSocket   socket   = vr_tcp_socket_reserve((VrAlloc*) &arena);

// localhost:50000
vr_tcp_listener_init(listener, vr_address_ipv4_local(50000));
vr_tcp_listener_bind_and_listen(listener);

vr_tcp_socket_accept(socket, listener);

VrUint8 msg_buffer[256] = {0};
VrSint  msg_size        = (VrSint) sizeof msg_buffer;
VrSint  msg_count       = 0;

msg_count = vr_tcp_socket_read(socket, msg_buffer, msg_size);

vr_tcp_socket_write_all(socket, msg_buffer, msg_count);
```

#### `VrTcpSocket`

Classe per lavorare con socket TCP.

```c
struct VrTcpSocket { void* impl; };
```

Per utilizzare questa classe lato client è sufficiente inizializare un socket e successivamente connettersi con un certo indirizzo IP. Ad esempio:

```c
VrArenaAlloc arena = /* ... */;

VrTcpSocket socket = vr_tcp_socket_reserve((VrAlloc*) &arena);

vr_tcp_socket_init(socket, vr_address_ipv4_empty());
vr_tcp_socket_connect(socket, vr_address_ipv4_local(50000));

VrUint8 msg_buffer[256] = {0};
VrSint  msg_size        = (VrSint) sizeof msg_buffer;
VrSint  msg_count       = 0;

msg_count = snprintf((VrChar8*) msg_buffer,
    msg_size - 1, "%s", "Ciao!");

vr_tcp_socket_write_all(socket, msg_buffer, msg_count);

msg_count = vr_tcp_socket_read(socket, msg_buffer, msg_size);
```

**Nota**: È sempre possibile eseguire la [`bind`](#vr_tcp_socket_bind) di un socket ad una porta libera, ma è consigliato lasciare che sia il sistema operativo a selezionarne una libera in automatico.

### Procedure

#### `vr_tcp_listener_reserve`

Riserva la memoria per un listener TCP.

```c
VrTcpListener vr_tcp_listener_reserve(VrAlloc* alloc)
```

Restituisce:

- Un listener invalido se l'operazione fallisce, uno valido altrimenti.

#### `vr_tcp_listener_init`

Inizializza un listener a partire da un indirizzo IP.

```c
VrBool32 vr_tcp_listener_init(VrTcpListener self, VrAddressIp addr)
```

Parametri:

- [`VrTcpListener self`](#vrtcplistener) - Il listener da inizializzare.
- [`VrAddressIp addr`](#vraddressip) - L'indirizzo IP, **non può essere [`vr_address_ip_none`](#vr_address_ip_none)** e la porta **non può essere zero**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

#### `vr_tcp_listener_uninit`

Distrugge un listener. Questa operazione chiude la connessione e dall'altro lato nessun socket riuscirà più a scrivere o leggere.

```c
void vr_tcp_listener_uninit(VrTcpListener self)
```

#### `vr_tcp_listener_bind_and_listen`

Lega un listener alla porta con cui è stato inizializzato e comincia ad ascoltare connessioni in arrivo su quella porta.

```c
VrBool32 vr_tcp_listener_bind_and_listen(VrTcpListener self)
```

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

#### `vr_tcp_socket_reserve`

Riserva la memoria per un socket TCP.

```c
VrTcpSocket vr_tcp_socket_reserve(VrAlloc* alloc)
```

Restituisce:

- Un socket invalido se l'operazione fallisce, uno valido altrimenti.

#### `vr_tcp_socket_init`

Inizializza un socket a partire da un indirizzo IP.

```c
VrBool32 vr_tcp_socket_init(VrTcpSocket self, VrAddressIp addr)
```

Parametri:

- [`VrTcpSocket self`](#vrtcpsocket) - Il socket da inizializzare.
- [`VrAddressIp addr`](#vraddressip) - L'indirizzo IP, **non può essere [`vr_address_ip_none`](#vr_address_ip_none)** e la porta **non può essere zero**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

#### `vr_tcp_socket_uninit`

Distrugge un socket. Questa operazione chiude la connessione e dall'altro lato nessun socket riuscirà più a scrivere o leggere.

```c
void vr_tcp_socket_uninit(VrTcpSocket self)
```

#### `vr_tcp_socket_bind`

Lega un socket alla porta con cui è stato inizializzato.

```c
VrBool32 vr_tcp_socket_bind(VrTcpSocket self)
```

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

#### `vr_tcp_socket_accept`

Accetta una nuova connessione su un socket a partire da un listener.

```c
VrBool32 vr_tcp_socket_accept(VrTcpSocket self, VrTcpListener listener)
```

Parametri:

- [`VrTcpSocket self`](#vrtcpsocket) - Il socket su cui accettare la nuova connessione, **deve essere riservato ma non inizializzato**.
- [`VrTcpListener listener`](#vrtcplistener) - Il listener in ascolto.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

#### `vr_tcp_socket_connect`

Comincia una nuova connessione su un socket verso un altro host a partire da un indirizzo IP.

```c
VrBool32 vr_tcp_socket_connect(VrTcpSocket self, VrAddressIp addr)
```

Parametri:

- [`VrTcpSocket self`](#vrtcpsocket) - Il socket con cui iniziare la nuova connessione.
- [`VrAddressIp addr`](#vraddressip) - L'indirizzo a cui connettersi.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

#### `vr_tcp_socket_write`

Scrive su un socket il contenuto di un blocco di memoria.

L'operazione potrebbe non inviare tutto il blocco, in generale è consigliato controllare quanti byte sono stati effettivamente inviati e scrivere i rimanenti con altre chiamate. L'operazione inoltre è bloccante, cioè attende che sia possibile scrivere almeno un byte prima di ritornare al chiamante.

**Nota**: Se il socket è in uno stato valido e i parametri sono corretti ma l'operazione comunque termina con zero byte scritti, può significare che la connessione dall'altro lato è stata interrotta.

```c
VrSint vr_tcp_socket_write(VrTcpSocket self, VrUint8* pntr, VrSint size)
```

Parametri:

- [`VrTcpSocket self`](#vrtcpsocket) - Il socket su cui scrivere.
- `VrUint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `VrSint size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte scritti compreso in `[1, size]` altrimenti.

#### `vr_tcp_socket_write_all`

Scrive su un socket il contenuto di un blocco di memoria.

L'operazione è bloccante, cioè attende che sia possibile scrivere almeno un byte prima di ritornare al chiamante.

**Nota**: Se il socket è in uno stato valido e i parametri sono corretti ma l'operazione comunque termina con zero byte scritti, può significare che la connessione dall'altro lato è stata interrotta.

```c
VrSint vr_tcp_socket_write_all(VrTcpSocket self, VrUint8* pntr, VrSint size)
```

Parametri:

- [`VrTcpSocket self`](#vrtcpsocket) - Il socket su cui scrivere.
- `VrUint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `VrSint size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte scritti compreso in `[1, size]` altrimenti.

#### `vr_tcp_socket_read`

Legge da un socket e copia il contenuto in un blocco di memoria.

L'operazione è bloccante, cioè attende che sia possibile leggere almeno un byte prima di ritornare al chiamante.

**Nota**: Se il socket è in uno stato valido e i parametri sono corretti ma l'operazione comunque termina con zero byte letti, può significare che la connessione dall'altro lato è stata interrotta.

```c
VrSint vr_tcp_socket_read(VrTcpSocket self, VrUint8* pntr, VrSint size)
```

Parametri:

- [`VrTcpSocket self`](#vrtcpsocket) - Il socket da cui leggere.
- `VrUint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `VrSint size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte letti compreso in `[1, size]` altrimenti.

## Socket UDP

### Tipi

#### `VrUdpSocket`

Classe per lavorare con socket UDP.

```c
struct VrUdpSocket { void* impl; };
```

Per utilizzare questa classe lato server è necessario inizializzare un socket e successivamente eseguire la [`bind`](#vr_udp_socket_bind). Ad esempio:

```c
VrArenaAlloc arena = /* ... */;

VrUdpSocket socket = vr_udp_socket_reserve((VrAlloc*) &arena);

vr_udp_socket_init(socket, vr_address_ipv4_local(50000));
vr_udp_socket_bind(socket);

VrUint8     msg_buffer[256] = {0};
VrSint      msg_size        = (VrSint) sizeof msg_buffer;
VrSint      msg_count       = 0;
VrAddressIp msg_addr        = {0};

msg_count = vr_udp_socket_read(socket, msg_buffer, msg_size, &msg_addr);

vr_udp_socket_write_all(socket, msg_buffer, msg_count, msg_addr);
```

Per utilizzare questa classe lato client è sufficiente inizializzare un socket. Ad esempio:

```c
VrUdpSocket socket      = vr_udp_socket_reserve(alloc);
VrAddressIp server_addr = vr_address_ipv4_local(50000);

vr_udp_socket_init(socket, vr_addres_ipv4_empty());

VrUint8     msg_buffer[256] = {0};
VrSint      msg_size        = (VrSint) sizeof msg_buffer;
VrSint      msg_count       = 0;
VrAddressIp msg_addr        = {0};

msg_count = snprintf((VrChar8*) msg_buffer,
    msg_size - 1, "%s", "Ciao!");

vr_udp_socket_write_all(socket, msg_buffer, msg_count, server_addr);

msg_count = vr_udp_socket_read(socket, msg_buffer, msg_size, &msg_addr);
```

**Nota**: È sempre possibile eseguire la [`bind`](#vr_udp_socket_bind) di un socket ad una porta libera, ma nel caso di un client è consigliato lasciare che sia il sistema operativo a selezionarne una libera in automatico.

### Procedure

#### `vr_udp_socket_reserve`

Riserva la memoria per un socket UDP.

```c
VrUdpSocket vr_udp_socket_reserve(VrAlloc* alloc)
```

Restituisce:

- Un socket invalido se l'operazione fallisce, uno valido altrimenti.

#### `vr_udp_socket_init`

Inizializza un socket a partire da un indirizzo IP.

```c
VrBool32 vr_udp_socket_init(VrUdpSocket self, VrAddressIp addr)
```

Parametri:

- [`VrUdpSocket self`](#vrudpsocket) - Il socket da inizializzare.
- [`VrAddressIp addr`](#vraddressip) - L'indirizzo IP, **non può essere [`vr_address_ip_none`](#vr_address_ip_none)** e la porta **non può essere zero**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

#### `vr_udp_socket_uninit`

Distrugge un socket.

```c
void vr_udp_socket_uninit(VrUdpSocket self)
```

#### `vr_udp_socket_bind`

Lega un socket alla porta con cui è stato inizializzato.

```c
VrBool32 vr_udp_socket_bind(VrUdpSocket self)
```

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

#### `vr_udp_socket_write`

Scrive su un socket il contenuto di un blocco di memoria e lo invia ad un certo indirizzo.

L'operazione potrebbe non inviare tutto il blocco, in generale è consigliato controllare quanti byte sono stati effettivamente inviati e scrivere i rimanenti con altre chiamate. L'operazione inoltre è bloccante, cioè attende che sia possibile scrivere almeno un byte prima di tornare al chiamante.

**Nota**: L'operazione può terminare con successo ma non è garantito che dall'altro lato il contenuto venga ricevuto. Nel caso in cui vengano svolte più chiamate non è nemmeno garantito l'ordine di arrivo delle varie parti.

```c
VrSint vr_udp_socket_write(VrTcpSocket self, VrUint8* pntr, VrSint size, VrAddressIp addr)
```

Parametri:

- [`VrUdpSocket self`](#vrudpsocket) - Il socket su cui scrivere.
- `VrUint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `VrSint size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.
- [`VrAddressIp addr`](#vraddressip) - L'indirizzo del destinatario.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte scritti compreso in `[1, size]` altrimenti.

#### `vr_udp_socket_write_all`

Scrive su un socket il contenuto di un blocco di memoria e lo invia ad un certo indirizzo.

L'operazione è bloccante, cioè attende che sia possibile scrivere almeno un byte prima di tornare al chiamante.

**Nota**: L'operazione può terminare con successo ma non è garantito che dall'altro lato il contenuto venga ricevuto. Nel caso in cui vengano svolte più chiamate non è nemmeno garantito l'ordine di arrivo delle varie parti.

```c
VrSint vr_udp_socket_write_all(VrTcpSocket self, VrUint8* pntr, VrSint size, VrAddressIp addr)
```

Parametri:

- [`VrUdpSocket self`](#vrudpsocket) - Il socket su cui scrivere.
- `VrUint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `VrSint size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.
- [`VrAddressIp addr`](#vraddressip) - L'indirizzo del destinatario.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte scritti compreso in `[1, size]` altrimenti.

#### `vr_udp_socket_read`

Legge da un socket e copia il contenuto in un blocco di memoria ricevuto da un certo indirizzo.

L'operazione è bloccante, cioè attende che sia possibile leggere almeno un byte prima di ritornare al chiamante.

**Nota**: L'operazione può terminare con successo ma non è garantito che il contenuto sia quello inviato originariamente dall'altro lato. In più se dall'altro lato viene interrotta la comunicazione l'operazione potrebbe attendere all'infinito.

```c
VrSint vr_udp_socket_read(VrUdpSocket self, VrUint8* pntr, VrSint size, VrAddressIp* addr)
```

Parametri:

- [`VrUdpSocket self`](#vrudpsocket) - Il socket da cui leggere.
- `VrUint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `VrSint size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.
- [`VrAddressIp* addr`](#vraddressip) - Endpoint del mittente, può essere NULL.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte letti compreso in `[1, size]` altrimenti.
