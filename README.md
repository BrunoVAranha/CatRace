# Corrida de Gatos
## Nome: Bruno Valem Aranha
## RA: 11039116
Projeto para a disciplina de Computação Gráfica na UFABC.

# Etapas de Funcionamento
## 1.
No cabeçalho (window.hpp) é criado um struct para representar cada gato, e uma variavel `fimCorrida` que indica se os gatos continuam se movendo ou não.  
Também é criada a variável `m_P`, um vetor de duas posições que vai armazenar as coordenadas do ponto a ser desenhado quando o programa rodar.  
Obs: O triângulo indicado no vetor não é desenhado! Ele serve apenas como referência para desenhar os três circulos posteriormente.

![image](https://github.com/BrunoVAranha/CatRace/assets/49883183/ebb5a688-b7bf-402f-9e1e-4d30188cc067)     ![image](https://github.com/BrunoVAranha/CatRace/assets/49883183/68cd140a-31bd-4487-ad72-0d45a988a9fc)


## 2.
### Antes da execução da janela:
#### (window.cpp)  
São criadas as variáveis que indicam qual gato a câmera irá seguir, começando no gato branco. A barra de espaço muda o foco da câmera.
Em seguida, são gerados os número aleatório que serão a velocidade de cada gato (o quanto cada um irá caminhar no eixo Z a cada frame).
![image](https://github.com/BrunoVAranha/CatRace/assets/49883183/54b8aed3-366d-4188-bf9a-94216c9cbc48)


## 3.
### Ínicio da corrida:
#### (window.cpp -> onPaint())  
Em cada frame: se a corrida não acabou, o gato tem sua posição alterada no eixo Z conforme a velocidade, e a função translate atualiza sua posição no mundo. Se o gato em questão estiver e foco, a função `syncGato` então sincroniza a câmera com o gato.  
![image](https://github.com/BrunoVAranha/CatRace/assets/49883183/75316234-6637-40b9-9fb1-3eba51807b8c)  

#### (camera.cpp -> syncGato())  
`m_eye`: A câmera se posiciona no eixo X do gato, um pouco acima do seu eixo Y (para não ficar dentro dele), e um pouco atrás do seu eixo Z (para vermos a cabeça do gato).  

`m_at`: O ponto para onde a câmera deve olhar também é atualizado a cada passo do gato. Ele é configurado para estar sempre um pouco em frente ao gato.  
![image](https://github.com/BrunoVAranha/CatRace/assets/49883183/f089804d-8ddc-4260-b9f2-fa165cfc2d8b)  


## 4.
### Fim da corrida:
No fim de cada frame: se um gato alcança a linha de chegada (percorre uma distância definida), `fimCorrida` passa a ser true e consequentemente todos os gatos param de se mover.  
![image](https://github.com/BrunoVAranha/CatRace/assets/49883183/b890f5e6-b315-452f-96a9-1d79f952456b)







