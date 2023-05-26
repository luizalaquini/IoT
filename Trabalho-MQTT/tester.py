import psutil
import time

#PID real do broker MQTT
mosquitto_pid = 4144

try:
    while True:
        p = psutil.Process(mosquitto_pid)

        # Obter a utilização percentual da CPU
        uso_cpu = p.cpu_percent(interval=1)
        print(f"Uso da CPU: {uso_cpu}%")

        # Obter a utilização de memória
        info_memoria = p.memory_info()
        uso_memoria = info_memoria.rss  # Resident Set Size: parte da memória do processo que é mantida na RAM
        print(f"Uso da memória: {uso_memoria / (1024 * 1024)} MB")

        time.sleep(2)  # pausa por 2 segundos

except psutil.NoSuchProcess:
    print("Processo não encontrado.")
