# Prompt Antigravity — Título + moldura do Distribuidor (Fallen Tale)

Cole o bloco abaixo no Antigravity (Gemini).
O Cursor **não** gera PNG. Grave os arquivos no **Cliente Full**.

O F5 lê o cliente, não a source.

---

## PROMPT (copiar a partir daqui)

Você é o artista de UI do MMORPG **Fallen Tale** (base Priston Tale).

Tarefa: duas peças **só do Distribuidor** (NPC de correio / entregas). Não invente família visual nova. Copie o metal já aprovado.

### Identidade visual (obrigatória)

- Fantasia medieval sombria / premium
- Pedra escura + metal oxidado (carvão, cinza muito escuro)
- Molduras **bronze / ouro antigo forjado** (bevel, desgaste, não “1px gold”)
- Tipografia serifada, dourado envelhecido
- Sem vidro, neon, glow forte, bloom, brilho branco, plástico, sci-fi
- PNG 32-bit. Alpha **real** onde houver recorte (sem xadrez, sem fundo preto/branco sobrando)

Textura de bronze envelhecido igual `window.png` / `btl.png` do login.

### Referências concretas (abrir e imitar o metal)

1. **Painel / bevel do login** (mestre da moldura):
   `C:\Cliente Full\game\images\login\window.png`
   `C:\Cliente Full\game\images\login\btl.png` (Entrar)
   `C:\Cliente Full\game\images\login\bte.png` (Sair)
2. **Ouro 3D do título** (mestre da placa 400×64 — copie o acabamento, **não** o baú):
   `C:\Cliente Full\game\images\warehouse\armazem.png`
   (cópia: `C:\Source Priston\Source Priston\game\images\warehouse\armazem.png`)
   Luz no canto superior esquerdo de cada glifo, bronze escuro nos entalhes, ícone no **mesmo metal** das letras, vão de cerca de uma letra, grupo centralizado.
3. Composição de título ImGui (ícone + nome na placa, fundo `RGB(20,24,32)`):
   `C:\Cliente Full\game\images\quest\desafios.png`
4. Botões 128×32 da seleção (`Bt_select.tga`) — **só** referência de metal. **Não** gere botões nesta tarefa.

Se já existir `C:\Cliente Full\game\images\postbox\distribuidor.png`, use só para ver o recorte; o ícone novo é **carta lacrada**, não pergaminho de quest.

### Pasta destino

`C:\Cliente Full\game\images\postbox\`

Crie a pasta se não existir. Cópia opcional no source: `C:\Source Priston\Source Priston\game\images\postbox\` (não substitui o cliente).

O C++ carrega exatamente:

- `game\images\postbox\distribuidor.png`
- `game\images\postbox\frame.png`

### Arquivos (dimensões exatas — não mudar)

| Arquivo | Texto na arte | Tamanho | Notas |
|---|---|---|---|
| `distribuidor.png` | DISTRIBUIDOR | **400×64** | Uma placa só. Ícone à esquerda: **carta lacrada** (envelope 3D fechado + lacre de cera), silhueta na altura das maiúsculas, mesmo ouro forjado das letras. Depois o título. Fundo opaco `RGB(20,24,32)` no retângulo inteiro. Recorte o desenho **dentro** dos 400×64. Sem halo. |
| `frame.png` | (nenhum) | **760×540** | **Só a moldura** da janela, tamanho pixel a pixel da janela ImGui (não esticar). Bronze envelhecido igual `window.png` do login: bevel ~22–24 px, rebites/desgaste nos cantos. **Miolo oco:** transparente (alpha 0) **ou** preenchido opaco `RGB(20,24,32)`. Faixa interna do topo (~50 px logo abaixo do aro) em `RGB(20,24,32)` para o título colar sem emenda. **Não** desenhar X, abas, ACEITAR, RECUSAR, ENVIAR, lista, ícones de item, texto DISTRIBUIDOR nesta peça. |

A janela do jogo é **NoResize 760×540**. O código blita `frame.png` **1:1**. Outro tamanho desalinha ou é ignorado.

### Regras técnicas

1. Mesmos nomes. Sem `_v2`, `-new`, JPG, pasta `game\images\ui\`.
2. Dimensões **exatas**. Título: canvas enorme vazio vira selo ilegível (a escala usa a imagem inteira). Moldura: não 9-slice, não cantos separados, não tile.
3. `distribuidor.png`: PNG 32-bit, fundo da placa **opaco** `RGB(20,24,32)`.
4. `frame.png`: PNG 32-bit. Se o miolo for oco, alpha 0 limpo (não xadrez). Aro de bronze **opaco**.
5. Um arquivo de título só. O código reduz a placa para a faixa do header (~210–320×32–40).
6. Palavra **DISTRIBUIDOR** em MAIÚSCULAS, sem acento.

### Não redesenhar agora

- Não gerar 9-slice (`frame-corner`, `frame-edge-*`, `frame-pip`) nem pasta `game\images\ui\`.
- Não gerar botões ACEITAR / RECUSAR / ENVIAR / X / abas Receber-Enviar — são ImGui.
- Não redesenhar login (`window.png`, `btl`, `bte`), seleção de personagem, `armazem.png`, `desafios.png`.
- Não gerar `Image\SinImage\Shopall\shop-1.bmp` (HUD de pedra do inventário).
- Inventário continua pedra; não mexer nele.
- Não pintar widgets, lista de itens ou o título dentro de `frame.png`.

### Checklist de entrega

- [ ] `distribuidor.png` em `C:\Cliente Full\game\images\postbox\` — 400×64, carta lacrada + DISTRIBUIDOR, fundo `RGB(20,24,32)`
- [ ] `frame.png` em `C:\Cliente Full\game\images\postbox\` — **760×540**, aro de bronze, miolo oco ou `RGB(20,24,32)`, sem widgets
- [ ] Metal igual `window.png` / `btl.png`; ouro do título igual `armazem.png`
- [ ] Sem glow / neon / vidro / 9-slice

## FIM DO PROMPT
