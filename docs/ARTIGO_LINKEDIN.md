# Como utilizar a IA para planejar, projetar e arquitetar soluções - sem deixar tudo a cargo dela

Quando comecei um projeto de Estruturas de Dados em C, eu já havia implementado fila, pilha, BST, AVL e heap separadamente. O desafio real apareceu depois: como fazer todas essas estruturas trabalharem juntas em um sistema coerente?

Foi aí que usei a inteligência artificial de uma forma diferente. Em vez de pedir “faça o projeto”, usei a IA como parceira para planejar, questionar decisões e revisar meu raciocínio.

## O problema antes do código

O sistema deveria cadastrar e classificar atendimentos. Casos comuns e moderados seguiriam para uma fila. Urgentes e críticos iriam para um heap máximo. BST e AVL permitiriam busca por código, uma pilha registraria o histórico e uma árvore de decisão faria a classificação.

À primeira vista, parecia apenas uma lista de estruturas. Mas algumas perguntas mudavam toda a arquitetura:

- Quem é dono da memória de cada atendimento?
- As árvores devem guardar cópias ou ponteiros?
- Quando o código automático deve ser incrementado?
- O que acontece se a classificação for atualizada, mas a inserção na fila falhar?
- Como desempatar dois atendimentos com a mesma prioridade?
- Quais transições de status são permitidas?

A IA ajudou a levantar essas perguntas. As respostas, porém, foram discutidas e decididas por mim.

## A especificação virou parte do aprendizado

Antes de integrar o código, definimos regras explícitas:

- o repositório seria o proprietário dos atendimentos;
- fila, heap, BST e AVL guardariam ponteiros para o mesmo objeto;
- códigos e ordens começariam em 1 e só avançariam após confirmação;
- o fluxo de status seria aberto, classificado, em atendimento e concluído;
- prioridade maior teria precedência e empates seriam resolvidos pela ordem de chegada;
- uma falha durante a classificação deveria restaurar o estado anterior.

Esse trabalho de especificação reduziu ambiguidades. Quando chegou a hora de implementar, cada módulo já tinha uma responsabilidade e um contrato.

## IA como mentora, não como piloto automático

O processo foi incremental. Para cada módulo:

1. eu definia o que ele deveria fazer;
2. discutia a interface e os casos de erro;
3. implementava linha por linha;
4. perguntava sobre trechos que não entendia;
5. compilava com avisos habilitados;
6. criava testes automáticos;
7. só depois seguia para o próximo módulo.

Durante o trabalho surgiram dúvidas importantes: por que uma função retorna um ponteiro `const`? Quando um cast de `malloc` é desnecessário em C? O rebalanceamento da AVL ocorre antes ou depois da inserção? Por que uma função que consulta o topo não deve alterar a pilha?

Essas perguntas fizeram a IA atuar como mentora. Receber uma explicação e confrontá-la com o código foi mais valioso do que receber uma solução pronta.

## O resultado técnico

O projeto terminou com módulos separados para entrada, modelo de atendimento, repositório, fila, pilha, BST, AVL, heap, árvore de decisão, validação, integração e experimentos.

Foram criados 12 testes automáticos. Todos passaram com compilação C11 e as opções `-Wall -Wextra -pedantic`.

Nos experimentos com 20 códigos, a BST atingiu altura 19 nas inserções crescente e decrescente, enquanto a AVL terminou com altura 4. Em uma ordem pseudoaleatória fixa, a BST ficou com altura 5 e a AVL com 4.

Mais importante que os números foi conseguir explicar por que isso aconteceu.

## Um projeto que não termina na primeira entrega

Depois de concluir a versão inicial, decidi manter o repositório como um laboratório interdisciplinar.

A primeira evolução planejada utilizará conceitos de Sistemas Operacionais para automatizar a execução dos atendimentos. Hoje, o usuário escolhe manualmente quando chamar o próximo caso comum ou prioritário. Em uma versão futura, um módulo escalonador poderá selecionar os atendimentos conforme políticas como FCFS, prioridade, Round Robin, envelhecimento e escalonamento híbrido.

Essa evolução não começará diretamente no código. Antes, o novo problema será analisado da mesma forma: requisitos, estados, métricas, fluxogramas, alternativas arquiteturais, critérios de aceitação e testes.

Outras etapas poderão incorporar persistência, concorrência, análise de desempenho, interfaces gráficas e sistemas distribuídos. Assim, o mesmo domínio servirá para conectar conhecimentos de diferentes disciplinas e mostrar como a arquitetura evolui quando surgem novos requisitos.

## O que aprendi sobre usar IA em projetos

Uma boa utilização da IA não começa com um pedido de código. Começa com contexto e perguntas.

Eu recomendo este fluxo:

- descreva o problema e o que você já sabe;
- peça que a IA identifique ambiguidades;
- responda uma decisão por vez;
- registre requisitos, regras e invariantes;
- desenhe o fluxo e as responsabilidades;
- implemente em etapas pequenas;
- exija testes e evidências;
- questione tudo o que você não consegue explicar;
- compare a sugestão com o comportamento real;
- documente a participação da IA com transparência.

Se você não consegue explicar, testar ou modificar o que foi produzido, a ferramenta assumiu espaço demais no processo.

## Conclusão

A IA pode acelerar o planejamento e ampliar a qualidade das perguntas. Pode revelar casos de borda, ajudar a comparar alternativas e manter a documentação coerente. Mas arquitetura envolve decisões, contexto e responsabilidade.

Neste projeto, a IA não substituiu o estudo das estruturas de dados. Ela ajudou a conectar o estudo a um processo real de engenharia: problema, interpretação, requisitos, arquitetura, implementação, teste e documentação.

Esse, para mim, é o uso mais interessante da IA no desenvolvimento de software: não pensar por nós, mas nos ajudar a pensar melhor.

O código, os testes, os relatórios, o roadmap e o prompt reutilizável estão disponíveis no GitHub:

https://github.com/DoravanBR/central-atendimento-estruturas-dados-c

**E você: tem usado IA apenas para gerar código ou também para melhorar o seu processo de decisão?**

#InteligenciaArtificial #DesenvolvimentoDeSoftware #ArquiteturaDeSoftware #EngenhariaDeSoftware #Programacao #LinguagemC #EstruturasDeDados #Aprendizado

