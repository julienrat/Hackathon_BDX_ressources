from https_client_lib import HTTPClient  # Importer votre bibliothèque

# Paramètres de connexion
ssid = 'Les Usines'
password = 'usinesnouvelles'
host = 'data.bordeaux-metropole.fr'
api_key = 'K7CB2JBXGI'  # Votre clé API ici
path = '/geojson/features/pc_captv_p?filter=%7B%22gid%22%3A%222451%22%7D&attributes=%5B%22gid%22%2C%22comptage_5m%22%2C%22mdate%22%2C%22libelle%22%5D&maxfeatures=3&orderby='

# Créer une instance du client HTTP
client = HTTPClient(ssid, password, host, api_key)

# Synchroniser l'heure
client.set_time()

# Obtenir les données
data = client.get_data(path, ['libelle', 'comptage_5m'])
print("Data retrieved:", data)

# Extraire les valeurs du champ 'libelle'
libelle_values = client.extract_field(path, 'libelle')
print("Libelle values:", libelle_values[0])

