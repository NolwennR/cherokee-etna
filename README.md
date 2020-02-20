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





