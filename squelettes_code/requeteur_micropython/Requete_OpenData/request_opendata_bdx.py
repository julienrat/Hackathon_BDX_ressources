from https_client_lib import HTTPClient  # Importer votre bibliothèque

# Paramètres de connexion
ssid = 'Les Usines'
password = 'usinesnouvelles'
host = 'opendata.bordeaux-metropole.fr'
# Intégrer la clé API dans le path (si nécessaire)
#path = '/geojson/features/pc_captv_p?filter=%7B%22gid%22%3A%222451%22%7D&attributes=%5B%22gid%22%2C%22comptage_5m%22%2C%22mdate%22%2C%22libelle%22%5D&maxfeatures=3&orderby=&key=K7CB2JBXGI'
path = '/api/explore/v2.1/catalog/datasets/bor_frequentation_piscine_tr/records?limit=20'

# Créer une instance du client HTTP
client = HTTPClient(ssid, password, host)

# Synchroniser l'heure
client.set_time()

# Obtenir les données avec gestion des erreurs
try:
    data = client.get_data(path, ['fmizonlib', 'entree', 'etablissement_etalib'])
    #data = client.get_data(path, ['comptage_5m', 'libelle'])
    print("Data retrieved:", data)
except Exception as e:
    print(f"Error retrieving data: {e}")

# Extraire les valeurs du champ 'fmizonlib'
libelle_values = client.extract_field(path, 'entree')
if libelle_values:
    print("Libelle values:", libelle_values)
else:
    print("No libelle values found.")