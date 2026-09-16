# Prompt Antigravity — Títulos ImGui no estilo do Armazém (Fallen Tale)

Cole o bloco abaixo no Antigravity (Gemini).  
O Cursor **não** gera PNG. Grave os arquivos no **Cliente Full**.

O F5 lê o cliente, não a source.

---

## PROMPT (copiar a partir daqui)

Você é o artista de UI do MMORPG **Fallen Tale** (base Priston Tale).

Tarefa: **redesenhar os títulos das janelas ImGui** para ficarem na **mesma família visual** do título do Armazém, que já foi aprovado. Não invente um estilo novo. Não “melhore” o Armazém. **Copie o ouro, o bisel e o ícone 3D** dele.

### Peça-mestre (abrir e imitar — obrigatório)

Abra este arquivo e trate-o como **referência absoluta**:

`C:\Cliente Full\game\images\warehouse\armazem.png`  
(cópia de conferência: `C:\Source Priston\Source Priston\game\images\warehouse\armazem.png`)

O que copiar **pixel a pixel em espírito** (não o baú, o **acabamento**):

- Fundo opaco `RGB(20,24,32)` — grafite quase preto, sem vinheta, sem textura de pedra atrás das letras
- Tipografia serifada 3D de **ouro antigo forjado**: luz no canto superior esquerdo de cada glifo, bronze escuro nos entalhes, aresta viva, sem halo
- Ícone à esquerda, **mesmo metal das letras**, silhueta 3D (volume, tampa, bandas, sombreamento interno) — não um carimbo plano nem um ícone colorido
- Vão de cerca de **uma letra** entre ícone e texto
- Grupo ícone + texto **centralizado** no canvas 400×64, com folga nas laterais
- Sem glow, bloom, brilho branco, ponto especular forte, neon, vidro, plástico

O `armazem.png` mostra um **baú/cofre** + a palavra **ARMAZÉM**. Os arquivos abaixo usam **outro ícone e outro texto**, mas o ouro tem que parecer saído da mesma fundição.

### Identidade visual (kit Fallen Tale)

- Fantasia medieval sombria / premium
- Pedra escura + metal oxidado (carvão, cinza muito escuro)
- Molduras bronze / ouro antigo forjado (bevel, desgaste, não “1px gold”)
- Tipografia serifada, dourado envelhecido
- PNG com fundo da placa **opaco** `RGB(20,24,32)` (o header ImGui usa essa mesma cor)

Apoio de metal (não substituem o mestre):

- Bronze envelhecido: `C:\Cliente Full\game\images\login\window.png`, `btl.png` (Entrar), `bte.png` (Sair)
- **Não** gere botões nesta tarefa

Os títulos atuais (abaixo) já existem e já têm ícone + nome. **Substitua-os.** O problema não é a composição — é o acabamento, que ficou mais “folha dourada estampada”. O Armazém ficou **forjado 3D**. Eleve os outros ao nível do Armazém.

### Pastas destino (Cliente Full — o jogo lê daqui)

Grave cada arquivo no path **exato**. Crie a pasta se faltar. Cópia opcional na source (`C:\Source Priston\Source Priston\game\images\...`) — não substitui o cliente.

### Arquivos (dimensões exatas — todos 400×64)

| Arquivo (path absoluto) | Texto na arte | Ícone (silhueta, altura das maiúsculas) | Notas |
|---|---|---|---|
| `C:\Cliente Full\game\images\quest\desafios.png` | DESAFIOS | Pergaminho enrolado (rolo de quest), mesmo metal do baú do Armazém | C++: `game\images\quest\desafios.png` |
| `C:\Cliente Full\game\images\quest\emandamento.png` | EM ANDAMENTO | Ampulheta 3D (dois cones + areia), ouro forjado | Taskbar / overlay compacto. **Mesmo canvas 400×64** — o código reduz. Não gerar versão menor. |
| `C:\Cliente Full\game\images\ranking\ranking.png` | RANKING | Taça / troféu | C++: `game\images\ranking\ranking.png` |
| `C:\Cliente Full\game\images\shop\loja-de-coins.png` | LOJA DE COINS | Duas ou três moedas empilhadas | Frase longa: reduzir um pouco o corpo da letra para caber com folga, **sem** mudar a família do ouro |
| `C:\Cliente Full\game\images\shop\loja-de-tempo.png` | LOJA DE TEMPO | Ampulheta 3D | Distinta da taskbar: aqui o rótulo é **LOJA DE TEMPO**, não “EM ANDAMENTO” |
| `C:\Cliente Full\game\images\mix\lista-de-mix.png` | LISTA DE MIX | Gema / diamante facetado | Texto completo **LISTA DE MIX** (não só “MIX”) |
| `C:\Cliente Full\game\images\settings\configuracoes.png` | CONFIGURAÇÕES (com acentos) | Engrenagem 3D | Acentos em Ã e Ç |

Os arquivos atuais (abrir só para **composição e ícone**, não para copiar o metal velho):

- `C:\Cliente Full\game\images\quest\desafios.png`
- `C:\Cliente Full\game\images\quest\emandamento.png`
- `C:\Cliente Full\game\images\ranking\ranking.png`
- `C:\Cliente Full\game\images\shop\loja-de-coins.png`
- `C:\Cliente Full\game\images\shop\loja-de-tempo.png`
- `C:\Cliente Full\game\images\mix\lista-de-mix.png`
- `C:\Cliente Full\game\images\settings\configuracoes.png`

Se o Cliente Full ainda não tiver algum, use a cópia na source: `C:\Source Priston\Source Priston\game\images\...` (mesmos nomes).

### Regras técnicas

1. **Mesmo nome, mesmo path.** Não inventar pasta, sufixo (`_v2`, `-new`) nem JPG.
2. **400×64 px em todos.** Outro tamanho vira selo ilegível — a escala usa a imagem inteira.
3. Canvas 400×64, desenho **recortado dentro** do retângulo, fundo da placa opaco `RGB(20,24,32)`. Sem canvas enorme com o título num cantinho.
4. Um arquivo por tela. O código reduz com proporção para a faixa do header (~210–320×32–40 na janela grande; overlay ~150–176×18–28).
5. PNG 32-bit. Fundo da placa **opaco** no retângulo inteiro (não transparente). Sem xadrez.
6. Frases longas (LOJA DE COINS, LOJA DE TEMPO, LISTA DE MIX, CONFIGURAÇÕES, EM ANDAMENTO): o grupo ícone+texto tem que caber com ~16–24 px de margem esquerda e direita. Não cortar a última letra. Não apertar até virar condensado feio — melhor letra um pouco menor, mesmo bisel do Armazém.
7. Palavras em **MAIÚSCULAS**, como na tabela. Acentos só onde a tabela pede (CONFIGURAÇÕES).
8. **Não** redesenhar `armazem.png`. Ele é o mestre.

### Não redesenhar agora

- Não gerar PNG de janela inteira, moldura, 9-slice, fundo tileado, botão X, VOLTAR, CONFIRMAR — o cromado é ImDrawList.
- Não gerar botões de login (`btl`, `bte`, `window.png`) nem TGA de seleção de personagem.
- Não gerar `Image\SinImage\Shopall\shop-1.bmp` nem artes de inventário / HUD de pedra.
- Não gerar ícones de item, moeda solta da loja (`coin.png`, `buyItem.png`, etc.).
- Não gerar um segundo tamanho para a taskbar.

### Checklist de entrega

- [ ] Sete arquivos, nomes e pastas da tabela, todos em `C:\Cliente Full\game\images\...`
- [ ] Todos 400×64, recorte interno, fundo `RGB(20,24,32)`
- [ ] Ouro / bisel / volume **iguais** a `warehouse\armazem.png` (não aos títulos antigos)
- [ ] Ícone 3D à esquerda + texto da tabela; ícones: pergaminho, ampulheta (×2 contextos), taça, moedas, gema, engrenagem
- [ ] `emandamento.png` = **EM ANDAMENTO** (taskbar); `loja-de-tempo.png` = **LOJA DE TEMPO**
- [ ] Sem glow / neon / vidro
- [ ] `armazem.png` intocado

## FIM DO PROMPT
