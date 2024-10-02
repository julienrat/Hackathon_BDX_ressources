
# Documentation de la Bibliothèque `http_client.py`

Cette bibliothèque fournit une classe `HTTPClient` pour faciliter les requêtes HTTP GET sur un serveur. Elle gère également la connexion au réseau Wi-Fi et la synchronisation de l'heure via un serveur NTP.

## Installation

Assurez-vous d'avoir MicroPython ou une version compatible qui prend en charge les modules `usocket`, `ssl`, `json`, `network`, et `ntptime`.

## Utilisation

### Importation de la bibliothèque

```python
from http_client import HTTPClient
```

### Initialisation de la classe

Pour utiliser la classe `HTTPClient`, vous devez fournir le nom du réseau Wi-Fi, le mot de passe, l'hôte (URL de l'API) et le port (par défaut 443 pour HTTPS).

```python
client = HTTPClient(ssid='votre_ssid', password='votre_mot_de_passe', host='votre_hote', port=443)
```

### Méthodes

#### `connect_wifi()`

Établit une connexion au réseau Wi-Fi.

**Usage :**
- Appelée automatiquement lors de l'initialisation de la classe.

#### `set_time()`

Synchronise l'heure avec un serveur NTP.

**Usage :**
```python
client.set_time()
```

#### `send_request(path)`

Envoie une requête HTTP GET à l'URL spécifiée et retourne la réponse JSON.

**Paramètres :**
- `path` (str): Le chemin de l'API à interroger.

**Retourne :**
- `dict`: La réponse JSON du serveur ou `None` en cas d'erreur.

**Usage :**
```python
response = client.send_request('/api/path')
```

#### `decode_chunked_body(chunked_body)`

Décode le corps de la réponse si elle est encodée en morceaux.

**Paramètres :**
- `chunked_body` (bytes): Le corps de la réponse à décoder.

**Retourne :**
- `bytes`: Le corps décodé.

#### `get_data(path, fields)`

Récupère des champs spécifiés à partir de la réponse du serveur et retourne leurs valeurs.

**Paramètres :**
- `path` (str): Le chemin de l'API à interroger.
- `fields` (str ou list): Les champs à extraire de la réponse.

**Retourne :**
- `list`: Une liste d'objets contenant les champs spécifiés.

**Usage :**
```python
data = client.get_data('/api/path', ['field1', 'field2'])
```

#### `extract_field(path, field)`

Extrait et retourne les valeurs d'un champ spécifique de la réponse du serveur.

**Paramètres :**
- `path` (str): Le chemin de l'API à interroger.
- `field` (str): Le champ à extraire.

**Retourne :**
- `list`: Une liste des valeurs du champ spécifié.

**Usage :**
```python
values = client.extract_field('/api/path', 'field_name')
```

## Exemples

Voici un exemple d'utilisation de la bibliothèque :

```python
from http_client import HTTPClient

# Initialisation du client HTTP
client = HTTPClient(ssid='votre_ssid', password='votre_mot_de_passe', host='votre_hote')

# Synchronisation de l'heure
client.set_time()

# Envoi d'une requête et récupération des données
data = client.get_data('/api/path', ['entree', 'sortie'])

# Affichage des données
print(data)
```

## Remarques

- Assurez-vous d'utiliser des chemins API valides lors de l'appel des méthodes.
- En cas d'erreur de connexion ou de décodage, des messages d'erreur appropriés seront affichés dans la console.

## Conclusion

La bibliothèque `http_client.py` simplifie le processus d'interrogation des API via HTTP tout en gérant la connexion réseau et la synchronisation de l'heure, ce qui est essentiel pour de nombreuses applications IoT.
