# Cherokee

## Architecture

### Serveur

- Server tcp spawn un nombre déterminé de workers au moment de l'initialization.
- Chaque worker prend un charge des requetes http et les traite, répartition des connections géré par le système d'exploitation
- Polling des workers grâce à epoll()
- Pour servir des pages statiques, chaque worker implémente un cache LRU. 

### Prise en charge de la requête

- Parse du header et generation de tokens
- Parse du body et generation de tokens
- Appel du handler associé
- Generation de la réponse (header / body)
- Stream au client
- Fermeture de la connection

## Tests

### Installation de criterion

```
$ sudo add-apt-repository ppa:snaipewastaken/ppa
$ sudo apt-get update
$ sudo apt-get install criterion-dev
```

### Tests pour le parsing
- parsing du header
- parsing du body
- parsing de l'url


## Benchmarking

### Apache benchmark
 install ```sudo apt-get install apache2-utils```
 
 run test ```ab -n 5000 -c 500 http://localhost:8070/```


## Les librairies
https://github.com/cofyc/argparse

https://github.com/rxi/log.c

https://github.com/benhoyt/inih

## Ressources

### Architecture serveur

http://aosabook.org/en/nginx.html

http://www.kegel.com/c10k.html 

https://pl.atyp.us/content/tech/servers.html

https://www.tutorialspoint.com/http/http_requests.htm

https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_71/rzab6/example.htm


### LRU

https://www.geeksforgeeks.org/lru-cache-implementation/


### Epoll

https://medium.com/@copyconstruct/the-method-to-epolls-madness-d9d2d6378642

https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/#.XphKxvngrmh


### Diagramme de decision

https://www.loggly.com/blog/http-status-code-diagram/


### Embeded Python

find flags: 
/usr/lib/python3.6/config-3.6m-x86_64-linux-gnu/python-config.py --libs

/usr/lib/python3.6/config-3.6m-x86_64-linux-gnu/python-config.py --cflags

find header:

locate Python.h