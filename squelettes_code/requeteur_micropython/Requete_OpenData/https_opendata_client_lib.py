# http_client.py

import usocket as socket
import ssl
import json
import network
import ntptime

class HTTPClient:
    def __init__(self, ssid, password, host, port=443):
        self.ssid = ssid
        self.password = password
        self.host = host
        self.port = port

        # Connexion au réseau Wi-Fi
        self.connect_wifi()

    def connect_wifi(self):
        """Establish a connection to the Wi-Fi network."""
        wlan = network.WLAN(network.STA_IF)
        wlan.active(True)
        if not wlan.isconnected():
            print('Connecting to network...')
            wlan.connect(self.ssid, self.password)
            while not wlan.isconnected():
                pass
        print('Connected to Wi-Fi:', wlan.ifconfig())

    def set_time(self):
        """Synchronize time with NTP server."""
        try:
            ntptime.settime()
            print("Time synchronized successfully.")
        except Exception as e:
            print("Error syncing time:", e)

    def send_request(self, path):
        """Send an HTTP GET request and return the JSON response."""
        addr = socket.getaddrinfo(self.host, self.port)[0][-1]
        sock = socket.socket()
        sock.connect(addr)
        sock = ssl.wrap_socket(sock, server_hostname=self.host)

        request = f"GET {path} HTTP/1.1\r\nHost: {self.host}\r\nConnection: close\r\n\r\n"
        sock.write(request)

        response = b""
        while True:
            chunk = sock.read(1024)
            if not chunk:
                break
            response += chunk

        sock.close()
        print(response)
        headers, body = response.split(b"\r\n\r\n", 1)

        # Directly decode the body without chunked decoding
        try:
            json_str = body.decode('utf-8')
            print("Raw JSON response:", json_str)  # Affiche la réponse JSON brute
            return json.loads(json_str)
        except ValueError as e:
            print("Error decoding JSON:", e)
            print("Failed JSON string:", json_str)  # Affiche le JSON qui échoue
            return None

    def decode_chunked_body(self, chunked_body):
        """Decode a chunked response body."""
        decoded = b""
        while chunked_body:
            try:
                hex_size, chunked_body = chunked_body.split(b"\r\n", 1)
                size = int(hex_size, 16)
                if size == 0:
                    break
                chunk, chunked_body = chunked_body[:size], chunked_body[size + 2:]  # +2 pour "\r\n"
                decoded += chunk
            except ValueError:
                break  # Si une erreur se produit, sortez de la boucle
        return decoded

    def get_data(self, path, fields):
        """Get specified fields from the server and return their values."""
        data = self.send_request(path)

        if data is None:
            return []  # Return empty list if there's an error

        # Si un seul champ est passé, le transformer en liste
        if isinstance(fields, str):
            fields = [fields]

        output_data = []
        for feature in data['results']:  # Changez 'features' en 'results'
            properties = feature
            output = {field: properties.get(field) for field in fields}
            output_data.append(output)  # Ajouter le résultat à la liste

        return output_data  # Retourner les données au lieu de les imprimer

    def extract_field(self, path, field):
        """Extract and return the values of a specific field from the server response."""
        data = self.send_request(path)
        if data is None:
            return []  # Return empty list if there's an error
        values = [feature.get(field) for feature in data['results']]  # Changez 'features' en 'results'
        return values
