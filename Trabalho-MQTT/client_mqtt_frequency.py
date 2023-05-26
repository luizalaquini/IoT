import time
import threading
import paho.mqtt.client as mqtt
import json

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")

def mqtt_publisher(frequency, client_id):
    client = mqtt.Client(client_id)
    client.on_connect = on_connect

    # Connect to the broker
    client.connect("localhost", 1883, 60)

    # frequência de publicação em segundos
    freq = frequency 

    try:
        while True:
            # criar e enviar mensagem
            message = json.dumps({'temp': 23, 'humidity': 50, 'pressure': 1013})
            client.publish('ajl/sala/ar', message)
            print(f'Client {client_id} published message')
            # esperar antes de publicar novamente
            time.sleep(freq)

    except KeyboardInterrupt:
        print("\nPrograma interrompido pelo usuário.")
        client.disconnect()

# Exemplo de uso
threads = []
for i in range(5):  # Substitua 5 pelo número de publicadores que você deseja criar
    thread = threading.Thread(target=mqtt_publisher, args=(5, f"client{i+1}"))
    thread.start()
    threads.append(thread)

# Aguarde todas as threads terminarem
for thread in threads:
    thread.join()
