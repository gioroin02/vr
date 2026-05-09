# Manuale VR system socket

Modulo che permette di lavorare con il sistema operativo per comunicare tramite socket TCP e UDP.

## Tabella dei contenuti

1. [Endpoint IP](#endpoint-ip)
    - [VR_Endpoint_IP_Kind](#vr-endpoint_ip_kind)
    - [VR_Endpoint_IPv4](#vr_endpoint_ipv4)
    - [VR_Endpoint_IPv6](#vr_endpoint_ipv6)
    - [vr_endpoint_ip_none](#vr_endpoint_ip_none)
    - [vr_endpoint_ip_empty](#vr_endpoint_ip_empty)
    - [vr_endpoint_ip_local](#vr_endpoint_ip_local)
    - [vr_endpoint_ip_ver4](#vr_endpoint_ip_ver4)
    - [vr_endpoint_ip_ver6](#vr_endpoint_ip_ver6)
    - [vr_endpoint_ip_is_equal](#vr_endpoint_ip_is_equal)
2. [Socket TCP](#socket-tcp)
    - [vr_socket_tcp_reserve](#vr_socket_tcp_reserve)
    - [vr_socket_tcp_init](#vr_socket_tcp_init)
    - [vr_socket_tcp_init_bound](#vr_socket_tcp_init_bound)
    - [vr_socket_tcp_uninit](#vr_socket_tcp_uninit)
    - [vr_socket_tcp_listen](#vr_socket_tcp_listen)
    - [vr_socket_tcp_accept](#vr_socket_tcp_accept)
    - [vr_socket_tcp_connect](#vr_socket_tcp_connect)
    - [vr_socket_tcp_write](#vr_socket_tcp_write)
    - [vr_socket_tcp_read](#vr_socket_tcp_read)
3. [Socket UDP](#socket-udp)
    - [vr_socket_udp_reserve](#vr_socket_udp_reserve)
    - [vr_socket_udp_init](#vr_socket_udp_init)
    - [vr_socket_udp_init_bound](#vr_socket_udp_init_bound)
    - [vr_socket_udp_uninit](#vr_socket_udp_uninit)
    - [vr_socket_udp_write](#vr_socket_udp_write)
    - [vr_socket_udp_read](#vr_socket_udp_read)

## Endpoint IP

Classe per lavorare con endpoint IP. Un endpoint è costituito da un indirizzo IPv4 o IPv6 e da una porta.

```c
struct VR_Endpoint_IP
{
    VR_Endpoint_IP_Kind kind;

    union {
        VR_Endpoint_IPv4 ip_ver4;
        VR_Endpoint_IPv6 ip_ver6;
    };

    uint16 port;
};
```

Membri:

- `VR_Endpoint_IP_Kind kind` - Il ipo di indirizzo IP.
- `VR_Endpoint_IPv4 ip_ver4` - L'indirizzo IPv4, è sempre disposto in **big endian**.
- `VR_Endpoint_IPv6 ip_ver6` - L'indirizzo IPv6, è sempre disposto in **big endian**.
- `uint16 port` - La porta, è sempre disposta come **l'endian nativo della macchina**.

### VR_Endpoint_IP_Kind

Elenca i vari tipi di indirizzi IP che un endpoint può rappresentare.

```c
enum VR_Endpoint_IP_Kind
{
    VR_Endpoint_IP_Kind_None,
    VR_Endpoint_IP_Kind_V4,
    VR_Endpoint_IP_Kind_V6,
};
```

Membri:

- `VR_Endpoint_IP_Kind_None` - Indirizzo IP invalido.
- `VR_Endpoint_IP_Kind_V4` - Indirizzo IPv4.
- `VR_Endpoint_IP_Kind_V6` - Indirizzo IPv6.

### VR_Endpoint_IPv4

Rappresenta un indirizzo IPv4. Gli elementi sono sempre disposti in **big endian**.

```c
union VR_Endpoint_IPv4
{
    uint8[4] elements;

    struct {
        uint8 elem_0, elem_1, elem_2, elem_3;
    };
};
```

### VR_Endpoint_IPv6

Rappresenta un indirizzo IPv6. Gli elementi sono sempre disposti in **big endian**.

```c
union VR_Endpoint_IPv6
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

### vr_endpoint_ip_none

Produce un endpoint IP invalido.

```c
VR_Endpoint_IP vr_endpoint_ip_none()
```

### vr_endpoint_ip_empty

Produce un endpoint vuoto a partire da un tipo di indirizzo IP. Se il tipo specificato è `VR_Endpoint_IP_Kind_None` diventa equivalente a [vr_endpoint_ip_none()](#vr_endpoint_ip_none).

```c
VR_Endpoint_IP vr_endpoint_ip_empty(VR_Endpoint_IP_Kind kind)
```

### vr_endpoint_ip_local

Produce un endpoint localhost a partire da un tipo di indirizzo IP e una porta. Se il tipo specificato è `VR_Endpoint_IP_Kind_None` diventa equivalente a [vr_endpoint_ip_none()](#vr_endpoint_ip_none).

```c
VR_Endpoint_IP vr_endpoint_ip_local(VR_Endpoint_IP_Kind kind, uint16 port)
```

### vr_endpoint_ip_ver4

Produce un endpoint IPv4 a partire da un indirizzo e una porta.

```c
VR_Endpoint_IP vr_endpoint_ip_ver4(VR_Endpoint_IPv4 ipv4, uint16 port)
```

### vr_endpoint_ip_ver6

Produce un endpoint IPv6 a partire da un indirizzo e una porta.

```c
VR_Endpoint_IP vr_endpoint_ip_ver6(VR_Endpoint_IPv6 ipv6, uint16 port)
```

### vr_endpoint_ip_is_equal

Determina se due endpoint IP contengono lo stesso indirizzo e la stessa porta.

```c
bool32 vr_endpoint_ip_is_equal(VR_Endpoint_IP self, VR_Endpoint_IP other)
```

Risultato:

- Zero se i due endpoint sono diversi, un valore diverso da zero altrimenti.

## Socket TCP

Classe per lavorare con socket TCP.

Per utilizzare questa classe lato client è sufficiente preparare un socket e connettersi ad un certo endpoint IP. Ad esempio:

```c
VR_Socket_TCP socket = vr_socket_tcp_reserve(&arena);

 // localhost:34137
VR_Endpoint_IP endpoint = vr_endpoint_ip_ver4(VR_ENDPOINT_IPV4_LOCAL, 34137);

vr_socket_tcp_init(socket, endpoint.kind);
vr_socket_tcp_connect(socket, endpoint);

uint8 buffer[256] = {0};

intptr count = snprintf((char8*) buffer, sizeof buffer - 1, "%s", "Ciao!");

vr_socket_tcp_write(socket, buffer, count);

count = vr_socket_tcp_read(socket, buffer, sizeof buffer);
```

Per utilizzare questa classe lato server invece è necessario preparare un socket **bound** che deve essere proposso a **listener** e successivamente riservare un socket per le connessioni che si intende accettare. Ad esempio:

```c
VR_Socket_TCP listener = vr_socket_tcp_reserve(&arena);

// localhost:34137
vr_socket_tcp_init_bound(listener, VR_Endpoint_IP_Kind_4, 34137);
vr_socket_tcp_listen(listener);

VR_Socket_TCP socket = vr_socket_tcp_reserve(&arena);

uint8 buffer[256] = {0};

vr_socket_tcp_accept(socket, listener);

intptr count = vr_socket_tcp_read(socket, buffer, sizeof buffer);

vr_socket_tcp_write(socket, buffer, count);
```

Nota: È sempre possibile creare un socket bound a patto che la porta sia disponibile, nel caso di un server è fondamentale ma nel caso di un client è consigliato lasciare che sia il sistema operativo a selezionarne una libera in automatico.

```c
struct VR_Socket_TCP { void* impl; };
```

### vr_socket_tcp_reserve

Riserva la memoria per un socket TCP.

```c
VR_Socket_TCP vr_socket_tcp_reserve(VR_Alloc* alloc)
```

Restituisce:

- Un socket invalido se l'operazione fallisce, uno valido altrimenti.

### vr_socket_tcp_init

Inizializza un socket a partire da un tipo di indirizzo IP.

```c
bool32 vr_socket_tcp_init(VR_Socket_TCP self, VR_Endpoint_IP_Kind kind)
```

Parametri:

- `VR_Socket_TCP self` - Il socket da inizializzare.
- `VR_Endpoint_IP_Kind kind` - Il tipo di indirizzo IP, **non può essere `VR_Endpoint_IP_Kind_None`**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_tcp_init_bound

Inizializza un socket **bound** a partire da un tipo di indirizzo IP e una porta.

```c
bool32 vr_socket_tcp_init_bound(VR_Socket_TCP self, VR_Endpoint_IP_Kind kind, uint16 port)
```

Parametri:

- `VR_Socket_TCP self` - Il socket da inizializzare.
- `VR_Endpoint_IP_Kind kind` - Il tipo di indirizzo IP, **non può essere `VR_Endpoint_IP_Kind_None`**.
- `uint16 port` - La porta, **non può essere zero**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_tcp_uninit

Distrugge un socket. Questa operazione chiude la connessione e dall'altro capo nessun socket riuscirà più a scrivere o leggere.

```c
void vr_socket_tcp_uninit(VR_Socket_TCP self)
```

### vr_socket_tcp_listen

Promuove un socket a listener.

```c
bool32 vr_socket_tcp_listen(VR_Socket_TCP listener)
```

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_tcp_accept

Accetta una nuova connessione su un socket a partire da un socket listener.

```c
bool32 vr_socket_tcp_accept(VR_Socket_TCP self, VR_Socket_TCP listener)
```

Parametri:

- `VR_Socket_TCP self` - Il socket su cui accettare la nuova connessione, **deve essere riservato ma non inizializzato**.
- `VR_Socket_TCP listener` - Il socket listener.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_tcp_connect

Inizia una nuova connessione su un socket verso un endpoint IP.

```c
bool32 vr_socket_tcp_connect(VR_Socket_TCP self, VR_Endpoint_IP endpoint)
```

Parametri:

- `VR_Socket_TCP self` - Il socket con cui iniziare la nuova connessione.
- `VR_Endpoint_IP endpoint` - L'endpoint a cui connettersi.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_tcp_write

Scrive su un socket **non listener** il contenuto di un blocco di memoria.

L'operazione potrebbe non inviare tutto il blocco, in generale è consigliato controllare quanti byte sono stati effettivamente inviati e scrivere i rimanenti con altre chiamate. L'operazione inoltre è bloccante, cioè attende che sia possibile scrivere almeno un byte prima di ritornare al chiamante.

Nota: Se il socket è in uno stato valido e i parametri sono corretti ma l'operazione comunque termina con zero byte scritti, può significare che la connessione dall'altro lato è stata interrotta.

```c
intptr vr_socket_tcp_write(VR_Socket_TCP self, uint8* pntr, intptr size)
```

Parametri:

- `VR_Socket_TCP self` - Il socket su cui scrivere.
- `uint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `intptr size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte scritti compreso in `[1, size]` altrimenti.

### vr_socket_tcp_read

Legge da un socket **non listener** e copia il contenuto in un blocco di memoria.

L'operazione è bloccante, cioè attende che sia possibile leggere almeno un byte prima di ritornare al chiamante.

Nota: Se il socket è in uno stato valido e i parametri sono corretti ma l'operazione comunque termina con zero byte letti, può significare che la connessione dall'altro lato è stata interrotta.

```c
intptr vr_socket_tcp_read(VR_Socket_TCP self, uint8* pntr, intptr size)
```

Parametri:

- `VR_Socket_TCP self` - Il socket da cui leggere.
- `uint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `intptr size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte letti compreso in `[1, size]` altrimenti.

## Socket UDP

Classe per lavorare con socket UDP.

Per utilizzare questa classe lato client è sufficiente preparare un socket. Ad esempio:

```c
VR_Socket_UDP  socket   = vr_socket_udp_reserve(alloc);
VR_Endpoint_IP endpoint = vr_endpoint_ip_ver4(VR_ENDPOINT_IPV4_LOCAL, 34137);

vr_socket_udp_init(socket, endpoint.kind);

uint8 buffer[256] = {0};

intptr count = snprintf((char8*) buffer, sizeof buffer - 1, "%s", "Ciao!");

vr_socket_udp_write(socket, buffer, count, endpoint);

count = vr_socket_udp_read(socket, buffer, sizeof buffer, &endpoint);
```

Per utilizzare questa classe lato server invece è necessario preparare un socket **bound**. Ad esempio:

```c
VR_Socket_UDP  socket   = vr_socket_udp_reserve(alloc);
VR_Endpoint_IP endpoint = {0};

vr_socket_udp_init_bound(listener, VR_Endpoint_IP_Kind_4, 34137);

uint8 buffer[256] = {0};

intptr count = vr_socket_udp_read(socket, buffer, sizeof buffer, &endpoint);

vr_socket_udp_write(socket, buffer, count, endpoint);
```

Nota: È sempre possibile creare un socket bound a patto che la porta sia disponibile, nel caso di un server è fondamentale ma nel caso di un client è consigliato lasciare che sia il sistema operativo a selezionarne una libera in automatico.

```c
struct VR_Socket_UDP { void* impl; };
```

### vr_socket_udp_reserve

Riserva la memoria per un socket UDP.

```c
VR_Socket_UDP vr_socket_udp_reserve(VR_Alloc* alloc)
```

Restituisce:

- Un socket invalido se l'operazione fallisce, uno valido altrimenti.

### vr_socket_upd_init

Inizializza un socket a partire da un tipo di indirizzo IP.

```c
bool32 vr_socket_udp_init(VR_Socket_UDP self, VR_Endpoint_IP_Kind kind)
```

Parametri:

- `VR_Socket_UDP self` - Il socket da inizializzare.
- `VR_Endpoint_IP_Kind kind` - Il tipo di indirizzo IP, **non può essere `VR_Endpoint_IP_Kind_None`**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_upd_init_bound

Inizializza un socket **bound** a partire da un tipo di indirizzo IP e una porta.

```c
bool32 vr_socket_udp_init_bound(VR_Socket_UDP self, VR_Endpoint_IP_Kind kind, uint16 port)
```

Parametri:

- `VR_Socket_UDP self` - Il socket da inizializzare.
- `VR_Endpoint_IP_Kind kind` - Il tipo di indirizzo specificato, **non può essere `VR_Endpoint_IP_Kind_None`**.
- `uint16 port` - La porta specificata, **non può essere zero**.

Restituisce:

- Zero se l'operazione fallisce, un valore diverso da zero altrimenti.

### vr_socket_upd_uninit

Distrugge un socket.

```c
void vr_socket_udp_uninit(VR_Socket_UDP self)
```

### vr_socket_udp_write

Scrive su un socket il contenuto di un blocco di memoria ad un certo endpoint.

L'operazione potrebbe non inviare tutto il blocco, in generale è consigliato controllare quanti byte sono stati effettivamente inviati e scrivere i rimanenti con altre chiamate. L'operazione inoltre è bloccante, cioè attende che sia possibile scrivere almeno un byte prima di tornare al chiamante.

Nota: L'operazione può terminare con successo ma non è garantito che dall'altro lato il contenuto venga ricevuto. In caso poi di più chiamate non è nemmeno garantito l'ordine di arrivo delle varie parti.

```c
intptr vr_socket_udp_write(VR_Socket_TCP self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint)
```

Parametri:

- `VR_Socket_UDP self` - Il socket su cui scrivere.
- `uint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `intptr size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.
- `VR_Endpoint_IP endpoint` - Endpoint del destinatario.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte scritti compreso in `[1, size]` altrimenti.

### vr_socket_udp_read

Legge da un socket e copia il contenuto in un blocco di memoria.

L'operazione è bloccante, cioè attende che sia possibile leggere almeno un byte prima di ritornare al chiamante.

Nota: L'operazione può terminare con successo ma non è garantito che il contenuto sia quello inviato originariamente dall'altro lato. In più se dall'altro lato viene interrotta la comunicazione l'operazione potrebbe attendere all'infinito.

```c
intptr vr_socket_udp_read(VR_Socket_UDP self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint)
```

Parametri:

- `VR_Socket_UDP self` - Il socket da cui leggere.
- `uint8* pntr` - Il blocco di memoria, **non può essere NULL**.
- `intptr size` - Dimensione (in byte) del blocco di memoria, **deve essere positiva**.
- `VR_Endpoint_IP* endpoint` - Endpoint del mittente, può essere NULL.

Restituisce:

- Zero se l'operazione fallisce, il numero di byte letti compreso in `[1, size]` altrimenti.
