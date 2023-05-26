#!/bin/bash

# Substitua 'nome_do_processo' pelo nome do processo que você deseja monitorar
nome_do_processo="mosquitto"

while true
do
    # Obtém o PID do processo
    pid=$(pgrep -f $nome_do_processo)

    if [ -n "$pid" ]; then
        # Usa 'ps' para obter a utilização de CPU e memória para o PID
        ps u -p $pid | tail -n +2
    else
        echo "Processo $nome_do_processo não encontrado."
    fi

    # Aguarda 1 segundo antes de verificar novamente
    sleep 1
done
