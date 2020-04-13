# Cherokee

## Architecture

### Serveur

- Server tcp spawn un nombre déterminé de workers au moment de l'initialization.
- Chaque worker prend un charge des requetes http et les traite, répartition des connections géré par le système d'exploitation
- Polling des workers grâce à epoll() ou poll()
- Pour servir des pages statiques, chaque worker implémente un cache LRU. 
        https://www.geeksforgeeks.org/lru-cache-implementation/

### Parser http

- Parse du header et generation de tokens
- Parse du body et generation de tokens
- Appel du handler associé
- Generation de la réponse (header / body)
- Stream au client
- Fermeture de la connection

### Modules

Les modules permettent la prise en charge des requêtes
- Module http
- Module directory listing

## Optimistion niveau code

- Utilisation de slab allocation: permet de l'espace mémoire pour des objects de même type, dans notre cas une connection.
        https://en.wikipedia.org/wiki/Slab_allocation
- Alignement mémoire des structures pour éviter le padding
- Utilisation des structures de tableau pour charger de la mémoire continue dans le cache

## Tests

### Installation de criterion

`̀ `
$ sudo add-apt-repository ppa:snaipewastaken/ppa
$ sudo apt-get update
$ sudo apt-get install criterion-dev
`̀ `

### Tests pour le parsing
- parsing du header
- parsing du body
- parsing de l'url


### Tests requêtes
- Si pas HTTP/1.1 retourne Not supported

## Benchmarking

### Apache benchmark
 install ```sudo apt-get install apache2-utils```
 
 run test ```ab -n 5000 -c 500 http://localhost:8050/```

## Pense-bêt

## En cours

## A faire

- CRUD => Nonox
- Not supported => Nonox
- Mettre en place le LRU => Cécé

#### Les librairies
https://github.com/cofyc/argparse
https://github.com/rxi/log.c
https://github.com/benhoyt/inih


#### Epoll
https://medium.com/@copyconstruct/the-method-to-epolls-madness-d9d2d6378642

#### Question suivi
- Listing du répertoire doit-on generer un html ? oui
- si index dans le dossier on sert l'index.html
- si fichier sans extension considère comme répertoire ? 
- Problème read image

## Architecture

### Diagramme de decision
https://www.loggly.com/blog/http-status-code-diagram/
