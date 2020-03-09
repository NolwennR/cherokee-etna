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

## Pense-bête
- Pour servir les fichiers statiques, ne pas oublier le vérifier la présence du dossier au lancemenet du serveur (dossier /files par ex)

# Travail en cours


## Fait

- manager peut spawner les workers
- worker écoute sur le port et récecptionne les requetes

## En cours

- parser http

## A faire

- Utiliser la librairie de parsing des arguments => Marcus
- Régler bug mémoire
- Mettre en place les tests unitaires => morgan
- Lire les fichiers statiques
- Envoyer les réponses
- Mettre en place le LRU

#### Les librairies
https://github.com/cofyc/argparse
https://github.com/rxi/log.c

#### Epoll
https://medium.com/@copyconstruct/the-method-to-epolls-madness-d9d2d6378642

