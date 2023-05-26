# Publicador MQTT Multithreading em Python

Este programa permite que você crie várias instâncias de um publicador MQTT, todas rodando simultaneamente em threads separadas. Cada publicador se conectará a um broker MQTT e publicará mensagens em um tópico específico em uma frequência definida.

## Requisitos

- Python 3.6 ou superior
- A biblioteca `paho-mqtt` para Python. Para instalar, use o comando `pip install paho-mqtt`.

## Uso

1. Clone este repositório ou baixe o arquivo `mqtt_publisher.py` para o seu sistema.

2. Abra um terminal e navegue até o diretório onde o arquivo `mqtt_publisher.py` está localizado.

3. Execute o programa com o comando `python mqtt_publisher.py`. Por padrão, este programa irá criar 5 publicadores, cada um publicando mensagens a cada 5 segundos.

## Customização

- Você pode alterar o número de publicadores e a frequência das mensagens editando as linhas no final do arquivo `mqtt_publisher.py`. 

    - Para alterar o número de publicadores, modifique o valor em `range(5)` para o número desejado de publicadores.
    - Para alterar a frequência de publicação das mensagens, modifique o primeiro argumento em `args=(5, f"client{i+1}")` para o número desejado de segundos entre as publicações.

- Você pode alterar o tópico ao qual as mensagens são publicadas, editando a linha `client.publish('ajl/sala/ar', message)`.

- Você pode alterar o conteúdo das mensagens alterando o objeto JSON em `message = json.dumps({'temp': 23, 'humidity': 50, 'pressure': 1013})`.

# Monitorando a Utilização de CPU e Memória

Além de publicar mensagens MQTT, você também pode estar interessado em monitorar o uso de CPU e memória do processo MQTT broker (ou qualquer outro processo). Para fazer isso, você pode usar um dos dois scripts fornecidos neste repositório: um script shell e um script Python.

## Requisitos

- Bash (para o script shell)
- Python 3.6 ou superior (para o script Python)
- A biblioteca `psutil` para Python. Para instalar, use o comando `pip install psutil`.

## Uso

### Script Shell

1. Baixe o arquivo `monitor.sh` para o seu sistema.

2. Abra o arquivo `monitor.sh` em um editor de texto e substitua `nome_do_processo` pelo nome do processo que deseja monitorar.

3. Salve e feche o arquivo `monitor.sh`.

4. Abra um terminal e navegue até o diretório onde o arquivo `monitor.sh` está localizado.

5. Torne o script executável com o comando `chmod +x monitor.sh`.

6. Execute o script com o comando `./monitor.sh`. O script exibirá a utilização de CPU e memória do processo a cada 5 segundos.

### Script Python

1. Baixe o arquivo `monitor.py` para o seu sistema.

2. Abra o arquivo `monitor.py` em um editor de texto e substitua `mosquitto_pid` pelo PID do processo que deseja monitorar.

3. Salve e feche o arquivo `monitor.py`.

4. Abra um terminal e navegue até o diretório onde o arquivo `monitor.py` está localizado.

5. Execute o script com o comando `python monitor.py`. O script exibirá a utilização de CPU e memória do processo a cada 5 segundos.

## Notas

Estes scripts são exemplos básicos e podem não ser adequados para todas as situações. Para um monitoramento mais robusto, você pode querer explorar ferramentas mais avançadas ou modificar esses scripts para atender às suas necessidades específicas.





