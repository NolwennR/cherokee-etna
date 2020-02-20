# Cherokee

## Architecture

### Serveur

- Server tcp spawn un nombre déterminé de workers au moment de l'initialization.
- Chaque worker prend un charge des requetes http et les traite, répartition des connections géré par le système d'exploitation
- Pour servir des pages statiques, chaque worker implémente un cache LRU. 
        https://www.geeksforgeeks.org/lru-cache-implementation/

### Parser http

- Parse du header et du body
- Generation de token
- Comportement en fonction des tokens


## Optimistion niveau code

- Utilisation de slab allocation: permet de l'espace mémoire pour des objects de même type, dans notre cas une connection.
        https://en.wikipedia.org/wiki/Slab_allocation
- Alignement mémoire des structures pour éviter le padding
- Utilisation des structures de tableau pour charger de la mémoire continue dans le cache





